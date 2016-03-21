#include <MemoryAllocator.h>

void* ForwardAlign(void *address, uint8 align)
{
	return (void*)(((uint32)address + align - 1) & (~(align - 1)));
}

uint32 ForwardAlign(uint32 size, uint8 align)
{
	return ((size + align - 1) & (~(align - 1)));
}

void ShutDownMemorySystem(MainAllocator *mainAlloc)
{
	if (mainAlloc->PoolSystem)
	{
		if (mainAlloc->PoolSystem->UnusedIndex)
		{
			for (uint32 i = 0; i < mainAlloc->PoolSystem->UnusedIndex->size(); i++)
			{
				mainAlloc->PoolSystem->UnusedIndex->pop();
			}
		}
	}
	free(mainAlloc->MainMemory.BaseAddress);
	mainAlloc->MainMemory.BaseAddress = NULL;
}

void InitMainMemorySystem(MainAllocator *mainAlloc, AllocatorTypes allocType, uint32 size, int8 align, uint32 poolBlockSize)
{
	crash(size <= 0);

	if (allocType == POOL_ALLOCATOR)
	{
		mainAlloc->MainMemory.Size = (size + sizeof(PoolAllocator) + 4) + ((sizeof(PoolBlock)) * (size / poolBlockSize));
		mainAlloc->MainMemory.BaseAddress = malloc(mainAlloc->MainMemory.Size);
		mainAlloc->MainMemory.CapAddress = (void*)((uint8*)mainAlloc->MainMemory.BaseAddress + mainAlloc->MainMemory.Size);

		mainAlloc->MainMemory.BaseAddress = ForwardAlign((void*)((uint8*)mainAlloc->MainMemory.BaseAddress), align);
		mainAlloc->MainMemory.CapAddress = ForwardAlign(mainAlloc->MainMemory.CapAddress, align);
		mainAlloc->MainMemory.Size = mainAlloc->MainMemory.Size - ((sizeof(PoolBlock)) * (size / poolBlockSize)) - sizeof(PoolAllocator);
	}
	else if (allocType == STACK_ALLOCATOR)
	{
		mainAlloc->MainMemory.Size = (size + sizeof(StackAllocator) +4);
		mainAlloc->MainMemory.BaseAddress = malloc(mainAlloc->MainMemory.Size);
		mainAlloc->MainMemory.CapAddress = (void*)((uint8*)mainAlloc->MainMemory.BaseAddress + mainAlloc->MainMemory.Size);

		mainAlloc->MainMemory.BaseAddress = ForwardAlign((void*)((uint8*)mainAlloc->MainMemory.BaseAddress), align);
		mainAlloc->MainMemory.CapAddress = ForwardAlign(mainAlloc->MainMemory.CapAddress, align);
	}
	else
	{
		mainAlloc->MainMemory.Size = (size + sizeof(FrameAllocator) + 4);
		mainAlloc->MainMemory.BaseAddress = malloc(mainAlloc->MainMemory.Size);
		mainAlloc->MainMemory.CapAddress = (void*)((uint8*)mainAlloc->MainMemory.BaseAddress + mainAlloc->MainMemory.Size);

		mainAlloc->MainMemory.BaseAddress = ForwardAlign((void*)((uint8*)mainAlloc->MainMemory.BaseAddress), align);
		mainAlloc->MainMemory.CapAddress = ForwardAlign(mainAlloc->MainMemory.CapAddress, align);
	}

	mainAlloc->FrameSystem = NULL;
	mainAlloc->StackSystem = NULL;
	mainAlloc->PoolSystem = NULL;

	crash(!mainAlloc->MainMemory.BaseAddress);
	
	switch (allocType)
	{
		case FRAME_ALLOCATOR:
		{
			mainAlloc->MainSystem = AllocatorTypes::FRAME_ALLOCATOR;
			mainAlloc->FrameSystem = (FrameAllocator*)mainAlloc->MainMemory.BaseAddress;
			InitFrameSystem(&mainAlloc->MainMemory, mainAlloc->FrameSystem, align);
		} break;
		case STACK_ALLOCATOR:
		{
			mainAlloc->MainSystem = AllocatorTypes::STACK_ALLOCATOR;
			mainAlloc->StackSystem = (StackAllocator*)mainAlloc->MainMemory.BaseAddress;
			InitStackSystem(&mainAlloc->MainMemory, mainAlloc->StackSystem, align);
		} break;
		case POOL_ALLOCATOR:
		{
			mainAlloc->MainSystem = AllocatorTypes::POOL_ALLOCATOR;
			mainAlloc->PoolSystem = (PoolAllocator*)mainAlloc->MainMemory.BaseAddress;
			if (poolBlockSize > 0)
			{
				InitPoolSystem(&mainAlloc->MainMemory, mainAlloc->PoolSystem, poolBlockSize, align);
			}
		} break;
		default:
		{
			mainAlloc->MainSystem = AllocatorTypes::FRAME_ALLOCATOR;
			mainAlloc->FrameSystem = (FrameAllocator*)mainAlloc->MainMemory.BaseAddress;
			InitFrameSystem(&mainAlloc->MainMemory, mainAlloc->FrameSystem, align);
		} break;
	}
}

AllocatorErrors SetMemorySystemDimensions(void *memory, BlockDimensions *dimensions, uint32 size, int8 align)
{
	if (!memory)
	{
		return AllocatorErrors::INVALID_MEMORY;
	}

	dimensions->BaseAddress = ForwardAlign(memory, align);
	
	dimensions->CapAddress = ForwardAlign((uint8*)memory + size, align);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitFrameSystem(BlockDimensions *sourceDimensions, FrameAllocator *destinationAllocator, int8 align)
{
	destinationAllocator->ByteAlignment = align;
	destinationAllocator->Dimensions = *sourceDimensions;

	destinationAllocator->StackPointer[HeapType::LOWER_HEAP] = (uint8*)destinationAllocator->Dimensions.BaseAddress + sizeof(FrameAllocator);
	destinationAllocator->StackPointer[HeapType::HIGHER_HEAP] = destinationAllocator->Dimensions.CapAddress;

	return AllocatorErrors::NO_ERRORS;
}

void ResetFrameAllocator(FrameAllocator *allocator)
{
	memset(allocator->Dimensions.BaseAddress, 0, allocator->Dimensions.Size);

	allocator->Dimensions.BaseAddress = NULL;
	allocator->Dimensions.CapAddress = NULL;

	allocator->StackPointer[HeapType::LOWER_HEAP] = NULL;
	allocator->StackPointer[HeapType::HIGHER_HEAP] = NULL;

	allocator->ByteAlignment = 0;
}

FrameBookmark FrameAlloc(FrameAllocator *allocator, HeapType heapType, uint32 size)
{
	crash(!allocator);
	FrameBookmark bookmark = {};

	uint32 frameSize = ForwardAlign(size, allocator->ByteAlignment);

	crash(frameSize > allocator->Dimensions.Size - (allocator->UsedSize[HeapType::HIGHER_HEAP] + allocator->UsedSize[HeapType::LOWER_HEAP]));


	if ((uint8*)allocator->StackPointer[HeapType::LOWER_HEAP] + frameSize > (uint8*)allocator->StackPointer[HeapType::HIGHER_HEAP])
	{
		return bookmark;
	}

	bookmark.PreviousSize += allocator->UsedSize[heapType];
	allocator->UsedSize[heapType] += frameSize;

	if (heapType == HeapType::HIGHER_HEAP)
	{
		bookmark.Dimensions.CapAddress = allocator->StackPointer[heapType];
		allocator->StackPointer[heapType] = (void*)((uint8*)allocator->StackPointer[heapType] - frameSize);
		bookmark.Dimensions.BaseAddress = allocator->StackPointer[heapType];
		bookmark.Heap = heapType;
		bookmark.Dimensions.Size = frameSize;
	}
	else
	{
		bookmark.Dimensions.BaseAddress = allocator->StackPointer[heapType];
		bookmark.Heap = heapType;
		allocator->StackPointer[heapType] = (void*)((uint8*)allocator->StackPointer[heapType] + frameSize);
		bookmark.Dimensions.CapAddress = allocator->StackPointer[heapType];
		bookmark.Dimensions.Size = frameSize;
	}

	return bookmark;
}

FrameBookmark GetFrameBookmark(FrameAllocator *allocator, HeapType heapType)
{
	FrameBookmark bookmark;

	bookmark.Dimensions.BaseAddress = allocator->StackPointer[heapType];
	bookmark.Heap = heapType;

	return bookmark;
}

void FrameRelease(FrameAllocator *allocator, FrameBookmark *bookmark)
{
	crash(!allocator);

	allocator->StackPointer[bookmark->Heap] = bookmark->Dimensions.BaseAddress;
	
	memset(allocator->StackPointer[bookmark->Heap], 0, allocator->UsedSize[bookmark->Heap] - bookmark->PreviousSize);

	allocator->UsedSize[bookmark->Heap] = bookmark->PreviousSize;
}

AllocatorErrors InitStackSystem(BlockDimensions *sourceDimensions, StackAllocator *destinationAllocator, int8 align)
{
	destinationAllocator->Dimensions = *sourceDimensions; 
	destinationAllocator->StackPointer = (uint8*)sourceDimensions->BaseAddress + sizeof(StackAllocator);
	destinationAllocator->UsedSize = 0;
	destinationAllocator->ByteAlignment = align;

	return AllocatorErrors::NO_ERRORS;
}

StackBookmark StackAlloc(StackAllocator *allocator, uint32 size)
{
	crash(!allocator);
	StackBookmark bookmark = {};

	uint32 requestedSize = ForwardAlign(size, allocator->ByteAlignment);
	
	crash((uint8*)allocator->StackPointer + requestedSize > (uint8*)allocator->Dimensions.CapAddress);

	if ((uint8*)allocator->StackPointer + requestedSize > (uint8*)allocator->Dimensions.CapAddress)
	{
		return bookmark;
	}

	bookmark.PreviousSize += allocator->UsedSize;
	allocator->UsedSize += requestedSize;

	bookmark.Dimensions.BaseAddress = allocator->StackPointer;
	allocator->StackPointer = (void*)((uint8*)allocator->StackPointer + requestedSize);
	bookmark.Dimensions.CapAddress = allocator->StackPointer;
	bookmark.Dimensions.Size = requestedSize;

	return bookmark;
}

StackBookmark GetStackBookmark(StackAllocator *allocator)
{
	StackBookmark bookmark;

	bookmark.Dimensions.BaseAddress = allocator->StackPointer;

	return bookmark;
}

void StackRelease(StackAllocator *allocator, StackBookmark *bookmark)
{
	crash(!allocator);

	allocator->StackPointer = bookmark;
	
	memset(allocator->StackPointer, 0, allocator->Dimensions.Size - allocator->UsedSize);
	
	allocator->UsedSize = bookmark->PreviousSize;
}

AllocatorErrors InitPoolSystem(BlockDimensions *sourceDimensions, PoolAllocator *destinationAllocator, uint32 blockSize, uint8 align, bool useQueue)
{
	destinationAllocator->ByteAlignment = align;
	destinationAllocator->Dimensions = *sourceDimensions;

	uint32 alignedBlockSize = ForwardAlign(blockSize , align);
	uint32 blockCount = destinationAllocator->Dimensions.Size / blockSize;

	crash((blockCount * (alignedBlockSize + sizeof(PoolBlock))) + sizeof(PoolAllocator) > sourceDimensions->Size + (blockCount * (alignedBlockSize + sizeof(PoolBlock))) + sizeof(PoolAllocator));
	crash(alignedBlockSize <= 0);
	crash(blockCount <= 1);
	
	destinationAllocator->BlockCount = blockCount;
	destinationAllocator->BlockSize = alignedBlockSize;
	destinationAllocator->FirstUsed = blockCount + 1;
	destinationAllocator->CurrentUsed = blockCount + 1;
	destinationAllocator->UseQueue = useQueue;

	if (destinationAllocator->UseQueue)
	{
		destinationAllocator->UnusedIndex = new std::queue<uint32>();
	}	

	destinationAllocator->Blocks = (PoolBlock*)((uint8*)destinationAllocator->Dimensions.BaseAddress + sizeof(PoolAllocator));

	for (uint32 i = 0; i < blockCount; i++)
	{
		void *base = ((uint8*)destinationAllocator->Dimensions.BaseAddress + sizeof(PoolAllocator)) + (blockCount * sizeof(PoolBlock)) + (alignedBlockSize * i);
		void *cap = (uint8*)base + alignedBlockSize;

		destinationAllocator->Blocks[i].Dimensions.BaseAddress = base;
		destinationAllocator->Blocks[i].Dimensions.CapAddress = cap;
		destinationAllocator->Blocks[i].Dimensions.Size = alignedBlockSize;
		destinationAllocator->Blocks[i].IsUsed = false;
		destinationAllocator->Blocks[i].NextUsedIndex = blockCount + 1;
		destinationAllocator->Blocks[i].PreviousUsedIndex = blockCount + 1;
		destinationAllocator->Blocks[i].BlockIndex = i;

		if (destinationAllocator->UseQueue)
		{
			(*destinationAllocator->UnusedIndex).push(i);
		}
	}

	destinationAllocator->UsedBlockCount = 0;

	return AllocatorErrors::NO_ERRORS;
}

PoolBlock PoolAlloc(PoolAllocator *allocator)
{
	crash(!allocator);
	crash(allocator->UsedBlockCount > allocator->BlockCount);
	crash(allocator->UseQueue != true);

	allocator->UsedBlockCount++;
	uint32 index = allocator->UnusedIndex->front();
	allocator->Blocks[index].IsUsed = true;

	if (allocator->FirstUsed == allocator->BlockCount + 1)
	{
		allocator->FirstUsed = index;
		allocator->CurrentUsed = index;
	}
	else
	{
		allocator->Blocks[allocator->CurrentUsed].NextUsedIndex = index;
		allocator->Blocks[index].PreviousUsedIndex = allocator->CurrentUsed;
		allocator->CurrentUsed = index;
	}

	allocator->UnusedIndex->pop();
	return allocator->Blocks[index];
}

PoolBlock PoolAlloc(PoolAllocator *allocator, uint32 index)
{
	crash(allocator->Blocks[index].IsUsed);
	crash(!allocator);
	crash(allocator->UsedBlockCount > allocator->BlockCount);
	allocator->UsedBlockCount++;
	allocator->Blocks[index].IsUsed = true;

	if (allocator->FirstUsed == allocator->BlockCount + 1)
	{
		allocator->FirstUsed = index;
		allocator->CurrentUsed = index;
	}
	else
	{
		allocator->Blocks[allocator->CurrentUsed].NextUsedIndex = index;
		allocator->Blocks[index].PreviousUsedIndex = allocator->CurrentUsed;
		allocator->CurrentUsed = index;
	}

	return allocator->Blocks[index];
}

void PoolDealloc(PoolAllocator *allocator, uint32 blockIndex)
{
	crash(!allocator);
	crash(blockIndex >= allocator->Dimensions.Size);
	crash(!allocator->Blocks[blockIndex].IsUsed);

	if (allocator->Blocks[blockIndex].PreviousUsedIndex == allocator->BlockCount + 1)
	{
		allocator->FirstUsed = allocator->BlockCount + 1;
		allocator->CurrentUsed = allocator->BlockCount + 1;
	}
	else
	{
		allocator->Blocks[allocator->Blocks[blockIndex].PreviousUsedIndex].NextUsedIndex = allocator->Blocks[blockIndex].NextUsedIndex;
		
		if (allocator->Blocks[blockIndex].NextUsedIndex != allocator->BlockCount + 1)
		{
			allocator->Blocks[allocator->Blocks[blockIndex].NextUsedIndex].PreviousUsedIndex = allocator->Blocks[blockIndex].PreviousUsedIndex;
		}

		allocator->CurrentUsed = allocator->Blocks[blockIndex].PreviousUsedIndex;
	}

	memset(allocator->Blocks[blockIndex].Dimensions.BaseAddress, NULL, allocator->Blocks[blockIndex].Dimensions.Size);
	allocator->Blocks[blockIndex].PreviousUsedIndex = allocator->BlockCount + 1;
	allocator->Blocks[blockIndex].NextUsedIndex = allocator->BlockCount + 1;
	allocator->Blocks[blockIndex].IsUsed = false;
	allocator->UsedBlockCount--;


	if (allocator->UseQueue)
	{
		allocator->UnusedIndex->push(blockIndex);
	}
}

AllocatorErrors InitPartialFrameSystem(FrameAllocator *sourceAllocator, FrameAllocator *destinationAllocator, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = ForwardAlign(size + sizeof(FrameAllocator), sourceAllocator->ByteAlignment);

	crash(allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]));

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);
	
	InitFrameSystem(&baseBookmark.Dimensions, destinationAllocator, sourceAllocator->ByteAlignment);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialFrameSystem(StackAllocator *sourceAllocator, FrameAllocator *destinationAllocator, uint32 size)
{
	uint32 allocSize = ForwardAlign(size + sizeof(FrameAllocator), sourceAllocator->ByteAlignment);

	crash(allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize));

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);

	InitFrameSystem(&baseBookmark.Dimensions, destinationAllocator, sourceAllocator->ByteAlignment);


	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialFrameSystem(PoolAllocator *sourceAllocator, FrameAllocator *destinationAllocator)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);
	uint32 allocSize = newPool.Dimensions.Size;

	InitFrameSystem(&newPool.Dimensions, destinationAllocator, sourceAllocator->ByteAlignment);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(PoolAllocator *sourceAllocator, PoolAllocator *destinationAllocator, uint32 destinationBlockSize, bool useQueue)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);

	uint32 allocSize = newPool.Dimensions.Size;
	uint32 newBlockSize = ForwardAlign(destinationBlockSize, sourceAllocator->ByteAlignment);
	uint32 blockCount = allocSize / newBlockSize;

	crash(blockCount * (newBlockSize + sizeof(PoolBlock)) != allocSize);
	crash(newBlockSize < sizeof(PoolBlock));
	crash(blockCount <= 1);
	newPool.Dimensions.Size = allocSize - ((blockCount * sizeof(PoolBlock)) + sizeof(PoolAllocator));
	InitPoolSystem(&newPool.Dimensions, destinationAllocator, newBlockSize, sourceAllocator->ByteAlignment, useQueue);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(FrameAllocator *sourceAllocator, PoolAllocator *destinationAllocator, HeapType sourceHeapType, uint32 size, uint32 blockSize, bool useQueue)
{
	uint32 newBlockSize = ForwardAlign(blockSize, sourceAllocator->ByteAlignment);
	uint32 blockCount = size / newBlockSize;

	uint32 allocSize = ForwardAlign((blockCount * (newBlockSize + sizeof(PoolBlock))) + sizeof(PoolAllocator), sourceAllocator->ByteAlignment);

	crash(blockCount <= 1);

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);
	baseBookmark.Dimensions.Size = allocSize - ((blockCount * sizeof(PoolBlock)) + sizeof(PoolAllocator));

	InitPoolSystem(&baseBookmark.Dimensions, destinationAllocator, newBlockSize, sourceAllocator->ByteAlignment, useQueue);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(StackAllocator *sourceAllocator, PoolAllocator *destinationAllocator, uint32 size, uint32 blockSize, bool useQueue)
{
	uint32 newBlockSize = ForwardAlign(blockSize, sourceAllocator->ByteAlignment);
	uint32 blockCount = size / newBlockSize;

	uint32 allocSize = ForwardAlign((blockCount * (newBlockSize + sizeof(PoolBlock))) + sizeof(PoolAllocator), sourceAllocator->ByteAlignment);

	crash(blockCount <= 1);
	crash(allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize));

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);
	baseBookmark.Dimensions.Size = allocSize - ((blockCount * sizeof(PoolBlock)) + sizeof(PoolAllocator));

	InitPoolSystem(&baseBookmark.Dimensions, destinationAllocator, newBlockSize, sourceAllocator->ByteAlignment, useQueue);


	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(FrameAllocator *sourceAllocator, StackAllocator *destinationAllocator, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = ForwardAlign(size + sizeof(StackAllocator), sourceAllocator->ByteAlignment);

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);

	InitStackSystem(&baseBookmark.Dimensions, destinationAllocator, sourceAllocator->ByteAlignment);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(StackAllocator *sourceAllocator, StackAllocator *destinationAllocator, uint32 size)
{
	uint32 allocSize = ForwardAlign(size + sizeof(StackAllocator), sourceAllocator->ByteAlignment);

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);
	
	InitStackSystem(&baseBookmark.Dimensions, destinationAllocator, sourceAllocator->ByteAlignment);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(PoolAllocator *sourceAllocator, StackAllocator *destinationAllocator)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);

	uint32 allocSize = newPool.Dimensions.Size;

	InitStackSystem(&newPool.Dimensions, destinationAllocator, sourceAllocator->ByteAlignment);

	return AllocatorErrors::NO_ERRORS;
}
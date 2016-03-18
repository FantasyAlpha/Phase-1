#include "Memory\MemoryAllocator.h"

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
	free(mainAlloc->MainMemory.BaseAddress);
	mainAlloc->MainMemory.BaseAddress = NULL;
}

void InitMainMemorySystem(MainAllocator *mainAlloc, AllocatorTypes allocType, uint32 size, int8 align, uint32 poolBlockSize)
{
	crash(size <= 0);

	if (allocType == POOL_ALLOCATOR)
	{
		mainAlloc->MainMemory.BaseAddress = malloc(size + (sizeof(PoolBlock) * (size / poolBlockSize)));
		mainAlloc->MainMemory.Size = size + (sizeof(PoolBlock) * (size / poolBlockSize));
	}
	else
	{
		mainAlloc->MainMemory.BaseAddress = malloc(size);
		mainAlloc->MainMemory.Size = size;
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
	AllocatorErrors error = SetMemorySystemDimensions(sourceDimensions->BaseAddress, &destinationAllocator->Dimensions, sourceDimensions->Size, align);

	crash(error != AllocatorErrors::NO_ERRORS);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	destinationAllocator->ByteAlignment = align;
	destinationAllocator->Dimensions.Size = sourceDimensions->Size;

	destinationAllocator->StackPointer[HeapType::LOWER_HEAP] = destinationAllocator->Dimensions.BaseAddress;
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
		allocator->StackPointer[heapType] = (void*)((uint8*)allocator->StackPointer[heapType] - frameSize);
		bookmark.Dimensions.BaseAddress = allocator->StackPointer[heapType];
		bookmark.Heap = heapType;
	}
	else
	{
		bookmark.Dimensions.BaseAddress = allocator->StackPointer[heapType];
		bookmark.Heap = heapType;
		allocator->StackPointer[heapType] = (void*)((uint8*)allocator->StackPointer[heapType] + frameSize);
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
	AllocatorErrors error = SetMemorySystemDimensions(sourceDimensions->BaseAddress, &destinationAllocator->Dimensions, sourceDimensions->Size, align);

	crash(error != AllocatorErrors::NO_ERRORS);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	destinationAllocator->StackPointer = sourceDimensions->BaseAddress;
	destinationAllocator->UsedSize = 0;
	destinationAllocator->ByteAlignment = align;
	destinationAllocator->Dimensions.Size = sourceDimensions->Size;

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

AllocatorErrors InitPoolSystem(BlockDimensions *sourceDimensions, PoolAllocator *destinationAllocator, uint32 blockSize, uint8 align)
{
	AllocatorErrors error = SetMemorySystemDimensions(sourceDimensions->BaseAddress, &destinationAllocator->Dimensions, sourceDimensions->Size, align);

	crash(error != AllocatorErrors::NO_ERRORS);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	destinationAllocator->ByteAlignment = align;
	destinationAllocator->Dimensions.Size = sourceDimensions->Size;

	uint32 alignedBlockSize = ForwardAlign(blockSize , align);
	uint32 blockCount = sourceDimensions->Size / blockSize;

	crash(blockCount * (blockSize + sizeof(PoolBlock)) != sourceDimensions->Size);
	crash(blockSize < sizeof(PoolBlock));
	crash(blockCount <= 1);
	
	destinationAllocator->BlockCount = blockCount;
	destinationAllocator->BlockSize = blockSize;

	destinationAllocator->Blocks = (PoolBlock*)destinationAllocator->Dimensions.BaseAddress;

	for (uint32 i = 0; i < blockCount - 1; i++)
	{
		void *base = (uint8*)destinationAllocator->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (blockSize * i);
		void *cap = (uint8*)base + blockSize;

		destinationAllocator->Blocks[i].Dimensions.BaseAddress = base;
		destinationAllocator->Blocks[i].Dimensions.CapAddress = cap;
		destinationAllocator->Blocks[i].Dimensions.Size = blockSize;
		destinationAllocator->Blocks[i].IsUsed = false;
		destinationAllocator->Blocks[i].BlockIndex = i;
		destinationAllocator->UnusedIndex.push(i);
	}

	destinationAllocator->UsedBlockCount = 0;

	return AllocatorErrors::NO_ERRORS;
}

PoolBlock PoolAlloc(PoolAllocator *allocator)
{
	crash(!allocator);
	crash(allocator->UsedBlockCount > allocator->BlockCount);
	allocator->UsedBlockCount++;
	uint32 index = allocator->UnusedIndex.front();
	allocator->Blocks[index].IsUsed = true;
	allocator->UnusedIndex.pop();
	return allocator->Blocks[index];
}

PoolBlock PoolAlloc(PoolAllocator *allocator, uint32 index)
{
	crash(allocator->Blocks[index].IsUsed);
	crash(!allocator);
	crash(allocator->UsedBlockCount > allocator->BlockCount);
	allocator->UsedBlockCount++;
	//uint32 index = allocator->UnusedIndex.front();
	allocator->Blocks[index].IsUsed = true;
	//allocator->UnusedIndex.pop();
	return allocator->Blocks[index];
}

void PoolDealloc(PoolAllocator *allocator, uint32 blockIndex, bool useQueue)
{
	crash(!allocator);
	crash(blockIndex >= allocator->Dimensions.Size);
	crash(!allocator->Blocks[blockIndex].IsUsed);

	memset(allocator->Blocks[blockIndex].Dimensions.BaseAddress, NULL, allocator->Blocks[blockIndex].Dimensions.Size);
	allocator->Blocks[blockIndex].IsUsed = false;
	allocator->UsedBlockCount--;
	if (useQueue)
	{
		allocator->UnusedIndex.push(blockIndex);
	}
}

AllocatorErrors InitPartialFrameSystem(FrameAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	crash(allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]));

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);
	
	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer[sourceHeapType];

	destinationAlloctor->StackPointer[HeapType::HIGHER_HEAP] = destinationAlloctor->Dimensions.CapAddress;
	destinationAlloctor->StackPointer[HeapType::LOWER_HEAP] = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialFrameSystem(StackAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	crash(allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize));

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer;

	destinationAlloctor->StackPointer[HeapType::HIGHER_HEAP] = destinationAlloctor->Dimensions.CapAddress;
	destinationAlloctor->StackPointer[HeapType::LOWER_HEAP] = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialFrameSystem(PoolAllocator *sourceAllocator, FrameAllocator *destinationAlloctor)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);
	uint32 allocSize = newPool.Dimensions.Size;

	destinationAlloctor->Dimensions.BaseAddress = newPool.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = newPool.Dimensions.CapAddress;

	destinationAlloctor->StackPointer[HeapType::HIGHER_HEAP] = destinationAlloctor->Dimensions.CapAddress;
	destinationAlloctor->StackPointer[HeapType::LOWER_HEAP] = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(PoolAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, uint32 destinationBlockSize)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);

	uint32 allocSize = newPool.Dimensions.Size;

	destinationAlloctor->Dimensions.BaseAddress = newPool.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = newPool.Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = newPool.Dimensions.Size;

	uint32 alignedBlockSize = ForwardAlign(destinationBlockSize, destinationAlloctor->ByteAlignment);
	uint32 blockCount = allocSize / destinationBlockSize;
	uint32 newBlockSize = allocSize / blockCount;
	crash(blockCount * (newBlockSize + sizeof(PoolBlock)) != allocSize);
	crash(newBlockSize < sizeof(PoolBlock));
	crash(blockCount <= 1);

	destinationAlloctor->BlockCount = blockCount;
	destinationAlloctor->BlockSize = newBlockSize;
	destinationAlloctor->Blocks = (PoolBlock*)destinationAlloctor->Dimensions.BaseAddress;

	for (uint32 i = 0; i < blockCount - 1; i++)
	{
		void *base = (uint8*)destinationAlloctor->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (newBlockSize * i);
		void *cap = (uint8*)base + newBlockSize;

		destinationAlloctor->Blocks[i].Dimensions.BaseAddress = base;
		destinationAlloctor->Blocks[i].Dimensions.CapAddress = cap;
		destinationAlloctor->Blocks[i].Dimensions.Size = newBlockSize;
		destinationAlloctor->Blocks[i].IsUsed = false;
		destinationAlloctor->Blocks[i].BlockIndex = i;
		destinationAlloctor->UnusedIndex.push(i);
	}

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(FrameAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size, uint32 blockSize)
{
	uint32 alignedBlockSize = ForwardAlign(blockSize, destinationAlloctor->ByteAlignment);
	uint32 blockCount = size / blockSize;
	uint32 newBlockSize = size / blockCount;

	uint32 allocSize = ForwardAlign(newBlockSize * blockCount, sourceAllocator->ByteAlignment);

	crash(blockCount <= 1);

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer[sourceHeapType];

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = allocSize;

	destinationAlloctor->BlockCount = blockCount;
	destinationAlloctor->BlockSize = newBlockSize;
	destinationAlloctor->Blocks = (PoolBlock*)destinationAlloctor->Dimensions.BaseAddress;

	for (uint32 i = 0; i < blockCount - 1; i++)
	{
		void *base = (uint8*)destinationAlloctor->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (newBlockSize * i);
		void *cap = (uint8*)base + newBlockSize;

		destinationAlloctor->Blocks[i].Dimensions.BaseAddress = base;
		destinationAlloctor->Blocks[i].Dimensions.CapAddress = cap;
		destinationAlloctor->Blocks[i].Dimensions.Size = newBlockSize;
		destinationAlloctor->Blocks[i].IsUsed = false;
		destinationAlloctor->Blocks[i].BlockIndex = i;
		destinationAlloctor->UnusedIndex.push(i);
	}

	destinationAlloctor->UsedBlockCount = 0;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(StackAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, uint32 size, uint32 blockSize)
{
	uint32 alignedBlockSize = ForwardAlign(blockSize, destinationAlloctor->ByteAlignment);
	uint32 blockCount = size / blockSize;
	uint32 newBlockSize = size / blockCount;

	uint32 allocSize = ForwardAlign(newBlockSize * blockCount, sourceAllocator->ByteAlignment);

	crash(blockCount <= 1);
	crash(allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize));

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer;
	destinationAlloctor->Dimensions.Size = allocSize;
	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;

	destinationAlloctor->BlockCount = blockCount;
	destinationAlloctor->BlockSize = newBlockSize;
	destinationAlloctor->Blocks = (PoolBlock*)destinationAlloctor->Dimensions.BaseAddress;

	for (uint32 i = 0; i < blockCount - 1; i++)
	{
		void *base = (uint8*)destinationAlloctor->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (newBlockSize * i);
		void *cap = (uint8*)base + newBlockSize;

		destinationAlloctor->Blocks[i].Dimensions.BaseAddress = base;
		destinationAlloctor->Blocks[i].Dimensions.CapAddress = cap;
		destinationAlloctor->Blocks[i].Dimensions.Size = newBlockSize;
		destinationAlloctor->Blocks[i].IsUsed = false;
		destinationAlloctor->Blocks[i].BlockIndex = i;
		destinationAlloctor->UnusedIndex.push(i);
	}

	destinationAlloctor->UsedBlockCount = 0;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(FrameAllocator *sourceAllocator, StackAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer[sourceHeapType];

	destinationAlloctor->StackPointer = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(StackAllocator *sourceAllocator, StackAllocator *destinationAlloctor, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	if (allocSize > sourceAllocator->Dimensions.Size - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);
	
	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer;

	destinationAlloctor->StackPointer = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(PoolAllocator *sourceAllocator, StackAllocator *destinationAlloctor)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);

	uint32 allocSize = newPool.Dimensions.Size;

	destinationAlloctor->Dimensions.BaseAddress = newPool.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = newPool.Dimensions.CapAddress;
	destinationAlloctor->StackPointer = destinationAlloctor->Dimensions.BaseAddress;
	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->Dimensions.Size = newPool.Dimensions.Size;

	return AllocatorErrors::NO_ERRORS;
}
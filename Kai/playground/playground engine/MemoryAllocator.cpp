#include "MemoryAllocator.h"

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
	free(mainAlloc->MainMemory.TotalMemory);
	mainAlloc->MainMemory.TotalMemory = NULL;
}

void StartMemorySystem(MainAllocator *mainAlloc, AllocatorTypes allocType, uint32 size, int8 align, uint32 poolBlockSize)
{
	crash(size <= 0);

	if (allocType == POOL_ALLOCATOR)
	{
		mainAlloc->MainMemory.TotalMemory = malloc(size + (sizeof(PoolBlock) * (size / poolBlockSize)));
		mainAlloc->MainMemory.Size = size + (sizeof(PoolBlock) * (size / poolBlockSize));
	}
	else
	{
		mainAlloc->MainMemory.TotalMemory = malloc(size);
		mainAlloc->MainMemory.Size = size;
	}

	mainAlloc->FrameSystem = NULL;
	mainAlloc->StackSystem = NULL;
	mainAlloc->PoolSystem = NULL;

	crash(!mainAlloc->MainMemory.TotalMemory);
	
	switch (allocType)
	{
		case FRAME_ALLOCATOR:
		{
			mainAlloc->MainSystem = AllocatorTypes::FRAME_ALLOCATOR;
			mainAlloc->FrameSystem = (FrameAllocator*)mainAlloc->MainMemory.TotalMemory;
			InitFrameSystem(mainAlloc, align);
		} break;
		case STACK_ALLOCATOR:
		{
			mainAlloc->MainSystem = AllocatorTypes::STACK_ALLOCATOR;
			mainAlloc->StackSystem = (StackAllocator*)mainAlloc->MainMemory.TotalMemory;
			InitStackSystem(mainAlloc, align);
		} break;
		case POOL_ALLOCATOR:
		{
			mainAlloc->MainSystem = AllocatorTypes::POOL_ALLOCATOR;
			mainAlloc->PoolSystem = (PoolAllocator*)mainAlloc->MainMemory.TotalMemory;
			if (poolBlockSize > 0)
			{
				InitPoolSystem(mainAlloc, poolBlockSize, align);
			}
		} break;
		default:
		{
			mainAlloc->MainSystem = AllocatorTypes::FRAME_ALLOCATOR;
			mainAlloc->FrameSystem = (FrameAllocator*)mainAlloc->MainMemory.TotalMemory;
			InitFrameSystem(mainAlloc, align);
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

AllocatorErrors InitFrameSystem(MainAllocator *mainAlloc, int8 align)
{
	AllocatorErrors error = SetMemorySystemDimensions(mainAlloc->MainMemory.TotalMemory, &mainAlloc->FrameSystem->Dimensions, mainAlloc->MainMemory.Size, align);

	crash(error != AllocatorErrors::NO_ERRORS);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	mainAlloc->FrameSystem->ByteAlignment = align;
	mainAlloc->FrameSystem->TotalSize = mainAlloc->MainMemory.Size;

	mainAlloc->FrameSystem->HeapFrame[HeapType::LOWER_HEAP] = mainAlloc->FrameSystem->Dimensions.BaseAddress;
	mainAlloc->FrameSystem->HeapFrame[HeapType::HIGHER_HEAP] = mainAlloc->FrameSystem->Dimensions.CapAddress;

	return AllocatorErrors::NO_ERRORS;
}

void ResetFrameAllocator(FrameAllocator *allocator)
{
	memset(allocator->Dimensions.BaseAddress, 0, allocator->TotalSize);

	allocator->Dimensions.BaseAddress = NULL;
	allocator->Dimensions.CapAddress = NULL;

	allocator->HeapFrame[HeapType::LOWER_HEAP] = NULL;
	allocator->HeapFrame[HeapType::HIGHER_HEAP] = NULL;

	allocator->ByteAlignment = 0;
}

FrameBookmark FrameAlloc(FrameAllocator *allocator, HeapType heapType, uint32 size)
{
	crash(!allocator);
	FrameBookmark bookmark = {};

	uint32 frameSize = ForwardAlign(size, allocator->ByteAlignment);

	crash(frameSize > allocator->TotalSize - (allocator->UsedSize[HeapType::HIGHER_HEAP] + allocator->UsedSize[HeapType::LOWER_HEAP]));


	if ((uint8*)allocator->HeapFrame[HeapType::LOWER_HEAP] + frameSize > (uint8*)allocator->HeapFrame[HeapType::HIGHER_HEAP])
	{
		return bookmark;
	}

	bookmark.PreviousSize += allocator->UsedSize[heapType];
	allocator->UsedSize[heapType] += frameSize;

	if (heapType == HeapType::HIGHER_HEAP)
	{
		allocator->HeapFrame[heapType] = (void*)((uint8*)allocator->HeapFrame[heapType] - frameSize);
		bookmark.BookMark = allocator->HeapFrame[heapType];
		bookmark.Heap = heapType;
	}
	else
	{
		bookmark.BookMark = allocator->HeapFrame[heapType];
		bookmark.Heap = heapType;
		allocator->HeapFrame[heapType] = (void*)((uint8*)allocator->HeapFrame[heapType] + frameSize);
	}

	return bookmark;
}

FrameBookmark GetFrameBookmark(FrameAllocator *allocator, HeapType heapType)
{
	FrameBookmark bookmark;

	bookmark.BookMark = allocator->HeapFrame[heapType];
	bookmark.Heap = heapType;

	return bookmark;
}

void FrameRelease(FrameAllocator *allocator, FrameBookmark *bookmark)
{
	crash(!allocator);

	allocator->HeapFrame[bookmark->Heap] = bookmark->BookMark;
	
	memset(allocator->HeapFrame[bookmark->Heap], 0, allocator->UsedSize[bookmark->Heap] - bookmark->PreviousSize);

	allocator->UsedSize[bookmark->Heap] = bookmark->PreviousSize;
}

AllocatorErrors InitStackSystem(MainAllocator *mainAlloc, uint8 align)
{
	AllocatorErrors error = SetMemorySystemDimensions(mainAlloc->MainMemory.TotalMemory, &mainAlloc->StackSystem->Dimensions, mainAlloc->MainMemory.Size, align);

	crash(error != AllocatorErrors::NO_ERRORS);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	mainAlloc->StackSystem->StackPointer = mainAlloc->StackSystem->Dimensions.BaseAddress;
	mainAlloc->StackSystem->UsedSize = 0;
	mainAlloc->StackSystem->ByteAlignment = align;
	mainAlloc->StackSystem->TotalSize = mainAlloc->MainMemory.Size;

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

	bookmark.Bookmark = allocator->StackPointer;
	allocator->StackPointer = (void*)((uint8*)allocator->StackPointer + requestedSize);

	return bookmark;
}

StackBookmark GetStackBookmark(StackAllocator *allocator)
{
	StackBookmark bookmark;

	bookmark.Bookmark = allocator->StackPointer;

	return bookmark;
}

void StackRelease(StackAllocator *allocator, StackBookmark *bookmark)
{
	crash(!allocator);

	allocator->StackPointer = bookmark;
	
	memset(allocator->StackPointer, 0, allocator->TotalSize - allocator->UsedSize);
	
	allocator->UsedSize = bookmark->PreviousSize;
}

AllocatorErrors InitPoolSystem(MainAllocator *mainAlloc, uint32 blockSize, uint8 align)
{
	AllocatorErrors error = SetMemorySystemDimensions(mainAlloc->MainMemory.TotalMemory, &mainAlloc->PoolSystem->Dimensions, mainAlloc->MainMemory.Size, align);

	crash(error != AllocatorErrors::NO_ERRORS);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	mainAlloc->PoolSystem->ByteAlignment = align;
	mainAlloc->PoolSystem->TotalSize = mainAlloc->MainMemory.Size;

	uint32 alignedBlockSize = ForwardAlign(blockSize , align);
	uint32 blockCount = mainAlloc->MainMemory.Size / blockSize;

	crash(blockCount * (blockSize + sizeof(PoolBlock)) != mainAlloc->MainMemory.Size);
	crash(blockSize < sizeof(PoolBlock));
	crash(blockCount <= 1);
	
	mainAlloc->PoolSystem->BlockCount = blockCount;
	mainAlloc->PoolSystem->BlockSize = blockSize;

	mainAlloc->PoolSystem->Blocks = (PoolBlock*)mainAlloc->PoolSystem->Dimensions.BaseAddress;

	for (uint32 i = 0; i < blockCount - 1; i++)
	{
		void *base = (uint8*)mainAlloc->PoolSystem->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (blockSize * i);
		void *cap = (uint8*)base + blockSize;

		mainAlloc->PoolSystem->Blocks[i].CurrentBlock = base;
		mainAlloc->PoolSystem->Blocks[i].Dimensions.BaseAddress = base;
		mainAlloc->PoolSystem->Blocks[i].Dimensions.CapAddress = cap;
		mainAlloc->PoolSystem->Blocks[i].BlockSize = blockSize;
		mainAlloc->PoolSystem->Blocks[i].IsUsed = false;
		mainAlloc->PoolSystem->Blocks[i].NextUnusedBlock = &mainAlloc->PoolSystem->Blocks[i + 1];
		mainAlloc->PoolSystem->Blocks[i].BlockIndex = i;
		mainAlloc->PoolSystem->UnusedIndex.push(i);
	}

	void *base = (uint8*)mainAlloc->PoolSystem->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (blockSize * (blockCount - 1));
	void *cap = (uint8*)base + blockSize;

	mainAlloc->PoolSystem->Blocks[blockCount - 1].CurrentBlock = base;
	mainAlloc->PoolSystem->Blocks[blockCount - 1].Dimensions.BaseAddress = base;
	mainAlloc->PoolSystem->Blocks[blockCount - 1].Dimensions.CapAddress = cap;
	mainAlloc->PoolSystem->Blocks[blockCount - 1].BlockSize = blockSize;
	mainAlloc->PoolSystem->Blocks[blockCount - 1].IsUsed = false;
	mainAlloc->PoolSystem->Blocks[blockCount - 1].NextUnusedBlock = NULL;
	mainAlloc->PoolSystem->Blocks[blockCount - 1].BlockIndex = blockCount - 1;
	mainAlloc->PoolSystem->UnusedIndex.push(blockCount - 1);

	mainAlloc->PoolSystem->UsedBlockCount = 0;

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

void PoolDealloc(PoolAllocator *allocator, uint32 blockIndex)
{
	crash(!allocator);
	crash(blockIndex >= allocator->TotalSize);
	crash(!allocator->Blocks[blockIndex].IsUsed);

	memset(allocator->Blocks[blockIndex].Dimensions.BaseAddress, NULL, allocator->Blocks[blockIndex].BlockSize);
	allocator->Blocks[blockIndex].IsUsed = false;
	allocator->UsedBlockCount--;
	allocator->UnusedIndex.push(blockIndex);
}

AllocatorErrors InitPartialFrameSystem(FrameAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	crash(allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]));

	if (allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);
	
	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.BookMark;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->HeapFrame[sourceHeapType];

	destinationAlloctor->HeapFrame[HeapType::HIGHER_HEAP] = destinationAlloctor->Dimensions.CapAddress;
	destinationAlloctor->HeapFrame[HeapType::LOWER_HEAP] = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialFrameSystem(StackAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	crash(allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize));

	if (allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Bookmark;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer;

	destinationAlloctor->HeapFrame[HeapType::HIGHER_HEAP] = destinationAlloctor->Dimensions.CapAddress;
	destinationAlloctor->HeapFrame[HeapType::LOWER_HEAP] = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialFrameSystem(PoolAllocator *sourceAllocator, FrameAllocator *destinationAlloctor)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);
	uint32 allocSize = newPool.BlockSize;

	destinationAlloctor->Dimensions.BaseAddress = newPool.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = newPool.Dimensions.CapAddress;

	destinationAlloctor->HeapFrame[HeapType::HIGHER_HEAP] = destinationAlloctor->Dimensions.CapAddress;
	destinationAlloctor->HeapFrame[HeapType::LOWER_HEAP] = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(PoolAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, uint32 destinationBlockSize)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);

	uint32 allocSize = newPool.BlockSize;

	destinationAlloctor->Dimensions.BaseAddress = newPool.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = newPool.Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = newPool.BlockSize;

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

		destinationAlloctor->Blocks[i].CurrentBlock = base;
		destinationAlloctor->Blocks[i].Dimensions.BaseAddress = base;
		destinationAlloctor->Blocks[i].Dimensions.CapAddress = cap;
		destinationAlloctor->Blocks[i].BlockSize = newBlockSize;
		destinationAlloctor->Blocks[i].IsUsed = false;
		destinationAlloctor->Blocks[i].NextUnusedBlock = &destinationAlloctor->Blocks[i + 1];
		destinationAlloctor->Blocks[i].BlockIndex = i;
		destinationAlloctor->UnusedIndex.push(i);
	}

	void *base = (uint8*)destinationAlloctor->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (newBlockSize * (blockCount - 1));
	void *cap = (uint8*)base + newBlockSize;

	destinationAlloctor->Blocks[blockCount - 1].CurrentBlock = base;
	destinationAlloctor->Blocks[blockCount - 1].Dimensions.BaseAddress = base;
	destinationAlloctor->Blocks[blockCount - 1].Dimensions.CapAddress = cap;
	destinationAlloctor->Blocks[blockCount - 1].BlockSize = newBlockSize;
	destinationAlloctor->Blocks[blockCount - 1].IsUsed = false;
	destinationAlloctor->Blocks[blockCount - 1].NextUnusedBlock = NULL;
	destinationAlloctor->Blocks[blockCount - 1].BlockIndex = blockCount - 1;
	destinationAlloctor->UnusedIndex.push(blockCount - 1);

	destinationAlloctor->UsedBlockCount = 0;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(FrameAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size, uint32 blockSize)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	if (allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.BookMark;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->HeapFrame[sourceHeapType];

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = allocSize;

	uint32 alignedBlockSize = ForwardAlign(blockSize, destinationAlloctor->ByteAlignment);
	uint32 blockCount = allocSize / blockSize;
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

		destinationAlloctor->Blocks[i].CurrentBlock = base;
		destinationAlloctor->Blocks[i].Dimensions.BaseAddress = base;
		destinationAlloctor->Blocks[i].Dimensions.CapAddress = cap;
		destinationAlloctor->Blocks[i].BlockSize = newBlockSize;
		destinationAlloctor->Blocks[i].IsUsed = false;
		destinationAlloctor->Blocks[i].NextUnusedBlock = &destinationAlloctor->Blocks[i + 1];
		destinationAlloctor->Blocks[i].BlockIndex = i;
		destinationAlloctor->UnusedIndex.push(i);
	}

	void *base = (uint8*)destinationAlloctor->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (newBlockSize * (blockCount - 1));
	void *cap = (uint8*)base + newBlockSize;

	destinationAlloctor->Blocks[blockCount - 1].CurrentBlock = base;
	destinationAlloctor->Blocks[blockCount - 1].Dimensions.BaseAddress = base;
	destinationAlloctor->Blocks[blockCount - 1].Dimensions.CapAddress = cap;
	destinationAlloctor->Blocks[blockCount - 1].BlockSize = newBlockSize;
	destinationAlloctor->Blocks[blockCount - 1].IsUsed = false;
	destinationAlloctor->Blocks[blockCount - 1].NextUnusedBlock = NULL;
	destinationAlloctor->Blocks[blockCount - 1].BlockIndex = blockCount - 1;
	destinationAlloctor->UnusedIndex.push(blockCount - 1);

	destinationAlloctor->UsedBlockCount = 0;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialPoolSystem(StackAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, uint32 size, uint32 blockSize)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	crash(allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize));

	if (allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Bookmark;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer;
	destinationAlloctor->TotalSize = allocSize;
	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;

	uint32 alignedBlockSize = ForwardAlign(blockSize, destinationAlloctor->ByteAlignment);
	uint32 blockCount = allocSize / blockSize;
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

		destinationAlloctor->Blocks[i].CurrentBlock = base;
		destinationAlloctor->Blocks[i].Dimensions.BaseAddress = base;
		destinationAlloctor->Blocks[i].Dimensions.CapAddress = cap;
		destinationAlloctor->Blocks[i].BlockSize = newBlockSize;
		destinationAlloctor->Blocks[i].IsUsed = false;
		destinationAlloctor->Blocks[i].NextUnusedBlock = &destinationAlloctor->Blocks[i + 1];
		destinationAlloctor->Blocks[i].BlockIndex = i;
		destinationAlloctor->UnusedIndex.push(i);
	}

	void *base = (uint8*)destinationAlloctor->Dimensions.BaseAddress + (blockCount * sizeof(PoolBlock)) + (newBlockSize * (blockCount - 1));
	void *cap = (uint8*)base + newBlockSize;

	destinationAlloctor->Blocks[blockCount - 1].CurrentBlock = base;
	destinationAlloctor->Blocks[blockCount - 1].Dimensions.BaseAddress = base;
	destinationAlloctor->Blocks[blockCount - 1].Dimensions.CapAddress = cap;
	destinationAlloctor->Blocks[blockCount - 1].BlockSize = newBlockSize;
	destinationAlloctor->Blocks[blockCount - 1].IsUsed = false;
	destinationAlloctor->Blocks[blockCount - 1].NextUnusedBlock = NULL;
	destinationAlloctor->Blocks[blockCount - 1].BlockIndex = blockCount - 1;
	destinationAlloctor->UnusedIndex.push(blockCount - 1);

	destinationAlloctor->UsedBlockCount = 0;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(FrameAllocator *sourceAllocator, StackAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	if (allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize[HeapType::LOWER_HEAP] + sourceAllocator->UsedSize[HeapType::HIGHER_HEAP]))
	{
		return INVALID_SIZE;
	}

	FrameBookmark baseBookmark = FrameAlloc(sourceAllocator, sourceHeapType, allocSize);

	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.BookMark;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->HeapFrame[sourceHeapType];

	destinationAlloctor->StackPointer = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(StackAllocator *sourceAllocator, StackAllocator *destinationAlloctor, uint32 size)
{
	uint32 allocSize = ForwardAlign(size, sourceAllocator->ByteAlignment);

	if (allocSize > sourceAllocator->TotalSize - (sourceAllocator->UsedSize))
	{
		return INVALID_SIZE;
	}

	StackBookmark baseBookmark = StackAlloc(sourceAllocator, allocSize);
	
	destinationAlloctor->Dimensions.BaseAddress = baseBookmark.Bookmark;
	destinationAlloctor->Dimensions.CapAddress = sourceAllocator->StackPointer;

	destinationAlloctor->StackPointer = destinationAlloctor->Dimensions.BaseAddress;

	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = allocSize;

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitPartialStackSystem(PoolAllocator *sourceAllocator, StackAllocator *destinationAlloctor)
{
	PoolBlock newPool = PoolAlloc(sourceAllocator);

	uint32 allocSize = newPool.BlockSize;

	destinationAlloctor->Dimensions.BaseAddress = newPool.Dimensions.BaseAddress;
	destinationAlloctor->Dimensions.CapAddress = newPool.Dimensions.CapAddress;
	destinationAlloctor->StackPointer = destinationAlloctor->Dimensions.BaseAddress;
	destinationAlloctor->ByteAlignment = sourceAllocator->ByteAlignment;
	destinationAlloctor->TotalSize = newPool.BlockSize;

	return AllocatorErrors::NO_ERRORS;
}
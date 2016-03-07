#include "MemoryAllocator.h"

namespace Pointer_Math
{
	void* ForwardAlign(void *address, uint8 align)
	{
		return (void*)(((uint32)address + align - 1) & (~(align - 1)));
	}

	uint32 ForwardAlign(uint32 size, uint8 align)
	{
		return ((size + align - 1) & (~(align - 1)));
	}
}

AllocatorErrors InitGameMemory(Game_Memory *memory, uint32 size)
{
	if (size <= 0)
	{
		return AllocatorErrors::INVALID_SIZE;
	}

	memory->TotalMemory = malloc(size);
	memory->Size = size;

	if (!memory->TotalMemory)
	{
		return AllocatorErrors::INVALID_MEMORY;
	}

	return AllocatorErrors::NO_ERRORS;
}

void ShutDownGameMemory(Game_Memory *memory)
{
	free(memory->TotalMemory);
	memory->TotalMemory = NULL;
}

void SetMainAllocatorType(Game_Memory memory, MainAllocator *mainAllocator, AllocatorTypes allocType)
{
	mainAllocator->MainMemory = memory;

	if (AllocatorTypes::FRAME_ALLOCATOR)
	{
		mainAllocator->MainSystem = AllocatorTypes::FRAME_ALLOCATOR;
	}
	else
	{
		mainAllocator->MainSystem = AllocatorTypes::FRAME_ALLOCATOR;
	}
}

void InitMemorySystem(MainAllocator *mainAlloc, uint32 size, int8 align)
{
	switch (mainAlloc->MainSystem)
	{
	case FRAME_ALLOCATOR:
	{
		InitFrameSystem(mainAlloc, size, align);
	} break;
	case STACK_ALLOCATOR:
	{
		InitStackSystem(mainAlloc, size, align);
	} break;
	default:
	{
		InitFrameSystem(mainAlloc, size, align);
	} break;
	}
}

AllocatorErrors SetMemorySystemDimensions(MainAllocator *mainAlloc, BlockDimensions *dimensions, int8 align)
{
	if (!mainAlloc->MainMemory.TotalMemory)
	{
		return AllocatorErrors::INVALID_MEMORY;
	}

	dimensions->BaseAddress = Pointer_Math::ForwardAlign(mainAlloc->MainMemory.TotalMemory, align);
	
	dimensions->CapAddress = Pointer_Math::ForwardAlign((void *)((uint8*)mainAlloc->MainMemory.TotalMemory + mainAlloc->MainMemory.Size), align);

	return AllocatorErrors::NO_ERRORS;
}

AllocatorErrors InitFrameSystem(MainAllocator *mainAlloc, uint32 size, int8 align)
{
	AllocatorErrors error = SetMemorySystemDimensions(mainAlloc, &mainAlloc->FrameSystem.Dimensions, align);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	mainAlloc->FrameSystem.ByteAlignment = align;
	mainAlloc->FrameSystem.TotalSize = size;

	mainAlloc->FrameSystem.HeapFrame[HeapType::LOWER_HEAP] = mainAlloc->FrameSystem.Dimensions.BaseAddress;
	mainAlloc->FrameSystem.HeapFrame[HeapType::HIGHER_HEAP] = mainAlloc->FrameSystem.Dimensions.CapAddress;

	return AllocatorErrors::NO_ERRORS;
}

void ShutDownFrameAllocator(FrameAllocator *allocator)
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
	FrameBookmark bookmark = {};

	uint32 frameSize = Pointer_Math::ForwardAlign(size, allocator->ByteAlignment);

	bookmark.PreviousSize += allocator->UsedSize[heapType];
	allocator->UsedSize[heapType] += frameSize;

	if ((uint8*)allocator->HeapFrame[HeapType::LOWER_HEAP] + frameSize > (uint8*)allocator->HeapFrame[HeapType::HIGHER_HEAP])
	{
		allocator->UsedSize[heapType] -= frameSize;
		return bookmark;
	}

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
	allocator->HeapFrame[bookmark->Heap] = bookmark->BookMark;
	
	memset(allocator->HeapFrame[bookmark->Heap], 0, allocator->UsedSize[bookmark->Heap] - bookmark->PreviousSize);

	allocator->UsedSize[bookmark->Heap] = bookmark->PreviousSize;
}

AllocatorErrors InitStackSystem(MainAllocator *mainAlloc, uint32 size, uint8 align)
{
	AllocatorErrors error = SetMemorySystemDimensions(mainAlloc, &mainAlloc->StackSystem.Dimensions, align);

	if (error != AllocatorErrors::NO_ERRORS)
	{
		return error;
	}

	mainAlloc->StackSystem.ByteAlignment = align;
	mainAlloc->StackSystem.TotalSize = size;

	return AllocatorErrors::NO_ERRORS;
}

StackBookmark StackAlloc(StackAllocator *allocator, uint32 size)
{
	StackBookmark bookmark = {};

	uint32 requestedSize = Pointer_Math::ForwardAlign(size, allocator->ByteAlignment);
	bookmark.PreviousSize += allocator->UsedSize;
	allocator->UsedSize += requestedSize;

	if ((uint8*)allocator->StackPointer + requestedSize > (uint8*)allocator->Dimensions.CapAddress)
	{
		allocator->UsedSize -= requestedSize;
		return bookmark;
	}

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
	allocator->StackPointer = bookmark;
	
	memset(allocator->StackPointer, 0, allocator->TotalSize - allocator->UsedSize);
	
	allocator->UsedSize = bookmark->PreviousSize;
}

AllocatorErrors InitPartialFrameSystem(FrameAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = Pointer_Math::ForwardAlign(size, sourceAllocator->ByteAlignment);

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
	uint32 allocSize = Pointer_Math::ForwardAlign(size, sourceAllocator->ByteAlignment);

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

AllocatorErrors InitPartialStackSystem(FrameAllocator *sourceAllocator, StackAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size)
{
	uint32 allocSize = Pointer_Math::ForwardAlign(size, sourceAllocator->ByteAlignment);

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
	uint32 allocSize = Pointer_Math::ForwardAlign(size, sourceAllocator->ByteAlignment);

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
#pragma once

#include <cstring>
#include <cstdlib>
#include <queue>
#include "Types.h"

enum AllocatorErrors
{
	NO_ERRORS = 0,
	NOT_ENOUGH_MEMORY = 1,
	INVALID_SIZE = 2,
	INVALID_MEMORY = 3,
	INVALID_HEAP = 4
};

struct Game_Memory
{
	void *TotalMemory;
	uint32 Size;
};

enum HeapType
{
	LOWER_HEAP = 0,
	HIGHER_HEAP = 1
};

struct BlockDimensions
{
	void *BaseAddress;
	void *CapAddress;
};

struct FrameAllocator
{
	BlockDimensions Dimensions;

	void *HeapFrame[2];

	uint32 TotalSize;
	uint32 UsedSize[2];
	uint8 ByteAlignment;
};

struct FrameBookmark
{
	void *BookMark;
	HeapType Heap;
	uint32 PreviousSize;
};

struct PoolBlock
{
	BlockDimensions Dimensions;
	void *CurrentBlock;
	uint32 BlockSize;
	uint32 BlockIndex;

	PoolBlock *NextUnusedBlock;

	bool IsUsed;
};

struct PoolAllocator
{
	BlockDimensions Dimensions;

	PoolBlock *Blocks;
	
	std::queue<uint32> UnusedIndex;
	
	uint32 TotalSize;
	uint32 BlockSize;
	uint32 BlockCount; 
	uint32 UsedBlockCount;
	uint8 ByteAlignment;
};

struct StackBookmark
{
	void *Bookmark;
	uint32 PreviousSize;
};

struct StackAllocator
{
	BlockDimensions Dimensions;

	void *StackPointer;

	uint32 TotalSize;
	uint32 UsedSize;
	uint8 ByteAlignment;
};

enum AllocatorTypes
{
	FRAME_ALLOCATOR = 0,
	STACK_ALLOCATOR = 1,
	POOL_ALLOCATOR = 2
};

struct MainAllocator
{
	Game_Memory MainMemory;
	FrameAllocator *FrameSystem;
	StackAllocator *StackSystem;
	PoolAllocator *PoolSystem;

	AllocatorTypes MainSystem;
};

void* ForwardAlign(void *address, uint8 align);

uint32 ForwardAlign(uint32 size, uint8 align);

void ShutDownMemorySystem(MainAllocator *mainAlloc);

void StartMemorySystem(MainAllocator *mainAlloc, AllocatorTypes allocType, uint32 size, int8 align, uint32 poolBlockSize = 0);

AllocatorErrors SetMemorySystemDimensions(void *memory, BlockDimensions *dimensions, uint32 size, int8 align);

AllocatorErrors InitFrameSystem(MainAllocator *mainAlloc, int8 align);

void ResetFrameAllocator(FrameAllocator *allocator);

FrameBookmark FrameAlloc(FrameAllocator *allocator, HeapType heapType, uint32 size);

FrameBookmark GetFrameBookmark(FrameAllocator *allocator, HeapType heapType);

void FrameRelease(FrameAllocator *allocator, FrameBookmark *bookmark);

AllocatorErrors InitStackSystem(MainAllocator *mainAlloc, uint8 align);

StackBookmark StackAlloc(StackAllocator *allocator, uint32 size);

StackBookmark GetStackBookmark(StackAllocator *allocator);

void StackRelease(StackAllocator *allocator, StackBookmark *bookmark);

AllocatorErrors InitPoolSystem(MainAllocator *mainAlloc, uint32 blockSize, uint8 align);

PoolBlock PoolAlloc(PoolAllocator *allocator);

void PoolDealloc(PoolAllocator *allocator, uint32 blockIndex);

AllocatorErrors InitPartialFrameSystem(FrameAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size);

AllocatorErrors InitPartialFrameSystem(StackAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, uint32 size);

AllocatorErrors InitPartialStackSystem(FrameAllocator *sourceAllocator, StackAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size);

AllocatorErrors InitPartialStackSystem(StackAllocator *sourceAllocator, StackAllocator *destinationAlloctor, uint32 size);

AllocatorErrors InitPartialStackSystem(PoolAllocator *sourceAllocator, StackAllocator *destinationAlloctor);

AllocatorErrors InitPartialFrameSystem(PoolAllocator *sourceAllocator, FrameAllocator *destinationAlloctor);

AllocatorErrors InitPartialPoolSystem(StackAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, uint32 size, uint32 blockSize);

AllocatorErrors InitPartialPoolSystem(FrameAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size, uint32 blockSize);

AllocatorErrors InitPartialPoolSystem(PoolAllocator *sourceAllocator, PoolAllocator *destinationAlloctor, uint32 destinationBlockSize);
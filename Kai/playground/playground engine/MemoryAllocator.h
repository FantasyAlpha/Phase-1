#pragma once

#include <stdint.h>
#include <cstring>
#include <cstdlib>

#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

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

//struct Pool
//{
//	void *CurrentAddress;
//	void *NextAddress;
//	void *PreviousAddress;
//
//	uint32 BlockSize;
//
//	bool IsUsed;
//};
//
//struct PoolAllocator
//{
//	BlockDimensions Dimensions;
//
//	Pool *Pools;
//
//	uint32 TotalSize;
//	uint32 BlockSize;
//	uint32 BlockCount; 
//	uint8 ByteAlignment;
//};

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
	//POOL_ALLOCATOR = 2
};

struct MainAllocator
{
	Game_Memory MainMemory;
	FrameAllocator FrameSystem;
	StackAllocator StackSystem;
	//PoolAllocator PoolSystem;

	AllocatorTypes MainSystem;
};

namespace Pointer_Math
{
	void* ForwardAlign(void *address, uint8 align);

	uint32 ForwardAlign(uint32 size, uint8 align);
}

AllocatorErrors InitGameMemory(Game_Memory *memory, uint32 size);

void ShutDownGameMemory(Game_Memory *memory);

void SetMainAllocatorType(Game_Memory memory, MainAllocator *mainAllocator, AllocatorTypes allocType);

void InitMemorySystem(MainAllocator *mainAlloc, uint32 size, int8 align);

AllocatorErrors SetMemorySystemDimensions(MainAllocator *mainAlloc, BlockDimensions *dimensions, int8 align);

AllocatorErrors InitFrameSystem(MainAllocator *mainAlloc, uint32 size, int8 align);

void ShutDownFrameAllocator(FrameAllocator *allocator);

FrameBookmark FrameAlloc(FrameAllocator *allocator, HeapType heapType, uint32 size);

FrameBookmark GetFrameBookmark(FrameAllocator *allocator, HeapType heapType);

void FrameRelease(FrameAllocator *allocator, FrameBookmark *bookmark);

AllocatorErrors InitStackSystem(MainAllocator *mainAlloc, uint32 size, uint8 align);

StackBookmark StackAlloc(StackAllocator *allocator, uint32 size);

StackBookmark GetStackBookmark(StackAllocator *allocator);

void StackRelease(StackAllocator *allocator, StackBookmark *bookmark);

AllocatorErrors InitPartialFrameSystem(FrameAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size);

AllocatorErrors InitPartialFrameSystem(StackAllocator *sourceAllocator, FrameAllocator *destinationAlloctor, uint32 size);

AllocatorErrors InitPartialStackSystem(FrameAllocator *sourceAllocator, StackAllocator *destinationAlloctor, HeapType sourceHeapType, uint32 size);

AllocatorErrors InitPartialStackSystem(StackAllocator *sourceAllocator, StackAllocator *destinationAlloctor, uint32 size);
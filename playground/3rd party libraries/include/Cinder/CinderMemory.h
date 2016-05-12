#ifndef CINDER_MEMORY_H
#define CINDER_MEMORY_H

#include <stdlib.h>
#include <string.h>

namespace Cinder
{
	namespace Memory
	{
		/*
		****************************************************************************************************************************************************
		Courtesy of David Graham and Mike McShaffry
		*****************************************************************************************************************************************************
		*/

		struct PoolHeader
		{
			unsigned int Current;
			unsigned int Next;
			unsigned int Previous;
		};

		struct MemoryPool
		{
			MemoryPool(){}
			MemoryPool(unsigned int chunkSize, unsigned int chunkCount, bool allowResize);

			~MemoryPool();

			PoolHeader *UsedHeaders;
			PoolHeader *UnusedHeaders;

			void *Elements;

			unsigned int ChunkSize;
			unsigned int ChunkCount;
			unsigned int GrowUsedTimes;

			unsigned int FirstUsed;
			unsigned int CurrentUsed;
			unsigned int FirstUnused;

			bool AllowResize;

			bool Init();

			void Destroy();

			void Reset();

			bool GrowMemory();

			void LinkChunks(unsigned int start);

			PoolHeader Alloc();

			void Dealloc(unsigned int index);

		private:
			MemoryPool(const MemoryPool &memPool){}
		};

		/*
		****************************************************************************************************************************************************
		*****************************************************************************************************************************************************
		*/

		struct StackChunk
		{
			void *Chunk;
			unsigned int size;
			void *Bookmark;
		};

		struct MemoryStack
		{
			MemoryStack();

			~MemoryStack();

			unsigned int TotalSize;
			unsigned int UsedSize;
			void *RawMemory;
			void *StackPointer;
			void *Front;
			void *End;
			bool AllowResize;

			bool Init(unsigned int totalSize, bool allowResize);

			void Destroy();

			void Reset();

			bool GrowMemory();

			StackChunk Alloc(unsigned int size);

			void Dealloc(StackChunk *chunk);

		private:
			MemoryStack(const MemoryStack &memStack){}
		};

	}
}

#endif

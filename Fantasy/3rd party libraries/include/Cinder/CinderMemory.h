#ifndef CINDER_MEMORY_H
#define CINDER_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include <set>

namespace Cinder
{
	namespace Memory
	{
		struct StackChunk
		{
			unsigned char *Chunk;
			unsigned int size;
			unsigned char *Bookmark;

		};

		struct MemoryStack
		{
			MemoryStack(unsigned int totalSize, bool allowResize);

			MemoryStack();
			~MemoryStack();

			unsigned int TotalSize;
			unsigned int UsedSize;
			unsigned char *RawMemory;
			unsigned char *StackPointer;
			unsigned char *Front;
			unsigned char *End;
			unsigned char *LastUsed;
			bool AllowResize;

			bool Init(unsigned int totalSize, bool allowResize);

			void Destroy();

			void Reset();

			bool GrowMemory(unsigned int size);

			StackChunk Alloc(unsigned int size);

			void Dealloc(unsigned char *bookmark, unsigned int size);
			void Pop(unsigned int size);
			StackChunk PushBack(unsigned int size);
			void PopBack();
			unsigned char* GetTop();
			unsigned char* GetBottom();
		private:
			MemoryStack(const MemoryStack &memStack){}
		};

		/*
		****************************************************************************************************************************************************
		*****************************************************************************************************************************************************
		*/

		struct ChunkNode
		{
			union 
			{
				unsigned int Current;
				unsigned int Middle;
			};

			union
			{
				unsigned int Next;
				unsigned int Last;
			};

			union
			{
				unsigned int Previous;
				unsigned int First;
			};
		};

		struct DoubleLinkedPool
		{
		public:
			DoubleLinkedPool(){}
			DoubleLinkedPool(unsigned int chunkSize, unsigned int chunkCount = 5, bool allowResize = false);

			~DoubleLinkedPool();
		public:

			void **MemPools;
			ChunkNode **UsedChunks;
			ChunkNode *PoolNodes;

			std::set<unsigned int> UsedPools;
			std::set<unsigned int> UnusedChunks;

			unsigned int ChunkSize;
			unsigned int ChunkCount;
			unsigned int TotalChunkCount;
			unsigned int PoolCount;
			unsigned int UsedCount;
			unsigned int GrowUsedTimes;
			unsigned int TotalSize;
			unsigned int MAX = 4294967295;
			unsigned int FixedChunkCount;
			unsigned int FirstUsed;

			bool AllowResize;

		public:

			bool Init();

			void Destroy();			

			unsigned int Alloc();

			void Dealloc(unsigned int value);

			unsigned int GetIndex(unsigned int value);
			unsigned int GetPool(unsigned int value);

			void* GetAddress(unsigned int value);

			void* GetNextAddress(unsigned int value);
			void* GetPrevAddress(unsigned int value);

			unsigned int GetNextIndex(unsigned int value);
			unsigned int GetPrevIndex(unsigned int value);

			bool IsEmpty();

		private:
			void Reset();

			bool GrowMemory();

			void AddChunks(unsigned int poolIndex);

			unsigned int GetBefore(unsigned int value);
			unsigned int GetAfter(unsigned int value);
			DoubleLinkedPool(const DoubleLinkedPool &memPool){}
		};

		struct MemoryPool
		{
			MemoryPool();

			MemoryPool(size_t size, size_t count, bool resize);

			~MemoryPool();
						
			bool Init();
			
			void Destroy();

			void Reset();

			unsigned char* GetNext(unsigned char *address);

			void* GetChunk(void* address);
			void* GetNextUsed(void* address);
			
			void SetNext(unsigned char *current, unsigned char *next);

			bool GrowMemory();

			unsigned char* AllocatePool();
			
			void* Alloc();

			void Free(void *address);

			unsigned char **RawPools;
			unsigned char *Head;
			unsigned char *First;
			unsigned int ChunkSize;
			unsigned int ChunkCount;
			unsigned int PoolCount;
			size_t UsedCount;
			bool AllowResize;

		private:
			MemoryPool(const MemoryPool &memPool){}
		};
	}
}

#endif

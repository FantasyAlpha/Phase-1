#include "..\Utilities.h"

//Return the contents of a text file
void LoadFile(char *path, DataFile *dataFile)
{	
	HANDLE fileHandle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fileSize;

		if (GetFileSizeEx(fileHandle, &fileSize))
		{
			unsigned int fileSize_32bit = (unsigned int)fileSize.QuadPart;
			dataFile->Data = VirtualAlloc(0, fileSize_32bit, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			
			if (dataFile->Data)
			{
				DWORD bytesRead;
				if (ReadFile(fileHandle, dataFile->Data, fileSize_32bit, &bytesRead, NULL) && fileSize_32bit == bytesRead)
				{
					dataFile->Length = fileSize_32bit;
				}
				else
				{
					//Failed to read file
				}
			}
			else
			{
				//Failed to allocate memory
			}
		}
		else
		{
			//Failed to get file size
		}
	}
	else
	{
		//Failed to create file handle
	}
}

void UnloadFile(DataFile *dataFile)
{
	if (dataFile->Data)
	{
		VirtualFree(dataFile->Data, 0, MEM_RELEASE);
	}
}
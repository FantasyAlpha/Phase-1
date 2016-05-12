#pragma once

#include <vector>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <GL\glew.h>

struct DataFile
{
	void *Data;
	unsigned long Length;
};

//Return the contents of a file
void LoadFile(char *path, DataFile *data);
void UnloadFile(DataFile *dataFile);

int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);

bool ReloadGlew();
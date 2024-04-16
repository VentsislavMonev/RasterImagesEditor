#pragma once
#include <iostream>
#include <vector>

enum class ImageType
{
	P1,
	P2,
	P3,
	defaultType
};

class Image
{
public:
private:
	//std::vector<std::vector<short>> pixels;
	ImageType format;
	unsigned short width;
	unsigned short lenght;
	
};


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "RGB.h"

int main()
{
	std::ifstream firstFile;
	std::ofstream newFile;
	firstFile.open("AllMyMoneySPent.ppm");
	newFile.open("newFile.ppm");

	std::string type, width, length, maxValue;

	std::string r, g, b;

	int rr, gg, bb;

	firstFile >> type;
	firstFile >> width;
	firstFile >> length;
	firstFile >> maxValue;

	newFile << type << std::endl;
	newFile << width << std::endl;
	newFile << length << std::endl;
	newFile << maxValue << std::endl;
	while (!firstFile.eof())
	{
		firstFile >> r;
		firstFile >> g;
		firstFile >> b;

		std::stringstream red(r);
		std::stringstream green(r);
		std::stringstream blue(r);
		red >> rr;
		green >> gg;
		blue >> bb;

		if (rr + 50 >= 255)
			rr = 255;
		else rr += 50;

		newFile << rr << " " << gg << " " << bb << std::endl;
	}
	for (size_t y = 0; y < 250; y++)
	{
		for (size_t x = 0; x < 250; x++)
		{
			//firstFile << x << " " << x << " " << x << std::endl;
		}
	}
	firstFile.close();

	RGB pixel(20, 200, 20);
	RGB pixel1(pixel);
	RGB pixel2;
	pixel2 = pixel1;


	std::cout << pixel1.getSum();
}
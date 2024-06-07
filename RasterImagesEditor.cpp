#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "RGB.h"
#include "Session.h"
#include "PPM.h"
#include "PGM.h"
#include "PBM.h"

void applyBlueFilter(std::vector<RGB>& pixels) {
	for (auto& pixel : pixels) {
		pixel.setRed(0); // Set red component to 0
		pixel.setGreen(0); // Set green component to 0
		// Leave blue component unchanged
	}
}

void copyPPM(const std::string& inputFilename, const std::string& outputFilename)
{
	std::ifstream inputFile(inputFilename, std::ios::binary);
	if (!inputFile) {
		std::cerr << "Error: Unable to open input file " << inputFilename << std::endl;
		return;
	}

	std::ofstream outputFile(outputFilename, std::ios::binary);
	if (!outputFile) {
		std::cerr << "Error: Unable to open output file " << outputFilename << std::endl;
		return;
	}

	std::string magic;
	int width, height, maxColorValue;
	inputFile >> magic >> width >> height >> maxColorValue;

	outputFile << magic << std::endl;
	outputFile << width << " " << height << std::endl;
	outputFile << maxColorValue << std::endl;
	std::vector<RGB> pixels(width * height);

	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			RGB da;
			inputFile.read((char*)&pixels[i + j], sizeof(RGB));

			/*unsigned char value = pixels[i + j].r() * 0.2126 + pixels[i + j].g() * 0.7152 + pixels[i + j].b() * 0.0722;
			pixels[i + j].setRed(value);
			pixels[i + j].setGreen(value);
			pixels[i + j].setBlue(value);*/
			unsigned char average = pixels[i + j].getAverage();
			pixels[i + j].setRed(pixels[i + j].r()/255);
			pixels[i + j].setGreen(pixels[i + j].g()/255);
			pixels[i + j].setBlue(pixels[i + j].b()/255);

			if (pixels[i + j].r() < 127)
				pixels[i + j].setEqual(0);
			else
				pixels[i + j].setEqual(255);

			outputFile.write((const char*)&pixels[i + j], sizeof(RGB));
		}
	}
	inputFile.close();
	outputFile.close();
}

int main()
{
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//std::string inputFilename = "sample_640×426.ppm";
	//std::string outputFilename = "newFile.ppm";
	

	//copyPPM(inputFilename, outputFilename);



	//std::ifstream firstFile("sample_640×426.ppm", std::ios::binary);
	//std::ofstream newFile("newFile.ppm", std::ios::binary);
	//std::string type, width, length, maxValue;
	//unsigned char rr, gg, bb;
	//firstFile >> type;
	//firstFile >> width;
	//firstFile >> length;
	//firstFile >> maxValue;

	//newFile << type << std::endl;
	//newFile << width<< " ";
	//newFile << length << std::endl;
	//newFile << maxValue;
	//for (size_t i = 0; i < 426; i++)
	//{
	//	for (size_t j = 0; j < 640; j++)
	//	{
	//		firstFile.read((char*)&rr, sizeof(rr));
	//		firstFile.read((char*)&gg, sizeof(gg));
	//		firstFile.read((char*)&bb, sizeof(bb));

	//		if (((int)bb - 50) < 0)
	//			bb = 0;
	//		else bb -= 50;

	//		newFile.write((const char*)&rr, sizeof(rr));
	//		newFile.write((const char*)&gg, sizeof(gg));
	//		newFile.write((const char*)&bb, sizeof(bb));
	//	}
	//}
	//while (!firstFile.eof())
	//{
	//	firstFile.read((char*)&rr, sizeof(rr));
	//	firstFile.read((char*)&gg, sizeof(gg));
	//	firstFile.read((char*)&bb, sizeof(bb));

	//	std::stringstream red(r);
	//	std::stringstream green(r);
	//	std::stringstream blue(r);


	//	//newFile << rr << " " << gg << " " << bb << std::endl;

	//	newFile.write((const char*)&rr, sizeof(rr));
	//	newFile.write((const char*)&gg, sizeof(gg));
	//	newFile.write((const char*)&bb, sizeof(bb));
	//	newFile.write("\n", sizeof("\n"));
	//}

	//firstFile << "P3" << std::endl;
	//firstFile << "500 500" << std::endl;
	//firstFile << "255" << std::endl;

	//for (size_t y = 0; y < 500; y++)
	//{
	//	for (size_t x = 0; x < 500; x++)
	//	{
	//		firstFile << (x * 69)%255 << " " << (y + 420) % 255 << " " << (x * y - 3) % 255 << std::endl;
	//	}
	//}
	//firstFile.close();

	//RGB pixel(2, 255,255);
	//RGB pixel1(pixel);
	//RGB pixel2;
	//pixel2 = pixel1;


	//std::cout << pixel.getSum();
}
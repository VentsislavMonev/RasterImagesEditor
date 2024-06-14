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

















std::string toLowwer(const std::string& string)
{
	std::string result = string;
	size_t size = string.size();
	for (size_t i = 0; i < size; i++)
	{
		if (result[i] >= 'A' && result[i] <= 'Z')
			result[i] += 'a' - 'A';
	}
	return result;
}

void splitInputs(const std::string& input, std::vector<std::string>& inputs)
{
	inputs.clear();
	std::string currentStr = "";
	size_t inputSize = input.size();
	for (size_t i = 0; i < inputSize; i++)
	{
		if (input[i] == ' ')
		{
			if (currentStr == "") continue;
			inputs.push_back(currentStr);
			currentStr = "";
		}
		else
		{
			currentStr += input[i];
		}
	}

	if (currentStr != "") inputs.push_back(currentStr);
}


void verifyInputs(const std::vector<std::string>& inputs,ImageProcesing::Commands& command)
{
	if (inputs.size() == 1)
	{
		std::string commandTxt = toLowwer(inputs[0]);

		if (commandTxt == "grayscale")		command = ImageProcesing::Commands::grayscale;
		else if (commandTxt == "monochrome")	command = ImageProcesing::Commands::monochrome;
		else if (commandTxt == "negative")		command = ImageProcesing::Commands::negative;
		else command = ImageProcesing::Commands::defaultCommand;
	}
	else if (inputs.size() == 2)
	{
		std::string fisrtCommandTxt = toLowwer(inputs[0]);
		std::string secondCommandTxt = toLowwer(inputs[1]);

		if (fisrtCommandTxt == "rotate" && secondCommandTxt == "left")
			command = ImageProcesing::Commands::rotateLeft;
		else if (fisrtCommandTxt == "rotate" && secondCommandTxt == "right")
			command = ImageProcesing::Commands::rotateRight;
		else if (fisrtCommandTxt == "flip" && secondCommandTxt == "top")
			command = ImageProcesing::Commands::flipTop;
		else if (fisrtCommandTxt == "flip" && secondCommandTxt == "left")
			command = ImageProcesing::Commands::flipLeft;
		else command = ImageProcesing::Commands::defaultCommand;
	}
	else command = ImageProcesing::Commands::defaultCommand;
	
}

void manageSessionCommand(const std::vector<std::string>& inputs, Session& session)
{
	if (inputs.size() == 1)
	{
		std::string firstCommandTxt = toLowwer(inputs[0]);

		if (firstCommandTxt == "save")	    session.save();
		//else if (commandTxt == "load")			command = ImageProcesing::Commands::load;
		else if (firstCommandTxt == "undo") session.undo();
		else if (firstCommandTxt == "redo") session.redo();
		else if (firstCommandTxt == "exit") session.exit();
	}
	if (inputs.size() >= 1)
	{
		std::string firstCommandTxt = toLowwer(inputs[0]);

		if (firstCommandTxt == "saveas")
		{
			session.saveas(inputs);
		}
		else if (firstCommandTxt == "add")
		{
			for (size_t i = 1; i < inputs.size(); ++i)
			{
				session.addFile(inputs[i]);
			}
		}
	}
	if (inputs.size() == 2)
	{
		std::string firstCommandTxt = toLowwer(inputs[0]);
		std::string seccondCommandTxt = toLowwer(inputs[1]);

		if (firstCommandTxt == "list" && seccondCommandTxt == "session")
			session.listSession();
	}
	if (inputs.size() == 5)
	{
		std::string firstCommandTxt = toLowwer(inputs[0]);
		std::string seccondCommandTxt = toLowwer(inputs[1]);
		std::string thirdCommandTxt = toLowwer(inputs[2]);

		if (firstCommandTxt == "make" && seccondCommandTxt == "collage" && thirdCommandTxt == "horizontal")
			session.makeHorizontalCollage(inputs[3],inputs[4]);
		else if (firstCommandTxt == "make" && seccondCommandTxt == "collage" && thirdCommandTxt == "vertical")
			session.makeVerticalCollage(inputs[3], inputs[4]);
		else if (firstCommandTxt == "crop")
			session.crop(inputs[1], inputs[2], inputs[3], inputs[4]);
	}
	
}

int main()
{

	std::vector<Session> sessions;
	std::string firstInput;
	std::vector<std::string> inputs;
	SessionProcesing::commands command;
	ImageProcesing::Commands imageCommand;

	//loads the session
	do
	{
		std::getline(std::cin, firstInput);
		splitInputs(firstInput, inputs);
	} while (inputs.empty() || inputs[0] != "load");

	Session session;
	for (size_t i = 1; i < inputs.size(); i++)
	{
		session.addFile(inputs[i]);
	}

	//manages picture commands
	do
	{
		std::getline(std::cin, firstInput);
		splitInputs(firstInput, inputs);
		verifyInputs(inputs, imageCommand);
		if (imageCommand != ImageProcesing::Commands::defaultCommand)
		{
			try
			{
				session.addCommand(imageCommand);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what()<<std::endl;
			}
		}
		else
		{
			try
			{
				manageSessionCommand(inputs, session);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	} while (inputs.empty() || inputs[0] != "exit");



	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
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
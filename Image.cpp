#include <fstream>
#include <iostream>
#include "Image.h"
#include "Utility.h"

Image::Image()
{
	format = ImageProcesing::ImageType::defaultType;
	width = length = 0;
}
Image::Image(const std::string& _file)
{
	std::ifstream input(_file);
	if (!input)
		throw std::runtime_error("File is bad!");

	setFileName(_file);
	setFormat(_file);
	
	std::string skip;
	std::string inputWidthTxt;
	std::string inputLengthTxt;

	skipComments(input);
	input >> skip;
	skipComments(input);
	input >> inputWidthTxt;
	skipComments(input);
	input >> inputLengthTxt;
	skipComments(input);

	setWidth(getNumb(inputWidthTxt));
	setLength(getNumb(inputLengthTxt));
}

bool Image::crop(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY)
{
	try
	{
		validateCoordinates(topLeftX, topLeftY, botRightX, botRightY);
		setWidth(botRightX - topLeftX+1);
		setLength(botRightY - topLeftY+1);

		//i do this so image can save after crop and remove the undo option
		manageCommands();
		commandsToDo.push_back(ImageProcesing::Commands::crop);
		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}	
}
bool Image::validateCoordinates(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY) const
{
	unsigned short width = getWidth();
	unsigned short length = getLength();
	if (topLeftX < 0) topLeftX = 0;
	if (topLeftY < 0) topLeftY = 0;
	if (botRightX >= width)  botRightX = width-1;
	if (botRightY >= length) botRightY = length-1;	

	if (topLeftX > botRightX || topLeftY > botRightY  )
		throw std::invalid_argument("Invalid Coordinates. The image" + fileName +" didn`t changed!");
}

void Image::addCommand(ImageProcesing::Commands command)
{
	if (command == ImageProcesing::Commands::defaultCommand)
		throw std::invalid_argument("There isn`t such a command!");
	commandsToDo.push_back(command);
	if(!commandsToNotDo.empty())
	{
		commandsToNotDo.clear();
	}
}
void Image::redo()
{
	if (commandsToNotDo.empty())
		return;
	ImageProcesing::Commands command = commandsToNotDo.back();
	commandsToDo.push_back(command);
	commandsToNotDo.pop_back();
}
void Image::undo()
{
	if (commandsToDo.empty())
		return;
	ImageProcesing::Commands command = commandsToDo.back();
	if (command == ImageProcesing::Commands::crop) return;
	commandsToNotDo.push_back(command);
	commandsToDo.pop_back();
}

void Image::setFileName(const std::string& _fileName)
{
	if (!_fileName.c_str())
	{
		throw std::invalid_argument("Invalid file name!");
	}

	size_t fileNameSize = _fileName.size()-4;
	for (size_t i = 0; i < fileNameSize; ++i)
	{
		if ((_fileName[i] < 'A' || _fileName[i] > 'Z') &&
			(_fileName[i] < 'a' || _fileName[i]> 'z')  &&
			(_fileName[i] < '0' || _fileName[i] >'9')  &&
			_fileName[i] != '.' && _fileName[i] != '_' &&
			_fileName[i] != '(' && _fileName[i] != ')')
		{
			throw std::invalid_argument("Inavalid file name!");
		}
	}
	fileName = _fileName;
	fileName.erase(fileName.size() - 4);
}
void Image::skipComments(std::ifstream& file) const
{
	std::string line;
	char ch;
	while (file >> std::ws && file.peek() == '#') 
	{
		std::getline(file, line);
	}
}
void Image::setFormat(const std::string& _format)
{
	std::string inputFormat=_format.substr(_format.size()-4);
	if (inputFormat == ".ppm")
		format = ImageProcesing::ImageType::P3;
	else if (inputFormat == ".pgm")
		format = ImageProcesing::ImageType::P2;
	else if (inputFormat == ".pbm")
		format = ImageProcesing::ImageType::P1;
	else throw std::invalid_argument("Invalid image type");
}
void Image::setWidth(int _width)
{
	if (_width < 0)
		throw std::invalid_argument("Width can`t be of a negative value!");
	else if (_width > std::numeric_limits<unsigned short>::max())
		throw std::invalid_argument("Width is too big!");
	else
		width = _width;
}
void Image::setLength(int _length)
{
	if (_length < 0)
		throw std::invalid_argument("Length can`t be of a negative value!");
	else if (_length > std::numeric_limits<unsigned short>::max())
		throw std::invalid_argument("Length is too big!");
	else
		length = _length;
}

void Image::getModifiedFile(std::string& _modifiedFile) const
{
	std::string modifiedTime = getModifiedTime();

	//adds the 2 strings together
	_modifiedFile = fileName + '_' + modifiedTime;

	//adds the file type to it
	_modifiedFile.append(".ppm");
}

const std::string& Image::getFileName() const
{
	return fileName;
}
ImageProcesing::ImageType Image::getFormat() const
{
	return format;
}
ImageProcesing::ImageType Image::getFormat(const std::string& _format) const
{
	if (_format == ".ppm") return ImageProcesing::ImageType::P3;
	else if (_format == ".pgm") return ImageProcesing::ImageType::P2;
	else if (_format == ".pbm") return ImageProcesing::ImageType::P1;
}
unsigned short Image::getWidth() const
{
	return width;
}
unsigned short Image::getLength() const
{
	return length;
}
bool Image::isChanged() const
{
	return !commandsToDo.empty();
}
const std::vector<ImageProcesing::Commands>& Image::getCommandsToDo() const
{
	return commandsToDo;
}
const std::vector<ImageProcesing::Commands>& Image::getCommandsToNotDo() const
{
	return commandsToNotDo;
}

void Image::manageCommands()
{
	int rotationsLeft = 0;
	int rotationsRight = 0;
	int flipsTop = 0;
	int flipsLeft = 0;
	bool isMonochrome = false;
	bool isGrayscale = false;

	//manages commands
	size_t commandsCount = commandsToDo.size();
	for (size_t i = 0; i < commandsCount; ++i)
	{
		switch (commandsToDo[i])
		{
		case ImageProcesing::Commands::monochrome:
			if (!isMonochrome)
			{
				monochrome();
				isMonochrome = true;
			}
			break;
		case ImageProcesing::Commands::grayscale:
			if (!isGrayscale)
			{
				grayScale();
				isGrayscale = true;
			}
			break;
		case ImageProcesing::Commands::negative:	negative();		  break;
		case ImageProcesing::Commands::rotateLeft:	++rotationsLeft;  break;
		case ImageProcesing::Commands::rotateRight: ++rotationsRight; break;
		case ImageProcesing::Commands::flipTop:		++flipsTop;		  break;
		case ImageProcesing::Commands::flipLeft:	++flipsLeft;	  break;
		case ImageProcesing::Commands::crop:						  break;

		case ImageProcesing::Commands::defaultCommand:
			throw std::runtime_error("Invalid command");
			break;
		}
	}
	commandsToDo.clear();

	manageRotations(rotationsLeft, rotationsRight);
	manageFlips(flipsTop, flipsLeft);
}
void Image::manageRotations(int rotationsLeft, int rotationsRight)
{
	rotationsLeft = rotationsLeft % sides;
	rotationsRight = rotationsRight % sides;
	if ( rotationsLeft>=rotationsRight)
	{
		for (size_t i = 0; i < rotationsLeft-rotationsRight; i++)
		{
			rotateLeft();
		}
	}
	else
	{
		for (size_t i = 0; i < rotationsRight; i++)
		{
			rotateRight();
		}
	}
}
void Image::manageFlips(int flipsTop, int flipsLeft)
{
	if (flipsTop % 2)  flipTop();
	if (flipsLeft % 2) flipLeft();
}
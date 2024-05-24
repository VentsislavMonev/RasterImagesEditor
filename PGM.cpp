#include "PGM.h"
#include "Utility.h"
#include <iostream>
#include <fstream>

PGM::PGM(const std::string& _file) :Image(_file)
{
	std::ifstream input(_file);

	if (!input)
		throw std::runtime_error("File is bad!");

	std::string skip;
	std::string inputFormatTxt;
	std::string inputMaxValueTxt;

	input >> inputFormatTxt >> skip >> skip >> inputMaxValueTxt;

	setFormat(inputFormatTxt);
	setMaxValue(getNumb(inputMaxValueTxt));
	setMatrix(input);

	input.close();
}


const std::vector<std::vector<unsigned char>>& PGM::getMatrix() const
{
	return pixels;
}
unsigned char PGM::getMaxValue() const
{
	return maxValue;
}


void PGM::monochrome()
{
	unsigned short width = getWidth();
	unsigned short length = getLength();

	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			if (pixels[i][j] < maxValue / 2)
				pixels[i][j] = 0;
			else pixels[i][j] = maxValue;
		}
	}
}
void PGM::negative()
{
	unsigned short width = getWidth();
	unsigned short length = getLength();
	unsigned short value = 0;
	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			pixels[i][j] = maxValue - pixels[i][j];
		}
	}
}

void PGM::rotateLeft()
{
	transposeMatrix();

	reverseRows();
}
void PGM::rotateRight()
{
	transposeMatrix();

	reverseColumns();
}
void PGM::flipTop()
{
	reverseRows();
}
void PGM::flipLeft()
{
	reverseColumns();
}

void PGM::save()
{
	if (!getCommandsToDo().empty())
	{
		manageCommands();

		//gets file modified name
		std::string modifiedFile;
		getModifiedFile(modifiedFile);

		//opens file with given name
		std::ofstream newImage(modifiedFile);
		if (!newImage)
			throw std::runtime_error("Bad file!");

		unsigned short width = getWidth();
		unsigned short length = getLength();

		writeFileHeader(newImage);
		writeMatrix(newImage, width, length);

		newImage.close();
	}
	else return;
}


void PGM::transposeMatrix()
{
	size_t width = getWidth();
	size_t length = getLength();
	std::vector<std::vector<unsigned char>> transponsedMatrix(width, std::vector<unsigned char>(length, unsigned char()));
	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			transponsedMatrix[j][i] = pixels[i][j];
		}
	}

	pixels = transponsedMatrix;
	setWidth(length);
	setLength(width);
}
void PGM::reverseColumns()
{
	unsigned short columnsCount = getWidth();
	unsigned short rowsCount = getLength();
	for (size_t i = 0; i < rowsCount; ++i)
		for (size_t j = 0; j < columnsCount / 2; ++j)
			std::swap(pixels[i][j], pixels[i][columnsCount - 1 - j]);
}
void PGM::reverseRows()
{
	unsigned short columnsCount = getLength();
	for (size_t i = 0; i < columnsCount / 2; ++i)
		std::swap(pixels[i], pixels[columnsCount - 1 - i]);
}


void PGM::writeFileHeader(std::ofstream& newImage) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");
	newImage << "P2" << std::endl;
	newImage << getWidth() << " " << getLength() << std::endl;
	newImage << static_cast<unsigned>(getMaxValue()) << std::endl;
}
void PGM::writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");

	for (size_t i = 0; i < _length; ++i)
	{
		for (size_t j = 0; j < _width; ++j)
		{
			newImage << static_cast<unsigned>(pixels[i][j])<< ' ';
		}
		newImage << std::endl;
	}
}
void PGM::manageCommands()
{
	int rotationsLeft = 0;
	int rotationsRight = 0;
	int flipsTop = 0;
	int flipsLeft = 0;

	//manages commands
	size_t commandsCount = getCommandsToDo().size();
	for (size_t i = 0; i < commandsCount; ++i)
	{
		switch (getCommandsToDo()[i])
		{
		case ImageProcesing::Commands::monochrome:	monochrome();	  break;
		case ImageProcesing::Commands::grayscale:	grayScale();	  break;
		case ImageProcesing::Commands::negative:	negative();		  break;
		case ImageProcesing::Commands::rotateLeft:	++rotationsLeft;  break;
		case ImageProcesing::Commands::rotateRight: ++rotationsRight; break;
		case ImageProcesing::Commands::flipTop:		++flipsTop;		  break;
		case ImageProcesing::Commands::flipLeft:	++flipsLeft;	  break;

		case ImageProcesing::Commands::defaultCommand:
			throw std::runtime_error("Invalid command");
			break;
		}
	}

	//manages rotations
	rotationsLeft = rotationsLeft % 4;
	rotationsRight = rotationsRight % 4;
	rotationsRight = (rotationsRight - rotationsLeft) % 4;
	for (size_t i = 0; i < rotationsRight; ++i)
	{
		rotateRight();
	}

	//manages flips
	if (flipsTop % 2) flipTop();
	if (flipsLeft % 2) flipLeft();
}


void PGM::setMatrix(std::ifstream& input)
{
	if (!input)
		throw std::runtime_error("File is bad!");

	int inputValue;
	unsigned short width = getWidth();
	unsigned short length = getLength();
	std::vector<unsigned char> row;

	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			input >> inputValue;
			row.push_back(setValue(inputValue));
		}
		pixels.push_back(row);
		row.clear();
	}
}
void PGM::setMaxValue(int _maxValue)
{
	if (_maxValue <= 0)
		throw std::invalid_argument("Max value can`t be less than or equal to 0");
	else if (_maxValue > std::numeric_limits<unsigned char>().max())
		throw std::invalid_argument("Max value can`t be bigger than 255");
	else
		maxValue = _maxValue;
}
void PGM::setFormat(const std::string& _format)
{
	if (_format != "P2")
		throw std::invalid_argument("Invalid image format");
}
unsigned char PGM::setValue(int _value)
{
	unsigned char value = 0;
	if (_value > maxValue)    throw std::invalid_argument("Value for red can only be less than or equal to " + static_cast<unsigned>(maxValue));
	else if (_value < 0) throw std::invalid_argument("Value for red can only be more than or equal to 0!");
	else value = _value;
	return value;
}


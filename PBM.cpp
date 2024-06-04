#include <fstream>
#include <iostream>

#include "PBM.h"
#include "Utility.h"

PBM::PBM(const std::string& _file) :Image(_file)
{
	std::ifstream newImage(_file);

	if (!newImage)
		throw std::runtime_error("File is bad!");

	std::string skip;
	std::string inputFormatTxt;

	newImage >> inputFormatTxt >> skip >> skip;

	setFormat(inputFormatTxt);
	setMatrix(newImage);

	newImage.close();
}

Image* PBM::clone() const
{
	return new PBM(*this);
}


void PBM::negative()
{
	unsigned short width = getWidth();
	unsigned short length = getLength();
	for (size_t i = 0; i < length; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			pixels[i][j] = maxValue - pixels[i][j];
		}
	}
}

void PBM::rotateLeft()
{
	transposeMatrix();
	reverseRows();
}
void PBM::rotateRight()
{
	transposeMatrix();
	reverseColumns();
}
void PBM::flipTop()
{
	reverseRows();
}
void PBM::flipLeft()
{
	reverseColumns();
}

void PBM::save()
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


const std::vector<std::vector<bool>>& PBM::getMatrix() const
{
	return pixels;
}
bool PBM::getMaxValue()
{
	return maxValue;
}


void PBM::transposeMatrix()
{
	size_t width = getWidth();
	size_t length = getLength();
	std::vector<std::vector<bool>> transponsedMatrix(width, std::vector<bool>(length, bool()));
	for (size_t i = 0; i < length; ++i)
		for (size_t j = 0; j < width; ++j)
			transponsedMatrix[j][i] = pixels[i][j];

	pixels = transponsedMatrix;
	setWidth(length);
	setLength(width);
}
void PBM::reverseColumns()
{
	size_t columnsCount = getWidth();
	size_t rowsCount = getLength();
	bool pixel = 0;
	for (size_t i = 0; i < rowsCount; i++)
	{
		for (size_t j = 0; j < columnsCount; j++)
		{
			pixel = pixels[i][j];
			pixels[i][j] = pixels[i][columnsCount - 1 - j];
			pixels[i][columnsCount - 1 - j] = pixel;
		}
	}
}
void PBM::reverseRows()
{
	size_t columnsCount = getWidth();
	size_t rowsCount = getLength();
	for (size_t i = 0; i < columnsCount; i++)
		std::swap(pixels[i], pixels[columnsCount - 1 - i]);
}


void PBM::writeFileHeader(std::ofstream& newImage) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");
	newImage << "P1" << std::endl;
	newImage << getWidth() << " " << getLength() << std::endl;
}

void PBM::writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");

	for (size_t i = 0; i < _length; ++i)
	{
		for (size_t j = 0; j < _width; ++j)
		{
			newImage << pixels[i][j] << ' ';
		}
		newImage << std::endl;
	}
}

void PBM::manageCommands()
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
	rotationsRight = rotationsRight - rotationsLeft;
	if (rotationsRight < 0)
		rotationsRight = 4 - rotationsRight * (-1);
	for (size_t i = 0; i < rotationsRight; ++i)
	{
		rotateRight();
	}

	//manages flips
	if (flipsTop % 2) flipTop();
	if (flipsLeft % 2) flipLeft();
}

void PBM::setMatrix(std::ifstream& newImage)
{
	if (!newImage)
		throw std::runtime_error("File is bad!");

	bool inputValue;
	unsigned short width = getWidth();
	unsigned short length = getLength();
	std::vector<bool> row;

	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			newImage >> inputValue;
			row.push_back(setValue(inputValue));
		}
		pixels.push_back(row);
		row.clear();
	}
}

void PBM::setFormat(const std::string& _format)
{
	if (_format != "P1")
		throw std::invalid_argument("Invalid image format");
}
bool PBM::setValue(int _value)
{
	if (_value == 0 || _value == 1)
		return _value;
	else throw std::invalid_argument("Value can be only 0 or 1");
}

#include "PPM.h"
#include "Utility.h"
#include <fstream>
#include <iostream>


PPM::PPM(const std::string& _file) :Image(_file)
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

void PPM::setMatrix(std::ifstream& input)
{
	if (!input)
		throw std::runtime_error("File is bad!");

	int red;
	int green;
	int blue;

	unsigned short width = getWidth();
	unsigned short length = getLength();
	std::vector<RGB> row;

	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			input >> red >> green >> blue;
			RGB pixel(red, green, blue);
			row.push_back(pixel);
		}
		pixels.push_back(row);
		row.clear();
	}
}

void PPM::setMaxValue(int _maxValue)
{
	if	(_maxValue <= 0)
		throw std::invalid_argument("Max value can`t be less than or equal to 0");
	else if (_maxValue > RGB::getMaxValue()) 
		throw std::invalid_argument("Max value can`t be bigger than 255");
	else 
		maxValue = _maxValue;
}

void PPM::setFormat(const std::string& _format)
{
	if (_format != "P3")
		throw std::invalid_argument("Invalid image format");
}

const std::vector<std::vector<RGB>>& PPM::getMatrix() const
{
	return pixels;
}

unsigned char PPM::getMaxValue() const
{
	return maxValue;
}

void PPM::save()
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

void PPM::grayScale()
{
	unsigned short width = getWidth();
	unsigned short length = getLength();
	for (size_t i = 0; i < length; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			pixels[i][j].setEqual(pixels[i][j].getGrayScaleValue());
		}
	}
}

void PPM::monochrome()
{
	unsigned short width  = getWidth();
	unsigned short lenght = getLength();

	for (size_t i = 0; i < lenght; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			if (pixels[i][j].getGrayScaleValue() < maxValue / 2)
				pixels[i][j].setEqual(0);
			else pixels[i][j].setEqual(maxValue);
		}
	}
}

void PPM::negative()
{
	unsigned short width = getWidth();
	unsigned short lenght = getLength();
	for (size_t i = 0; i < lenght; ++i)
	{
		for (size_t j = 0; j < width; ++j)
		{
			pixels[i][j].setRed(maxValue - pixels[i][j].r());
			pixels[i][j].setGreen(maxValue - pixels[i][j].g());
			pixels[i][j].setBlue(maxValue - pixels[i][j].b());
		}
	}
}

void PPM::rotateLeft()
{
	transposeMatrix();

	reverseRows();
}

void PPM::rotateRight()
{
	transposeMatrix();

	reverseColumns();
}

void PPM::flipTop()
{
	reverseRows();
}

void PPM::flipLeft()
{
	reverseColumns();
}

void PPM::transposeMatrix()
{
	size_t width = getWidth();
	size_t length = getLength();
	RGB basic;
	std::vector<std::vector<RGB>> transponsedMatrix(width, std::vector<RGB>(length, basic));
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

void PPM::reverseColumns()
{
	unsigned short columnsCount = getWidth();
	unsigned short rowsCount = getLength();
	for (size_t i = 0; i < rowsCount; ++i)
		for (size_t j = 0; j < columnsCount/2; ++j)
			std::swap(pixels[i][j], pixels[i][columnsCount-1-j]);
}

void PPM::reverseRows()
{
	unsigned short columnsCount = getLength();
	for (size_t i = 0; i < columnsCount /2; ++i)
			std::swap(pixels[i], pixels[columnsCount -1-i]);

}

void PPM::writeFileHeader(std::ofstream& newImage) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");
	newImage << "P3" << std::endl;
	newImage << getWidth() << " " << getLength()<< std::endl;
	newImage << (unsigned)getMaxValue() << std::endl;
}

void PPM::writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");

	for (size_t i = 0; i < _length; ++i)
	{
		for (size_t j = 0; j < _width; ++j)
		{
			newImage << static_cast<unsigned>(pixels[i][j].r()) << ' ' 
					 << static_cast<unsigned>(pixels[i][j].g()) << ' ' 
					 << static_cast<unsigned>(pixels[i][j].b()) << std::endl;
		}
	}
}

void PPM::manageCommands()
{
	int rotationsLeft  = 0;
	int rotationsRight = 0;
	int flipsTop       = 0;
	int flipsLeft	   = 0;

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

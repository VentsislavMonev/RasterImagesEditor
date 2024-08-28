#include <fstream>
#include <iostream>

#include "PPM.h"
#include "Utility.h"


PPM::PPM(const std::string& _file) :Image(_file)
{
	std::ifstream newImage(_file);

	if (!newImage)
		throw std::runtime_error("File is bad!");

	std::string skip;
	std::string inputFormatTxt;
	std::string inputMaxValueTxt;
	
	skipComments(newImage);
	newImage >> inputFormatTxt;
	skipComments(newImage);
	newImage >> skip;
	skipComments(newImage);
	newImage >> skip;
	skipComments(newImage);
	newImage >> inputMaxValueTxt;
	skipComments(newImage);

	setFormat(inputFormatTxt);
	setMaxValue(getNumb(inputMaxValueTxt));
	setMatrix(newImage);

	newImage.close();
}

Image* PPM::clone() const
{
	return new PPM(*this);
}

void PPM::setMatrix(std::ifstream& newImage)
{
	if (!newImage)
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
			newImage >> red >> green >> blue;
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

void PPM::setFileName(const std::string& _fileName)
{
	if (_fileName.size()>4 && _fileName.substr(_fileName.size() - 4) != ".ppm")
		throw std::invalid_argument("Invalid file name!");
	Image::setFileName(_fileName);
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

bool PPM::crop(int topLeftX, int topLeftY, int botRightX, int botRightY)
{
	//validates the coordinates
	if (!Image::manageCrop(topLeftX, topLeftY, botRightX, botRightY))
		return false;

	unsigned short newWidth = botRightX - topLeftX + 1;
	unsigned short newLength = botRightY - topLeftY + 1;

	std::vector<std::vector<RGB>> newMatrix;
	std::vector<RGB> row;

	for (size_t i = topLeftY; i < topLeftY+newLength; ++i)
	{
		for (size_t j = topLeftX; j < topLeftX+newWidth; ++j)
		{
			row.push_back(pixels[i][j]);
		}
		newMatrix.push_back(row);
		row.clear();
	}
	pixels = newMatrix;

	setWidth(newWidth);
	setLength(newLength);

	return true;
}

void PPM::print() const
{
	std::cout << getFileName() << " - portable pixmap format" << std::endl;
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

void PPM::save(const std::string& newName)
{
	if (fileExist(newName))
		throw std::invalid_argument("File with name: " + newName + " already exist");
	setFileName(newName);
	manageCommands();

	//opens file with given name
	std::ofstream newImage(newName);
	if (!newImage)
		throw std::runtime_error("Bad file!");

	unsigned short width = getWidth();
	unsigned short length = getLength();

	writeFileHeader(newImage);
	writeMatrix(newImage, width, length);

	newImage.close();
}

void PPM::grayScale()
{
	unsigned short width = getWidth();
	unsigned short length = getLength();
	for (size_t i = 0; i < length; ++i)
		for (size_t j = 0; j < width; ++j)
			pixels[i][j].setEqual(pixels[i][j].getGrayScaleValue());
}

void PPM::monochrome()
{
	unsigned short width  = getWidth();
	unsigned short length = getLength();

	for (size_t i = 0; i < length; ++i)
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
	unsigned short length = getLength();
	for (size_t i = 0; i < length; ++i)
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
	unsigned short width = getWidth();
	unsigned short length = getLength();
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
	for (size_t i = 0; i < columnsCount / 2; ++i)
		std::swap(pixels[i], pixels[columnsCount - 1 - i]);
}

void PPM::writeFileHeader(std::ofstream& newImage) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");
	newImage << "P3" << std::endl;
	newImage << getWidth() << " " << getLength()<< std::endl;
	newImage << static_cast<unsigned>(getMaxValue()) << std::endl;
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
					 << static_cast<unsigned>(pixels[i][j].b()) << ' ';
		}
		newImage << std::endl;
	}
}
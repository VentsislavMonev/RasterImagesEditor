#include <iostream>
#include <fstream>

#include "PGM.h"
#include "Utility.h"

PGM::PGM(const std::string& _file) :Image(_file)
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

Image* PGM::clone() const
{
	return new PGM(*this);
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
		for (size_t j = 0; j < width; ++j)
			pixels[i][j] = maxValue - pixels[i][j];
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

bool PGM::crop(int topLeftX, int topLeftY, int botRightX, int botRightY)
{
	//validates the coordinates
	if (!Image::manageCrop(topLeftX, topLeftY, botRightX, botRightY))
		return false;

	unsigned short newWidth = botRightX - topLeftX + 1;
	unsigned short newLength = botRightY - topLeftY + 1;

	std::vector<std::vector<unsigned char>> newMatrix;
	std::vector<unsigned char> row;

	for (size_t i = topLeftY; i < topLeftY + newLength; ++i)
	{
		for (size_t j = topLeftX; j < topLeftX + newWidth; ++j)
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

void PGM::print() const
{
	std::cout << getFileName() << " - portable graymap format" << std::endl;
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

void PGM::save(const std::string& newName)
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

void PGM::makeHorizontalCollage(const Image* other) const
{
	const PGM* otherPGM = dynamic_cast<const PGM*>(other);
	if (otherPGM == nullptr)
	{
		throw std::invalid_argument("The provided image is not a PGM type.");
	}

	//gets file modified name
	std::string modifiedFileName;
	getCollageName(modifiedFileName, otherPGM);

	//opens file with given name
	std::ofstream collage(modifiedFileName);
	if (!collage)
		throw std::runtime_error("Bad file!");

	//new file header data
	unsigned short newWidth = getWidth() + otherPGM->getWidth();
	unsigned short newLength = std::max(getLength(), otherPGM->getLength());
	unsigned char newMaxValue = std::max(getMaxValue(), otherPGM->getMaxValue());

	//writes collage header
	writeCollageHeader(newWidth, newLength, collage);
	collage << unsigned(newMaxValue) << std::endl;

	writeCollageHorizontalMatrix(newWidth, newLength, otherPGM, collage);
}

void PGM::makeVerticalCollage(const Image* other) const
{
	const PGM* otherPGM = dynamic_cast<const PGM*>(other);
	if (otherPGM == nullptr)
	{
		throw std::invalid_argument("The provided image is not a PGM type.");
	}

	//gets file modified name
	std::string modifiedFileName;
	getCollageName(modifiedFileName, otherPGM);

	//opens file with given name
	std::ofstream collage(modifiedFileName);
	if (!collage)
		throw std::runtime_error("Bad file!");

	//new file header data
	unsigned short newWidth = std::max(getWidth(), otherPGM->getWidth());
	unsigned short newLength = getLength() + otherPGM->getLength();
	unsigned char newMaxValue = std::max(getMaxValue(), otherPGM->getMaxValue());

	//writes collage header
	writeCollageHeader(newWidth, newLength, collage);
	collage << unsigned(newMaxValue) << std::endl;

	writeCollageVerticalMatrix(newWidth, newLength, otherPGM, collage);
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

void PGM::writeCollageHorizontalMatrix(unsigned short newWidth, unsigned short newLength, const PGM* other, std::ofstream& collage) const
{
	if (!collage)
		throw std::runtime_error("Bad file!");

	unsigned short difference1 = (newLength - getLength()) / 2;
	unsigned short difference2 = (newLength - other->getLength()) / 2;

	int x2 = 0;
	for (int y = 0; y < newLength; ++y) 
	{
		for (int x = 0; x < newWidth; ++x) 
		{
			if (x < getWidth())
			{
				// writing pixels from the first image
				if (y >= difference1 && y < difference1 + getLength())
					collage << static_cast<unsigned>(pixels[y - difference1][x]) << ' ';
				else
					collage << 0 << ' ';
			}
			else
			{
				// Writing pixels from second image
				x2 = x - getWidth();
				if (y >= difference2 && y < difference2 + other->getLength())
					collage << static_cast<unsigned>(other->getMatrix()[y - difference2][x2]) << ' ';
				else
					collage << 0 << ' ';
			}
		}
		collage << std::endl;
	}
}

void PGM::writeCollageVerticalMatrix(unsigned short newWidth, unsigned short newLength, const PGM* other, std::ofstream& collage) const
{
	if (!collage)
		throw std::runtime_error("Bad file!");

	unsigned short difference1 = (newWidth - getWidth()) / 2;
	unsigned short difference2 = (newWidth - other->getWidth()) / 2;

	int y2 = 0;
	for (int y = 0; y < newLength; ++y)
	{
		for (int x = 0; x < newWidth; ++x)
		{
			if (y < getLength())
			{
				// Writing pixels from the first image
				if (x >= difference1 && x < difference1 + getWidth())
					collage << static_cast<unsigned>(pixels[y][x - difference1]) << ' ';
				else
					collage << 0 << ' ';
			}
			else
			{
				// Writing pixels from the second image
				y2 = y - getLength();
				if (x >= difference2 && x < difference2 + other->getWidth())
					collage << static_cast<unsigned>(other->getMatrix()[y2][x - difference2]) << ' ';
				else
					collage << 0 << ' ';
			}
		}
		collage << std::endl;
	}
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

void PGM::setFileName(const std::string& _fileName)
{
	if (_fileName.size() > 4 && _fileName.substr(_fileName.size() - 4) != ".pgm")
		throw std::invalid_argument("Invalid file name!");
	Image::setFileName(_fileName);
}

void PGM::setMatrix(std::ifstream& newImage)
{
	if (!newImage)
		throw std::runtime_error("File is bad!");

	int inputValue;
	unsigned short width = getWidth();
	unsigned short length = getLength();
	std::vector<unsigned char> row;

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
	if (_value > maxValue)    throw std::invalid_argument("Value can only be less than or equal to " + static_cast<unsigned>(maxValue));
	else if (_value < 0) throw std::invalid_argument("Value can only be more than or equal to 0!");
	else value = _value;
	return value;
}


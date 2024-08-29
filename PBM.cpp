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

	skipComments(newImage);
	newImage >> inputFormatTxt;
	skipComments(newImage);
	newImage >> skip;
	skipComments(newImage);
	newImage >> skip;
	skipComments(newImage);

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
		for (size_t j = 0; j < width; j++)
			pixels[i][j] = maxValue - pixels[i][j];
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

bool PBM::crop(int topLeftX, int topLeftY, int botRightX, int botRightY)
{
	//validates the coordinates
	if (!Image::manageCrop(topLeftX, topLeftY, botRightX, botRightY))
		return false;

	unsigned short newWidth = botRightX - topLeftX + 1;
	unsigned short newLength = botRightY - topLeftY + 1;

	std::vector<std::vector<bool>> newMatrix;
	std::vector<bool> row;

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

void PBM::print() const
{
	std::cout << getFileName() << " - 32bit Bitmap" << std::endl;
}

// TODO get width get length da staa v write file header i v write Matrix
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
}

void PBM::save(const std::string& newName)
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

void PBM::makeHorizontalCollage(const Image* other) const
{
	const PBM* otherPBM = dynamic_cast<const PBM*>(other);
	if (otherPBM == nullptr)
	{
		throw std::invalid_argument("The provided image is not a PBM type.");
	}

	//gets file modified name
	std::string modifiedFileName;
	getCollageName(modifiedFileName, otherPBM);

	//opens file with given name
	std::ofstream collage(modifiedFileName);
	if (!collage)
		throw std::runtime_error("Bad file!");

	//new file header data
	unsigned short newWidth = getWidth() + otherPBM->getWidth();
	unsigned short newLength = std::max(getLength(), otherPBM->getLength());

	//writes collage header
	writeCollageHeader(newWidth, newLength, collage);

	writeCollageHorizontalMatrix(newWidth, newLength, otherPBM, collage);
}

void PBM::makeVerticalCollage(const Image* other) const
{
	const PBM* otherPBM = dynamic_cast<const PBM*>(other);
	if (otherPBM == nullptr)
	{
		throw std::invalid_argument("The provided image is not a PBM type.");
	}

	//gets file modified name
	std::string modifiedFileName;
	getCollageName(modifiedFileName, otherPBM);

	//opens file with given name
	std::ofstream collage(modifiedFileName);
	if (!collage)
		throw std::runtime_error("Bad file!");

	//new file header data
	unsigned short newWidth = std::max(getWidth(), otherPBM->getWidth());
	unsigned short newLength = getLength() + otherPBM->getLength();

	//writes collage header
	writeCollageHeader(newWidth, newLength, collage);

	writeCollageVerticalMatrix(newWidth, newLength, otherPBM, collage);
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
		for (size_t j = 0; j < columnsCount/2; j++)
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
	for (size_t i = 0; i < columnsCount/2; i++)
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


void PBM::setFileName(const std::string& _fileName)
{
	if (_fileName.size() > 4 && _fileName.substr(_fileName.size() - 4) != ".pbm")
		throw std::invalid_argument("Invalid file name!");
	Image::setFileName(_fileName);
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

void PBM::writeCollageHorizontalMatrix(unsigned short newWidth, unsigned short newLength, const PBM* other, std::ofstream& collage) const
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
						collage << pixels[y - difference1][x] << ' ';
				else
					collage << 1 << ' ';
			}
			else
			{
				// Writing pixels from second image
				x2 = x - getWidth();
				if (y >= difference2 && y < difference2 + other->getLength())
					collage << other->getMatrix()[y - difference2][x2] << ' ';
				else
					collage << 1 << ' ';
			}
		}
		collage << std::endl;
	}
}

void PBM::writeCollageVerticalMatrix(unsigned short newWidth, unsigned short newLength, const PBM* other, std::ofstream& collage) const
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
					collage << pixels[y][x - difference1] << ' ';
				else
					collage << 1 << ' ';
			}
			else
			{
				// Writing pixels from the second image
				y2 = y - getLength();
				if (x >= difference2 && x < difference2 + other->getWidth())
					collage << other->getMatrix()[y2][x - difference2] << ' ';
				else
					collage << 1 << ' ';
			}
		}
		collage << std::endl;
	}
}

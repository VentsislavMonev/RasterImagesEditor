#include "PPM.h"
#include "Utility.h"

PPM::PPM(const std::string& _fileName) :Image(_fileName)
{
	std::ifstream input(_fileName);

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

	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < length; ++j)
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

void PPM::save() const
{
	//gets file modified name
	std::string modifiedFile;
	getModifiedFile(modifiedFile);
	
	//opens file with given name
	std::ofstream newImage(modifiedFile);
	if (!newImage)
		throw std::runtime_error("Bad file!");

	unsigned short width = getWidth();
	unsigned short length= getLength();

	writeFileHeader(newImage, width, length);
	writeMatrix(newImage, width, length);

	newImage.close();
}

void PPM::grayScale()
{
	unsigned short width = getWidth();
	unsigned short length = getLength();
	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < length; ++j)
		{
			pixels[i][j].setEqual(pixels[i][j].getGrayScaleValue());
		}
	}
}

void PPM::monochrome()
{
	unsigned short width  = getWidth();
	unsigned short lenght = getLength();

	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < lenght; ++j)
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
	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < lenght; ++j)
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

	reverseColumns();
}

void PPM::rotateRight()
{
	transposeMatrix();

	reverseColumns();
}

void PPM::flipTop()
{
	reverseColumns();
}

void PPM::flipLeft()
{
	reverseRows();
}

void PPM::transposeMatrix()
{
	unsigned short width = getWidth();
	unsigned short lenght = getLength();
	for (size_t i = 0; i < width; ++i)
		for (size_t j = 0; j < lenght; ++j)
			std::swap(pixels[i][j], pixels[j][i]);
}

void PPM::reverseColumns()
{
	unsigned short rowsCount = getLength();
	for (size_t i = 0; i < rowsCount/2; ++i)
			std::swap(pixels[i], pixels[rowsCount-1-i]);
}

void PPM::reverseRows()
{
	unsigned short columnsCount = getWidth();
	unsigned short lenght = getLength();
	for (size_t i = 0; i < columnsCount; ++i)
		for (size_t j = 0; j < lenght/2; ++j)
			std::swap(pixels[i][j], pixels[i][columnsCount-1-i]);
}

void PPM::getModifiedFile(std::string& _modifiedFile) const
{
	std::string modifiedTime = getModifiedTime();

	//adds the 2 strings together
	_modifiedFile = getFileName() + modifiedTime;

	//adds the file type to it
	_modifiedFile.append(".ppm");
}

void PPM::writeFileHeader(std::ofstream& newImage, unsigned short _width, unsigned short _length) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");

	newImage << "P3" << std::endl;
	newImage << _width << " " << _length << std::endl;
	newImage << (unsigned)getMaxValue() << std::endl;
}

void PPM::writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length) const
{
	if (!newImage)
		throw std::runtime_error("Bad file!");

	for (size_t i = 0; i < _width; ++i)
	{
		for (size_t j = 0; j < _length; ++j)
		{
			newImage << static_cast<unsigned>(pixels[i][j].r()) << ' ' 
					 << static_cast<unsigned>(pixels[i][j].g()) << ' ' 
					 << static_cast<unsigned>(pixels[i][j].b()) << std::endl;
		}
	}
}

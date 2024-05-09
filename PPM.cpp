#include "PPM.h"

PPM::PPM(const std::string& _fileName)
{
	setFileName(_fileName);
	std::ifstream input(_fileName);

	if (!input)
		throw std::runtime_error("File is bad!");

	std::string inputFormatTxt;
	std::string inputWidthTxt;
	std::string inputLengthTxt;
	std::string inputMaxValueTxt;
	
	input >> inputFormatTxt >> inputWidthTxt >> inputLengthTxt >> inputMaxValueTxt;
	setFileHeader(inputFormatTxt, inputWidthTxt, inputLengthTxt, inputMaxValueTxt);

	setMatrix(input);
	input.close();
}

void PPM::setFileName(const std::string& _fileName)
{
	if (!_fileName.c_str())
	{
		throw std::invalid_argument("Invalid file name!");
	}
	//checkLastFour(_fileName);		//tva ne tuka
	Image::setFileName(_fileName);
}

void PPM::checkLastFour(const std::string& _fileName) const
{
	size_t fileNameSize = _fileName.size();
	if (_fileName.at(fileNameSize - 4) != '.' ||
		_fileName.at(fileNameSize - 3) != 'p' ||
		_fileName.at(fileNameSize - 2) != 'p' ||
		_fileName.at(fileNameSize - 1) != 'm')
	{
		throw std::invalid_argument("Invalid file name!");
	}

}

void PPM::setMatrix(std::ifstream& input)
{
	if (!input)
		throw std::runtime_error("File is bad!");

	int red;
	int green;
	int blue;
	int aloda = 0;//

	unsigned short width = getWidth();
	unsigned short length = getLength();
	std::vector<RGB> row;
	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < length; ++j)
		{
			if (i == 1 && j == 0)//
				aloda = 1;//
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
	std::string newFileName = "geicho.ppm";
	std::ofstream newImage(newFileName);
	if (!newImage)
		throw std::runtime_error("Bad file!");

	unsigned short width = getWidth();
	unsigned short length= getLength();
	newImage << "P3" << std::endl;
	newImage << width << " " << length << std::endl;
	newImage << (int)getMaxValue()<< std::endl;
	int da = 0;
	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < length; ++j)
		{
			if (i == 1)
				da=1;
			newImage << (unsigned)pixels[i][j].r() << ' ' << (unsigned)pixels[i][j].g() << ' ' << (unsigned)pixels[i][j].b() << std::endl;
		}
	}

	newImage.close();
}

void PPM::grayScale()
{
	unsigned char grayScaleValue = 0;
	
	unsigned short width = getWidth();
	unsigned short length = getLength();
	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < length; ++j)
		{
			grayScaleValue= pixels[i][j].r() * 0.2126 + pixels[i][j].g() * 0.7152 + pixels[i][j].b() * 0.0722;
			pixels[i][j].setEqual(grayScaleValue);
		}
	}
}

//ako se setq po-dobur nachin ili nqkak si da preizpolzvam koda ot grayscale
void PPM::monochrome()
{
	unsigned char grayScaleValue = 0;
	const float colorimetricValueRed   = 0.2126;
	const float colorimetricValueGreen = 0.7152;
	const float colorimetricValueBlue  = 0.0722;

	unsigned short width  = getWidth();
	unsigned short lenght = getLength();

	for (size_t i = 0; i < width; ++i)
	{
		for (size_t j = 0; j < lenght; ++j)
		{
			grayScaleValue = pixels[i][j].r() * colorimetricValueRed + pixels[i][j].g() * colorimetricValueGreen + pixels[i][j].b() * colorimetricValueBlue;
			if (grayScaleValue < maxValue/2)
				grayScaleValue = 0;
			else grayScaleValue = maxValue;
			pixels[i][j].setEqual(grayScaleValue);
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

void PPM::setFileHeader(std::string& inputFormatTxt, std::string& inputWidthTxt, std::string& inputLengthTxt, std::string& inputMaxValueTxt)
{
	ImageProcesing::ImageType inputFormat;
	int inputWidth;
	int inputLength;
	int inputMaxValue;

	if (inputFormatTxt != "P3")
	{
		inputFormat = ImageProcesing::ImageType::defaultType;
	}
	else inputFormat = ImageProcesing::ImageType::P3;
	inputWidth = getNumb(inputWidthTxt);
	inputLength = getNumb(inputLengthTxt);
	inputMaxValue = getNumb(inputMaxValueTxt);

	Image::setFormat(inputFormat);
	Image::setWidth(inputWidth);
	Image::setLength(inputLength);
	setMaxValue(inputMaxValue);
}

int PPM::getNumb(std::string& str)
{
	size_t i = 0;
	int sign = 1;
	int number = 0;
	if (str[i] == '-')
	{
		sign = -1;
		++i;
	}
	for (i ; i < str.size(); ++i)
	{
		if (str[i] < '0' || str[i]>'9') throw std::invalid_argument("Invalid input!");
		number = number * 10 + (str[i] - '0');
	}
	number = number * sign;
	return number;
}

#include "PPM.h"

PPM::PPM(ImageProcesing::ImageType _format, int _width, int _length, int _maxValue):Image(_format, _width, _length)
{
	//setMatrix()
	setMaxValue(_maxValue);
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

short PPM::getMaxValue() const
{
	return maxValue;
}

bool PPM::grayScale()
{
	unsigned char grayScaleValue = 0;
	
	for (size_t i = 0; i < getWidth(); ++i)
	{
		for (size_t j = 0; j < getLength(); ++j)
		{
			grayScaleValue= pixels[i][j].r() * 0.2126 + pixels[i][j].g() * 0.7152 + pixels[i][j].b() * 0.0722;
			pixels[i][j].setEqual(grayScaleValue);
		}
	}
}

//ako se setq po-dobur nachin ili nqkak si da preizpolzvam koda ot grayscale
//da dostupvam li direktno pixels[i][j] ili da si dobavqm RGB pixel
bool PPM::monochrome()
{
	unsigned char grayScaleValue = 0;
	RGB pixel;
	const float colorimetricValueRed   = 0.2126;
	const float colorimetricValueGreen = 0.7152;
	const float colorimetricValueBlue  = 0.0722;

	for (size_t i = 0; i < getWidth(); ++i)
	{
		for (size_t j = 0; j < getLength(); ++j)
		{
			pixel = pixels[i][j];
			grayScaleValue = pixel.r() * colorimetricValueRed + pixel.g() * colorimetricValueGreen + pixel.b() * colorimetricValueBlue;
			if (grayScaleValue < 127)
				grayScaleValue = 0;
			else grayScaleValue = maxValue;
			pixels[i][j].setEqual(grayScaleValue);
		}
	}
}

bool PPM::negative()
{
	RGB pixel;
	for (size_t i = 0; i < getWidth(); ++i)
	{
		for (size_t j = 0; j < getLength(); ++j)
		{
			pixel = pixels[i][j];
			pixel.setRed(maxValue - pixel.r());
			pixel.setGreen(maxValue - pixel.g());
			pixel.setBlue(maxValue - pixel.b());
			pixels[i][j] = pixel;
		}
	}
}

bool PPM::rotateLeft()
{
	transposeMatrix();

	reverseColumns();
}

void PPM::transposeMatrix()
{
	for (size_t i = 0; i < getWidth(); ++i)
		for (size_t j = 0; j < getLength(); ++j)
			std::swap(pixels[i][j], pixels[j][i]);
}

void PPM::reverseColumns()
{
	unsigned short rowsCount = getLength();
	for (size_t i = 0; i < getWidth(); ++i)
		for (size_t j = 0; j < getLength() / 2; j++)
			std::swap(pixels[i][j], pixels[i][rowsCount - 1 - j]);
}

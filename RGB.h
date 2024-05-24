#pragma once

static const float colorimetricValueRed = 0.2126;
static const float colorimetricValueGreen = 0.7152;
static const float colorimetricValueBlue = 0.0722;

class RGB
{
public:
	RGB();
	RGB(int _red, int _green, int _blue);

	unsigned getSum()const;
	unsigned getAverage()const;
	unsigned getGrayScaleValue()const;

	void setEqual(int value);
	void setEqualToRed();
	void setEqualToGreen();
	void setEqualToBlue();

	void setRed(int _red);
	void setGreen(int _green);
	void setBlue(int _blue);

	unsigned char r()const;
	unsigned char g()const;
	unsigned char b()const;

	static unsigned getMaxValue();
private:
	unsigned char red;
	unsigned char green;
	unsigned char blue;

	static const unsigned maxValue = 255;
};


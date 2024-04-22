#pragma once
#include <iostream>

class RGB
{
public:
	RGB();
	RGB(unsigned _red, unsigned _green, unsigned _blue);

	unsigned getSum()const;
	unsigned getAverage()const;

	void setRed(unsigned _red);
	void setGreen(unsigned _green);
	void setBlue(unsigned _blue);

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


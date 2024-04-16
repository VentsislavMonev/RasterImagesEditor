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

	unsigned r()const;
	unsigned g()const;
	unsigned b()const;

	static unsigned getMaxValue();
private:
	unsigned red;
	unsigned green;
	unsigned blue;

	static const unsigned maxValue = 255;
};


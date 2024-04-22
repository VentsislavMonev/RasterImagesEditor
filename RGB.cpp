#include "RGB.h"

RGB::RGB() :red(0), green(0), blue(0) {}

RGB::RGB(unsigned _red, unsigned _green, unsigned _blue)
{
    setRed(_red);
    setGreen(_green);
    setBlue(_blue);
}

unsigned RGB::getSum()const
{
    return red + green + blue;
}

unsigned RGB::getAverage() const
{
    return getSum() / 3;
}


// da napravq setovete s int i da prihvashtam ako mi se podava -
void RGB::setRed(unsigned _red)
{
    if (_red > 255) throw std::invalid_argument("Can have value for red only in between 0-255!");
    else red = _red;
}

void RGB::setGreen(unsigned _green)
{
    if (_green > 255) throw std::invalid_argument("Can have value for green only in between 0-255!");
    else green = _green;
}

void RGB::setBlue(unsigned _blue)
{
    if (_blue > 255) throw std::invalid_argument("Can have value for blue only in between 0-255!");
    else blue = _blue;
}

unsigned char RGB::r() const
{
    return red;
}

unsigned char RGB::g() const
{
    return green;
}

unsigned char RGB::b() const
{
    return blue;
}

unsigned RGB::getMaxValue()
{
    return maxValue;
}

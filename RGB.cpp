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

void RGB::setRed(unsigned _red)
{
    if (_red > 255) throw std::invalid_argument("Cant have a red value greater than 255!");
    else red = _red;
}

void RGB::setGreen(unsigned _green)
{
    if (_green > 255) throw std::invalid_argument("Cant have a green value greater than 255!");
    else green = _green;
}

void RGB::setBlue(unsigned _blue)
{
    if (_blue > 255) throw std::invalid_argument("Cant have a blue value greater than 255!");
    else blue = _blue;
}

unsigned RGB::r() const
{
    return red;
}

unsigned RGB::g() const
{
    return green;
}

unsigned RGB::b() const
{
    return blue;
}

unsigned RGB::getMaxValue()
{
    return maxValue;
}

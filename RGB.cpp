#include "RGB.h"

RGB::RGB() :red(0), green(0), blue(0) {}

RGB::RGB(int _red, int _green, int _blue)
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
    //RGB have 3 colours
    return getSum() / 3;
}

void RGB::setEqual(int value)
{
    setRed(value);
    setEqualToRed();
}

void RGB::setEqualToRed()
{
    green = blue = red;
}

void RGB::setEqualToGreen()
{
    blue = red = green;
}

void RGB::setEqualToBlue()
{
    red = green = blue;
}


void RGB::setRed(int _red)
{
    if (_red > maxValue)    throw std::invalid_argument("Value for red can only be less than or equal to 255!");
    else if (_red < 0) throw std::invalid_argument("Value for red can only be more than or equal to 0!");
    else red = _red;
}

void RGB::setGreen(int _green)
{
    if (_green > maxValue)    throw std::invalid_argument("Value for green can only be less than or equal to 255!");
    else if (_green < 0) throw std::invalid_argument("Value for green can only be more than or equal to 0!");
    else green = _green;
}

void RGB::setBlue(int _blue)
{
    if (_blue > maxValue)    throw std::invalid_argument("Value for blue can only be less than or equal to 255!");
    else if (_blue < 0) throw std::invalid_argument("Value for blue can only be more than or equal to 0!");
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

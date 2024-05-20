#pragma once
#include <iostream>
#include <string>
int  getNumb(const std::string& str)
{
	size_t i = 0;
	int sign = 1;
	int number = 0;
	if (str[i] == '-')
	{
		sign = -1;
		++i;
	}
	for (i; i < str.size(); ++i)
	{
		if (str[i] < '0' || str[i]>'9') throw std::invalid_argument("Invalid input!");
		number = number * 10 + (str[i] - '0');
	}
	number = number * sign;
	return number;
}

std::string getString(int numb)
{
	std::string result = "";
	bool isNegative = false;
	if (numb < 0)
		isNegative = true;

	do
	{
		char digit = '0' + numb % 10;
		numb /= 10;
		result = digit + result;
	} while (numb > 0);

	if (isNegative)
	{
		result = '-' + result;
	}
	return result;
}
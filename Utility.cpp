#include "Utility.h"

int getNumb(const std::string& str)
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

std::string getModifiedTime()
{
	time_t time = std::time(0);
	tm date;
	localtime_s(&date, &time);
	std::string modifiedTime = getString(date.tm_year + 1900) + getString(date.tm_mon + 1) + getString(date.tm_mday) +
		getString(date.tm_hour) + getString(date.tm_min) + getString(date.tm_sec);
	return modifiedTime;
}
#include "Image.h"

Image::Image()
{
	format = ImageProcesing::ImageType::defaultType;
	width = length = 0;
}

Image::Image(ImageProcesing::ImageType _format, int _width, int _length)
{
	setFormat(_format);
	setWidth(_width);
	setLength(_length);
}

void Image::setFormat(ImageProcesing::ImageType _format)
{
	if (_format != ImageProcesing::ImageType::defaultType)
	{
		format = _format;
	}
	else throw std::runtime_error("Invalid format for iamge!");
}

//taq proverka nujna li e
void Image::addCommand(ImageProcesing::Commands command)
{
	if (command == ImageProcesing::Commands::defaultCommand)
		throw std::invalid_argument("There isn`t such a command!");
	commandsToDo.push_back(command);
	if(!commandsToNotDo.empty())
	{
		commandsToNotDo.clear();
	}
}

void Image::redo()
{
	if (commandsToNotDo.empty())
		return;
	ImageProcesing::Commands command = commandsToNotDo.back();
	commandsToDo.push_back(command);
	commandsToNotDo.pop_back();
}

void Image::undo()
{
	if (commandsToDo.empty())
		return;
	ImageProcesing::Commands command = commandsToDo.back();
	commandsToNotDo.push_back(command);
	commandsToDo.pop_back();
}

void Image::setWidth(int _width)
{
	if (_width < 0)
		throw std::invalid_argument("Width can`t be of a negative value!");
	else if (_width > std::numeric_limits<unsigned short>::max())
		throw std::invalid_argument("Width is too big!");
	else
		width = _width;
}

void Image::setLength(int _length)
{
	if (_length < 0)
		throw std::invalid_argument("Length can`t be of a negative value!");
	else if (_length > std::numeric_limits<unsigned short>::max())
		throw std::invalid_argument("Length is too big!");
	else
		length = _length;
}

ImageProcesing::ImageType Image::getFormat() const
{
	return format;
}

unsigned short Image::getWidth() const
{
	return width;
}

unsigned short Image::getLength() const
{
	return length;
}

const std::vector<ImageProcesing::Commands>& Image::getCommandsToDo() const
{
	return commandsToDo;
}

const std::vector<ImageProcesing::Commands>& Image::getCommandsToNotDo() const
{
	return commandsToNotDo;
}

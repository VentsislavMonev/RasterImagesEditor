#include "Image.h"
#include "Utility.h"

Image::Image()
{
	format = ImageProcesing::ImageType::defaultType;
	width = length = 0;
}

Image::Image(const std::string& _file)
{
	std::ifstream input(_file);
	if (!input)
		throw std::runtime_error("File is bad!");

	setFileName(_file);
	setFormat(_file);
	
	std::string inputWidthTxt;
	std::string inputLengthTxt;
	std::string skip;

	input >> skip >> inputWidthTxt >> inputLengthTxt;

	setWidth(getNumb(inputWidthTxt));
	setLength(getNumb(inputLengthTxt));
}

void Image::setFormat(const std::string& _format)
{
	if (_format == ".ppm")
		format = ImageProcesing::ImageType::P3;
	else if (_format == ".pgm")
		format = ImageProcesing::ImageType::P2;
	else if (_format == ".pbm")
		format = ImageProcesing::ImageType::P1;
	else throw std::invalid_argument("Invalid image type");
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

void Image::setFileName(const std::string& _fileName)
{
	if (!_fileName.c_str())
	{
		throw std::invalid_argument("Invalid file name!");
	}

	size_t fileNameSize = _fileName.size()-4;
	for (size_t i = 0; i < fileNameSize; ++i)
	{
		if ((_fileName[i] < 'A' || _fileName[i] > 'Z') &&
			(_fileName[i] < 'a' || _fileName[i]> 'z') &&
			(_fileName[i] < '0' || _fileName[i] >'9'&&
			_fileName[i] !='.'&& _fileName[i] != '_'))
		{
			throw std::invalid_argument("Inavalid file name!");
		}
	}
	fileName = _fileName;
	fileName.erase(fileName.size() - 4);
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

const std::string& Image::getFileName() const
{
	return fileName;
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

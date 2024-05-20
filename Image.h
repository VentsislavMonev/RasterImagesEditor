#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

namespace ImageProcesing
{
	enum class ImageType
	{
		P1,
		P2,
		P3,
		defaultType
	};

	enum class Commands
	{
		grayscale,
		monochrome,
		negative,
		rotateLeft,
		rotateRight,
		flipTop,
		flipLeft,
		crop,
		defaultCommand
	};
}

class Image
{
public:
	Image();
	Image(const std::string& _file);
	virtual ~Image() = default;

//getters
public:
	const std::string& getFileName()const;
	ImageProcesing::ImageType getFormat()const;
	unsigned short getWidth()const;
	unsigned short getLength()const;
	const std::vector<ImageProcesing::Commands>& getCommandsToDo()const;
	const std::vector<ImageProcesing::Commands>& getCommandsToNotDo()const;

//methods
public:
	virtual void grayScale()  = 0; // da ostavq tazi funkciq samo v PPM
	virtual void monochrome() = 0;
	virtual void negative() = 0;

	virtual void rotateLeft() = 0;
	virtual void rotateRight()= 0;
	virtual void flipTop()	  = 0;
	virtual void flipLeft()   = 0;
 
    //virtual void crop(int topLeftX, int topLeftY, int botRightX, int botLeftY);

	void redo();
	void undo();

protected:
	void addCommand(ImageProcesing::Commands command);

	void setFormat(const std::string& _format);

//setters
private:
	void setFileName(const std::string& _fileName);
	void setWidth(int _width);
	void setLength(int _length);

//members
private:
	std::string fileName;
	ImageProcesing::ImageType format;
	unsigned short width;
	unsigned short length;
	std::vector<ImageProcesing::Commands> commandsToDo;
	std::vector<ImageProcesing::Commands> commandsToNotDo;
};


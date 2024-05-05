#pragma once
#include <iostream>
#include <vector>

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
	Image(ImageProcesing::ImageType _format, int _width, int _length);
	virtual ~Image() = default;

//getters
public:
	ImageProcesing::ImageType getFormat()const;
	unsigned short getWidth()const;
	unsigned short getLength()const;
	const std::vector<ImageProcesing::Commands>& getCommandsToDo()const;
	const std::vector<ImageProcesing::Commands>& getCommandsToNotDo()const;

//methods
public:
	virtual bool grayScale()  = 0;
	virtual bool monochrome() = 0;
	virtual bool negative()   = 0;

	virtual bool rotateLeft() = 0;
	virtual bool rotateRight()= 0;
	virtual bool flipTop()	  = 0;
	virtual bool flipLeft()   = 0;
 
    //virtual void crop(int topLeftX, int topLeftY, int botRightX, int botLeftY);

	void addCommand(ImageProcesing::Commands command);
	void redo();
	void undo();

//setters
protected:
	void setWidth(int _width);
	void setLength(int _length);

private:
	void setFormat(ImageProcesing::ImageType _format);

//members
private:
	ImageProcesing::ImageType format;
	unsigned short width;
	unsigned short length;
	std::vector<ImageProcesing::Commands> commandsToDo;
	std::vector<ImageProcesing::Commands> commandsToNotDo;
};


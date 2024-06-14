#pragma once
#include <vector>
#include <string>

const short sides = 4;

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
		monochrome,
		grayscale,
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

	virtual Image* clone() const = 0;

//getters
public:
	const std::string& getFileName()const;
	ImageProcesing::ImageType getFormat()const;
	ImageProcesing::ImageType getFormat(const std::string& _format)const;
	unsigned short getWidth()const;
	unsigned short getLength()const;

	bool isChanged()const;

//methods
public:
	virtual void grayScale()  = 0;
	virtual void monochrome() = 0;
	virtual void negative()   = 0;

	virtual void rotateLeft() = 0;
	virtual void rotateRight()= 0;
	virtual void flipTop()	  = 0;
	virtual void flipLeft()   = 0;
 
    virtual bool crop(int& topLeftX, int& topLeftY, int& botRightX, int& botLeftY) = 0;

	void addCommand(ImageProcesing::Commands command);
	void redo();
	void undo();

	virtual void print()const = 0;
	virtual void save() = 0;
	virtual void save(const std::string& newName) = 0;

//matrix manipulation functions
protected:
	virtual void transposeMatrix() = 0;
	virtual void reverseColumns()  = 0;
	virtual void reverseRows()     = 0;

protected:
	virtual void setFileName(const std::string& _fileName);
	void setWidth(int _width);
	void setLength(int _length);

	const std::vector<ImageProcesing::Commands>& getCommandsToDo()const;
	const std::vector<ImageProcesing::Commands>& getCommandsToNotDo()const;

	void skipComments(std::ifstream& file)const;

	void manageCommands();
	void getModifiedFile(std::string& _modifiedFileName)const;

//setters
private:
	virtual void setFormat(const std::string& _format);
	bool validateCoordinates(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY)const;

	void manageRotations(int rotationsLeft, int rotationsRight);
	void manageFlips(int flipsTop, int flipsLeft);

//members
private:
	std::string fileName;
	ImageProcesing::ImageType format;
	unsigned short width;
	unsigned short length;
	std::vector<ImageProcesing::Commands> commandsToDo;
	std::vector<ImageProcesing::Commands> commandsToNotDo;
};


#pragma once

#include <vector>
#include "RGB.h"
#include "Image.h"

class PPM:public Image
{
public:
	PPM() = default;
	PPM(const std::string& _fileName);
	virtual ~PPM() = default;

	//getters
public:
	const std::vector<std::vector<RGB>>& getMatrix()const;
	unsigned char getMaxValue()const;

	virtual void save()const override;

	//functions
public:
	virtual void grayScale() override;
	virtual void monochrome() override;
	virtual void negative() override;

	virtual void rotateLeft() override;
	virtual void rotateRight() override;
	virtual void flipTop() override;
	virtual void flipLeft() override;

	//virtual void crop(int topLeftX, int topLeftY, int botRightX, int botRightY) override;

	//helper protected functions 
protected:
	void transposeMatrix();
	void reverseColumns();
	void reverseRows();
	//setters
private:
	void setMatrix(std::ifstream& input);
	void setMaxValue(int _maxValue);
	virtual void setFormat(const std::string& _format) override;
	//helper private functions
private:
	void getModifiedFile(std::string& _modifiedFileName)const;
	void writeFileHeader(std::ofstream& newImage, unsigned short _width, unsigned short _length)const;
	void writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length)const;

private:
	std::vector<std::vector<RGB>> pixels;
	unsigned char maxValue;
};


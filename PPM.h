#pragma once

#include "RGB.h"
#include "Image.h"

class PPM:public Image
{
public:
	PPM() = default;
	PPM(const std::string& _file);
	virtual ~PPM() = default;

	virtual Image* clone() const override;

	//getters
public:
	const std::vector<std::vector<RGB>>& getMatrix()const;
	unsigned char getMaxValue()const;

	//functions
public:
	virtual void monochrome()  override;
	virtual void grayScale()   override;
	virtual void negative()	   override;

	virtual void rotateLeft()  override;
	virtual void rotateRight() override;
	virtual void flipTop()	   override;
	virtual void flipLeft()	   override;

	virtual bool crop(int topLeftX, int topLeftY, int botRightX, int botRightY)override;

	virtual void save() override;

	//helper protected functions 
protected:
	virtual void transposeMatrix() override;
	virtual void reverseColumns()  override;
	virtual void reverseRows()	   override;

	//setters
private:
	void setMatrix(std::ifstream& newImage);
	void setMaxValue(int _maxValue);
	virtual void setFormat(const std::string& _format) override;

	//helper private functions
private:
	void writeFileHeader(std::ofstream& newImage)const;
	void writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length)const;

private:
	std::vector<std::vector<RGB>> pixels;
	unsigned char maxValue;
};


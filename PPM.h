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

public:
	// set-er za matricata

	const std::vector<std::vector<RGB>>& getMatrix()const;
	unsigned char getMaxValue()const;
	void save()const;

public:
	virtual void grayScale() override;
	virtual void monochrome() override;
	virtual void negative() override;

	virtual void rotateLeft() override;
	virtual void rotateRight() override;
	virtual void flipTop() override;
	virtual void flipLeft() override;

	//virtual void crop(int topLeftX, int topLeftY, int botRightX, int botRightY) override;
protected:
	void transposeMatrix();
	void reverseColumns();
	void reverseRows();
private:
	virtual void setFileName(const std::string& _fileName) override;
	void checkLastFour(const std::string& _fileName)const;
	void setMatrix(std::ifstream& input);
	void setMaxValue(int _maxValue);
	void setFileHeader(std::string& inputFormatTxt, std::string& inputWidthTxt, std::string& inputLengthTxt, std::string& inputMaxValueTxt);
	int  getNumb(std::string& str);
private:
	std::vector<std::vector<RGB>> pixels;
	unsigned char maxValue;
};


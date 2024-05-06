#pragma once

#include <vector>
#include "RGB.h"
#include "Image.h"

class PPM:public Image
{
public:
	PPM();
	//da vidq kak shte podavam matricata
	PPM(ImageProcesing::ImageType _format, int _width, int _length, int _maxValue);
	virtual ~PPM() = default;

public:
	// set-er za matricata
	void setMaxValue(int _maxValue);

	const std::vector<std::vector<RGB>>& getMatrix()const;
	short getMaxValue()const;

public:
	virtual bool grayScale() override;
	virtual bool monochrome() override;
	virtual bool negative() override;

	virtual bool rotateLeft() override;
	virtual bool rotateRight() override;
	virtual bool flipTop() override;
	virtual bool flipLeft() override;

	//virtual void crop(int topLeftX, int topLeftY, int botRightX, int botRightY) override;
protected:
	void transposeMatrix();
	void reverseColumns();
	void reverseRows();
private:
	std::vector<std::vector<RGB>> pixels;
	unsigned char maxValue;
};


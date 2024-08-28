#pragma once
#include "Image.h"

class PBM:public Image
{
public:
	PBM()=default;
	PBM(const  std::string& _file);
	virtual ~PBM()=default;

	virtual Image* clone() const override;

	//functions
public:
	virtual void monochrome() override {};
	virtual void grayScale() override {}
	virtual void negative() override;

	virtual void rotateLeft() override;
	virtual void rotateRight() override;
	virtual void flipTop() override;
	virtual void flipLeft() override;

	virtual bool crop(int topLeftX, int topLeftY, int botRightX, int botRightY)override;

	virtual void print()const override;
	virtual void save() override;
	virtual void save(const std::string& newName);

	//getters
public:
	const std::vector<std::vector<bool>>& getMatrix()const;
	static bool getMaxValue();

	//helper protected functions 
protected:
	virtual void transposeMatrix()  override;
	virtual void reverseColumns()   override;
	virtual void reverseRows()      override;

	//setters
private:
	virtual void setFileName(const std::string& _fileName);
	virtual void setFormat(const std::string& _format) override;
	void setMatrix(std::ifstream& newImage);

	bool setValue(int _value);

	//helper private functions
private:
	void writeFileHeader(std::ofstream& newImage)const;
	void writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length)const;

	//members
private:
	//vector ot bool mnogo zle !
	//	std::vector<bool> vec;
	//	bool& boolean = vec[0];

	std::vector<std::vector<bool>> pixels;
	static const bool maxValue = 1;
};

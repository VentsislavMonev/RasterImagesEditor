#pragma once

#include"Image.h"

class PGM: public Image
{
public:
	PGM() = default;
	PGM(const std::string& _file);
	virtual ~PGM()=default;

	virtual Image* clone() const override;

	//getters
public:
	const std::vector<std::vector<unsigned char>>& getMatrix()const;
	unsigned char getMaxValue()const;

	//functions
public:
	virtual void monochrome() override;
	virtual void grayScale() override {}
	virtual void negative() override;

	virtual void rotateLeft() override;
	virtual void rotateRight() override;
	virtual void flipTop() override;
	virtual void flipLeft() override;

	virtual bool crop(int& topLeftX, int& topLeftY, int& botRightX, int& botRightY)override;

	virtual void print()const override;
	virtual void save() override;
	virtual void save(const std::string& newName);

	//helper protected functions 
protected:
	virtual void transposeMatrix() override;
	virtual void reverseColumns()   override;
	virtual void reverseRows()      override;


	//setters
private:
	virtual void setFileName(const std::string& _fileName);
	void setMatrix(std::ifstream& newImage);
	void setMaxValue(int _maxValue);
	virtual void setFormat(const std::string& _format) override;
	unsigned char setValue(int _value);

	//helper private functions
private:
	void writeFileHeader(std::ofstream& newImage)const;
	void writeMatrix(std::ofstream& newImage, unsigned short _width, unsigned short _length)const;

	//members
private:
	std::vector<std::vector<unsigned char>> pixels;
	unsigned char maxValue;
};


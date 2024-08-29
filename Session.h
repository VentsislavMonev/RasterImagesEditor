#pragma once

#include "Image.h"

class Session
{
	// big 4
public:
	Session();
	Session(const Session& other);
	Session& operator=(const Session& other);
	~Session();

	//functions
public:
	void addFile(const std::string& fileName);
	void addCommand(ImageProcesing::Commands command);
	void crop(const std::string& topLeftX, const std::string& topLeftY, const std::string& botRightX, const std::string& botRightY);
	void undo();
	void redo();
	void save();
	void saveas(const std::vector<std::string>& newNames);
	void listSession()const;
	void exit();

	//getters
public:
	size_t getSize()const;
	unsigned getId()const;
	Image* getImage(const std::string& fileName)const;

	//make collage
public:
	void makeHorizontalCollage(const std::string& firstImage, const std::string& secondImage);
	void makeVerticalCollage(const std::string& firstImage, const std::string& secondImage);

private:
	void checkIfImagesForCollageInSession(int& index1, int& index2, const std::string& firstImage, const std::string& secondImage)const;

	//private helper functions
private:
	void addPPM(const std::string& image);
	void addPGM(const std::string& image);
	void addPBM(const std::string& image);
	void addImage(Image* other);
	
	bool checkIfImageExist(const std::string& fileName)const;
	bool areChanged()const;

	//setters
private:
	void setID();

// TODO otdelen klas za Image** images
	//members
private:	
	Image** images;
	size_t size = 0;
	size_t capacity = 8;
	unsigned id;

	void free();
	void copyFrom(const Session& other);
	void resize();
};



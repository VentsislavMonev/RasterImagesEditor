#pragma once

#include"Image.h"

namespace SessionProcesing
{
	enum class commands
	{
		load,
		grayscale,
		monochrome,
		negative,
		rotateLeft,
		rotateRight,
		flipTop,
		flipLeft,
		undo,
		redo,
		add,
		crop,
		save,
		saveas,
		listSession,
		makeCollage,
		exit
	};
}

class Session
{
public:
	Session();
	Session(const Session& other);
	Session& operator=(const Session& other);
	~Session();

public:
	void addFile(const std::string& fileName);
	void addCommand(ImageProcesing::Commands command);
	void undo();
	void redo();
	void save();
	void listSession()const;
	void exit();

	size_t getSize()const;

private:
	void addPPM(const std::string& image);
	void addPGM(const std::string& image);
	void addPBM(const std::string& image);
	void addImage(Image* other);
	void checkIfImageExist(const std::string& fileName)const;
	bool areChanged()const;

private:	
	Image** images;
	size_t size = 0;
	size_t capacity = 8;
	static unsigned id;

	void free();
	void copyFrom(const Session& other);
	void resize();
};



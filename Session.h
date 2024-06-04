#pragma once

#include"Image.h"

class Session
{
public:
	Session();
	Session(const Session& other);
	Session& operator=(const Session& other);
	~Session();

public:
	void addPPM(const std::string& image);
	void addPGM(const std::string& image);
	void addPBM(const std::string& image);

	void addCommand(ImageProcesing::Commands command);
	void undo();
	void redo();
	void saveAll();

	size_t getSize()const;

private:	
	Image** images;
	size_t size = 0;
	size_t capacity = 8;
	static unsigned id;

	void free();
	void copyFrom(const Session& other);
	void resize();
	
	void addImage(Image* other);
};



#include <iostream>

#include "Session.h"

#include "PPM.h"
#include "PGM.h"
#include "PBM.h"

unsigned Session::id = 0;

Session::Session()
{
    images = new Image * [capacity] {nullptr};
    std::cout << "Session with ID: " << ++id << " started" << std::endl;
}

Session::Session(const Session& other)
{
    copyFrom(other);
}

Session& Session::operator=(const Session& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

Session::~Session()
{
    free();
}

void Session::addPPM(const std::string& image)
{
    addImage(new PPM(image));
}

void Session::addPGM(const std::string& image)
{
    addImage(new PGM(image));
}

void Session::addPBM(const std::string& image)
{
    addImage(new PBM(image));
}

void Session::addCommand(ImageProcesing::Commands command)
{
    for (size_t i = 0; i < size; i++)
    {
        images[i]->addCommand(command);
    }
}

void Session::undo()
{
    for (size_t i = 0; i < size; i++)
    {
        images[i]->undo();
    }
}

void Session::redo()
{
    for (size_t i = 0; i < size; i++)
    {
        images[i]->redo();
    }
}

void Session::saveAll()
{
    for (size_t i = 0; i < size; i++)
    {
        images[i]->save();
    }
}

size_t Session::getSize() const
{
    return size;
}

void Session::free()
{
    for (size_t i = 0; i < size; i++)
    {
        delete images[i];
    }
    delete[] images;
}

void Session::copyFrom(const Session& other)
{
    size = other.size;
    capacity = other.capacity;
    id = other.id;
    
    images = new Image * [capacity];
    for (size_t i = 0; i < size; i++)
    {
        images[i] = other.images[i]->clone();
    }
}

void Session::resize()
{
    capacity *= capacity * 2;
    Image** resized = new Image * [capacity];
    for (size_t i = 0; i < size; i++)
    {
        resized[i] = images[i];
    }
    delete[] images;
    images = resized;
}

void Session::addImage(Image* other)
{

    if (size >=  capacity)
        resize();
    images[size++] = other;
}

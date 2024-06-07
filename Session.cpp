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

void Session::addFile(const std::string& fileName)
{
    checkIfImageExist(fileName);
    std::string format = fileName.substr(fileName.size() - 4);
    if (format == ".ppm")
        addPPM(fileName);
    else if (format == ".pgm")
        addPGM(fileName);
    else if (format == ".pbm")
        addPBM(fileName);
    else std::cout << "\""<< fileName<<"\" " <<"is not supported and wasnt added to the session" << std::endl;
}

void Session::addPPM(const std::string& image)
{
    try 
    {
        addImage(new PPM(image));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
}

void Session::addPGM(const std::string& image)
{
    try
    {
        addImage(new PGM(image));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
}

void Session::addPBM(const std::string& image)
{
    try
    {
        addImage(new PBM(image));
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
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

void Session::save()
{
    for (size_t i = 0; i < size; i++)
    {
        images[i]->save();
    }
}

void Session::listSession() const
{
    std::cout << "Files in session with ID " << id << ":" << std::endl;
    for (size_t i = 0; i < size; i++)
    {
        images[i]->print();
    }
}

void Session::exit()
{
    std::string answer;
    if (areChanged())
    {
        std::cout << "Session contains unsaved work. Save it now (y/n)?";
        do
        {
            std::cin >> answer;
        } while (answer != "y" && answer != "yes" && answer != "n" && answer != "no");
        if (answer == "y" || answer == "yes")
        {
            save();
        }
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

void Session::checkIfImageExist(const std::string& fileName) const
{
    std::string file = fileName.substr(0, fileName.size() - 4);
    for (size_t i = 0; i < size; i++)
    {
        if (images[i]->getFileName() == file)
        {
            std::cout << "Image:" << file << " already in session" << std::endl;
            return;
        }
    }
}

bool Session::areChanged() const
{
    for (size_t i = 0; i < size; i++)
    {
        if (images[i]->isChanged())
            return true;
    }
    return false;
}

void Session::addImage(Image* other)
{
    if (size >=  capacity)
        resize();
    images[size++] = other;
}

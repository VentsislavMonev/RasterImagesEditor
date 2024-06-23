#include <iostream>

#include "Session.h"

#include "PPM.h"
#include "PGM.h"
#include "PBM.h"
#include "Utility.h"

namespace CollageFunctions
{
    void writeCollageHorizontalMatrixPPM(unsigned short newWidth, unsigned short newLength, const PPM& image1, const PPM& image2, std::ofstream& newImage)
    {
        unsigned short difference1 = (newLength - image1.getLength()) / 2;
        unsigned short difference2 = (newLength - image2.getLength()) / 2;

        int x2 = 0;
        for (int y = 0; y < newLength; ++y) {
            for (int x = 0; x < newWidth; ++x) {
                if (x < image1.getWidth())
                {
                    // writing pixels from the first image
                    if (y >= difference1 && y < difference1 + image1.getLength())
                    {
                        newImage << static_cast<unsigned>(image1.getMatrix()[y - difference1][x].r()) << ' '
                                 << static_cast<unsigned>(image1.getMatrix()[y - difference1][x].g()) << ' '
                                 << static_cast<unsigned>(image1.getMatrix()[y - difference1][x].b()) << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
                else
                {
                    // Writing pixels from img2
                    x2 = x - image1.getWidth();
                    if (y >= difference2 && y < difference2 + image2.getLength())
                    {
                        newImage << static_cast<unsigned>(image2.getMatrix()[y - difference2][x2].r()) << ' '
                                 << static_cast<unsigned>(image2.getMatrix()[y - difference2][x2].g()) << ' '
                                 << static_cast<unsigned>(image2.getMatrix()[y - difference2][x2].b()) << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
            }
            newImage << std::endl;
        }
    }
    void writeCollageHorizontalMatrixPGM(unsigned short newWidth, unsigned short newLength, const PGM& image1, const PGM& image2, std::ofstream& newImage)
    {
        unsigned short difference1 = (newLength - image1.getLength()) / 2;
        unsigned short difference2 = (newLength - image2.getLength()) / 2;

        int x2 = 0;
        for (int y = 0; y < newLength; ++y) {
            for (int x = 0; x < newWidth; ++x) {
                if (x < image1.getWidth())
                {
                    // writing pixels from the first image
                    if (y >= difference1 && y < difference1 + image1.getLength())
                        newImage << static_cast<unsigned>(image1.getMatrix()[y - difference1][x]) << ' ';
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
                else
                {
                    // Writing pixels from img2
                    x2 = x - image1.getWidth();
                    if (y >= difference2 && y < difference2 + image2.getLength())
                        newImage << static_cast<unsigned>(image2.getMatrix()[y - difference2][x2]) << ' ';
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
            }
            newImage << std::endl;
        }
    }
    void writeCollageHorizontalMatrixPBM(unsigned short newWidth, unsigned short newLength, const PBM& image1, const PBM& image2, std::ofstream& newImage)
    {
        unsigned short difference1 = (newLength - image1.getLength()) / 2;
        unsigned short difference2 = (newLength - image2.getLength()) / 2;

        int x2 = 0;
        for (int y = 0; y < newLength; ++y) {
            for (int x = 0; x < newWidth; ++x) {
                if (x < image1.getWidth())
                {
                    // writing pixels from the first image
                    if (y >= difference1 && y < difference1 + image1.getLength())
                        newImage << image1.getMatrix()[y - difference1][x] << ' ';
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
                else
                {
                    // Writing pixels from img2
                    x2 = x - image1.getWidth();
                    if (y >= difference2 && y < difference2 + image2.getLength())
                        newImage << image2.getMatrix()[y - difference2][x2] << ' ';
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
            }
            newImage << std::endl;
        }
    }

    void writeCollageVerticalMatrixPPM(unsigned short newWidth, unsigned short newLength, const PPM& image1, const PPM& image2, std::ofstream& newImage)
    {
        int y2 = 0;
        for (int y = 0; y < newLength; ++y) 
        {
            for (int x = 0; x < newWidth; ++x) 
            {
                if (y < image1.getLength())
                {
                    // Writing pixels from image1
                    if (x < image1.getWidth())
                    {
                        newImage << static_cast<unsigned>(image1.getMatrix()[y][x].r()) << ' '
                                 << static_cast<unsigned>(image1.getMatrix()[y][x].g()) << ' '
                                 << static_cast<unsigned>(image1.getMatrix()[y][x].b()) << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
                else
                {
                    // Writing pixels from image2
                    y2 = y - image1.getLength();
                    if (x < image2.getWidth()) 
                    {
                        newImage << static_cast<unsigned>(image2.getMatrix()[y2][x].r()) << ' '
                                 << static_cast<unsigned>(image2.getMatrix()[y2][x].g()) << ' '
                                 << static_cast<unsigned>(image2.getMatrix()[y2][x].b()) << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
            }
        }
    }
    void writeCollageVerticalMatrixPGM(unsigned short newWidth, unsigned short newLength, const PGM& image1, const PGM& image2, std::ofstream& newImage)
    {
        int y2 = 0;
        for (int y = 0; y < newLength; ++y)
        {
            for (int x = 0; x < newWidth; ++x)
            {
                if (y < image1.getLength())
                {
                    // Writing pixels from image1
                    if (x < image1.getWidth())
                    {
                        newImage << static_cast<unsigned>(image1.getMatrix()[y][x]) << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
                else
                {
                    // Writing pixels from image2
                    y2 = y - image1.getLength();
                    if (x < image2.getWidth())
                    {
                        newImage << static_cast<unsigned>(image2.getMatrix()[y2][x]) << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
            }
        }
    }
    void writeCollageVerticalMatrixPBM(unsigned short newWidth, unsigned short newLength, const PBM& image1, const PBM& image2, std::ofstream& newImage)
    {
        int y2 = 0;
        for (int y = 0; y < newLength; ++y)
        {
            for (int x = 0; x < newWidth; ++x)
            {
                if (y < image1.getLength())
                {
                    // Writing pixels from image1
                    if (x < image1.getWidth())
                    {
                        newImage << image1.getMatrix()[y][x] << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
                else
                {
                    // Writing pixels from image2
                    y2 = y - image1.getLength();
                    if (x < image2.getWidth())
                    {
                        newImage << image2.getMatrix()[y2][x] << ' ';
                    }
                    else
                        newImage << 0 << ' ' << 0 << ' ' << 0 << ' ';
                }
            }
        }
    }
}

Session::Session()
{
    images = new Image * [capacity] {nullptr};
    setID();
    std::cout << "Session with ID: " << id << " started" << std::endl;
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
    if (fileName.size() > 4 && !checkIfImageExist(fileName))
    {
        std::string format = fileName.substr(fileName.size() - 4);
        if (format == ".ppm")
            addPPM(fileName);
        else if (format == ".pgm")
            addPGM(fileName);
        else if (format == ".pbm")
            addPBM(fileName);
        else std::cout << "\"" << fileName << "\" " << " type is not supported and wasnt added to the session" << std::endl;
    }
    else std::cout << fileName + " wasn`t added" << std::endl;
}

void Session::setID()
{
    id = getCurrentTime();
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
        std::cout << " " + image + " wasnt added!" << std::endl;
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
        std::cout << " " + image + " wasnt added!" << std::endl;
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
        std::cout << " " + image + " wasnt added!" << std::endl;
    }
}

void Session::addCommand(ImageProcesing::Commands command)
{
    for (size_t i = 0; i < size; ++i)
        images[i]->addCommand(command);
}

void Session::crop(const std::string& topLeftX, const std::string& topLeftY, const std::string& botRightX, const std::string& botRightY)
{
    int topLX = getNumb(topLeftX);
    int topLY = getNumb(topLeftY);
    int botRX = getNumb(botRightX);
    int botRY = getNumb(botRightY);
    for (size_t i = 0; i < size; ++i)
        images[i]->crop(topLX, topLY, botRX, botRY);
}

void Session::undo()
{
    for (size_t i = 0; i < size; ++i)
        images[i]->undo();
}

void Session::redo()
{
    for (size_t i = 0; i < size; ++i)
        images[i]->redo();
}

void Session::save()
{
    for (size_t i = 0; i < size; ++i)
        images[i]->save();
}

void Session::saveas(const std::vector<std::string>& newNames)
{
    size_t sizeNames = newNames.size()-1;
    if (sizeNames > size)
        throw std::invalid_argument("Too many arguments the images weren`t saved");

    for (size_t i = 0; i < sizeNames; ++i)
    {
        try
        {
            images[i]->save(newNames[i + 1]);
        }
        catch (const std::exception& e)
        {
            std::cout << e.what();
            std::cout << " " + images[i]->getFileName() + " wasn`t saved!" << std::endl;
        }
    }
}

void Session::listSession() const
{
    if(size==0)
        std::cout << "Session with ID " << id << " is empty" << std::endl;
    else
    {
        std::cout << "Files in session with ID " << id << ":" << std::endl;
        for (size_t i = 0; i < size; i++)
            images[i]->print();
    }
}


void Session::makeHorizontalCollage(const std::string& firstImage, const std::string& secondImage)
{
    try
    {
        checkIfImagesForCollageInSession(firstImage, secondImage);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "A collage wasn`t made!" << std::endl;
        return;
    }

    std::string format = firstImage.substr(firstImage.size() - 4);
    if (format == ".ppm")
        makeHorizontalCollagePPM(firstImage, secondImage);
    else if (format == ".pgm")
        makeHorizontalCollagePGM(firstImage, secondImage);
    else if (format == ".pbm")
        makeHorizontalCollagePBM(firstImage, secondImage);
    
}


void Session::makeHorizontalCollagePPM(const std::string& firstImage, const std::string& secondImage)
{
    size_t index1 = 0;
    size_t index2 = 0;
    PPM image1(firstImage);
    PPM image2(secondImage);

    //gets file modified name
    std::string modifiedFile;
    getCollageName(modifiedFile, image1, image2);

    //opens file with given name
    std::ofstream newImage(modifiedFile);
    if (!newImage)
        throw std::runtime_error("Bad file!");

    // new file header
    unsigned short newWidth   = image1.getWidth() + image2.getWidth();
    unsigned short newLength  = std::max(image1.getLength(), image2.getLength());
    unsigned char newMaxValue = std::max(image1.getMaxValue(), image2.getMaxValue());

    writeCollageHeader(newWidth, newLength, image1, image2, newImage);
    newImage << static_cast<unsigned>(newMaxValue) << std::endl;

    //write matrix
    CollageFunctions::writeCollageHorizontalMatrixPPM(newWidth, newLength, image1, image2,newImage);
}

void Session::makeHorizontalCollagePGM(const std::string& firstImage, const std::string& secondImage)
{
    size_t index1 = 0;
    size_t index2 = 0;
    PGM image1(firstImage);
    PGM image2(secondImage);

    //gets file modified name
    std::string modifiedFile;
    getCollageName(modifiedFile, image1, image2);

    //opens file with given name
    std::ofstream newImage(modifiedFile);
    if (!newImage)
        throw std::runtime_error("Bad file!");

    // new file header
    unsigned short newWidth = image1.getWidth() + image2.getWidth();
    unsigned short newLength = std::max(image1.getLength(), image2.getLength());
    unsigned char newMaxValue = std::max(image1.getMaxValue(), image2.getMaxValue());

    writeCollageHeader(newWidth, newLength, image1, image2, newImage);
    newImage << static_cast<unsigned>(newMaxValue) << std::endl;

    //write matrix
    CollageFunctions::writeCollageHorizontalMatrixPGM(newWidth, newLength, image1, image2, newImage);

}

void Session::makeHorizontalCollagePBM(const std::string& firstImage, const std::string& secondImage)
{
    size_t index1 = 0;
    size_t index2 = 0;
    PBM image1(firstImage);
    PBM image2(secondImage);

    //gets file modified name
    std::string modifiedFile;
    getCollageName(modifiedFile, image1, image2);

    //opens file with given name
    std::ofstream newImage(modifiedFile);
    if (!newImage)
        throw std::runtime_error("Bad file!");

    // new file header
    unsigned short newWidth = image1.getWidth() + image2.getWidth();
    unsigned short newLength = std::max(image1.getLength(), image2.getLength());
    writeCollageHeader(newWidth, newLength, image1, image2, newImage);

    //write matrix
    CollageFunctions::writeCollageHorizontalMatrixPBM(newWidth, newLength, image1, image2, newImage);
}


void Session::makeVerticalCollage(const std::string& firstImage, const std::string& secondImage)
{
    try
    {
        checkIfImagesForCollageInSession(firstImage, secondImage);
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "A collage wasn`t made!" << std::endl;
        return;
    }

    std::string format = firstImage.substr(firstImage.size() - 4);
    if (format == ".ppm")
        makeVerticalCollagePPM(firstImage, secondImage);
    else if (format == ".pgm")
        makeVerticalCollagePGM(firstImage, secondImage);
    else if (format == ".pbm")
        makeVerticalCollagePBM(firstImage, secondImage);
}

void Session::makeVerticalCollagePPM(const std::string& firstImage, const std::string& secondImage)
{
    size_t index1 = 0;
    size_t index2 = 0;
    PPM image1(firstImage);
    PPM image2(secondImage);

    //gets file modified name
    std::string modifiedFile;
    getCollageName(modifiedFile, image1, image2);

    //opens file with given name
    std::ofstream newImage(modifiedFile);
    if (!newImage)
        throw std::runtime_error("Bad file!");

    // new file header
    unsigned short newWidth = std::max(image1.getWidth(), image2.getWidth());
    unsigned short newLength = image1.getLength() + image2.getLength();
    unsigned char newMaxValue = std::max(image1.getMaxValue(), image2.getMaxValue());

    writeCollageHeader(newWidth, newLength, image1, image2, newImage);
    newImage << static_cast<unsigned>(newMaxValue) << std::endl;

    //write matrix
    CollageFunctions::writeCollageVerticalMatrixPPM(newWidth, newLength, image1, image2, newImage);
}

void Session::makeVerticalCollagePGM(const std::string& firstImage, const std::string& secondImage)
{
    size_t index1 = 0;
    size_t index2 = 0;
    PGM image1(firstImage);
    PGM image2(secondImage);

    //gets file modified name
    std::string modifiedFile;
    getCollageName(modifiedFile, image1, image2);

    //opens file with given name
    std::ofstream newImage(modifiedFile);
    if (!newImage)
        throw std::runtime_error("Bad file!");

    // new file header
    unsigned short newWidth = std::max(image1.getWidth(), image2.getWidth());
    unsigned short newLength = image1.getLength() + image2.getLength();
    unsigned char newMaxValue = std::max(image1.getMaxValue(), image2.getMaxValue());

    writeCollageHeader(newWidth, newLength, image1, image2, newImage);
    newImage << static_cast<unsigned>(newMaxValue) << std::endl;

    //write matrix
    CollageFunctions::writeCollageVerticalMatrixPGM(newWidth, newLength, image1, image2, newImage);
}

void Session::makeVerticalCollagePBM(const std::string& firstImage, const std::string& secondImage)
{
    size_t index1 = 0;
    size_t index2 = 0;
    PBM image1(firstImage);
    PBM image2(secondImage);

    //gets file modified name
    std::string modifiedFile;
    getCollageName(modifiedFile, image1, image2);

    //opens file with given name
    std::ofstream newImage(modifiedFile);
    if (!newImage)
        throw std::runtime_error("Bad file!");

    // new file header
    unsigned short newWidth = std::max(image1.getWidth(), image2.getWidth());
    unsigned short newLength = image1.getLength() + image2.getLength();

    writeCollageHeader(newWidth, newLength, image1, image2, newImage);

    //write matrix
    CollageFunctions::writeCollageVerticalMatrixPBM(newWidth, newLength, image1, image2, newImage);
}

void Session::exit()
{
    std::string answer;
    if (areChanged())
    {
        std::cout << "Session with id : " << id << " contains unsaved work. Save it now (y/n)?" << std::endl;
        do
        {
            std::cin >> answer;
        } while (answer != "y" && answer != "yes" && answer != "n" && answer != "no");
        if (answer == "y" || answer == "yes")
            save();
    }
}

size_t Session::getSize() const
{
    return size;
}

unsigned Session::getId() const
{
    return id;
}

Image* Session::getImage(const std::string& fileName) const
{
    std::string name   = fileName.substr(0, fileName.size() - 4);
    std::string format = fileName.substr(fileName.size() - 4);
    for (size_t i = 0; i < size; i++)
    {
        if (images[i]->getFormat(format) == images[i]->getFormat() &&
            images[i]->getFileName() == name)
        {
            return images[i];
        }
    }
    throw std::invalid_argument("Image " + fileName + " doesnt exist in session");
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

bool Session::checkIfImageExist(const std::string& fileName) const
{
    for (size_t i = 0; i < size; i++)
    {
        if (images[i]->getFileName()+images[i]->getFileTypeStr() == fileName)
        {
            std::cout << "Image:" << fileName << " already in session ";
            return true;
        }
    }
    return false;
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

void Session::getCollageName(std::string& modifiedFile, const Image& image1, const Image& image2) const
{
    modifiedFile = image1.getFileName() + '_' + image2.getFileName() + '_';
    std::string modifiedTime = getModifiedTime();
    modifiedFile = modifiedFile + modifiedTime + image1.getFileTypeStr();
}

void Session::writeCollageHeader(unsigned short newWidth, unsigned short newLength, const Image& image1, const Image& image2, std::ofstream& newImage) const
{
    ImageProcesing::ImageType da = image1.getFormat();
    switch (da)
    {
    case ImageProcesing::ImageType::P1:
        newImage << "P1" << std::endl;
        newImage << newWidth << " " << newLength << std::endl;
        break;
    case ImageProcesing::ImageType::P2:
        newImage << "P2" << std::endl;
        newImage << newWidth << " " << newLength << std::endl;
        break;
    case ImageProcesing::ImageType::P3:
        newImage << "P3" << std::endl;
        newImage << newWidth << " " << newLength << std::endl;
        break;
    case ImageProcesing::ImageType::defaultType:
        throw std::runtime_error("Something went wrong!");
        break;
    }
}

void Session::checkIfImagesForCollageInSession(const std::string& firstImage, const std::string& secondImage) const
{
    int index1 = -1;
    int index2 = -1;
    for (size_t i = 0; i < size; i++)
    {
        if (images[i]->getFileName() + images[i]->getFileTypeStr() == firstImage)
        {
            index1 = i;
        }
        if (images[i]->getFileName() + images[i]->getFileTypeStr() == secondImage)
        {
            index2 = i;
        }
    }
    if (!(index1>=0 && index2>=0))
        throw std::invalid_argument("Images are not in the session!");

    if (images[index1]->getFileTypeStr() != images[index2]->getFileTypeStr())
        throw std::invalid_argument("Cannot make a collage from different types!(" + images[index1]->getFileTypeStr() + " and " + images[index2]->getFileTypeStr() + ")");
}

void Session::addImage(Image* other)
{
    if (size >=  capacity)
        resize();
    images[size++] = other;
}

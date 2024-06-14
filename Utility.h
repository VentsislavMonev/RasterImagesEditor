#pragma once

#include <iostream>
#include <string>
#include <fstream>

bool fileExist(const std::string& name);

int  getNumb(const std::string& str);

std::string getString(int numb);

//gets the current time and transfers it to string
std::string getModifiedTime();

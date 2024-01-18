#include "student.hpp"
#include <iostream>
void Student::printName()
{
    std::cout << name << std::endl;
}

void Student::setName(std::string inName)
{
    name = inName;
}
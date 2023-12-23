#pragma once
#include <string>
class Student
{
private:
    int id;
    std::string name;

public:
    void printName();
    void setName(std::string inName);
};
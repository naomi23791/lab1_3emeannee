// ============================================================
// Fichier : Student.h
// ============================================================
#pragma once

#include "Element.h"

class Student : public Element
{
public:
    Student();
    Student(const std::wstring& lastName, const std::wstring& firstName, const std::wstring& patronymic, int age,
        const std::wstring& group, double average);
    virtual ~Student();

    void readFromConsole() override;
    void printToConsole() const override;
    int  getType() const override;
    void readFromFile(std::wifstream& in) override;
    void writeToFile(std::wofstream& out) const override;
    // Dans Student.h, après les autres overrides :
    void modify() override;

private:
    std::wstring m_group;
    double       m_average;
};
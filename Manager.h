
// ============================================================
// Fichier : Manager.h
// ============================================================
#pragma once

#include "Element.h"

class Manager : public Element
{
public:
    Manager();
    Manager(const std::wstring& lastName, const std::wstring& firstName, const std::wstring& patronymic, int age,
        const std::wstring& department, double salary);
    virtual ~Manager();

    void readFromConsole() override;
    void printToConsole() const override;
    int  getType() const override;
    void readFromFile(std::wifstream& in) override;
    void writeToFile(std::wofstream& out) const override;

private:
    std::wstring m_department;
    double       m_salary;
};
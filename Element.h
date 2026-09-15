// ============================================================
// Fichier : Element.h
// Rôle : classe de base abstraite
// ============================================================
#pragma once

#include <string>
#include <fstream>

class Element
{
public:
    Element();
    Element(const std::wstring& lastName, const std::wstring& firstName,const std::wstring& patronymic, int age);
    virtual ~Element();

    std::wstring getFullName() const;
    virtual void readFromConsole() = 0;
    virtual void printToConsole() const = 0;
    virtual int  getType() const = 0;
    virtual void readFromFile(std::wifstream& in) = 0;
    virtual void writeToFile(std::wofstream& out) const = 0;

protected:
    std::wstring m_lastName;      // Фамилия
    std::wstring m_firstName;     // Имя
    std::wstring m_patronymic;    // Отчество (peut être vide)
    int          m_age;
};
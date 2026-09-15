#include "Student.h"
#include <iostream>
#include <limits>   // pour numeric_limits

Student::Student() : Element(), m_group(L""), m_average(0.0) {}
Student::Student(const std::wstring& lastName, const std::wstring& firstName,const std::wstring& patronymic, int age,
    const std::wstring& group, double average)
    :Element(lastName, firstName, patronymic,age), m_group(group), m_average(average) {
}
Student::~Student() {}

// -------- Fonction utilitaire : lit un int valide --------
static int readInt(const std::wstring& prompt, int minVal, int maxVal)
{
    int value;
    while (true)
    {
        std::wcout << prompt;
        if (std::wcin >> value && value >= minVal && value <= maxVal)
            return value;

        // Erreur : on nettoie le flux et on redemande
        std::wcout << L"Ошибка : введите число от " << minVal
            << L" до " << maxVal << L"." << std::endl;
        std::wcin.clear();                                  // réinitialise l'état d'erreur
        std::wcin.ignore(10000, L'\n');                     // vide le tampon
    }
}

// -------- Fonction utilitaire : lit un double valide --------
static double readDouble(const std::wstring& prompt, double minVal, double maxVal)
{
    double value;
    while (true)
    {
        std::wcout << prompt;
        if (std::wcin >> value && value >= minVal && value <= maxVal)
            return value;

        std::wcout << L"Ошибка : введите число от " << minVal
            << L" до " << maxVal << L"." << std::endl;
        std::wcin.clear();
        std::wcin.ignore(10000, L'\n');
    }
}

void Student::printToConsole() const
{
    std::wcout << L"[Студент] ФИО = " << getFullName()
        << L", Возраст = " << m_age
        << L", Группа = " << m_group
        << L", Средний балл = " << m_average
        << std::endl;
}

int Student::getType() const { return 1; }

void Student::readFromConsole()
{
    std::wcout << L"Фамилия : ";
    std::getline(std::wcin >> std::ws, m_lastName);

    std::wcout << L"Имя : ";
    std::getline(std::wcin >> std::ws, m_firstName);

    std::wcout << L"Отчество (можно пропустить) : ";
    std::wcin.ignore(10000, L'\n');    // ✅ consomme le '\n' restant
    std::getline(std::wcin, m_patronymic);   // ✅ lit la ligne (peut être vide)

    m_age = readInt(L"Возраст : ", 1, 150);

    std::wcout << L"Группа : ";
    std::wcin >> m_group;

    m_average = readDouble(L"Средний балл : ", 0.0, 20.0);
}

void Student::readFromFile(std::wifstream& in)
{
    std::getline(in, m_lastName, L'|');
    std::getline(in, m_firstName, L'|');
    std::getline(in, m_patronymic, L'|');
    if (m_patronymic == L"-") m_patronymic.clear();
    in >> m_age;
    in.ignore(1, L'|');
    std::getline(in, m_group, L'|');
    in >> m_average;
    in.ignore(1, L'\n');
}

void Student::writeToFile(std::wofstream& out) const
{
    out << m_lastName << L'|'
        << m_firstName << L'|'
        << (m_patronymic.empty() ? L"-" : m_patronymic) << L'|'
        << m_age << L'|'
        << m_group << L'|'
        << m_average << L'\n';
}
#include "Student.h"
#include <iostream>
#include <string>

Student::Student() : Element(), m_group(L""), m_average(0.0) {}

Student::Student(const std::wstring& lastName, const std::wstring& firstName,
    const std::wstring& patronymic, int age,
    const std::wstring& group, double average)
    : Element(lastName, firstName, patronymic, age),
    m_group(group), m_average(average)
{
}

Student::~Student() {}

// ------------------------------------------------------------
// Saisie clavier (création)
// ------------------------------------------------------------
void Student::readFromConsole()
{
    std::wcout << L"Фамилия : ";
    std::getline(std::wcin >> std::ws, m_lastName);

    std::wcout << L"Имя : ";
    std::getline(std::wcin >> std::ws, m_firstName);

    std::wcout << L"Отчество (можно пропустить) : ";
    std::getline(std::wcin, m_patronymic);

    int age;
    while (true)
    {
        std::wcout << L"Возраст : ";
        if (std::wcin >> age && age >= 1 && age <= 150) { m_age = age; break; }
        std::wcout << L"Ошибка : возраст от 1 до 150." << std::endl;
        std::wcin.clear();
        std::wcin.ignore(10000, L'\n');
    }

    std::wcout << L"Группа : ";
    std::wcin >> m_group;

    double avg;
    while (true)
    {
        std::wcout << L"Средний балл : ";
        if (std::wcin >> avg && avg >= 0.0 && avg <= 20.0) { m_average = avg; break; }
        std::wcout << L"Ошибка : балл от 0 до 20." << std::endl;
        std::wcin.clear();
        std::wcin.ignore(10000, L'\n');
    }
}

// ------------------------------------------------------------
// Affichage
// ------------------------------------------------------------
void Student::printToConsole() const
{
    std::wcout << L"[Студент] ФИО = " << getFullName()
        << L", Возраст = " << m_age
        << L", Группа = " << m_group
        << L", Средний балл = " << m_average
        << std::endl;
}

// ------------------------------------------------------------
// Identifiant de type
// ------------------------------------------------------------
int Student::getType() const { return 1; }

// ------------------------------------------------------------
// Modification interactive
// ------------------------------------------------------------
void Student::modify()
{
    std::wcout << L"\n--- Изменение данных студента ---" << std::endl;
    std::wcout << L"Оставьте поле пустым, чтобы сохранить текущее значение." << std::endl;

    std::wstring input;

    std::wcout << L"Фамилия [" << m_lastName << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_lastName = input;

    std::wcout << L"Имя [" << m_firstName << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_firstName = input;

    std::wcout << L"Отчество [" << (m_patronymic.empty() ? L"-" : m_patronymic) << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_patronymic = input;

    std::wcout << L"Возраст [" << m_age << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty())
    {
        try {
            int v = std::stoi(input);
            if (v >= 1 && v <= 150) m_age = v;
            else std::wcout << L"Ошибка : возраст от 1 до 150." << std::endl;
        }
        catch (...) { std::wcout << L"Ошибка : не число." << std::endl; }
    }

    std::wcout << L"Группа [" << m_group << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_group = input;

    std::wcout << L"Средний балл [" << m_average << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty())
    {
        try {
            double v = std::stod(input);
            if (v >= 0.0 && v <= 20.0) m_average = v;
            else std::wcout << L"Ошибка : балл от 0 до 20." << std::endl;
        }
        catch (...) { std::wcout << L"Ошибка : не число." << std::endl; }
    }

    std::wcout << L"OK. Данные обновлены : " << getFullName() << std::endl;
}

// ------------------------------------------------------------
// Lecture fichier
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// Écriture fichier
// ------------------------------------------------------------
void Student::writeToFile(std::wofstream& out) const
{
    out << m_lastName << L'|'
        << m_firstName << L'|'
        << (m_patronymic.empty() ? L"-" : m_patronymic) << L'|'
        << m_age << L'|'
        << m_group << L'|'
        << m_average << L'\n';
}
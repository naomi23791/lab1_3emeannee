#include "Manager.h"
#include <iostream>

Manager::Manager() : Element(), m_department(L""), m_salary(0.0) {}
Manager::Manager(const std::wstring& lastName, const std::wstring& firstName, const std::wstring& patronymic, int age,
    const std::wstring& department, double salary)
    :Element(lastName, firstName, patronymic, age), m_department(department), m_salary(salary) {
}
Manager::~Manager() {}

// -------- Fonctions utilitaires (mêmes que dans Student.cpp) --------
static int readInt(const std::wstring& prompt, int minVal, int maxVal)
{
    int value;
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

void Manager::readFromConsole()
{
    std::wcout << L"Фамилия : ";
    std::getline(std::wcin >> std::ws, m_lastName);

    std::wcout << L"Имя : ";
    std::getline(std::wcin >> std::ws, m_firstName);

    std::wcout << L"Отчество (можно пропустить) : ";
    std::wcin.ignore(10000, L'\n');    // ✅ consomme le '\n' restant
    std::getline(std::wcin, m_patronymic);   // ✅ lit la ligne (peut être vide)

    m_age = readInt(L"Возраст : ", 1, 150);

    std::wcout << L"Отдел : ";
    std::wcin >> m_department;

    m_salary = readDouble(L"Зарплата : ", 0.0, 1000000.0);
}

void Manager::printToConsole() const
{
    std::wcout << L"[Менеджер] ФИО = " << getFullName()
        << L", Возраст = " << m_age
        << L", Отдел = " << m_department
        << L", Зарплата = " << m_salary
        << std::endl;
}
// ------------------------------------------------------------
// Identifiant de type : 2 pour Manager
// ------------------------------------------------------------
int Manager::getType() const
{
    return 2;
}

void Manager::readFromFile(std::wifstream& in)
{
    std::getline(in, m_lastName, L'|');
    std::getline(in, m_firstName, L'|');
    std::getline(in, m_patronymic, L'|');
    if (m_patronymic == L"-") m_patronymic.clear();
    in >> m_age;
    in.ignore(1, L'|');
    std::getline(in, m_department, L'|');
    in >> m_salary;
    in.ignore(1, L'\n');
}

void Manager::writeToFile(std::wofstream& out) const
{
    out << m_lastName << L'|'
        << m_firstName << L'|'
        << (m_patronymic.empty() ? L"-" : m_patronymic) << L'|'
        << m_age << L'|'
        << m_department << L'|'
        << m_salary << L'\n';
}
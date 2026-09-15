#include "Manager.h"
#include <iostream>

Manager::Manager() : Element(), m_department(L""), m_salary(0.0) {}

Manager::Manager(const std::wstring& lastName, const std::wstring& firstName,
    const std::wstring& patronymic, int age,
    const std::wstring& department, double salary)
    : Element(lastName, firstName, patronymic, age),
    m_department(department), m_salary(salary)
{
}

Manager::~Manager() {}

// ------------------------------------------------------------
// Saisie clavier (création)
// ------------------------------------------------------------
void Manager::readFromConsole()
{
    std::wcout << L"Фамилия : ";
    std::getline(std::wcin >> std::ws, m_lastName);

    std::wcout << L"Имя : ";
    std::getline(std::wcin >> std::ws, m_firstName);

    std::wcout << L"Отчество (можно пропустить) : ";
    std::getline(std::wcin, m_patronymic);   // ✅ PAS de ignore

    int age;
    while (true)
    {
        std::wcout << L"Возраст : ";
        if (std::wcin >> age && age >= 1 && age <= 150) { m_age = age; break; }
        std::wcout << L"Ошибка : возраст от 1 до 150." << std::endl;
        std::wcin.clear();
        std::wcin.ignore(10000, L'\n');
    }

    std::wcout << L"Отдел : ";
    std::wcin >> m_department;

    double salary;
    while (true)
    {
        std::wcout << L"Зарплата : ";
        if (std::wcin >> salary && salary >= 0.0 && salary <= 1000000.0)
        {
            m_salary = salary;
            break;
        }
        std::wcout << L"Ошибка : зарплата от 0 до 1 000 000." << std::endl;
        std::wcin.clear();
        std::wcin.ignore(10000, L'\n');
    }
}

// ------------------------------------------------------------
// Affichage
// ------------------------------------------------------------
void Manager::printToConsole() const
{
    std::wcout << L"[Менеджер] ФИО = " << getFullName()
        << L", Возраст = " << m_age
        << L", Отдел = " << m_department
        << L", Зарплата = " << m_salary
        << std::endl;
}

// ------------------------------------------------------------
// Identifiant de type
// ------------------------------------------------------------
int Manager::getType() const { return 2; }

// ------------------------------------------------------------
// Modification interactive
// ------------------------------------------------------------
void Manager::modify()
{
    std::wcout << L"\n--- Изменение данных менеджера ---" << std::endl;
    std::wcout << L"Оставьте поле пустым, чтобы сохранить текущее значение." << std::endl;

    std::wcin.clear();
    std::wstring input;

    // ---- Фамилия ----
    std::wcout << L"Фамилия [" << m_lastName << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_lastName = input;

    // ---- Имя ----
    std::wcout << L"Имя [" << m_firstName << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_firstName = input;

    // ---- Отчество ----
    std::wcout << L"Отчество [" << (m_patronymic.empty() ? L"-" : m_patronymic) << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_patronymic = input;

    // ---- Возраст ----
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

    // ---- Отдел ----
    std::wcout << L"Отдел [" << m_department << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty()) m_department = input;

    // ---- Зарплата ----
    std::wcout << L"Зарплата [" << m_salary << L"] : ";
    std::getline(std::wcin, input);
    if (!input.empty())
    {
        try {
            double v = std::stod(input);
            if (v >= 0.0 && v <= 1000000.0) m_salary = v;
            else std::wcout << L"Ошибка : зарплата от 0 до 1 000 000." << std::endl;
        }
        catch (...) { std::wcout << L"Ошибка : не число." << std::endl; }
    }

    // ✅ NE PAS nettoyer le buffer ici ! Laisser le \n pour que l'appelant le gère
    std::wcout << L"✅ Данные обновлены : " << getFullName() << std::endl;
}

// ------------------------------------------------------------
// Lecture fichier
// ------------------------------------------------------------
void Manager::readFromFile(std::wifstream& in)
{
    // ✅ Nettoyage défensif : ignorer espaces/tabulations initiaux
    while (in.peek() == L' ' || in.peek() == L'\t')
        in.ignore(1);

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

// ------------------------------------------------------------
// Écriture fichier
// ------------------------------------------------------------
void Manager::writeToFile(std::wofstream& out) const
{
    out << m_lastName << L'|'
        << m_firstName << L'|'
        << (m_patronymic.empty() ? L"-" : m_patronymic) << L'|'
        << m_age << L'|'
        << m_department << L'|'
        << m_salary << L'\n';
}
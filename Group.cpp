// ============================================================
// Fichier : Group.cpp
// ============================================================
#include "Group.h"
#include "Student.h"
#include "Manager.h"
#include <iostream>
#include <locale>
#include <vector>

// ------------------------------------------------------------
// Utilitaire local : lit un entier (nécessaire pour modifyByName)
// ------------------------------------------------------------
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

Group::Group() {}
Group::~Group() { clear(); }

// ------------------------------------------------------------
// Ajout d'un élément (avec vérification des doublons)
// ------------------------------------------------------------
bool Group::addElement(Element* element, bool silent)
{
    if (element == nullptr) return false;

    for (const Element* e : m_elements)
    {
        if (e == nullptr) continue;
        if (e->getType() == element->getType() &&
            e->getFullName() == element->getFullName())
        {
            if (!silent)
                std::wcout << L"Элемент \"" << element->getFullName()
                << L"\" уже существует в списке." << std::endl;
            delete element;
            return false;
        }
    }
    m_elements.push_back(element);
    return true;
}

// ------------------------------------------------------------
// Affichage de tous les éléments
// ------------------------------------------------------------
void Group::printAll() const
{
    if (m_elements.empty())
    {
        std::wcout << L"Список пуст." << std::endl;
        return;
    }
    for (const Element* e : m_elements)
    {
        if (e == nullptr) continue;
        e->printToConsole();
    }
}

// ------------------------------------------------------------
// Recherche par nom
// ------------------------------------------------------------
void Group::findByName(const std::wstring& name) const
{
    int found = 0;
    for (const Element* e : m_elements)
    {
        if (e == nullptr) continue;
        if (e->getFullName().find(name) != std::wstring::npos)
        {
            e->printToConsole();
            ++found;
        }
    }
    if (found == 0)
        std::wcout << L"Элемент не найден." << std::endl;
    else
        std::wcout << L"Найдено элементов : " << found << std::endl;
}

// ------------------------------------------------------------
// Modification par nom
// ✅ IMPORTANT : le wcin.ignore() doit être fait AVANT modify()
//                 dans les DEUX branches (1 résultat ET plusieurs)
// ------------------------------------------------------------
void Group::modifyByName(const std::wstring& name)
{
    // 1. Trouver toutes les correspondances
    std::vector<Element*> matches;
    for (Element* e : m_elements)
    {
        if (e == nullptr) continue;
        if (e->getFullName().find(name) != std::wstring::npos)
            matches.push_back(e);
    }

    // 2. Aucun résultat
    if (matches.empty())
    {
        std::wcout << L"Элемент не найден." << std::endl;
        return;
    }

    // 3. Un seul résultat → modification directe
    if (matches.size() == 1)
    {
        std::wcout << L"Найден 1 элемент :" << std::endl;
        matches[0]->printToConsole();

        // ✅ Nettoyer le '\n' résiduel AVANT d'appeler modify()
        std::wcin.ignore(10000, L'\n');

        matches[0]->modify();
        return;
    }

    // 4. Plusieurs résultats → demander lequel
    std::wcout << L"Найдено " << matches.size() << L" элементов :" << std::endl;
    for (size_t i = 0; i < matches.size(); ++i)
    {
        std::wcout << L"  [" << (i + 1) << L"] ";
        matches[i]->printToConsole();
    }

    int idx = readInt(L"Номер для изменения (0 = отмена) : ",
        0, static_cast<int>(matches.size()));

    // ✅ Nettoyer le '\n' laissé par readInt AVANT d'appeler modify()
    std::wcin.ignore(10000, L'\n');

    if (idx == 0)
    {
        std::wcout << L"Изменение отменено." << std::endl;
        return;
    }

    matches[idx - 1]->modify();
}

// ------------------------------------------------------------
// Lecture depuis un fichier
// ------------------------------------------------------------
int Group::readFromFile(const std::wstring& filename)
{
    std::wifstream in(filename.c_str());
    if (!in.is_open())
    {
        std::wcout << L"Невозможно открыть файл : " << filename << std::endl;
        return -1;
    }
    in.imbue(std::locale(""));

    wchar_t firstChar = in.peek();
    if (firstChar == WEOF)
    {
        std::wcout << L"Файл пуст : " << filename << std::endl;
        in.close();
        return 0;
    }

    int count = 0;
    int duplicates = 0;
    int type = 0;

    while (in >> type)
    {
        Element* e = nullptr;
        if (type == 1)      e = new Student();
        else if (type == 2) e = new Manager();
        else
        {
            std::wcout << L"Неизвестный тип элемента : " << type << std::endl;
            continue;
        }

        if (e)
        {
            // ✅ Consommer l'espace après le type ("1 motie|...")
            in.ignore(1, L' ');

            e->readFromFile(in);
            if (addElement(e, true))
                ++count;
            else
                ++duplicates;
        }
    }
    in.close();

    if (duplicates > 0)
        std::wcout << L"Пропущено дубликатов : " << duplicates << std::endl;

    return count;
}

// ------------------------------------------------------------
// Écriture dans un fichier
// ------------------------------------------------------------
void Group::writeToFile(const std::wstring& filename) const
{
    std::wofstream out(filename.c_str());
    if (!out.is_open())
    {
        std::wcout << L"Невозможно записать в файл : " << filename << std::endl;
        return;
    }
    out.imbue(std::locale(""));

    for (const Element* e : m_elements)
    {
        if (e == nullptr) continue;
        out << e->getType() << L' ';
        e->writeToFile(out);
    }
    out.close();
}

// ------------------------------------------------------------
// Vider la liste (libère la mémoire)
// ------------------------------------------------------------
void Group::clear()
{
    for (Element* e : m_elements)
        delete e;
    m_elements.clear();
}

// ------------------------------------------------------------
// Indique si la liste est vide
// ------------------------------------------------------------
bool Group::isEmpty() const { return m_elements.empty(); }

// ------------------------------------------------------------
// Retourne le nombre d'éléments
// ------------------------------------------------------------
size_t Group::size() const { return m_elements.size(); }
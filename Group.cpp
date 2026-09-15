// ============================================================
// Fichier : Group.cpp
// Rôle : implémentation de la classe Group (conteneur)
// Version finale avec :
//   - addElement (bool, vérification des doublons)
//   - printAll
//   - findByName
//   - readFromFile (retourne int)
//   - writeToFile
//   - clear
//   - isEmpty / size
// ============================================================

#include "Group.h"
#include "Student.h"
#include "Manager.h"
#include <iostream>
#include <locale>

// ------------------------------------------------------------
// Constructeur
// ------------------------------------------------------------
Group::Group() {}

// ------------------------------------------------------------
// Destructeur : libère toute la mémoire
// ------------------------------------------------------------
Group::~Group()
{
    clear();
}

// ------------------------------------------------------------
// Ajoute un élément (avec vérification des doublons)
// Retourne true si ajouté, false si doublon refusé
// ------------------------------------------------------------
bool Group::addElement(Element* element, bool silent)
{
    // ✅ Sécurité : refuser un pointeur nul
    if (element == nullptr) return false;

    // Parcours de tous les éléments déjà présents
    for (const Element* e : m_elements)
    {
        if (e == nullptr) continue;   // ✅ sécurité

        // ✅ Doublon = même TYPE **ET** même ФИО
        if (e->getType() == element->getType() &&
            e->getFullName() == element->getFullName())
        {
            // N'afficher le message QUE si silent == false
            if (!silent)
                std::wcout << L"Элемент \"" << element->getFullName()
                << L"\" уже существует в списке." << std::endl;

            delete element;           // libère l'objet non ajouté
            return false;             // échec
        }
    }

    // Aucun doublon trouvé → on ajoute
    m_elements.push_back(element);
    return true;                      // succès
}
// ------------------------------------------------------------
// Affiche tous les éléments
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
        if (e == nullptr) continue;   // ✅ sécurité
        e->printToConsole();
    }
}

// ------------------------------------------------------------
// Recherche un élément par son nom
// ------------------------------------------------------------
void Group::findByName(const std::wstring& name) const
{
    int found = 0;
    for (const Element* e : m_elements)
    {
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
// Lit une liste depuis un fichier
// Retourne :
//   > 0 : nombre d'éléments chargés
//   0   : fichier vide
//  -1   : erreur (fichier inexistant / illisible)
// ------------------------------------------------------------
int Group::readFromFile(const std::wstring& filename)
{
    // --------------------------------------------------------
    // 1. Ouverture du fichier en lecture (wifstream = Unicode)
    // --------------------------------------------------------
    std::wifstream in(filename.c_str());   // ✅ déclaration de "in"

    if (!in.is_open())
    {
        std::wcout << L"Невозможно открыть файл : " << filename << std::endl;
        return -1;                          // erreur : fichier introuvable
    }
    in.imbue(std::locale(""));              // locale Unicode pour le contenu

    // --------------------------------------------------------
    // 2. Détection du fichier vide
    // --------------------------------------------------------
    wchar_t firstChar = in.peek();
    if (firstChar == WEOF)
    {
        std::wcout << L"Файл пуст : " << filename << std::endl;
        in.close();
        return 0;                           // fichier vide
    }

    // --------------------------------------------------------
    // 3. Compteurs
    // --------------------------------------------------------
    int count = 0;     // nombre d'éléments ajoutés
    int duplicates = 0;     // nombre de doublons refusés
    int type = 0;     // ✅ déclaration de "type" (lu depuis le fichier)

    // --------------------------------------------------------
    // 4. Boucle de lecture
    // --------------------------------------------------------
    while (in >> type)      // ✅ lit le type (1 = Student, 2 = Manager)
    {
        Element* e = nullptr;

        if (type == 1)      e = new Student();
        else if (type == 2) e = new Manager();
        else
        {
            std::wcout << L"Неизвестный тип элемента : " << type << std::endl;
            continue;       // ignore ce type et passe au suivant
        }

        if (e)
        {
            e->readFromFile(in);            // ✅ lit les attributs
            if (addElement(e, true))        // silent = true → pas de message
                ++count;
            else
                ++duplicates;               // addElement a déjà fait delete e
        }
    }
    in.close();

    // --------------------------------------------------------
    // 5. Message récapitulatif
    // --------------------------------------------------------
    if (duplicates > 0)
        std::wcout << L"Пропущено дубликатов : " << duplicates << std::endl;

    return count;
}
// ------------------------------------------------------------
// Écrit la liste dans un fichier
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
        out << e->getType() << L' ';
        e->writeToFile(out);
    }
    out.close();
}

// ------------------------------------------------------------
// Vide la liste (libère la mémoire)
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
bool Group::isEmpty() const
{
    return m_elements.empty();
}

// ------------------------------------------------------------
// Retourne le nombre d'éléments
// ------------------------------------------------------------
size_t Group::size() const
{
    return m_elements.size();
}
// ============================================================
// Fichier : main.cpp
// Rôle : menu interactif complet (options 0 à 8)
// ============================================================

#include <iostream>
#include <cwctype>
#include <locale>
#include <string>
#include <fstream>
#include <limits>
#include "Group.h"
#include "Student.h"
#include "Manager.h"

// ------------------------------------------------------------
// Lit un entier valide dans [minVal, maxVal]
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

// ------------------------------------------------------------
// Lit une réponse oui/non
// ------------------------------------------------------------
static bool readYesNo(const std::wstring& prompt)
{
    std::wstring line;
    while (true)
    {
        std::wcout << prompt;
        std::wcin >> line;

        if (line.empty()) continue;

        wchar_t c = towlower(line[0]);
        if (c == L'д' || c == L'y' || c == L'o' || c == L'd') return true;
        if (c == L'н' || c == L'n') return false;

        std::wcout << L"Ошибка : введите 'д' (да) или 'н' (нет)." << std::endl;
    }
}

// ------------------------------------------------------------
// Vérifie si un fichier existe
// ------------------------------------------------------------
static bool fileExists(const std::wstring& filename)
{
    std::wifstream in(filename.c_str());
    return in.is_open();
}

// ------------------------------------------------------------
// Affiche le menu et lit le choix (0..8)
// ------------------------------------------------------------
int showMenu()
{
    std::wcout << L"\n===== МЕНЮ =====" << std::endl;
    std::wcout << L"1. Добавить студента" << std::endl;
    std::wcout << L"2. Добавить менеджера" << std::endl;
    std::wcout << L"3. Показать список" << std::endl;
    std::wcout << L"4. Сохранить в файл" << std::endl;
    std::wcout << L"5. Загрузить из файла" << std::endl;
    std::wcout << L"6. Очистить список" << std::endl;
    std::wcout << L"7. Найти по имени" << std::endl;
    std::wcout << L"8. Изменить элемент" << std::endl;
    std::wcout << L"0. Выход" << std::endl;

    return readInt(L"Ваш выбор : ", 0, 8);
}

// ------------------------------------------------------------
// Fonction principale
// ------------------------------------------------------------
int main()
{
    try
    {
        std::locale::global(std::locale(""));
        std::wcout.imbue(std::locale(""));
        std::wcin.imbue(std::locale(""));
    }
    catch (const std::runtime_error&) { /* ignoré sous MinGW */ }

    const std::wstring FILENAME = L"data.txt";
    Group group;
    int choice = -1;

    do
    {
        choice = showMenu();

        switch (choice)
        {
            // -------- 1. Ajouter un étudiant --------
        case 1:
        {
            Student* s = new Student();
            s->readFromConsole();
            if (group.addElement(s))
                std::wcout << L"Элемент добавлен." << std::endl;
            else
                std::wcout << L"Элемент не добавлен (дубликат)." << std::endl;
            break;
        }

        // -------- 2. Ajouter un manager --------
        case 2:
        {
            Manager* m = new Manager();
            m->readFromConsole();
            if (group.addElement(m))
                std::wcout << L"Элемент добавлен." << std::endl;
            else
                std::wcout << L"Элемент не добавлен (дубликат)." << std::endl;
            break;
        }

        // -------- 3. Afficher la liste --------
        case 3:
            group.printAll();
            break;

            // -------- 4. Sauvegarder dans un fichier --------
        case 4:
        {
            if (group.isEmpty())
            {
                std::wcout << L"Список пуст. Нечего сохранять." << std::endl;
                break;
            }

            if (fileExists(FILENAME))
            {
                std::wifstream in(FILENAME.c_str());
                in.imbue(std::locale(""));
                int fileLines = 0;
                std::wstring tmp;
                while (std::getline(in, tmp)) ++fileLines;
                in.close();

                std::wcout << L"╔══════════════════════════════════════════════╗" << std::endl;
                std::wcout << L"║  ВНИМАНИЕ : файл уже существует !            ║" << std::endl;
                std::wcout << L"║  Файл    : " << FILENAME << std::endl;
                std::wcout << L"║  В файле : " << fileLines << L" элемент(ов)" << std::endl;
                std::wcout << L"║  В списке: " << group.size() << L" элемент(ов)" << std::endl;
                std::wcout << L"║                                              ║" << std::endl;
                std::wcout << L"║  При сохранении файл будет ПОЛНОСТЬЮ         ║" << std::endl;
                std::wcout << L"║  ЗАМЕНЁН содержимым текущего списка.         ║" << std::endl;
                std::wcout << L"╚══════════════════════════════════════════════╝" << std::endl;

                if (!readYesNo(L"Продолжить и заменить файл? (д/н) : "))
                {
                    std::wcout << L"Сохранение отменено." << std::endl;
                    break;
                }
            }

            group.writeToFile(FILENAME);
            std::wcout << L"Сохранение выполнено." << std::endl;
            break;
        }

        // -------- 5. Charger depuis un fichier --------
        case 5:
        {
            if (!fileExists(FILENAME))
            {
                std::wcout << L"Файл не найден : " << FILENAME << std::endl;

                if (readYesNo(L"Создать новый файл? (д/н) : "))
                {
                    std::wofstream out(FILENAME.c_str());
                    out.close();
                    std::wcout << L"Файл создан : " << FILENAME << std::endl;
                }
                else
                {
                    std::wcout << L"Загрузка отменена." << std::endl;
                }
                break;
            }

            if (!group.isEmpty())
            {
                std::wcout << L"Текущий список (" << group.size()
                    << L" элемент(ов)) будет заменён содержимым файла." << std::endl;

                if (!readYesNo(L"Продолжить? (д/н) : "))
                {
                    std::wcout << L"Загрузка отменена." << std::endl;
                    break;
                }

                group.clear();
            }

            int loaded = group.readFromFile(FILENAME);

            if (loaded > 0)
                std::wcout << L"Загружено элементов : " << loaded << std::endl;
            else if (loaded == 0)
                std::wcout << L"Файл пуст. Ничего не загружено." << std::endl;

            break;
        }

        // -------- 6. Vider la liste --------
        case 6:
        {
            if (group.isEmpty())
            {
                std::wcout << L"Список уже пуст." << std::endl;
                break;
            }

            if (!readYesNo(L"Вы уверены, что хотите очистить список? (д/н) : "))
            {
                std::wcout << L"Очистка отменена." << std::endl;
                break;
            }

            group.clear();
            std::wcout << L"Список очищен." << std::endl;
            break;
        }

        // -------- 7. Rechercher par nom --------
        case 7:
        {
            if (group.isEmpty())
            {
                std::wcout << L"Список пуст. Поиск невозможен." << std::endl;
                break;
            }

            std::wcout << L"Введите имя или фамилию для поиска : ";
            std::wstring name;
            std::getline(std::wcin >> std::ws, name);
            group.findByName(name);
            break;
        }

        // -------- 8. Modifier un élément --------
        case 8:
        {
            if (group.isEmpty())
            {
                std::wcout << L"Список пуст. Изменение невозможно." << std::endl;
                break;
            }

            std::wcout << L"Введите имя или фамилию для поиска : ";
            std::wstring name;
            std::getline(std::wcin >> std::ws, name);

            group.modifyByName(name);
            break;
        }

        // -------- 0. Quitter --------
        case 0:
        {
            if (!group.isEmpty())
            {
                if (readYesNo(L"Сохранить данные перед выходом? (д/н) : "))
                {
                    group.writeToFile(FILENAME);
                    std::wcout << L"Данные сохранены." << std::endl;
                }
            }
            std::wcout << L"До свидания !" << std::endl;
            break;
        }

        default:
            std::wcout << L"Неверный выбор." << std::endl;
            break;
        }

    } while (choice != 0);

    return 0;
}
// ============================================================
// Fichier : Group.h
// ============================================================
#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "Element.h"

class Group
{
public:
    Group();
    ~Group();

    void findByName(const std::wstring& name) const;
    bool addElement(Element* element, bool silent = false);
    void printAll() const;
    int readFromFile(const std::wstring& filename);
    void writeToFile(const std::wstring& filename) const;
    void clear();
    bool isEmpty() const;
    size_t size() const;

private:
    std::vector<Element*> m_elements;
};
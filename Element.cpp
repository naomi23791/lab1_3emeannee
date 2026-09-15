#include "Element.h"

Element::Element()
    : m_lastName(L""), m_firstName(L""), m_patronymic(L""), m_age(0)
{
}

Element::Element(const std::wstring& lastName, const std::wstring& firstName,const std::wstring& patronymic, int age)
    : m_lastName(lastName), m_firstName(firstName),
    m_patronymic(patronymic), m_age(age)
{
}

Element::~Element() {}

std::wstring Element::getFullName() const
{
    // ✅ Nettoie une chaîne : supprime \n, \r, espaces en début/fin
    auto clean = [](std::wstring s) {
        // Fin
        while (!s.empty() &&
            (s.back() == L'\n' || s.back() == L'\r' || s.back() == L' ' || s.back() == L'\t'))
            s.pop_back();
        // Début
        size_t start = 0;
        while (start < s.size() &&
            (s[start] == L' ' || s[start] == L'\t'))
            ++start;
        return s.substr(start);
        };

    std::wstring full;
    auto append = [&full, &clean](const std::wstring& part) {
        std::wstring p = clean(part);
        if (p.empty()) return;
        if (!full.empty()) full += L' ';
        full += p;
        };
    append(m_lastName);
    append(m_firstName);
    append(m_patronymic);
    return full;
}
#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <QString>
#include <QByteArray>

template<typename T1, typename T2, typename T3>
struct Triple
{
    T1 first;
    T2 second;
    T3 third;

    Triple() = default;
    Triple(const T1& f, const T2& s, const T3& t)
        : first(f), second(s), third(t) {}
};

#endif // TEMPLATES_H
// Polynomial.cpp
#include "Polynomial.h"
#include <map>
#include <sstream>
#include <iostream>
#include <functional> // for std::greater

using namespace std;

// Note the space between the two >'s for older compilers
static map<const Polynomial*, map<int, int, std::greater<int> > > storage;

void Polynomial::insertTerm(int coefficient, int exponent)
{
    if (exponent < 0) return;
    auto& terms = storage[this];
    terms[exponent] += coefficient;
    if (terms[exponent] == 0)
        terms.erase(exponent);
}

string Polynomial::toString() const
{
    auto it = storage.find(this);
    if (it == storage.end() || it->second.empty()) return "0";

    const map<int, int, std::greater<int> >& terms = it->second;

    ostringstream out;
    bool first = true;
    for (map<int, int, std::greater<int> >::const_iterator itr = terms.begin(); itr != terms.end(); ++itr)
    {
        int exponent = itr->first;
        int coef = itr->second;
        if (coef == 0) continue;

        bool neg = coef < 0;
        int abscoef = neg ? -coef : coef;

        if (first)
        {
            if (neg) out << "-";
        }
        else
        {
            out << (neg ? " - " : " + ");
        }

        if (exponent == 0)
        {
            out << abscoef;
        }
        else if (exponent == 1)
        {
            if (abscoef != 1) out << abscoef;
            out << "x";
        }
        else
        {
            if (abscoef != 1) out << abscoef;
            out << "x^" << exponent;
        }
        first = false;
    }
    return out.str();
}

Polynomial Polynomial::add(const Polynomial& other) const
{
    Polynomial result;
    auto& rt = storage[&result];

    // add this polynomial
    auto it = storage.find(this);
    if (it != storage.end())
    {
        for (map<int, int, std::greater<int> >::const_iterator itr = it->second.begin(); itr != it->second.end(); ++itr)
        {
            rt[itr->first] += itr->second;
            if (rt[itr->first] == 0) rt.erase(itr->first);
        }
    }

    // add other polynomial
    auto it2 = storage.find(&other);
    if (it2 != storage.end())
    {
        for (map<int, int, std::greater<int> >::const_iterator itr = it2->second.begin(); itr != it2->second.end(); ++itr)
        {
            rt[itr->first] += itr->second;
            if (rt[itr->first] == 0) rt.erase(itr->first);
        }
    }

    return result;
}

Polynomial Polynomial::multiply(const Polynomial& other) const
{
    Polynomial result;
    auto& rt = storage[&result];

    auto it1 = storage.find(this);
    auto it2 = storage.find(&other);
    if (it1 == storage.end() || it2 == storage.end()) return result;

    for (map<int, int, std::greater<int> >::const_iterator a = it1->second.begin(); a != it1->second.end(); ++a)
    {
        for (map<int, int, std::greater<int> >::const_iterator b = it2->second.begin(); b != it2->second.end(); ++b)
        {
            int e = a->first + b->first;
            int coeff = a->second * b->second;
            rt[e] += coeff;
            if (rt[e] == 0) rt.erase(e);
        }
    }

    return result;
}

Polynomial Polynomial::derivative() const
{
    Polynomial result;
    auto& rt = storage[&result];

    auto it = storage.find(this);
    if (it == storage.end()) return result;

    for (map<int, int, std::greater<int> >::const_iterator itr = it->second.begin(); itr != it->second.end(); ++itr)
    {
        int exponent = itr->first;
        int coef = itr->second;
        if (exponent > 0) rt[exponent - 1] += coef * exponent;
    }

    return result;
}

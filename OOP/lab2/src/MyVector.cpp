#include "MyVector.h"
#include <cstring>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
using namespace std;

Vector::Vector(double c1, double c2)
{
    x = c1;
    y = c2;
}

Vector::Vector()
{
    x = y = 0.;
}

void Vector::Out() const
{
    cout << "\nVector:  x = " << x << ",  y = " << y;
}

//====== Переопределение операций =====//
Vector &Vector::operator=(const Vector &v) // Присвоение
{
    if (this == &v)
        return *this;
    x = v.x;
    y = v.y;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Vector &v)
{
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

bool Vector::operator<(const Vector &s)
{
    return pow(this->x, 2) + pow(this->y, 2) < pow(s.x, 2) + pow(s.y, 2);
}

bool Vector::operator==(const Vector &s)
{
    return this->x == s.x && this->y == s.y;
}
// Класс Vector, инкапсулируюет функциональность вектора на плоскости
// Добавьте в класс все необходимые конструкторы и методы необходимые для функционирования в данной лабораторной
#include <iostream>
class Vector
{
private:
    double x, y; // Координаты вектора на плоскости
public:
    //========== Три конструктора
    Vector(double c1, double c2);
    Vector(); // Default

    //====== Переопределение операций =====//
    Vector &operator=(const Vector &v); // Присвоение
    void Out() const;

    friend std::ostream &operator<<(std::ostream &os, const Vector &s);

    bool operator<(const Vector &s);
    bool operator==(const Vector &s);
};

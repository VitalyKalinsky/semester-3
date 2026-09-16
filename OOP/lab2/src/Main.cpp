#include "MyVector.h"
#include "MyString.hpp"
#include <iostream>
using namespace std;
template <class T>
void pr(T &v, MyString s)
{
    cout << "\n\n\t" << s << "  # Sequence:\n";

    // Итератор любого контейнера
    class T::iterator p;
    int i;

    for (p = v.begin(), i = 0; p != v.end(); p++, i++)
        cout << endl
             << i + 1 << ". " << *p;
    cout << '\n';
}

template <class T>
void Swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
    cout << "a=" << a << ", b=" << b << endl;
}
int main(int argc, char const *argv[])
{
    {
        int i = 1, j = -1;
        Swap(i, j);

        double a = 0.5, b = -5.5;
        Swap(a, b);

        Vector u(1, 2), w(-3, -4);
        Swap(u, w);

        // Если вы достаточно развили класс MyString в предыдущей работе,
        // то следующий фрагмент тоже должен работать корректно.

        MyString s1("Your fault"), s2("My forgiveness");
        Swap(s1, s2);
    }
    return 0;
}

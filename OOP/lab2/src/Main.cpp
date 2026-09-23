#include "MyVector.h"
#include "MyString.hpp"
#include "MyStack.hpp"
#include <iostream>
#include <vector>
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
    int i;
    vector<vector<double>> vdd(5);
    for (i = 0; i < vdd.size(); i++)
        vdd[i] = vector<double>(i + 1, double(i + 0.1 * i));

    cout << "\n\n\tTest vector of vector<double>\n";
    for (i = 0; i < vdd.size(); i++)
    {
        cout << endl;
        for (int j = 0; j < vdd[i].size(); j++)
            cout << vdd[i][j] << "  ";
    }
}

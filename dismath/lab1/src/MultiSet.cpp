#include "MultiSet.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

MultiSet::MultiSet() : counts(U_SIZE, 0) {}

// объединение: max(a, b)
MultiSet uni(MultiSet &A, MultiSet &B)
{
    MultiSet result;
    transform(A.begin(), A.end(), B.begin(), result.begin(), [](int a, int b)
              { return max(a, b); });
    return result;
}

// пересечение: min(a, b)
MultiSet insertion(MultiSet &A, MultiSet &B)
{
    MultiSet result;
    transform(A.begin(), A.end(), B.begin(), result.begin(), [](int a, int b)
              { return min(a, b); });
    return result;
}

// дополнение: m - a, m - кратность эл-та в универсуме
MultiSet complement(MultiSet &A)
{
    MultiSet result;
    transform(A.begin(), A.end(), multi_universe.begin(), result.begin(), [](int a, int m)
              { return m - a; });
    return result;
}

// разность: A n -B
// A \ B = A n (not B)
MultiSet difference(MultiSet &A, MultiSet &B)
{
    MultiSet notB = complement(B);
    return insertion(A, notB);
}

// симметрическая разность: (A u B) \ (A n B)
MultiSet sym_difference(MultiSet &A, MultiSet &B)
{
    MultiSet AuB = uni(A, B);
    MultiSet AnB = insertion(A, B);
    return difference(AuB, AnB);
}

// арифметическая сумма
MultiSet operator+(MultiSet &A, MultiSet &B)
{
    MultiSet result;
    transform(A.begin(), A.end(), B.begin(), result.begin(), [](int a, int b)
              { return a + b; });

    transform(result.begin(), result.end(), multi_universe.begin(), result.begin(), [](int sum, int m)
              { return min(sum, m); });
    return result;
}

// арифметическая разность
MultiSet operator-(MultiSet &A, MultiSet &B)
{
    MultiSet result;
    transform(A.begin(), A.end(), B.begin(), result.begin(), [](int a, int b)
              { return max(0, a - b); });
    return result;
}

// арифметическое произведение
MultiSet operator*(MultiSet &A, MultiSet &B)
{
    MultiSet result;
    for (int i = 0; i < U_SIZE; i++)
    {
        result[i] = min(A[i] * B[i], multi_universe[i]);
    }
    return result;
}

// арифметическое деление
MultiSet operator/(MultiSet &A, MultiSet &B)
{
    MultiSet result;
    transform(A.begin(), A.end(), B.begin(), result.begin(), [](int a, int b)
              { return (b == 0) ? 0 : (a / b); });
    return result;
}
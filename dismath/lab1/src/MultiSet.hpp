#ifndef SET_HPP
#define SET_HPP

#include <vector>

class MultiSet
{
    std::vector<int> counts;

public:
    MultiSet();
    std::vector<int>::iterator begin()
    {
        return counts.begin();
    }
    int size()
    {
        return counts.size();
    }
    void clear()
    {
        counts.clear();
    }
    std::vector<int>::iterator end()
    {
        return counts.end();
    }
    int &operator[](size_t pos)
    {
        return counts[pos];
    }
    void resize(size_t new_size, const int val)
    {
        counts.resize(new_size, val);
    }
    bool isBigSize()
    {
        int count = 0;
        for (size_t i = 0; i < size(); i++)
        {
            if (count >= 32)
                return true;
            if (counts[i] > 0)
                count++;
        }
        return false;
    }
};

extern int U_SIZE;
extern MultiSet multi_universe;

MultiSet uni(MultiSet &A, MultiSet &B);
MultiSet insertion(MultiSet &A, MultiSet &B);
MultiSet difference(MultiSet &A, MultiSet &B);
MultiSet sym_difference(MultiSet &A, MultiSet &B);
MultiSet complement(MultiSet &A);

MultiSet operator+(MultiSet &A, MultiSet &B);
MultiSet operator-(MultiSet &A, MultiSet &B);
MultiSet operator*(MultiSet &A, MultiSet &B);
MultiSet operator/(MultiSet &A, MultiSet &B);

#endif
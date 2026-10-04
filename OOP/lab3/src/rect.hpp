#ifndef RECT_HPP
#define RECT_HPP
#include <iostream>
#include "point.hpp"
using namespace std;
class Rect
{
private:
    Point center;
    int width;
    int height;

public:
    Rect() : center(0, 0), width(0), height(0) {};
    Rect(int x, int y, int w, int h) : center(x, h), width(w), height(h) {};

    void set_width(int new_width)
    {
        if (new_width < 0)
        {
            cout << "Ширина не может быть отирицательной!" << endl;
            return;
        }
        width = new_width;
    }

    void set_height(int new_height)
    {
        if (new_height < 0)
        {
            cout << "Высота не может быть отирицательной!" << endl;
            return;
        }
        height = new_height;
    }

    friend ostream &operator<<(ostream &out, const Rect &r)
    {
        out << "{" << r.center << ", w=" << r.width << ", h=" << r.height << "}";
        return out;
    }

    bool operator<(const Rect &other) const
    {
        return this->center.center_dist() < other.center.center_dist();
    }
};
#endif
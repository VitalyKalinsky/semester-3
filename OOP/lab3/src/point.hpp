#ifndef POINT_HPP
#define POINT_HPP
#include <iostream>
#include <cmath>
class Point
{
private:
    double x, y;

public:
    Point(double x1, double y1) : x(x1), y(y1) {};
    double getX() const
    {
        return x;
    }
    double getY() const
    {
        return y;
    }
    void setX(double x)
    {
        this->x = x;
    }
    void setY(double y)
    {
        this->y = y;
    }
    friend std::ostream &operator<<(std::ostream &out, const Point &p)
    {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }
    bool operator<(const Point &other) const
    {
        if (x != other.x)
            return x < other.x;
        return y < other.y;
    }
    bool operator==(const Point &other) const
    {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Point &other) const
    {
        return !(*this == other);
    }
    double center_dist() const
    {
        return sqrt(pow(this->x, 2) + pow(this->y, 2));
    }
};
#endif
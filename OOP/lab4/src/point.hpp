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
        out << "(" << p.getX() << ", " << p.getY() << ")";
        return out;
    }
    std::weak_ordering operator<=>(const Point &other) const
    {
        if (this->center_dist() < other.center_dist())
            return std::weak_ordering::less;
        if (this->center_dist() > other.center_dist())
            return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }
    bool operator==(const Point &other) const
    {
        return (*this <=> other) == 0;
    }
    double center_dist() const
    {
        return sqrt(pow(this->x, 2) + pow(this->y, 2));
    }
};
#endif
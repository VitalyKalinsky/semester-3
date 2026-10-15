#ifndef POINT3D_HPP
#define POINT3D_HPP
#include <iostream>
#include <cmath>
struct Point3D
{
public:
    double x, y, z;
    
    friend std::ostream &operator<<(std::ostream &out, const Point3D &p)
    {
        out << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        return out;
    }

    std::weak_ordering operator<=>(const Point3D &other) const
    {
        if (this->center_dist() < other.center_dist())
            return std::weak_ordering::less;
        if (this->center_dist() > other.center_dist())
            return std::weak_ordering::greater;
        return std::weak_ordering::equivalent;
    }
    bool operator==(const Point3D &other) const
    {
        return (*this <=> other) == 0;
    }
    double center_dist() const
    {
        return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    }
};
#endif
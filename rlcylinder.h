#ifndef RLCYLINDER_H
#define RLCYLINDER_H

#include <rlmath.h>
#include <rlenuvector.h>

class Cylinder {

    public:

        ENUVector center;
        double radius;
        double height;

        Cylinder() {}
        Cylinder(ENUVector center, double radius, double height) : center(center), radius(radius), height(height) {}

        bool containsPoint(ENUVector point) {
            return center.getDistanceTo(point) <= radius && abs(center.up - point.up) < height / 2.0;
        }

        bool intersects(Cylinder another) {
            return center.getDistanceTo(another.center) <= radius*2 && abs(center.up - another.center.up) < height;
        }

};

#endif // RLCYLINDER_H

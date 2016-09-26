#ifndef RLVECTOR2D_H
#define RLVECTOR2D_H

#include <rlmath.h>

class Vector2D {

    public:
        double x;
        double y;

        Vector2D() {}
        Vector2D(double x, double y) : x(x), y(y) {};

        Vector2D& translate(double x, double y) {
            this->x += x;
            this->y += y;
            return *this;
        }

        Vector2D& rotate(double theta) {
            Math::rotateVector2DVector(x, y, theta);
            return *this;
        }

        Vector2D operator-(Vector2D another) {
            return Vector2D(x - another.x, y - another.y);
        }

        double normalize() {
            return Math::normalize3DVector(x, y, 0);
        }

};

#endif // RLVECTOR2D_H

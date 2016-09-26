/*
 *
 * This class represent the East-North-Up (ENU) position relative to a certain point in eath's surface
 * All values are represented by meters
 *
 */

#ifndef RLENUVECTOR_H
#define RLENUVECTOR_H

#include <iostream>

#include <rlmath.h>

using namespace std;

class ENUVector {

    public:
        double east;
        double north;
        double up;

        ENUVector() {}

        ENUVector(double east, double north, double up) : east(east), north(north), up(up) {}

        static ENUVector fromVel(double vel, double theta, double up) {
            double east, north, up_ = up;
            Math::detailVelocity(vel, theta, north, east);
            return ENUVector(east, north, up_);
        }

        double getDistanceTo(const ENUVector& another) {
            return Math::distance(east, north, up, another.east, another.north, another.up);
        }

        ENUVector& translate(double east, double north, double up) {
            this->east += east;
            this->north += north;
            this->up += up;
            return *this;
        }

        ENUVector& rotateZ(double theta) {
            Math::rotateVector2DVector(east, north, theta);
            return *this;
        }

        ENUVector& rotateZTo(double theta) {
            Math::rotateVector2DVectorTo(east, north, theta);
            return *this;
        }

        double normalizeHorizontal() {
            return Math::normalize3DVector(east, north, 0);
        }

        double getAngle() {
            return Math::radiansToDegrees(atan2(north, east));
        }

        double getAngleInverse() {
            return Math::radiansToDegrees(atan2(east, north));
        }

        ENUVector operator+(ENUVector another) {
            return ENUVector(east + another.east, north + another.north, up + another.up);
        }

        ENUVector operator-(ENUVector another) {
            return ENUVector(east - another.east, north - another.north, up - another.up);
        }

        ENUVector operator*(double escalar) {
            return ENUVector(east * escalar, north * escalar, up * escalar);
        }

        string toString() {
            char buff[1000];
            sprintf(buff, "ENU(%.3f, %.3f, %.3f)", east, north, up);
            return string(buff);
        }

};

#endif // RLENUVECTOR_H

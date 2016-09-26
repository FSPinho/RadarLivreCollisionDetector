#ifndef RLMATH_H
#define RLMATH_H

#include <cmath>

class Math {

    public:

        // Earth's radius 6,371,000.0 m
        constexpr static const double EARTH_RADIUS = 6371000;
        constexpr static const double PI = 3.141592653589793;

        Math() {}

        static double degreesToRadians(double degrees) {
            return degrees * PI / 180.0;
        }

        static double radiansToDegrees(double rad) {
            return rad * 180.0 / PI;
        }

        static double haversineDistance(double lat1, double lng1, double alt1, double lat2, double lng2, double alt2) {
            lat1 = degreesToRadians(lat1);
            lat2 = degreesToRadians(lat2);
            lng1 = degreesToRadians(lng1);
            lng2 = degreesToRadians(lng2);
            double latDistance = lat2 - lat1;
            double lngDistance = lng2 - lng1;

            double sinLatDis = sin(latDistance * 0.5);
            double sinLngDis = sin(lngDistance * 0.5);
            double a = (sinLatDis * sinLatDis) + cos(lat1) * cos(lat2) * (sinLngDis * sinLngDis);
            double c = 2.0 * atan2(sqrt(a), sqrt(1 - a));
            double distance = EARTH_RADIUS * c;
            return distance;
            double diference = alt2 - alt1;
            return sqrt((distance * distance) + (diference * diference));
        }

        static double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
            return sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0) + pow(z1 - z2, 2.0));
        }

        static void rotateVector2DVector(double& x, double& y, double theta) {
            theta = degreesToRadians(theta);
            double aux = x * cos(theta) - y * sin(theta);
            y = x * sin(theta) + y * cos(theta);
            x = aux;
        }

        static void rotateVector2DVectorTo(double& x, double& y, double theta) {
            theta = degreesToRadians(theta);
            double n = normalize3DVector(x, y, 0);
            x = n * cos(theta);
            y = n * sin(theta);
        }

        static double normalize3DVector(double x, double y, double z) {
            return sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
        }

        static bool isSimetric(double x, double y) {
            return !((x > 0 && y > 0) || (x < 0 && y < 0));
        }

        static double degreesToMetres(double lat, double lng) {
            return haversineDistance(lat, lng, 0, lat + 1, lng, 0);
        }

        static double metresToDegrees(double lat, double lng, double metres) {
            return metres / degreesToMetres(lat, lng);
        }

        static void detailVelocity(double vel, double theta, double& north, double& east) {
            theta = degreesToRadians(theta);
            north = sin(theta) * vel;
            east = cos(theta) * vel;
        }

        static int factorial(int x) {
            return x == 0 || x == 1? 1: x * factorial(x - 1);
        }

        static double knotsToMetres(double knots) {
            return (knots * 1.852) / 3.6;
        }

        static double ftToMetres(double ft) {
            return ft * 0.3048;
        }

        static double metresToKnots(double metres) {
            return metres * 3.6 / 1.852;
        }

        static double metresToFt(double metres) {
            return metres / 0.3048;
        }

};


#endif // RLMATH_H

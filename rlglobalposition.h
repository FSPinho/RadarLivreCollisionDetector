#ifndef RLGLOBALPOSITION_H
#define RLGLOBALPOSITION_H

#include <rlmath.h>
#include <rlenuvector.h>

/*
 * Created by Felipe Pinho in 28/07/2016
 * This class represent the globl position in earth's surface, based on latitude, longitude and altitude.
 * The latitude and longitude values are represented by degrees and the altitude value is represented by meters
 *
 */

class GlobalPosition {

    public:

        double latitude;
        double longitude;
        double altitude;

        GlobalPosition(const double& latitude, const double& longitude, const double& altitude) : latitude (latitude), longitude (longitude), altitude (altitude) { };

        double getDistanceTo(const GlobalPosition& another) {
            return Math::haversineDistance(latitude, longitude, altitude, another.latitude, another.longitude, another.altitude);
        }

        ENUVector getPositionRelativeTo(const GlobalPosition& origin) {
            return ENUVector(
                        // East position
                        Math::haversineDistance(
                            origin.latitude, longitude, origin.altitude,
                            origin.latitude, origin.longitude, origin.altitude)
                            * (longitude > origin.longitude? 1: -1),
                        // North position
                        Math::haversineDistance(
                            latitude, origin.longitude, origin.altitude,
                            origin.latitude, origin.longitude, origin.altitude)
                            * (latitude > origin.latitude? 1: -1),
                        // Up position
                        altitude - origin.altitude
            );
        }

        string toString() {
            char buff[50];
            sprintf(buff, "LLA(%.3f, %.3f, %.3f)", latitude, longitude, altitude);
            return string(buff);
        }

};

#endif // RLGLOBALPOSITION_H

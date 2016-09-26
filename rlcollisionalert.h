#ifndef RLCOLLISIONALERT_H
#define RLCOLLISIONALERT_H

#include <iostream>

#include <rlaircraft.h>

using namespace std;

class CollisionAlert {

    public:

        enum AlertType {
            CAZ_CONFLICT = 0,
            PAZ_CONFLICT = 1
        };

        AlertType type;
        long timestamp;
        long timestampOfColision;
        Aircraft * own;
        Aircraft * target;
        double aircraftsDistance;

        CollisionAlert() {}
        CollisionAlert(AlertType type, long timestamp, long timestampOfColision, Aircraft * own, Aircraft * target, double aircraftsDistance)
            : type(type), timestamp(timestamp), timestampOfColision(timestampOfColision), own(own), target(target), aircraftsDistance(aircraftsDistance) {}

        string toString() {
            char buff[1000];
            sprintf(buff, "%s conflict detected in the next %ld seconds", type==CAZ_CONFLICT? "CAZ_CONFLICT": "PAZ_CONFLICT", (timestampOfColision - timestamp) / 1000);
            return string(buff);
        }

};

#endif // RLCOLLISIONALERT_H

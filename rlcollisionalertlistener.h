#ifndef RLCOLLISIONALERTLISTENER_H
#define RLCOLLISIONALERTLISTENER_H

#include <rlcollisionalert.h>

class CollisionAlertListener {
public:
    CollisionAlertListener() {}

    virtual void onAlert(CollisionAlert alert) = 0;

};

#endif // RLCOLLISIONALERTLISTENER_H

#include <iostream>

#include <rlrepository.h>
#include <rlremoterepository.h>
#include <rlsimulatorrepository.h>
#include <rlcollisiondetector.h>

class AlertListener: public CollisionAlertListener {
    public:
        void onAlert(CollisionAlert a) {
            cout << endl << a.toString() << endl;
        }
};

int main() {

    Repository * r = new SimulatorRepository(3000);

    cout << "Have " << r->aircrafts.size() << " in repository!" << endl;

    AlertListener * listener= new AlertListener();
    CollisionDetector * detector = new CollisionDetector(r, listener, true);

    for(int i = 0; i < 10; i++) {
        detector->findIminentCollisions(false);
        usleep(1000000);
    }

    delete detector;
    delete listener;

}

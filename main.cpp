#include <iostream>

#include <rlrepository.h>
#include <rlremoterepository.h>
#include <rlsimulatorrepository.h>
#include <rlcollisiondetector.h>

class AlertListener: public CollisionAlertListener {
    public:
        void onAlert(CollisionAlert a) {
            // cout << endl << a.toString() << endl;
        }
};

int main() {

    Repository * r = new SimulatorRepository(5000);
    bool threads = false;
    unsigned int iterations = 10;

    cout << "#TEST" << endl
         << "#AIRPLANES: " << r->aircrafts.size() << endl
         << "#THREADS: " << threads << endl
         << "#ITERATIONS: " << iterations << endl << endl;

    AlertListener * listener= new AlertListener();
    CollisionDetector * detector = new CollisionDetector(r, listener, threads);

    for(int i = 0; i < iterations; i++) {
        cout << "TEST " << i << endl;
        detector->findIminentCollisions(false);
        cout << endl;
    }

}

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

int main(int argc, char * argv[]) {

    int airplaneCount = 3000;
    unsigned int iterations = 10;
    bool useThreads = 1;

    if(argc >= 4) {
        airplaneCount = atoi(argv[1]);
        iterations = atoi(argv[2]);
        useThreads = atoi(argv[3]);
    }


    Repository * r = new SimulatorRepository(airplaneCount);

    cout << "#TEST" << endl
         << "#AIRPLANES: " << r->aircrafts.size() << endl
         << "#THREADS: " << (useThreads? "TRUE": "FALSE") << endl
         << "#ITERATIONS: " << iterations << endl << endl;

    AlertListener * listener= new AlertListener();
    CollisionDetector * detector = new CollisionDetector(r, listener, useThreads);

    for(unsigned int i = 0; i < iterations; i++) {
        cout << "TEST " << i << endl;
        detector->findIminentCollisions(false);
        cout << endl;
    }

}

#include <iostream>

#include <rlrepository.h>
#include <rlremoterepository.h>
#include <rlsimulatorrepository.h>
#include <rlcollisiondetector.h>
#include <rlutil.h>
#include <rlstring.h>

class AlertListener: public CollisionAlertListener {
    public:
        void onAlert(CollisionAlert a) {
            Log::i() << "Alerta encontrado: " << a.toString() << LBR;
        }
};

int main() {

    Log::i(false) << MSG_WELLCAME << LBR2;

    Log::i(false) << MSG_INITIALIZING << LBR;

    Repository * repository = new RemoteRepository();

    AlertListener * listener= new AlertListener();
    CollisionDetector * detector = new CollisionDetector(repository, listener, true, true);

    Log::i(false) << MSG_INITIALIZING_DONE << LBR2;

    while(true) {

        Log::i() << "Recarregando aviões apartir do banco da dados..." << LBR;

        repository->reloadAircrafts();

        Log::i() << repository->aircrafts.size() << " aviões encontrados!" << LBR;

        Log::i() << "Verificando iminênsia de colisões..." << LBR;

        detector->findIminentCollisions(false);

        Log::i(false) << LBR2;

        usleep(1000000);

    }

}

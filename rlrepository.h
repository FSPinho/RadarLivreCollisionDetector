#ifndef RLREPOSITORY_H
#define RLREPOSITORY_H

#include <climits>
#include <list>

#include <rlaircraft.h>

#include <radarlivre_api_flightinfo.h>
#include <radarlivre_api_flightinfo-odb.hxx>
#include <radarlivre_api_observation.h>
#include <radarlivre_api_observation-odb.hxx>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>

using namespace std;
using namespace std;
using namespace odb::core;

class Repository {

    private:
        // ...

    public:
        list<Aircraft*> aircrafts;

        Repository() {}

        ~Repository() {
            Util::clear(aircrafts);
        }

        virtual bool reloadAircrafts() = 0;

        list<Aircraft*> getAircrafts() {
            return aircrafts;
        }

        list<Aircraft*> getAircraftsCopy(int limit = INT_MAX) {
            list<Aircraft*> copy;
            int index = 0;
            for(auto i: aircrafts) {
                if(index >= limit) break;
                copy.push_back(i->getCopy());
                index++;
            }
            return copy;
        }

};

#endif // RLREPOSITORY_H

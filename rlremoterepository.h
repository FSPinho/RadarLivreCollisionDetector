#ifndef RLAIRCRAFTREPOSITORY_H
#define RLAIRCRAFTREPOSITORY_H

#include <vector>

#include <rlutil.h>
#include <rlaircraft.h>
#include <rlrepository.h>
#include <cpr/cpr.h>

using namespace std;

class RemoteRepository : public Repository {

    public:

        RemoteRepository() {}

        bool reloadAircrafts() {

            try {

                auto db = new odb::pgsql::database ("postgres", "postgres", "radarlivre_v4", "localhost", 5432);

                typedef odb::query<radarlivre_api_flightinfo> queryFlightInfo;
                typedef odb::result<radarlivre_api_flightinfo> resultFlightInfo;

                typedef odb::query<radarlivre_api_observation> queryObservation;
                typedef odb::result<radarlivre_api_observation> resultObservation;

                transaction t(db->begin());

                resultFlightInfo rFlightInfo(db->query<radarlivre_api_flightinfo>(queryFlightInfo::timestamp > Util::getSystemTimestamp(-100000000000)));

                aircrafts.clear();
                for (resultFlightInfo::iterator iFlightInfo(rFlightInfo.begin ()); iFlightInfo != rFlightInfo.end(); ++iFlightInfo) {

                    resultObservation rObservation(
                                db->query<radarlivre_api_observation>(
                                    (queryObservation::flight_id == iFlightInfo->getFlightId() && queryObservation::timestamp > Util::getSystemTimestamp(-100000000000))
                                    + "ORDER BY timestamp DESC LIMIT 2"
                                )
                    );

                    vector<ADSBInfo*> trajectory;
                    for (resultObservation::iterator iObservation(rObservation.begin ()); iObservation != rObservation.end(); ++iObservation) {
                        radarlivre_api_observation obs = *iObservation;
                        trajectory.push_back(new ADSBInfo(
                                    obs.getTimestamp(),
                                    GlobalPosition(obs.getLatitude(), obs.getLongitude(), obs.getAltitude()),
                                    ENUVector::fromVel(obs.getHorizontalVelocity(), obs.getGroundTrackHeading(), obs.getVerticalVelocity()),
                                    obs.getGroundTrackHeading()
                        ));
                    }
                    aircrafts.push_back(new Aircraft(iFlightInfo->getFlightId(), trajectory));
                }

                t.commit();

            } catch(const odb::exception& e) {

                cerr << e.what() << endl;

            }

            return true;
        }

};

#endif // RLAIRCRAFTREPOSITORY_H

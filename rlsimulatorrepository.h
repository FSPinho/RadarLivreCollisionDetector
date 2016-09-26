#ifndef RLAIRCRAFTSIMULATOR_H
#define RLAIRCRAFTSIMULATOR_H

#include <string>
#include <vector>
#include <cmath>
#include <ctime>
#include <thread>
#include <unistd.h>

#include <rlrepository.h>
#include <rlmath.h>
#include <rlutil.h>
#include <cpr/cpr.h>
#include <sstream>

using namespace std;

class SimulatorRepository : public Repository {

    private:
        thread currentPropagationThread;

        int aircraftCount;
        double minLat;
        double minLng;
        double minAlt;
        double minVel;
        double maxLat;
        double maxLng;
        double maxAlt;
        double maxVel;

    public:

        SimulatorRepository(
                int aircraftCount,
                double minLat = -90,
                double minLng = -180,
                double minAlt = 0,
                double minVel = 800 / 3.6,
                double maxLat = 90,
                double maxLng = 180,
                double maxAlt = 1,
                double maxVel = 900 / 3.6) : Repository() {


            this->aircraftCount = aircraftCount;
            this->minLat = minLat;
            this->minLng = minLng;
            this->minAlt = minAlt;
            this->minVel = minVel;
            this->maxLat = maxLat;
            this->maxLng = maxLng;
            this->maxAlt = maxAlt;
            this->maxVel = maxVel;

            srand(time(NULL));

            for(int i = 0; i < aircraftCount; i++) {
                double lat = minLat + (double)(rand() % (int)((maxLat - minLat) * 1000))/1000.0;
                double lng = minLng + (double)(rand() % (int)((maxLng - minLng) * 1000))/1000.0;
                double alt = minAlt + (double)(rand() % (int)((maxAlt - minAlt) * 1000))/1000.0;
                double vel = minVel + (double)(rand() % (int)((maxVel - minVel) * 1000))/1000.0;

                double angle = Math::radiansToDegrees(atan2(lng, lat)) + 180;
                double angle2 = angle;

                // double variation = ((float)(rand() % 1000) / 1000.0) - 1;
                angle2 = angle + 0;

                vector<ADSBInfo*> trajectory;
                trajectory.push_back(
                            new ADSBInfo(time(NULL),
                                GlobalPosition(lat, lng, alt),
                                ENUVector(vel, 0, 0).rotateZTo(angle)
                            )
                );
                trajectory.push_back(
                            new ADSBInfo(time(NULL),
                                GlobalPosition(lat, lng, alt),
                                ENUVector(vel, 0, 0).rotateZTo(angle2)
                            )
                );

                aircrafts.push_back(new Aircraft(rand(), trajectory));
            }

/*
            vector<ADSBInfo*> trajectory;
            trajectory.push_back(
                        new ADSBInfo(Util::getSystemTimestamp(-1000),
                            GlobalPosition(0.05, 0.001, 0),
                            ENUVector(0, -300 / 3.6, 0)
                        )
            );
            trajectory.push_back(
                        new ADSBInfo(Util::getSystemTimestamp(),
                             GlobalPosition(0.05, 0.001, 0),
                             ENUVector(0, -300 / 3.6, 0)
                        )
            );

            aircrafts.push_back(new Aircraft(rand(), trajectory));

            vector<ADSBInfo*> trajectory2;
            trajectory2.push_back(
                        new ADSBInfo(Util::getSystemTimestamp(-1000),
                             GlobalPosition(0.001, 0.05, 0),
                             ENUVector(-300 / 3.6, 0, 0)
                        )
            );
            trajectory2.push_back(
                        new ADSBInfo(Util::getSystemTimestamp(),
                             GlobalPosition(0.001, 0.05, 0),
                             ENUVector(-300 / 3.6, 0, 0)
                        )
            );

            aircrafts.push_back(new Aircraft(rand(), trajectory2));

            vector<ADSBInfo*> trajectory3;
            trajectory3.push_back(
                        new ADSBInfo(Util::getSystemTimestamp(-1000),
                             GlobalPosition(0.001, -0.05, 0),
                             ENUVector(300 / 3.6, 0, 0)
                        )
            );
            trajectory3.push_back(
                        new ADSBInfo(Util::getSystemTimestamp(),
                             GlobalPosition(0.001, -0.05, 0),
                             ENUVector(300 / 3.6, 0, 0)
                        )
            );

            aircrafts.push_back(new Aircraft(rand(), trajectory3));
*/

            currentPropagationThread = thread(&SimulatorRepository::initPropagation, this);

        }

        void initPropagation() {
            // int pCount = 0;
            while(true) {

                long timestamp = Util::getSystemTimestamp();

                //if(pCount % 1000 == 0)
                //    sendSimulationToServer();
                //pCount += 1000;

                for(auto i: aircrafts) {

                    Aircraft * a = i;
                    ADSBInfo * info = a->getLastKnownInfo();
                    ENUVector newVelocity = info->velocity;

                    int trajectorySize = a->trajectory.size();
                    if(trajectorySize >= 2) {
                        double groundTrackHeadingTurnRate =
                                a->trajectory[trajectorySize - 1]->groundTrackHeading -
                                a->trajectory[trajectorySize - 2]->groundTrackHeading;
                        newVelocity.rotateZ(groundTrackHeadingTurnRate);
                    }

                    double distance = info->velocity.normalizeHorizontal();
                    double R = Math::EARTH_RADIUS;
                    double lat1 = Math::degreesToRadians(info->globalPosition.latitude);
                    double lng1 = Math::degreesToRadians(info->globalPosition.longitude);
                    double bearing = Math::degreesToRadians(info->velocity.getAngleInverse());

                    double lat2 = Math::radiansToDegrees(
                                asin( sin(lat1) * cos(distance / R) +
                                cos(lat1) * sin(distance / R) * cos(bearing))
                    );

                    double lng2 = Math::radiansToDegrees(
                                lng1 + atan2(sin(bearing) * sin(distance / R) * cos(lat1),
                                             cos(distance / R) - sin(lat1) * sin(lat2))
                    );

                    a->addInfo(
                        new ADSBInfo(Util::getSystemTimestamp(),
                            GlobalPosition(lat2, lng2, info->globalPosition.altitude),
                            newVelocity
                        )
                    );

                }

                long elapsed = Util::getSystemTimestamp() - timestamp;

                usleep((1000 - (elapsed < 1000? elapsed: 0)) * 1000);
            }
        }

        void sendSimulationToServer() {
            stringstream ss;

            ss << "[\n";
            unsigned int index = 0;
            for(auto a: aircrafts) {
                ADSBInfo * i = a->getLastKnownInfo();
                ss << "\t{\n";
                ss << "\t\t\"modeSCode\": \"" << a->id << "\", \n";
                ss << "\t\t\"callsign\": \"" << a->id << "\", \n";
                ss << "\t\t\"latitude\": " << i->globalPosition.latitude << ", \n";
                ss << "\t\t\"longitude\": " << i->globalPosition.longitude << ", \n";
                ss << "\t\t\"altitude\": " << Math::metresToFt(i->globalPosition.altitude) << ", \n";
                ss << "\t\t\"horizontalVelocity\": " << Math::metresToKnots(i->velocity.normalizeHorizontal()) << ", \n";
                ss << "\t\t\"verticalVelocity\": " << Math::metresToKnots(i->velocity.up) << ", \n";
                ss << "\t\t\"groundTrackHeading\": " << i->velocity.getAngleInverse() << ", \n";
                ss << "\t\t\"timestamp\":" << Util::getSystemTimestamp() << ", \n";
                ss << "\t\t\"timestampSent\":" << Util::getSystemTimestamp() << ", \n";
                ss << "\t\t\"collectorKey\": " << "\"24510ff9-6a9f-40f4-b265-778cfc807cc4\", \n" <<  endl;
                ss << "\t\t\"messageDataId\": \"\"" << ", \n";
                ss << "\t\t\"messageDataPositionEven\": \"\"" << ", \n";
                ss << "\t\t\"messageDataPositionOdd\": \"\"" << ", \n";
                ss << "\t\t\"messageDataVelocity\": \"\"" << "\n";
                ss << "\t}";
                index++;
                if(index < aircrafts.size())
                    ss << ", \n";
            }

            ss << "]\n";

            string json = ss.str();

            auto r = cpr::Post(cpr::Url{"http://localhost:8000/api/adsb_info/"},
                                  cpr::Authentication{"root", "root7685"},
                                  cpr::Header{{"Content-Type", "application/json"}},
                                  cpr::Body{json});

            auto r2 = cpr::Put(cpr::Url{"http://localhost:8000/api/collector/24510ff9-6a9f-40f4-b265-778cfc807cc4/"},
                                  cpr::Authentication{"root", "root7685"},
                                  cpr::Header{{"Content-Type", "application/json"}},
                                  cpr::Body{"{}"});

            if(r.status_code >= 400)
                cout << "Status code: " << r.status_code << " : " << r.text << endl;

        }

        bool reloadAircrafts() {
            return true;
        }

};

#endif // RLAIRCRAFTSIMULATOR_H
















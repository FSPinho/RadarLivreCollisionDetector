#ifndef RADARLIVRE_API_OBSERVATION_H
#define RADARLIVRE_API_OBSERVATION_H

#include <odb/core.hxx>
#include <string>

using namespace std;

// #pragma db value(double) type("NUMERIC")

// #pragma db object
class radarlivre_api_observation {

public:

    unsigned long getId() const { return id; }
    void setId(unsigned long value) { id = value; }
    double getLatitude() const { return latitude; }
    void setLatitude(double value) { latitude = value; }
    double getLongitude() const { return longitude; }
    void setLongitude(double value) { longitude = value; }
    double getAltitude() const { return altitude; }
    void setAltitude(double value) { altitude = value; }
    double getVerticalVelocity() const { return verticalVelocity; }
    void setVerticalVelocity(double value) { verticalVelocity = value; }
    double getHorizontalVelocity() const { return horizontalVelocity; }
    void setHorizontalVelocity(double value) { horizontalVelocity = value; }
    double getGroundTrackHeading() const { return groundTrackHeading; }
    void setGroundTrackHeading(double value) { groundTrackHeading = value; }
    unsigned long getTimestamp() const { return timestamp; }
    void setTimestamp(unsigned long value) { timestamp = value; }
    unsigned long getFlightId() const { return flight_id; }
    void setFlightId(unsigned long value) { flight_id = value; }

private:

    radarlivre_api_observation() {}

    friend class odb::access;

    // #pragma db id
    unsigned long id;
    unsigned long flight_id;

    double latitude;
    double longitude;
    double altitude;

    double verticalVelocity;
    double horizontalVelocity;
    double groundTrackHeading;

    unsigned long timestamp;
};

// #pragma db object(radarlivre_api_observation)
// #pragma db member(radarlivre_api_observation::id) id

#endif // RADARLIVRE_API_OBSERVATION_H





















#ifndef RLFLIGHTINFO_H
#define RLFLIGHTINFO_H

#include <odb/core.hxx>
#include <string>

using namespace std;

// #pragma db object
class radarlivre_api_flightinfo {

public:
    unsigned long getId() const { return id; }
    void setId(unsigned long value) { id = value; }

    unsigned long getFlightId() const { return flight_id; }
    void setFlightId(unsigned long value) { flight_id = value; }

    unsigned long getAirlineId() const { return airline_id; }
    void setAirlineId(unsigned long value) { airline_id = value; }

    unsigned long getTimestamp() const { return timestamp; }
    void setTimestamp(unsigned long value) { timestamp = value; }

private:
    radarlivre_api_flightinfo() {}

    friend class odb::access;

    // #pragma db id
    unsigned long id;
    unsigned long flight_id;
    unsigned long airline_id;
    unsigned long timestamp;

};


// #pragma db object(radarlivre_api_flightinfo)
// #pragma db member(radarlivre_api_flightinfo::id) id

#endif // RLFLIGHT_H


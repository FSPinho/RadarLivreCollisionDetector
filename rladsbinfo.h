#ifndef RLADSBINFO_H
#define RLADSBINFO_H

#include <iostream>

#include <rlvector2d.h>
#include <rlglobalposition.h>

using namespace std;

class ADSBInfo {

    public:

        long timestamp;
        GlobalPosition globalPosition;
        ENUVector velocity;
        double groundTrackHeading;


        ADSBInfo(long timestamp, GlobalPosition globalPosition, ENUVector velocity, double groundTrackHeading) :
        timestamp(timestamp), globalPosition(globalPosition), velocity(velocity), groundTrackHeading(groundTrackHeading) { }

        ADSBInfo(long timestamp, GlobalPosition globalPosition, ENUVector velocity) :
        ADSBInfo(timestamp, globalPosition, velocity, velocity.getAngle()) {}

        string toString() {
            char buff[2048];
            sprintf(buff, "ADSBInfo(timestamp=%ld, globPosition=%s, vel=%s, gtr=%.3f)", timestamp, globalPosition.toString().c_str(), velocity.toString().c_str(), groundTrackHeading);
            return string(buff);
        }

};

#endif // RLADSBINFO_H

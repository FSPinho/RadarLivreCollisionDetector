#ifndef RLPROPAGATEDINFO_H
#define RLPROPAGATEDINFO_H

#include <iostream>
#include <rlenuvector.h>

using namespace std;

class PropagatedInfo {

    public:

        long timestamp;
        ENUVector relativePosition;
        ENUVector velocity;
        double groundTrackHeading;
        double groundTrackHeadingTurnRate;

        PropagatedInfo() {}
        PropagatedInfo(long timestamp, ENUVector relativePosition, ENUVector velocity, double groundTrackHeading, double groundTrackHeadingTurnRate) :
        timestamp(timestamp), relativePosition(relativePosition), velocity(velocity), groundTrackHeading(groundTrackHeading), groundTrackHeadingTurnRate(groundTrackHeadingTurnRate) { }

        ~PropagatedInfo() {

        }

        string toString() {
            char buff[2048];
            sprintf(buff, "PropInfo(timestamp=%ld, relPosition=%s, vel=%s, gtr=%.3f)", timestamp, relativePosition.toString().c_str(), velocity.toString().c_str(), groundTrackHeading);
            return string(buff);
        }

};

#endif // RLPROPAGATEDINFO_H

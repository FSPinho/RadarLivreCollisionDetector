#ifndef RLAIRCRAFT_H
#define RLAIRCRAFT_H

#include <iostream>
#include <vector>
#include <list>

#include <rladsbinfo.h>
#include <rlpropagatedinfo.h>
#include <rlcylinder.h>
#include <rlutil.h>

using namespace std;

class Aircraft {

    public:

        constexpr static const double RCAZ = 528.0 * 0.3048;
        constexpr static const double HCAZ = 200.0 * 0.3048;
        constexpr static const double RPAZMIN = 528.0 * 0.3048;
        constexpr static const double HPAZMIN = 300.0 * 0.3048;
        constexpr static const double DEFAULT_HORIZONTAL_BUFFER_TIME = 10;
        constexpr static const double DEFAULT_VERTICAL_BUFFER_TIME = 10;
        constexpr static const double DEFAULT_LATITUDE_SECTION = 5;

        long id;
        int area;
        vector<ADSBInfo*> trajectory;
        list<PropagatedInfo*> propagatedTrajectory;

        Aircraft(long id) : Aircraft(id, vector<ADSBInfo*>()) {}

        Aircraft(long id, vector<ADSBInfo*> trajectory) : id(id), trajectory(trajectory) {

            if(this->trajectory.empty()) {
                this->trajectory.push_back(
                            new ADSBInfo(0, GlobalPosition(0, 0, 0), ENUVector(0, 0, 0), 0)
                );
                this->trajectory.push_back(
                            new ADSBInfo(0, GlobalPosition(0, 0, 0), ENUVector(0, 0, 0), 0)
                );
            }

            area = getLastKnownInfo()->globalPosition.latitude / DEFAULT_LATITUDE_SECTION;

        }

        ~Aircraft() {
            Util::clear(trajectory);
            Util::clear(propagatedTrajectory);
        }

        bool operator ==(Aircraft* another) {
            return id == another->id;
        }

        bool operator !=(Aircraft* another) {
            return !(this == another);
        }

        Cylinder getCAZ() {
            return Cylinder(getLastPropagatedInfo()->relativePosition, RCAZ, HCAZ);
        }

        bool hasCazConflict(Aircraft* another) {
            Cylinder caz = getCAZ();
            Cylinder cazAnother = caz;
            cazAnother.center = another->getLastPropagatedInfo()->relativePosition;
            return caz.intersects(cazAnother);
        }

        Cylinder getPAZ(Aircraft * another, long interval=0) {

            PropagatedInfo * lastInfo = getLastPropagatedInfo();
            PropagatedInfo * anotherLastInfo = another->getLastPropagatedInfo();

            Vector2D relativeHorizontalVelocity(lastInfo->velocity.east - anotherLastInfo->velocity.east,
                                                lastInfo->velocity.north - anotherLastInfo->velocity.north);

            double escalarRelativeHorizontalVelocity = relativeHorizontalVelocity.normalize();
            double escalarRelativeVerticalVelocity = lastInfo->velocity.up - anotherLastInfo->velocity.up;

            double dInterval = (double)interval * 0.001;
            double radiusIncrease = dInterval * lastInfo->velocity.normalizeHorizontal();
            double heightIncrease = dInterval * lastInfo->velocity.up;

            return Cylinder(
                        getLastPropagatedInfo()->relativePosition,
                        RPAZMIN + max(0.0, escalarRelativeHorizontalVelocity) * DEFAULT_HORIZONTAL_BUFFER_TIME + radiusIncrease,
                        HPAZMIN + max(0.0, escalarRelativeVerticalVelocity) * DEFAULT_VERTICAL_BUFFER_TIME + (heightIncrease * 2)
            );
        }

        bool hasPazConflict(Aircraft* another, long interval=0) {
            Cylinder paz = getPAZ(another, interval);
            Cylinder pazAnother = paz;
            pazAnother.center = another->getLastPropagatedInfo()->relativePosition;
            return paz.intersects(pazAnother);
        }

        PropagatedInfo * propagateTrajectory(long timestampInterval /* Milliseconds */) {

            double interval = ((double)timestampInterval * 0.001);

            PropagatedInfo * i = getLastPropagatedInfo();

            long newTimestamp = i->timestamp + timestampInterval;
            double newGroundTrackHeading = i->groundTrackHeading + (i->groundTrackHeadingTurnRate * interval);
            ENUVector newPosition = i->relativePosition + (i->velocity * interval);
            ENUVector newVelocity = i->velocity.rotateZTo(newGroundTrackHeading);

            addPropagatedInfo(
                        new PropagatedInfo (
                            newTimestamp,
                            newPosition,
                            newVelocity,
                            newGroundTrackHeading,
                            i->groundTrackHeadingTurnRate
                        )
            );

            return getLastPropagatedInfo();
        }

        ADSBInfo * getLastKnownInfo() {
            return trajectory.at(trajectory.size() - 1);
        }

        double getEstimatedGroundTrackHeadingTurnRate() {
            if(trajectory.size() >= 2) {
                double turnRateInterval = double(trajectory.at(trajectory.size() - 1)->timestamp -
                        trajectory.at(trajectory.size() - 2)->timestamp);

                if(turnRateInterval == 0.0) return 0.0;

                double turnRate = (trajectory.at(trajectory.size() - 1)->groundTrackHeading -
                        trajectory.at(trajectory.size() - 2)->groundTrackHeading) / turnRateInterval;

                return turnRate;
            }

            return 0;
        }

        PropagatedInfo * setFirstPropagationRelativeToMyself() {
            return setFirstPropagationRelativeTo(this);
        }

        PropagatedInfo * setFirstPropagationRelativeTo(Aircraft * another) {
            if(this != another) {
                ADSBInfo * lastInfo = getLastKnownInfo();
                ADSBInfo * anotherLastInfo = another->getLastKnownInfo();
                ENUVector position = lastInfo->globalPosition.getPositionRelativeTo(anotherLastInfo->globalPosition);
                addPropagatedInfo(
                    new PropagatedInfo(
                        lastInfo->timestamp,
                        position,
                        lastInfo->velocity,
                        lastInfo->groundTrackHeading,
                        getEstimatedGroundTrackHeadingTurnRate()
                    )
                );
            }

            return getLastPropagatedInfo();
        }

        PropagatedInfo * getLastPropagatedInfo() {
            if(propagatedTrajectory.empty()) {
                ADSBInfo * i = getLastKnownInfo();
                addPropagatedInfo(new PropagatedInfo(
                    i->timestamp,
                    ENUVector(0, 0, 0),
                    i->velocity,
                    i->groundTrackHeading,
                    getEstimatedGroundTrackHeadingTurnRate())
                );
            }

            return propagatedTrajectory.back();
        }

        void addPropagatedInfo(PropagatedInfo * info) {
            propagatedTrajectory.push_back(info);
            if(propagatedTrajectory.size() > 2) {
                delete propagatedTrajectory.front();
                propagatedTrajectory.pop_front();
            }
        }

        void addInfo(ADSBInfo * info) {
            trajectory.push_back(info);
        }

        void fitTwo() {
            while(trajectory.size() > 2) {
                delete trajectory[0];
                trajectory.erase(trajectory.begin());
            }
        }

        Aircraft * getCopy() {
            vector<ADSBInfo*> copyTrajectory;
            int limit = 2;
            for(auto &i: trajectory) {
                copyTrajectory.push_back(new ADSBInfo(i->timestamp,
                                                      i->globalPosition,
                                                      i->velocity,
                                                      i->groundTrackHeading));
                if(!(--limit)) break;
            }
            return new Aircraft(id, copyTrajectory);
        }

        bool isValidToColisionDetection() {
            return trajectory.size() >= 2;
        }

        bool isNear(Aircraft* another) {
            return abs(area - another->area) <= 1;
        }

        string toString() {
            char buff[1000];
            if(trajectory.size() >= 2)
                sprintf(buff, "Aircraft(id=%ld, \n\ttrajectory[\n\t\t%s, \n\t\t%s])", id, trajectory[0]->toString().c_str(), trajectory[1]->toString().c_str());
            else if(trajectory.size() == 1)
                sprintf(buff, "Aircraft(id=%ld, \n\ttrajectory[\n\t\t%s])", id, trajectory[0]->toString().c_str());
            else
                sprintf(buff, "Aircraft(id=%ld, \n\ttrajectory[])", id);
            return string(buff);
        }

};

#endif // RLAIRCRAFT_H

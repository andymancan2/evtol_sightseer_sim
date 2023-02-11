//
// Created by ajlou on 2/11/2023.
//

#ifndef EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H
#define EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H

#include <cstdint>
#include <exception>

enum evtol_prop_exc_ids
{
    INVALID_PASSENGER_PARAM,
    INVALID_ENERGY_USE,
    INVALID_CRUISE,
    INVALID_BATT_CAPACITY,
};
struct evtol_prop_exc : public std::exception
{
    const uint32_t kEId;
    evtol_prop_exc( uint32_t EId );
    const uint32_t  getEId();
};


class evtol_prop {
public:
    const uint32_t kCruiseSpeedMph;
    const uint32_t kBattKwh;
    const double kTimeToChargeHrs;
    const double kEnergyUseKwhPerMile;
    const uint32_t kMaxPassengers;
    const double kFaultProbPerHour;
    const double kMaxDistance;
    evtol_prop(
            uint32_t cruiseSpeedMph,
            uint32_t battKwh,
            double timeToChargeHrs,
            double energyUseKwhPerMil,
            uint32_t maxPassergers,
            double faultProbPerHour
    );
    void disp( void );

};


#endif //EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H

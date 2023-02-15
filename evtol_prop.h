//
// Created by ajlou on 2/11/2023.
//

#ifndef EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H
#define EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H

#include <cstdint>
#include <exception>
#include <map>

enum evtol_prop_exc_ids_e
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
    const char* kName;
    const uint32_t kCruiseSpeedMph;
    const uint32_t kBattKwh;
    const double kTimeToChargeHrs;
    const double kEnergyUseKwhPerMile;
    const uint32_t kMaxPassengers;
    const double kFaultProbPerHour;
    const double kMaxDistance;
    const double kMaxCruiseHrs;
    evtol_prop(
            const char* name,
            uint32_t cruiseSpeedMph,
            uint32_t battKwh,
            double timeToChargeHrs,
            double energyUseKwhPerMil,
            uint32_t maxPassergers,
            double faultProbPerHour
    );
    //evtol_prop();
    void disp( void );

};

enum evtol_companies_e
{
    ALPHA,
    BRAVO,
    CHARLIE,
    DELTA,
    ECHO,

    NUM_COMPANIES
};

class evtol_list
{
    evtol_prop alpha;
    evtol_prop bravo;
    evtol_prop charlie;
    evtol_prop delta;
    evtol_prop echo;
    evtol_prop *lst[ NUM_COMPANIES ];
    uint16_t maxPassCnt;
public:
    evtol_list();
    evtol_prop * getCompanyProperty( evtol_companies_e company );
    double getCruiseMinEnergy( evtol_companies_e company );
    double getChargeMinEnergy( evtol_companies_e company );
    double getCruiseMinDistance( evtol_companies_e company );
    uint16_t getRndVehPassCnt( void );
    void unitTest( void );

    void disp( void );
};



#endif //EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H

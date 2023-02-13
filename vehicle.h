//
// Created by ajlou on 2/12/2023.
//

#ifndef EVTOL_SIGHTSEER_SIM_VEHICLE_H
#define EVTOL_SIGHTSEER_SIM_VEHICLE_H

#include "evtol_prop.h"
#include <vector>

enum vehicle_state_e
{
    IDLE,
    IN_FLIGHT,
    WAITING_FOR_CHARGER,
    CHARGE_IN_PROGRESS
};

enum sim_result_e
{
    NO_CHANGE,
    FLIGHT_COMPLETE,
    CHARGE_COMOPLETE
};

class vehicle {
    const evtol_companies_e kCompany;
    vehicle_state_e currentState;
    double batteryCapacity;
public:
    vehicle( evtol_companies_e company );
    sim_result_e sim( void );
    void startFlight( void );
    //TODO: Add mission log.
};

const uint16_t kNumChargers = 3;
#if 0
enum charger_state_e
{
    EMPTY,
    OCCUPIED
};
class charger
{
    charger_state_e state;

};
#endif
const uint16_t kEmptyCharger = 0xffff;
const uint16_t kInvalidVeh = 0xfffe;
class charger_stations
{
    uint16_t chargers[ kNumChargers ];
public:
    charger_stations( void );
    bool addVehToCharger( uint16_t vehID );
    bool freeVehFromCharger( uint16_t vehID );
};
class flight_mission {
    const uint8_t neededPassengerCnt;
public:
    flight_mission( uint8_t passCnt );
};
class veh_sim {
public:
    std::vector< vehicle > v;
    std::vector< uint16_t > idleVs;
    std::vector< uint16_t > needChargingQ;
    std::vector< flight_mission > missionQ;
    charger_stations chargerStations;

    veh_sim( uint32_t seed=0);
    uint16_t findIdleVeh( flight_mission mission );
    uint16_t findChargingQVeh( flight_mission mission );
    void simMinute( void );
    void simulate( uint16_t simMinutes,  uint32_t seed=1000 );
};



#endif //EVTOL_SIGHTSEER_SIM_VEHICLE_H

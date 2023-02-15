//
// Created by ajlou on 2/12/2023.
//

#ifndef EVTOL_SIGHTSEER_SIM_VEHICLE_H
#define EVTOL_SIGHTSEER_SIM_VEHICLE_H

#include "evtol_prop.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#define VEHICLE_STATES \
    vstype(IDLE), \
    vstype(IN_FLIGHT),   \
    vstype(WAIT_CHARGE),  \
    vstype(IN_CHARGE)
#define vstype(x) VS_##x
typedef enum { VEHICLE_STATES } vehicle_state_e;

enum sim_result_e
{
    NO_CHANGE,
    FLIGHT_COMPLETE,
    CHARGE_COMOPLETE
};

class vehicle {
public:
    const evtol_companies_e kCompany;
    const uint32_t faultScaler;
private:
    vehicle_state_e currentState;
    double batteryCapacity;
    uint16_t missionPassCnt;
    uint16_t totFaultCount;
    uint16_t missionFaultCount;
    uint16_t numFlights;
    uint16_t numCharges;
    uint16_t totFlightTime;
    uint16_t missionFlightTime;
    uint16_t totChargeTime;
    uint16_t missionChargeTime;
    double   totDistance;
    double   missionDistance;
    double   totPassMiles;
public:
    vehicle( evtol_companies_e company );
    sim_result_e sim( void );
    void startFlight( uint16_t numPass );
    void startCharging( void );
    vehicle_state_e getCurrentState( void );
    void disp( void );
    void displayStatistics( string s );
    string logEntry( void );
};

const uint16_t kNumChargers = 3;

const uint16_t kEmptyCharger   = 0xffff;
const uint16_t kInvalidVeh     = 0xfffe;
const uint16_t kInvalidMission = 0xfffd;

class charger_stations
{
    uint16_t chargers[ kNumChargers ];
public:
    charger_stations( void );
    bool addVehToCharger( uint16_t vehID );
    bool freeVehFromCharger( uint16_t vehID );
    uint16_t  getNumOfChargersAvail( void );
    uint16_t  getNumOfChargersInUse( void );
    void getChargingList( std::vector<uint16_t> &chargingList );
};
class veh_sim {
private:
    ofstream log;
    const uint32_t seedForRandomVehicleConfiguration;
    uint32_t seedForStartingSimulation;
public:
    std::vector< vehicle > v;
    std::vector< uint16_t > idleVs;
    std::vector< uint16_t > needChargingQ;
    std::vector< uint16_t > missionQ;
    charger_stations chargerStations;

    veh_sim( uint32_t seed=0);
    ~veh_sim();
    uint16_t findMissionCableVeh( vector<uint16_t> idleVect,uint16_t mission );
    void reorderChargerWaitingQForNextMissions( uint16_t numChargersAvail );
    void simMinute( void );
    void simulate( uint16_t simMinutes,  uint32_t seed=1000 );
    void addLogHeader( void );
    void addLogRow( uint16_t simMin );
    void checkSimConsistency( void );
    void displayVehicleStats( void );
};



#endif //EVTOL_SIGHTSEER_SIM_VEHICLE_H

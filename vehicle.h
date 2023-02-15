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

//TODO: This 'C' language pattern for managing enums and string versions of enums is convenient.
//      But may need tobe revised for coding standard.
#define VEHICLE_STATES \
    vstype(IDLE), \
    vstype(IN_FLIGHT),   \
    vstype(WAIT_CHARGE),  \
    vstype(IN_CHARGE)
#define vstype(x) VS_##x
typedef enum { VEHICLE_STATES } vehicle_state_e;

/// \brief A vehicle simulation result may cause a hi-level simulation to move a vehicle between lists.
enum sim_result_e
{
    NO_CHANGE,
    FLIGHT_COMPLETE,
    CHARGE_COMOPLETE
};

/// \brief Class to simulate a evtol vehicle.
class vehicle {
public:
    const evtol_companies_e kCompany;
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
    sim_result_e sim( );
    void startFlight( uint16_t numPass );
    void startCharging( );
    vehicle_state_e getCurrentState( );
    void disp( );
    void displayStatistics( string s );
    string logEntry( );
};

const uint16_t kNumSimVehicles =  20;
const uint16_t kNumChargers = 3;

/// \brief List of constants below representing an invalid index in an array or queue.
const uint16_t kEmptyCharger   = 0xffff;
const uint16_t kInvalidVeh     = 0xfffe;

/// \brief Simulate the chargers in the simulation.
class charger_stations
{
    /// \brief A value in array is the vehicle ID, which is the index in the simulated vector v.
    uint16_t chargers[ kNumChargers ];
public:
    charger_stations( );
    ~charger_stations( );
    bool addVehToCharger( uint16_t vehID );
    bool freeVehFromCharger( uint16_t vehID );
    uint16_t  getNumOfChargersAvail( );
    uint16_t  getNumOfChargersInUse( );
};

/// \brief The hi-level class to perform the simulation.
class veh_sim {
private:
    /// \brief The log file is a per-minute tab-separated file, which viewed best as a spreadsheet.
    ofstream log;
    /// \brief Seed used for random vehicle configuration.
    const uint32_t seedForRandomVehicleConfiguration;
    /// \brief Seed used for randomizing the simulation execution.
    uint32_t seedForStartingSimulation;
public:
    /// \brief The list of vehicles which the size remains constant for the simulation.
    std::vector< vehicle > v;
    /// \brief A list of idle vehicles.
    std::vector< uint16_t > idleVs;
    /// \brief A list of vehicles that need charging.
    std::vector< uint16_t > needChargingQ;
    /// \brief A list of future missions to perform limited by the number of vehicles in the simulation.
    std::vector< uint16_t > missionQ;
    /// \brief Used to simulate the charging for vehicles.
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

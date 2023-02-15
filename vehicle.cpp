//
// Created by ajlou on 2/12/2023.
//

#include "vehicle.h"
#include "util.h"
#include "sim_log.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <algorithm>

using namespace std;

// Global variables
evtol_list evtolLst;
//std::vector< vehicle > v;
#undef vstype
#define vstype(x) #x
static const char *strVehStates[] = { VEHICLE_STATES };

vehicle::vehicle( evtol_companies_e company )
    : kCompany( company ),
    currentState( VS_IDLE ),
    faultScaler( 100000 ),
    totFaultCount( 0 ),
    numFlights( 0 ),
    numCharges( 0 ),
    totFlightTime( 0 ),
    totChargeTime( 0 ),
    totDistance( 0.0 ),
    missionPassCnt( 0 )
{
    missionChargeTime = 0;
    missionFlightTime = 0;
    missionDistance = 0.0;
    missionFaultCount = 0;
    totPassMiles = 0.0;

    batteryCapacity = evtolLst.getCompanyProperty( kCompany )->kBattKwh;

}
vehicle_state_e vehicle::getCurrentState( void )
{
    return currentState;
}
sim_result_e vehicle::sim( void )
{
    sim_result_e RV = NO_CHANGE;
    vehicle_state_e nextState = currentState;

    switch (currentState)
    {
        case VS_IDLE:
            break;
        case VS_IN_FLIGHT:
            missionFlightTime++;
            missionDistance += evtolLst.getCruiseMinDistance( kCompany );
            if (rand() < (( evtolLst.getCompanyProperty(kCompany)->kFaultProbPerHour * RAND_MAX ) / 60))
            {
                missionFaultCount++;
            }
            batteryCapacity -= evtolLst.getCruiseMinEnergy( kCompany );
            //if (cmpd(batteryCapacity, 0.0, evtolLst.getCruiseMinEnergy( kCompany )))
            if (cmpLessOrEqual(batteryCapacity, evtolLst.getCruiseMinEnergy( kCompany )))
            {
                batteryCapacity = 0.0;
                nextState = VS_WAIT_CHARGE;
                RV = FLIGHT_COMPLETE;
                totPassMiles += (missionDistance * missionPassCnt);
                missionPassCnt = 0;
                numFlights++;
                totFlightTime += missionFlightTime;
                missionFlightTime = 0;
                totDistance += missionDistance;
                missionDistance = 0;
                totFaultCount += missionFaultCount;
                missionFaultCount = 0;
            }
            else
            {
                //batteryCapacity -= evtolLst.getCruiseMinEnergy( kCompany );
            }
            break;
        case VS_WAIT_CHARGE:
            break;
        case VS_IN_CHARGE:
            missionChargeTime++;
            if (cmpGreaterOrEqual(batteryCapacity,
                    evtolLst.getCompanyProperty( kCompany )->kBattKwh))
            {
                batteryCapacity = evtolLst.getCompanyProperty( kCompany )->kBattKwh;
                nextState = VS_IDLE;
                RV = CHARGE_COMOPLETE;
                totChargeTime += missionChargeTime;
                missionChargeTime = 0;
                numCharges++;
            }
            else
            {
                batteryCapacity += evtolLst.getChargeMinEnergy( kCompany );
            }
            break;
        default:
            assert( 1 );//Should not reach here.
            break;
    }
    currentState = nextState;


    return RV;
}
void vehicle::startFlight( uint16_t numPass )
{
    currentState = VS_IN_FLIGHT;
    missionPassCnt = numPass;
}
void vehicle::startCharging( void )
{
    currentState = VS_IN_CHARGE;
}

string vehicle::logEntry( void )
{
    string s;
    extern const char *strCompanies[];

    switch (currentState)
    {
        case VS_IDLE:
            s = (string)strVehStates[currentState] + "-" + (string)strCompanies[ kCompany ];
            break;
        case VS_IN_FLIGHT:
            s = (string)strVehStates[currentState] + "-"
                    + to_string( missionPassCnt ) + "-"
                    + to_string(missionFlightTime);
            break;
        case VS_WAIT_CHARGE:
            s = (string)strVehStates[currentState] + "-" + (string)strCompanies[ kCompany ];
            break;
        case VS_IN_CHARGE:
            s = (string)strVehStates[currentState] + "-"
                + to_string( missionChargeTime );
            break;
    }
    return s;
}
charger_stations::charger_stations( void )
{
    for (uint16_t i=0; i < kNumChargers; i++) {
        chargers[i] = kEmptyCharger;
    }
}

void vehicle::disp( void )
{
    cout << "company: " << kCompany << endl
         << "   battery: " << batteryCapacity << endl
         << "      minute cruise energy: " << evtolLst.getCruiseMinEnergy( kCompany ) << endl
         << "      minute charge energy: " << evtolLst.getChargeMinEnergy( kCompany ) << endl
         << "   numFlights: " << numFlights << endl
         << "      totFlightTime: " << totFlightTime << ", missionFlightTime:" << missionFlightTime << endl
         << "      totDistance: " << totDistance << ", missionDistance: " << missionDistance << endl
         << "      totFaultCount: " << totFaultCount << ", missionFaultCount: " << missionFaultCount << endl
         << "   numCharges: " << numCharges << endl
         << "      totChargeTime: " << totChargeTime << ", missionChargeTime: " << missionChargeTime << endl;
}
/// \brief Display statistics for this vehicle.
//Keep track of the following statistics per vehicle type:
//- average flight time per flight
//   NOTE: For flight safety, a flight may end 1-minute early to due to 1 minute rouding errors.
//- average distance traveled per flight
//- average time charging per charge session
//   NOTE: For flight safety, a vehicle may charge an extra minute due to 1 minute rouding errors.
//- total number of faults
//- total number of passenger miles.
//NOTE: A partial flight or partial charge cycle is not incorporated
//      into the statistics when the simulation stops.
void vehicle::displayStatistics( string s )
{
    string flightTimePerFlight;
    string distancePerFlight;
    string timeChargingPerSession;
    if (numFlights > 0)
    {
        flightTimePerFlight = to_string((double)totFlightTime / numFlights );
        distancePerFlight = to_string( totDistance / numFlights );
    }
    else
    {
        flightTimePerFlight = "No flights were found.";
        distancePerFlight = "No flights were found.";
    }
    if (numCharges > 0)
    {
        timeChargingPerSession = to_string( (double)totChargeTime / numCharges );
    }
    else
    {
        timeChargingPerSession = "No charge cycles were found.";
    }
    cout << "Vehicle: " << s << endl;
    cout << "   Average flight time per flight (minutes) = " << flightTimePerFlight << endl;
    cout << "   Average distance traveled per flight (miles) = " << distancePerFlight << endl;
    cout << "   Average time charging per charge session (minutes) = " << timeChargingPerSession << endl;
    cout << "   Total number of faults = " << totFaultCount << endl;
    cout << "   Total number of passenger miles = " << totPassMiles << endl;
}
bool charger_stations::addVehToCharger( uint16_t vehID )
{
    bool RV = false;
    for (uint16_t i=0; i < kNumChargers; i++)
    {
        if (chargers[i] == kEmptyCharger)
        {
            chargers[i] = vehID;
            RV = true;
            break;
        }
    }
    return RV;
}
bool charger_stations::freeVehFromCharger( uint16_t vehID )
{
    bool RV = false;
    for (uint16_t i=0; i < kNumChargers; i++)
    {
        if (chargers[i] == vehID)
        {
            chargers[i] = kEmptyCharger;
            RV = true;
            break;
        }
    }
    return RV;
}
uint16_t  charger_stations::getNumOfChargersAvail( void )
{
    uint16_t numAvail = 0;

    for (uint16_t i=0; i < kNumChargers; i++)
    {
        if (chargers[i] == kEmptyCharger)
        {
            numAvail++;
        }
    }
    return numAvail;
}

uint16_t  charger_stations::getNumOfChargersInUse( void )
{
    uint16_t numInUse = 0;

    for (uint16_t i=0; i < kNumChargers; i++)
    {
        if (chargers[i] != kEmptyCharger)
        {
            numInUse++;
        }
    }
    return numInUse;
}

#if 0
void charger_stations::getChargingList( std::vector<uint16_t> &chargingList )
{

}
#endif

veh_sim::veh_sim( uint32_t seed )
    : seedForRandomVehicleConfiguration( seed ),
      log("simLog.tsv", std::ofstream::out)
{
    uint16_t rndVehCntByCompany[ C_NUM_COMPANIES ];
    uint16_t rndVehiclesLeftCnt = kNumSimVehicles;
    uint16_t rndVehForThisCompany = 0;
    uint16_t company = 0;

    srand( seed );
    //TODO: Combine next two loops into one, but now nice for random distribution review.
    for (company=C_ALPHA; company < (C_NUM_COMPANIES-1); company++)
    {
        cout << "  company = " << company << endl;
        rndVehCntByCompany[ company ] = ((( rand() % rndVehiclesLeftCnt ) / (C_NUM_COMPANIES - company - 1) ) + 1);
        rndVehiclesLeftCnt -= rndVehCntByCompany[ company ];
        cout << "    rndVehiclesLeftCnt = " << rndVehiclesLeftCnt << endl;
    }
    rndVehCntByCompany[ (C_NUM_COMPANIES-1) ] = rndVehiclesLeftCnt;
    cout << "rndVehCntByCompany: " << rndVehCntByCompany[0]
        << ", " << rndVehCntByCompany[1]
        << ", " << rndVehCntByCompany[2]
        << ", " << rndVehCntByCompany[3]
        << ", " << rndVehCntByCompany[4]
        << endl;
    cout << "v size = " << v.size() << "\n";
    for (company=C_ALPHA; company < (C_NUM_COMPANIES); company++)
    {
        cout << "  company = " << company << endl;
        cout << "    rndVehCntByCompany[ company ] = " << rndVehCntByCompany[ company ] << endl;
        for (uint16_t i=0; i < rndVehCntByCompany[ company ]; i++ )
        {
            v.push_back(  vehicle( (evtol_companies_e)company )  );
        }
        cout << "    v size = " << v.size() << "\n";
    }
    cout << "v size = " << v.size() << "\n";
    assert( v.size() == kNumSimVehicles );

    for (uint16_t i=0; i < kNumSimVehicles; i++ )
    {
        idleVs.push_back( i );
    }

    //sLog.addRowHeader();
    addLogHeader();
    addLogRow( 0 );
}

void veh_sim::addLogHeader( void )
{
    //log.open("simLog.tsv", std::ofstream::out);
    log << "TmMin\tIdleSz\tNeedChgQSz\tmssnQSz\tNumChg\t"\
            "V0\tV1\tV2\tV3\tV4\tV5\tV6\tV7\tV8\tV9\t"\
            "V10\tV11\tV12\tV13\tV14\tV15\tV16\tV17\tV18\tV19\n";
}

void veh_sim::addLogRow( uint16_t simMin )
{
    log << simMin << "\t" << idleVs.size() << "\t" << needChargingQ.size() << "\t"
        << missionQ.size() << "\t" << chargerStations.getNumOfChargersAvail() << "\t"
        << v[0].logEntry() << "\t" << v[1].logEntry() << "\t" << v[2].logEntry() << "\t" << v[3].logEntry() << "\t" << v[4].logEntry() << "\t"
        << v[5].logEntry() << "\t" << v[6].logEntry() << "\t" << v[7].logEntry() << "\t" << v[8].logEntry() << "\t" << v[9].logEntry() << "\t"
        << v[10].logEntry() << "\t" << v[11].logEntry() << "\t" << v[12].logEntry() << "\t" << v[13].logEntry() << "\t" << v[14].logEntry() << "\t"
        << v[15].logEntry() << "\t" << v[16].logEntry() << "\t" << v[17].logEntry() << "\t" << v[18].logEntry() << "\t" << v[19].logEntry() << "\t"
        << endl;
#if 0
    stringstream stream;
    stream << simMin << "\t" << idleVs.size() << "\t" << needChargingQ.size() << "\t"
           << missionQ.size() << "\t" << chargerStations.getNumOfChargersAvail() << endl;
    string s;
    //stream >> s;
    s = stream.str();
    cout << "row: " << s;
    sLog.addRow( s );
#endif

}

uint16_t veh_sim::findMissionCableVeh( vector<uint16_t> vehIDVect, uint16_t mission )
{
    uint16_t RV = kInvalidVeh;

    for (uint16_t i=0; i < vehIDVect.size(); i++)
    {
        if (evtolLst.getCompanyProperty( v[vehIDVect[i]].kCompany )->kMaxPassengers > mission)
        {
            if ((RV == kInvalidVeh) ||
                (evtolLst.getCompanyProperty( v[vehIDVect[i]].kCompany )->kMaxPassengers
                 < evtolLst.getCompanyProperty( v[RV].kCompany )->kMaxPassengers ))
            {
                RV = i;
            }
        }
    }

    return RV;
}

void veh_sim::reorderChargerWaitingQForNextMissions( uint16_t numChargersAvail )
{
#if 0
    vector<uint16_t> chargingList;
    vector<bool> idleVehFound;
    vector<uint16_t> nextMissionIdxs;
    chargerStations.getChargingList(chargingList);
    uint16_t missionIdx=0;
    uint16_t vIdx;
    vector<uint16_t> idleCopy( idleVs );

    if (missionQ.size() == 0)
    {
        return;
    }
    sort(chargingList.begin(), chargingList.end(), compareMaxPassengers);
    //sort(idleVs.begin(), idleVs.end(), compareMaxPassengers);
    for (uint16_t i=0; i < chargingList.size(); i++) {
        if (missionQ.size() >= (missionIdx + 1)) {
            vIdx = findMissionCableVeh(idleCopy, missionQ[missionIdx]);
            if (vIdx != kInvalidVeh) {
                idleCopy.erase(idleCopy.begin() + vIdx);
                idleVehFound.push_back(true);
            } else {
                idleVehFound.push_back(false);
            }
        } else {
            idleVehFound.push_back(false);
        }
        missionIdx++;
    }

    missionIdx = 0;
    for (uint16_t i=0; i < idleVehFound.size(); i++) {
        if ( !idleVehFound[i] &&
             (missionQ.size() >= (missionIdx + 1)))
        {
            vIdx = findMissionCableVeh(needChargingQ, missionQ[missionIdx]);
            if (vIdx != kInvalidVeh) {
                //TODO: reorder
            }
        }
        missionIdx++;
    }
#endif
}
/// \brief Simulate one minute.
void veh_sim::simMinute( void )
{

    // Queue up a backlog of missions(flights) to the depth of simulated vehicles.
    if (missionQ.size() < kNumSimVehicles)
    {
        missionQ.push_back( evtolLst.getRndVehPassCnt() );
    }

    // Update each vehicle's simulation.
    for (uint16_t i=0; i < kNumSimVehicles; i++)
    {
        switch (v[i].sim())
        {
            case NO_CHANGE:
                break;
            case FLIGHT_COMPLETE:
                needChargingQ.push_back( i );
                break;
            case CHARGE_COMOPLETE:
                idleVs.push_back( i );
                chargerStations.freeVehFromCharger( i );
                break;
            default:
                assert(1);
                break;
        }
    }

    // Start a single mission.
    if (idleVs.size() > 0) {
        for (uint16_t i=0; i < missionQ.size(); i++) {
            uint16_t idleVehFound = findMissionCableVeh( idleVs, missionQ[i] );
            if (idleVehFound != kInvalidVeh) {
                uint16_t vehID = idleVs[idleVehFound];
                uint16_t numPass = missionQ[i];
                missionQ.erase( missionQ.begin() + i );
                idleVs.erase(idleVs.begin() + idleVehFound);
                v[vehID].startFlight(numPass);
                break;// Only start one mission per minute, thus break.
            }
        }
    }

    // Add vehicles to the charger.
    uint16_t numChargersAvail = chargerStations.getNumOfChargersAvail();
    if (numChargersAvail > 0) {
        reorderChargerWaitingQForNextMissions( numChargersAvail );
        for (uint16_t i=0; i < numChargersAvail; i++) {
            if (needChargingQ.size() > 0)
            {
                uint16_t vehID = needChargingQ[0];
                needChargingQ.erase(needChargingQ.begin() );
                chargerStations.addVehToCharger( vehID );
                v[ vehID ].startCharging();
            }
        }
    }
}
void veh_sim::simulate( uint16_t simMinutes, uint32_t seed )
{
    seedForStartingSimulation = seed;
    srand( seed );
    checkSimConsistency();
    for (uint16_t i=0; i < simMinutes; i++)
    {
        simMinute();
        addLogRow( i );
        checkSimConsistency();
    }
}

veh_sim::~veh_sim()
{
    log.close();
}

void veh_sim::checkSimConsistency( void )
{
    uint16_t idleCnt=0, inFlightCnt=0, waitChargeCnt=0, inChargeCnt=0;

    for (uint16_t i=0; i < kNumSimVehicles; i++) {
        switch (v[i].getCurrentState()) {
            case VS_IDLE:
                idleCnt++;
                break;
            case VS_IN_FLIGHT:
                inFlightCnt++;
                break;
            case VS_WAIT_CHARGE:
                waitChargeCnt++;
                break;
            case VS_IN_CHARGE:
                inChargeCnt++;
                break;
            default:
                cout << "Bad vehicle sim state: " << v[i].getCurrentState() << "  idx =" << i << endl;
                assert(1);
                break;
        }
    }
    //cout << "idleVs.sz: " << idleVs.size() << " idleCnt: " << idleCnt << endl;
    //cout << "needChargingQ.sz: " << needChargingQ.size() << " waitChargeCnt: " << waitChargeCnt << endl;
    //cout << "chargerStations.sz: " << chargerStations.getNumOfChargersInUse() << " inChargeCnt: " << inChargeCnt << endl;
    assert( idleVs.size() == idleCnt );
    assert( needChargingQ.size() == waitChargeCnt );
    assert( chargerStations.getNumOfChargersInUse() == inChargeCnt );
}

void veh_sim::displayVehicleStats( void )
{
    cout << "Display vehicle statistics for simulation results.\n"
         << "   Random seed used for allocating mix of evotl companies: " << seedForRandomVehicleConfiguration << endl
         << "   Random seed used for starting the simulation: " << seedForStartingSimulation << endl;
    for (uint16_t i=0; i < kNumSimVehicles; i++)
    {
        v[i].displayStatistics( "V" + to_string(i) );
    }


}


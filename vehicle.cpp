//
// Created by ajlou on 2/12/2023.
//

#include "vehicle.h"
#include "util.h"
#include <stdlib.h>
#include <iostream>
#include <assert.h>

using namespace std;

evtol_list evtolLst;

vehicle::vehicle( evtol_companies_e company )
    : kCompany( company ),
    currentState( IDLE )
{
    batteryCapacity = evtolLst.getCompanyProperty( kCompany )->kBattKwh;
}
sim_result_e vehicle::sim( void )
{
    sim_result_e RV = NO_CHANGE;
    vehicle_state_e nextState = currentState;

    switch (currentState)
    {
        case IDLE:
            break;
        case IN_FLIGHT:
            if (cmpd(batteryCapacity, 0.0, evtolLst.getCruiseMinEnergy( kCompany )))
            {
                batteryCapacity = 0.0;
                nextState = WAITING_FOR_CHARGER;
                RV = FLIGHT_COMPLETE;
            }
            else
            {
                batteryCapacity -= evtolLst.getCruiseMinEnergy( kCompany );
            }
            break;
        case WAITING_FOR_CHARGER:
            break;
        case CHARGE_IN_PROGRESS:
            if (cmpd(batteryCapacity, 0.0, evtolLst.getChargeMinEnergy( kCompany )))
            {
                batteryCapacity = evtolLst.getCompanyProperty( kCompany )->kBattKwh;
                nextState = IDLE;
                RV = CHARGE_COMOPLETE;
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
    currentState = IN_FLIGHT;
}
void vehicle::startCharging( void )
{
    currentState = CHARGE_IN_PROGRESS;
}
charger_stations::charger_stations( void )
{
    for (uint16_t i=0; i < kNumChargers; i++) {
        chargers[i] = kEmptyCharger;
    }
}
bool charger_stations::addVehToCharger( uint16_t vehID )
{
    bool RV = false;
    for (uint16_t i=0; i < kNumChargers; i++)
    {
        if (chargers[i] != kEmptyCharger)
        {
            chargers[i] = vehID;
            RV = true;
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
        }
    }
    return RV;
}
uint16_t  charger_stations::getNumOfChargersAvail( void )
{
    uint16_t numAvail = 0;

    for (uint16_t i=0; i < kNumChargers; i++)
    {
        if (chargers[i] != kEmptyCharger)
        {
            numAvail++;
        }
    }
    return numAvail;
}
void charger_stations::getChargingList( std::vector<uint16_t> &chargingList )
{

}

veh_sim::veh_sim( uint32_t seed )
{
    uint16_t rndVehCntByCompany[ NUM_COMPANIES ];
    uint16_t rndVehiclesLeftCnt = kNumSimVehicles;
    uint16_t rndVehForThisCompany = 0;
    uint16_t company = 0;

    srand( seed );
    //TODO: Combine next two loops into one, but now nice for random distribution review.
    for (company=ALPHA; company < (NUM_COMPANIES-1); company++)
    {
        cout << "  company = " << company << endl;
        rndVehCntByCompany[ company ] = ((( rand() % rndVehiclesLeftCnt ) / (NUM_COMPANIES - company - 1) ) + 1);
        rndVehiclesLeftCnt -= rndVehCntByCompany[ company ];
        cout << "    rndVehiclesLeftCnt = " << rndVehiclesLeftCnt << endl;
    }
    rndVehCntByCompany[ (NUM_COMPANIES-1) ] = rndVehiclesLeftCnt;
    cout << "rndVehCntByCompany: " << rndVehCntByCompany[0]
        << ", " << rndVehCntByCompany[1]
        << ", " << rndVehCntByCompany[2]
        << ", " << rndVehCntByCompany[3]
        << ", " << rndVehCntByCompany[4]
        << endl;
    cout << "v size = " << v.size() << "\n";
    for (company=ALPHA; company < (NUM_COMPANIES); company++)
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
}

uint16_t veh_sim::findIdleVeh( uint16_t mission )
{
    uint16_t RV = kInvalidVeh;

    return RV;
}
void veh_sim::reorderChargerWaitingQForNextMissions( uint16_t numChargersAvail )
{
    vector<uint16_t> chargingList;
    vector<uint16_t> nextMissionIdxs;
    chargerStations.getChargingList(chargingList);
}
/// \brief Simulate one minute.
void veh_sim::simMinute( void )
{
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
            uint16_t idleVehFound = findIdleVeh(missionQ[i] );
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

    // Update the charger based on future missions.
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
    srand( seed );
    for (uint16_t i=0; i < simMinutes; i++)
    {
        simMinute();
    }
}
//
// Created by ajlou on 2/11/2023.
//

#include "evtol_prop.h"
#include "util.h"
#include <iostream>
#include <assert.h>
using namespace std;

evtol_prop_exc::evtol_prop_exc( uint32_t EId )
    : kEId( EId )
{
}

const uint32_t  evtol_prop_exc::getEId()
{
    return kEId;
}

evtol_prop_exc InvalidNumPassangers( INVALID_PASSENGER_PARAM );
evtol_prop_exc InvalidEnergyUse( INVALID_ENERGY_USE );
evtol_prop_exc InvalidCruise( INVALID_CRUISE );
evtol_prop_exc InvalidBattCapacity( INVALID_BATT_CAPACITY );

//
// Created by ajlou on 2/10/2023.
//
evtol_prop::evtol_prop(uint32_t cruiseSpeedMph, uint32_t battKwh, double timeToChargeHrs, double energyUseKwhPerMil,
                     uint32_t maxPassergers, double faultProbPerHour) :
        kCruiseSpeedMph( cruiseSpeedMph ),
        kBattKwh( battKwh ),
        kTimeToChargeHrs( timeToChargeHrs ),
        kEnergyUseKwhPerMile( energyUseKwhPerMil ),
        kMaxPassengers( maxPassergers ),
        kFaultProbPerHour( faultProbPerHour ),
        kMaxDistance( kBattKwh / kEnergyUseKwhPerMile )
{
    if (kCruiseSpeedMph == 0)
    {
        throw InvalidCruise;
    }
    if (kBattKwh == 0)
    {
        throw InvalidBattCapacity;
    }
    if ( cmpd( kEnergyUseKwhPerMile,  0.0)  )
    {
        throw InvalidEnergyUse;
    }
    if ( kMaxPassengers == 0 )
    {
        throw InvalidNumPassangers;
    }

}

void evtol_prop::disp( void )
{
    cout << "evol properties:"
         << "\n cruise speed: " << kCruiseSpeedMph
         << "\n bat kwh: " << kBattKwh
         << "\n time to charge hrs: " << kTimeToChargeHrs
         << "\n energy use kwh/mile: " << kEnergyUseKwhPerMile
         << "\n max passengers: " << kMaxPassengers
         << "\n fault probability per hour: " << kFaultProbPerHour
         << "\n max distance: " << kMaxDistance << "\n";
}

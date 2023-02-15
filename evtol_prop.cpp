//
// Created by ajlou on 2/11/2023.
//

#include "evtol_prop.h"
#include "util.h"
#include <iostream>
#include <cassert>


using namespace std;

// Globals
#undef etype
#define etype(x) #x
const char *strCompanies[] = { COMPANY_LIST };

/// \brief Get an ascii string representing the company name from an enum.
/// \param c
/// \return
const char * getCompanyName( evtol_companies_e c )
{
    return strCompanies[ c ];
}

/// \brief Constructor for evtol prop exceptions.
/// \param EId
evtol_prop_exc::evtol_prop_exc(uint32_t EId)
        : kEId(EId) {
}

/// \brief Get the evtol prop exception ID.
/// \return
const uint32_t evtol_prop_exc::getEId() {
    return kEId;
}

/// \brief  List of invalid parameter exceptions evtol prop class.
evtol_prop_exc InvalidNumPassangers(INVALID_PASSENGER_PARAM);
evtol_prop_exc InvalidEnergyUse(INVALID_ENERGY_USE);
evtol_prop_exc InvalidCruise(INVALID_CRUISE);
evtol_prop_exc InvalidBattCapacity(INVALID_BATT_CAPACITY);

//
// Created by ajlou on 2/10/2023.
//
/// \brief Construct evtol properties.
/// \param cruiseSpeedMph
/// \param battKwh
/// \param timeToChargeHrs
/// \param energyUseKwhPerMil
/// \param maxPassergers
/// \param faultProbPerHour
evtol_prop::evtol_prop(const char *name, uint32_t cruiseSpeedMph, uint32_t battKwh, double timeToChargeHrs,
                       double energyUseKwhPerMil,
                       uint32_t maxPassergers, double faultProbPerHour) :
        kName(name),
        kCruiseSpeedMph(cruiseSpeedMph),
        kBattKwh(battKwh),
        kTimeToChargeHrs(timeToChargeHrs),
        kEnergyUseKwhPerMile(energyUseKwhPerMil),
        kMaxPassengers(maxPassergers),
        kFaultProbPerHour(faultProbPerHour),
        kMaxDistance(kBattKwh / kEnergyUseKwhPerMile),
        kMaxCruiseHrs(kMaxDistance / kCruiseSpeedMph) {
    if (kCruiseSpeedMph == 0) {
        throw InvalidCruise;
    }
    if (kBattKwh == 0) {
        throw InvalidBattCapacity;
    }
    if (cmpZero(kEnergyUseKwhPerMile, 0.0)) {
        throw InvalidEnergyUse;
    }
    if (kMaxPassengers == 0) {
        throw InvalidNumPassangers;
    }

}

/// \brief Diplay evtol properites.
void evtol_prop::disp(void) {
    cout << "evol properties:" << kName
         << "\n cruise speed: " << kCruiseSpeedMph
         << "\n bat kwh: " << kBattKwh
         << "\n time to charge hrs: " << kTimeToChargeHrs
         << "\n energy use kwh/mile: " << kEnergyUseKwhPerMile
         << "\n max passengers: " << kMaxPassengers
         << "\n fault probability per hour: " << kFaultProbPerHour
         << "\n max distance: " << kMaxDistance
         << "\n max cruise hrs: " << kMaxCruiseHrs
         << "\n";
}

/// \brief Construct a list of evtol companies that is addressable by an enum.
evtol_list::evtol_list()
        : alpha("ALPHA", 120, 320, .6, 1.6, 4, 0.25),
          bravo("BRAVO", 100, 100, .2, 1.5, 5, 0.1),
          charlie("CHARLIE", 160, 220, .8, 2.2, 3, 0.05),
          delta("DELTA", 90, 120, .62, .8, 2, 0.22),
          echo("ECHO", 30, 150, .3, 5.8, 2, 0.61),
          lst{&alpha, &bravo, &charlie, &delta, &echo}
{
    maxPassCnt = 0;

    for (uint16_t i=0; i < C_NUM_COMPANIES; i++)
    {
        if (lst[i]->kMaxPassengers > maxPassCnt)
        {
            maxPassCnt = lst[i]->kMaxPassengers;
        }
    }
}

/// \brief Test that enum indexing is working.
void evtol_list::unitTest( void )
{
    assert( getCompanyProperty(C_ALPHA)->kName == alpha.kName );
    assert( getCompanyProperty(C_ALPHA)->kName == "ALPHA" );
    assert( getCompanyProperty(C_BRAVO)->kName == "BRAVO" );
    assert( getCompanyProperty(C_CHARLIE)->kName == "CHARLIE" );
    assert( getCompanyProperty(C_DELTA)->kName == "DELTA" );
    assert( getCompanyProperty(C_ECHO)->kName == "ECHO" );
}

/// \brief A function to access the company propeties by enum.
/// \param company
/// \return
evtol_prop * evtol_list::getCompanyProperty( evtol_companies_e company )
{
  return lst[ company ];
}

/// \brief Return the energy usuage at cruise on a per minute basis.
/// \param company
/// \return
double evtol_list::getCruiseMinEnergy( evtol_companies_e company )
{
    double RV;

    RV = getCruiseMinDistance( company ) * lst[ company ]->kEnergyUseKwhPerMile;
    return RV;
}

/// \brief Return the energy charge capability on a per minute basis.
/// \param company
/// \return
double evtol_list::getChargeMinEnergy( evtol_companies_e company )
{
    double timeToChargeMins = lst[ company ]->kTimeToChargeHrs * 60;
    double RV = (lst[ company ]->kBattKwh / timeToChargeMins);

    return RV;
}

/// \brief Get the distance at cruise speed for a per minute basis.
/// \param company
/// \return
double evtol_list::getCruiseMinDistance( evtol_companies_e company )
{
    double RV = (double)lst[ company ]->kCruiseSpeedMph / 60;

    return RV;
}

/// \brief Get a random passenger count based on the maximum pass count of company types.
/// \return
uint16_t evtol_list::getRndVehPassCnt( void )
{
    uint16_t RV = (rand() % (maxPassCnt - 1)) + 1;// At least one.

    return RV;
}

/// \brief Display the evtol properies of all companies.
void evtol_list::disp(void) {
    for (uint8_t i = 0; i < C_NUM_COMPANIES; i++) {
        lst[i]->disp();
    }
}
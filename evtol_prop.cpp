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
/// \param name
/// \param company
/// \param cruiseSpeedMph
/// \param battKwh
/// \param timeToChargeHrs
/// \param energyUseKwhPerMil
/// \param maxPassergers
/// \param faultProbPerHour
evtol_prop::evtol_prop(const char *name, evtol_companies_e company, uint32_t cruiseSpeedMph, uint32_t battKwh, double timeToChargeHrs,
                       double energyUseKwhPerMil,
                       uint32_t maxPassergers, double faultProbPerHour) :
        kName(name),
        kCompany(company),
        kCruiseSpeedMph(cruiseSpeedMph),
        kBattKwh(battKwh),
        kTimeToChargeHrs(timeToChargeHrs),
        kEnergyUseKwhPerMile(energyUseKwhPerMil),
        kMaxPassengers(maxPassergers),
        kFaultProbPerHour(faultProbPerHour),
        kMaxDistance(kBattKwh / kEnergyUseKwhPerMile),
        kMaxCruiseHrs(kMaxDistance / kCruiseSpeedMph),
        kMinuteCruiseDistance((double)kCruiseSpeedMph / 60),
        kCruiseMinuteEnergy(kMinuteCruiseDistance * kEnergyUseKwhPerMile),
        kTimeToChargeMinutes(kTimeToChargeHrs * 60),
        kMinuteChargeEnergy(kBattKwh / kTimeToChargeMinutes)
 {
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

/// \brief Display the evtol properies of all companies.
void evtol_list::disp(void) {
    for (uint8_t i = 0; i < C_NUM_COMPANIES; i++) {
        lst[i].disp();
    }
}

/// \brief Get a random passenger count based on the maximum pass count of company types.
/// \return
uint16_t evtol_list::getRndVehPassCnt( void )
{
    uint16_t RV = (rand() % (maxPassengers - 1)) + 1;// At least one.

    return RV;
}

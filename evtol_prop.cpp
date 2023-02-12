//
// Created by ajlou on 2/11/2023.
//

#include "evtol_prop.h"
#include "util.h"
#include <iostream>


using namespace std;

evtol_prop_exc::evtol_prop_exc(uint32_t EId)
        : kEId(EId) {
}

const uint32_t evtol_prop_exc::getEId() {
    return kEId;
}

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
    if (cmpd(kEnergyUseKwhPerMile, 0.0)) {
        throw InvalidEnergyUse;
    }
    if (kMaxPassengers == 0) {
        throw InvalidNumPassangers;
    }

}

/// \brief
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

/// \brief
evtol_list::evtol_list()
        : alpha("ALPHA", 120, 320, .6, 1.6, 4, 0.25),
          bravo("BRAVO", 100, 100, .2, 1.5, 5, 0.1),
          charlie("CHARLIE", 160, 220, .8, 2.2, 3, 0.05),
          delta("DELTA", 90, 120, .62, .8, 2, 0.22),
          echo("ECHO", 30, 150, .3, 5.8, 2, 0.61),
          lst{&alpha, &bravo, &charlie, &delta, &echo} {

}

/// \brief
void evtol_list::disp(void) {
    for (uint8_t i = 0; i < NUM_COMPANIES; i++) {
        lst[i]->disp();
    }
}
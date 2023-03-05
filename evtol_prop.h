//
// Created by ajlou on 2/11/2023.
//

#ifndef EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H
#define EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H


#include <cstdint>
#include <exception>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <exception>
using namespace std;

//TODO: This 'C' language pattern for managing enums and string versions of enums is convenient.
//      But may need to be revised for coding standard.
#define COMPANY_LIST \
    etype(ALPHA), \
    etype(BRAVO),   \
    etype(CHARLIE),  \
    etype(DELTA),  \
    etype(ECHO), \
    etype(NUM_COMPANIES)
#define etype(x) C_##x
typedef enum { COMPANY_LIST } evtol_companies_e;
const char * getCompanyName( evtol_companies_e c );

/// \brief Enumeration of exception IDs for invalid properties.
enum evtol_prop_exc_ids_e
{
    INVALID_PASSENGER_PARAM,
    INVALID_ENERGY_USE,
    INVALID_CRUISE,
    INVALID_BATT_CAPACITY,
};

/// \brief Exception list for invalid evtol properties.
struct evtol_prop_exc : public std::exception
{
    const uint32_t kEId;
    evtol_prop_exc( uint32_t EId );
    const uint32_t  getEId();
};

/// \brief Represents the evtol for a single company.
class evtol_prop {
public:
    const char* kName;
    const evtol_companies_e kCompany;
    const uint32_t kCruiseSpeedMph;
    const uint32_t kBattKwh;
    const double kTimeToChargeHrs;
    const double kEnergyUseKwhPerMile;
    const uint32_t kMaxPassengers;
    const double kFaultProbPerHour;
    const double kMaxDistance;
    const double kMaxCruiseHrs;
    const double kMinuteCruiseDistance;
    const double kCruiseMinuteEnergy;
    const double kTimeToChargeMinutes;
    const double kMinuteChargeEnergy;
    evtol_prop(
            const char* name,
            evtol_companies_e company,
            uint32_t cruiseSpeedMph,
            uint32_t battKwh,
            double timeToChargeHrs,
            double energyUseKwhPerMil,
            uint32_t maxPassergers,
            double faultProbPerHour
    );
    void disp( void );
};

class evtol_list
{
private:
    evtol_prop lst[C_NUM_COMPANIES] {
        { "ALPHA", C_ALPHA, 120, 320, .6, 1.6, 4, 0.25 },
        {"BRAVO", C_BRAVO, 100, 100, .2, 1.5, 5, 0.1 },
        { "CHARLIE", C_CHARLIE, 160, 220, .8, 2.2, 3, 0.05 },
        { "DELTA", C_DELTA, 90, 120, .62, .8, 2, 0.22 },
        { "ECHO", C_ECHO, 30, 150, .3, 5.8, 2, 0.61 } };
    uint16_t maxPassengers { 0 };

public:
    evtol_list()
    {
        for (auto it : lst) {
            if (it.kMaxPassengers > maxPassengers) {
                maxPassengers = it.kMaxPassengers;
            }
        }
    }

    const evtol_prop& getCompanyProperty( evtol_companies_e company )
    {
        return lst[ company ];
    };

    uint16_t getMaxPassOfAllCompanies() { return maxPassengers; }
    uint16_t getRndVehPassCnt( void );

    void disp( void );
};

#endif //EVTOL_SIGHTSEER_SIM_EVTOL_PROP_H

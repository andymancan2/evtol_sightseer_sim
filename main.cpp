///

#include "evtol_prop.h"
#include "vehicle.h"
#include <iostream>
#include <assert.h>
#include <cmath>

using namespace std;

/// \brief Bottom up unit test.
/// TODO: Install google test then utilize mock classes for a top-down testing.
void unitTest( void )
{
    try {
        evtol_prop tst1( "tst1", 1, 2, 3, 1, 0, 7);
        assert(0); // Should not reach here with exception thrown.
    } catch( evtol_prop_exc e) {
        std::cout << "Exception caught:" << e.getEId() << "\n";
        assert( e.getEId() == INVALID_PASSENGER_PARAM );
    }
    try {
        evtol_prop tst2("tst2", 1, 2, 3, 0.0, 5, 7);
        assert(0); // Should not reach here with exception thrown.
    } catch( evtol_prop_exc e) {
        std::cout << "Exception caught:" << e.getEId() << "\n";
        assert( e.getEId() == INVALID_ENERGY_USE );
    }
    try {
        evtol_prop tst3("tst3", 0, 2, 3, 1.0, 5, 7);
        assert(0); // Should not reach here with exception thrown.
    } catch( evtol_prop_exc e) {
        std::cout << "Exception caught:" << e.getEId() << "\n";
        assert( e.getEId() == INVALID_CRUISE );
    }
    try {
        evtol_prop tst4("tst4", 1, 0, 3, 1.0, 5, 7);
        assert(0); // Should not reach here with exception thrown.
    } catch( evtol_prop_exc e) {
        std::cout << "Exception caught:" << e.getEId() << "\n";
        assert( e.getEId() == INVALID_BATT_CAPACITY );
    }
    evtol_prop tst("tst", 1, 2, 3, 1.0, 5, 7);
    tst.disp();
    evtol_prop tstA("alpha", 120, 320, .6, 1.6, 4, 0.25);
    tstA.disp();
    extern evtol_list evtolLst;
    evtolLst.disp();
    cout << "Perform visual inspection that above print statements match the company specs.\n";
    cout << "Alpha name = " << evtolLst.getCompanyProperty(ALPHA)->kName << endl;
    evtolLst.unitTest();
    charger_stations cs;
    assert( cs.addVehToCharger(0) == true );
    assert( cs.addVehToCharger( 1 ) == true );
    assert( cs.addVehToCharger( 2 ) == true );
    assert( cs.addVehToCharger( 3 ) == false );
    assert( cs.freeVehFromCharger( 3 ) == false );
    assert( cs.freeVehFromCharger( 2 ) == true );
    assert( cs.freeVehFromCharger( 1 ) == true );
    assert( cs.freeVehFromCharger( 0 ) == true );
    for (uint16_t company=ALPHA; company < NUM_COMPANIES; company++) {
        vehicle v( (evtol_companies_e)company);
        cout << "kCompany: " << v.kCompany << endl;
        assert(v.getCurrentState() == IDLE);
        assert(v.sim() == NO_CHANGE);
        cout << "kBattKwh: " << evtolLst.getCompanyProperty((evtol_companies_e)company)->kBattKwh << endl;
        cout << "cruise min energy: " << evtolLst.getCruiseMinEnergy((evtol_companies_e)company) << endl;
        //uint16_t vFlightMins = fabs(evtolLst.getCompanyProperty((evtol_companies_e)company)->kBattKwh /
        //                            evtolLst.getCruiseMinEnergy((evtol_companies_e)company));
        uint16_t vFlightMins = ceil((evtolLst.getCompanyProperty((evtol_companies_e)company)->kBattKwh
                                       - evtolLst.getCruiseMinEnergy((evtol_companies_e)company)) /
                                    evtolLst.getCruiseMinEnergy((evtol_companies_e)company)) - 1;
        cout << "vFlightMins = " << vFlightMins << endl;
        v.startFlight(1);
        assert(v.getCurrentState() == IN_FLIGHT);
        for (uint16_t i = 0; i < vFlightMins; i++) {
            assert(v.sim() == NO_CHANGE);
            assert(v.getCurrentState() == IN_FLIGHT);
        }
        v.disp();
        assert(v.sim() == FLIGHT_COMPLETE);
        assert(v.getCurrentState() == WAITING_FOR_CHARGER);
        assert(v.sim() == NO_CHANGE);
        assert(v.getCurrentState() == WAITING_FOR_CHARGER);
        assert(v.sim() == NO_CHANGE);
        v.startCharging();
        assert(v.getCurrentState() == CHARGE_IN_PROGRESS);
        uint16_t vChargeMins = ceil(evtolLst.getCompanyProperty((evtol_companies_e)company)->kTimeToChargeHrs * 60) ;
        cout << "vChargeMins:" << vChargeMins << endl;
        cout << "charge per min: " << evtolLst.getChargeMinEnergy( (evtol_companies_e)company )<< endl;
        for (uint16_t i = 0; i < vChargeMins; i++) {
            //cout << "i:" << i << ", vChargeMins:" << vChargeMins << endl;
            assert(v.sim() == NO_CHANGE);
            assert(v.getCurrentState() == CHARGE_IN_PROGRESS);
        }
        v.disp();
        assert(v.sim() == CHARGE_COMOPLETE);
        assert(v.getCurrentState() == IDLE);
        assert(v.sim() == NO_CHANGE);
        assert(v.getCurrentState() == IDLE);
        v.disp();
    }

    veh_sim vv;
#if 0
    cout << "UT done\n";

    double p=0.05;//0.25;
    double faultsPerMin = p / 60;
    uint32_t minuteOccurence = ceil( 1 / faultsPerMin );
    uint32_t fault_count = 0;
    uint32_t fc2 = 0;
    cout << "p: " << p << endl;
    cout << "faultsPerMin: " << faultsPerMin << endl;
    cout << "minuteOccurence: " << minuteOccurence << endl;
    for (uint32_t i=0; i < (160000*60); i++)
    {
        if ((rand() % minuteOccurence) == 0)
        {
            fault_count++;
        }
        if (rand() < (faultsPerMin*RAND_MAX) )
        {
            fc2++;
        }
    }
    cout << "fault_count: " << fault_count << endl;
    cout << "fc2: " << fc2 << endl;
#endif
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    unitTest();
    return 0;
}

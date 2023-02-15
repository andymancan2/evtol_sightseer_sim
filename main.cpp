///

#include "evtol_prop.h"
#include "vehicle.h"
#include <iostream>
#include <assert.h>
#include <cmath>

using namespace std;

/// \brief Unit tests for evtol_prop class.
void evtol_prop_unitTest( )
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
}

/// \brief Unit tests for evtol_list class.
void evtol_list_unitTest()
{
    extern evtol_list evtolLst;
    evtolLst.disp();
    cout << "Perform visual inspection that above print statements match the company specs.\n";
    cout << "Alpha name = " << evtolLst.getCompanyProperty(C_ALPHA)->kName << endl;
    evtolLst.unitTest();
}

/// \brief Unit test for charge_stations class.
void charge_stations_unitTest()
{
    charger_stations cs;
    assert( cs.addVehToCharger(0) == true );
    assert( cs.addVehToCharger( 1 ) == true );
    assert( cs.addVehToCharger( 2 ) == true );
    assert( cs.addVehToCharger( 3 ) == false );
    assert( cs.freeVehFromCharger( 3 ) == false );
    assert( cs.freeVehFromCharger( 2 ) == true );
    assert( cs.freeVehFromCharger( 1 ) == true );
    assert( cs.freeVehFromCharger( 0 ) == true );
}

/// \brief A unit test for the vechicle class for each company type.
void vehicle_unitTest()
{
    extern evtol_list evtolLst;

    for (uint16_t company=C_ALPHA; company < C_NUM_COMPANIES; company++) {
        vehicle v( (evtol_companies_e)company );
        cout << "kCompany: " << v.kCompany << endl;
        assert(v.getCurrentState() == VS_IDLE);
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
        assert(v.getCurrentState() == VS_IN_FLIGHT);
        for (uint16_t i = 0; i < vFlightMins; i++) {
            assert(v.sim() == NO_CHANGE);
            assert(v.getCurrentState() == VS_IN_FLIGHT);
        }
        //v.disp();
        assert(v.sim() == FLIGHT_COMPLETE);
        assert(v.getCurrentState() == VS_WAIT_CHARGE);
        assert(v.sim() == NO_CHANGE);
        assert(v.getCurrentState() == VS_WAIT_CHARGE);
        assert(v.sim() == NO_CHANGE);
        v.startCharging();
        assert(v.getCurrentState() == VS_IN_CHARGE);
        uint16_t vChargeMins = ceil(evtolLst.getCompanyProperty((evtol_companies_e)company)->kTimeToChargeHrs * 60) ;
        cout << "vChargeMins:" << vChargeMins << endl;
        cout << "charge per min: " << evtolLst.getChargeMinEnergy( (evtol_companies_e)company )<< endl;
        for (uint16_t i = 0; i < vChargeMins; i++) {
            //cout << "i:" << i << ", vChargeMins:" << vChargeMins << endl;
            assert(v.sim() == NO_CHANGE);
            assert(v.getCurrentState() == VS_IN_CHARGE);
        }
        //v.disp();
        assert(v.sim() == CHARGE_COMOPLETE);
        assert(v.getCurrentState() == VS_IDLE);
        assert(v.sim() == NO_CHANGE);
        assert(v.getCurrentState() == VS_IDLE);
        //v.disp();
    }
}

/// \brief Bottom up unit test.
/// TODO: Install google test then utilize mock classes for a top-down testing.
void unitTest( void )
{
    evtol_prop_unitTest();
    evtol_list_unitTest();
    charge_stations_unitTest();
    vehicle_unitTest();

    // Note the veh_sim high-level class was not unit-tested.
    // The veh_sim class was tested by running the simulation with a log file.
    // In the future, a mock-class test frame work could be utilized for testing veh_sim class.
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    unitTest();

    veh_sim vs;
    vs.simulate( 180 );
    vs.displayVehicleStats();
    return 0;
}

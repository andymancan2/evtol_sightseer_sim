///

#include "evtol_prop.h"
#include "vehicle.h"
#include <iostream>
#include <assert.h>

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
    veh_sim vv;
    cout << "UT done\n";
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    unitTest();
    return 0;
}

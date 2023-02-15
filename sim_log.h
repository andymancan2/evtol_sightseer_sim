//
// Created by ajlou on 2/14/2023.
//

#ifndef EVTOL_SIGHTSEER_SIM_SIM_LOG_H
#define EVTOL_SIGHTSEER_SIM_SIM_LOG_H

#include "vehicle.h"
#include <stdio.h>
#include <iostream>

class veh_sim;

class sim_log {
private:
    FILE *fp;
public:
    const char *fmt;
public:
    sim_log();
    ~sim_log();
    void addRowHeader( void );
    void addRow( string s );
    void close( void );
};


#endif //EVTOL_SIGHTSEER_SIM_SIM_LOG_H

//
// Created by ajlou on 2/14/2023.
//

#include "sim_log.h"
#include "vehicle.h"


sim_log::sim_log()
    : fmt( "%s\t%s\t%s\t%s\t%s" \
           "\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s" \
           "\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n" )
{
    fp = fopen("simLog.tsv", "w+");
}
sim_log::~sim_log()
{
    //close();
}
void sim_log::close( void )
{
    fclose( fp );
}
void sim_log::addRowHeader( void )
{
    fprintf( fp,
             fmt,
             "TmMin", "IdleSz", "NeedChgQSz", "mssnQSz", "NumChg",
             "V0", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V8", "V9",
             "V10", "V11", "V12", "V13", "V14", "V15", "V16", "V17", "V18", "V19" );
}
void sim_log::addRow( string S )
{
#if 0
    fprintf( fp,
             fmt,

             )
#endif
    fprintf( fp, "%s", S.c_str() );
}

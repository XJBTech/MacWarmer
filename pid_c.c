#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "warmer.h"
#include "pid.h"

void * pid_c ( mt_ctrl_t * c )
{
    PidType * myPID = ( PidType * ) malloc ( sizeof ( PidType ) );
    PID_init ( myPID, 10, 3, 5, PID_Direction_Direct );
    PID_SetMode ( myPID, PID_Mode_Automatic );
    PID_SetSampleTime ( myPID, 500 );
    pthread_mutex_lock ( &c->m );
    PID_SetOutputLimits ( myPID, 0, c->procs * 100 );
    pthread_mutex_unlock ( &c->m );

    while ( 1 )
    {
        pthread_mutex_lock ( &c->m );
        pthread_cond_wait ( &c->cv, &c->m );
        myPID->mySetpoint = c->set_temp;
        myPID->myInput = c->current_temp;
        pthread_mutex_unlock ( &c->m );

        PID_Compute ( myPID );

        pthread_mutex_lock ( &c->m );
        c->cpu_usage_percentage = myPID->myOutput;
        pthread_mutex_unlock ( &c->m );
    }

    return NULL;
}

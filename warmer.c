/*
 * Mac Warmer
 * warmer.c
 * author      Rijn
 */

#include "warmer.h"
#include "pid_c.h"
#include "smc.h"
#include "monitor.h"
#include "calculatepi.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/sysctl.h>

void invalid_opt ( char * argv[] )
{
    fprintf ( stderr, "Usage: %s [-s set_temp]\n",
              argv[0] );
    exit ( EXIT_FAILURE );
}

int get_nprocs()
{
    int mib[2], maxproc;
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    len = sizeof ( maxproc );
    sysctl ( mib, 2, &maxproc, &len, NULL, 0 );

    return maxproc;
}

int main ( int argc, char * argv[] )
{

    mt_ctrl_t * c = ( mt_ctrl_t * ) malloc ( sizeof ( mt_ctrl_t ) );
    pthread_mutex_init ( &c->m, NULL );
    pthread_cond_init ( &c->cv, NULL );
    c->cpu_usage_percentage = 0;
    c->set_temp = 0;
    c->procs = get_nprocs();

    int ch, set_temp_flag = 0;

    while ( ( ch = getopt ( argc, argv, "s:p:" ) ) != -1 )
    {
        switch ( ch )
        {
            case 's':
                c->set_temp = ( double ) atoi ( optarg );
                set_temp_flag = 1;
                break;

            case 'p':
                c->procs = ( double ) atoi ( optarg );
                break;

            default: /* '?' */
                invalid_opt ( argv );
                break;
        }
    }

    if ( !set_temp_flag )
    {
        invalid_opt ( argv );
    }

    printf ( "set\ttemp = %d\n\tprocess = %d\n", ( int ) c->set_temp, c->procs );

    pthread_t cpu_usage_controller, monitor_controller, pid_controller;
    pthread_create ( &cpu_usage_controller, NULL, ( void * ) control_cpu_usage, ( void * ) c );
    pthread_create ( &monitor_controller, NULL, ( void * ) monitor, ( void * ) c );
    pthread_create ( &pid_controller, NULL, ( void * ) pid_c, ( void * ) c );

    pthread_join ( cpu_usage_controller, NULL );
    pthread_join ( monitor_controller, NULL );
    pthread_join ( pid_controller, NULL );
    pthread_exit ( NULL );

    free ( c );
}
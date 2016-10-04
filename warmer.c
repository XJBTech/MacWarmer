/*
 * Mac Warmer
 * warmer.c
 * author      Rijn
 */

#include "warmer.h"
#include "pid.h"
#include "smc.h"
#include "monitor.h"
#include "calculatepi.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

void invalid_opt ( char * argv[] )
{
    fprintf ( stderr, "Usage: %s [-s set_temp]\n",
              argv[0] );
    exit ( EXIT_FAILURE );
}

int main ( int argc, char * argv[] )
{

    mt_ctrl_t * c = ( mt_ctrl_t * ) malloc ( sizeof ( mt_ctrl_t ) );
    pthread_mutex_init ( &c->m, NULL );
    pthread_cond_init ( &c->cv, NULL );
    c->cpu_usage_percentage = 0;
    c->set_temp = 0;

    int ch, set_temp_flag = 0;

    while ( ( ch = getopt ( argc, argv, "s:" ) ) != -1 )
    {
        switch ( ch )
        {
            case 's':
                c->set_temp = ( double ) atoi ( optarg );
                set_temp_flag = 1;
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

    pthread_t cpu_usage_controller, monitor_controller;//, pid_controller;
    pthread_create ( &cpu_usage_controller, NULL, ( void * ) control_cpu_usage, ( void * ) c );
    pthread_create ( &monitor_controller, NULL, ( void * ) monitor, ( void * ) c );

    pthread_join ( cpu_usage_controller, NULL );
    pthread_join ( monitor_controller, NULL );
    pthread_exit ( NULL );

    free ( c );
}
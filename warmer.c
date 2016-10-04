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

int main ( int argc, char * argv[] )
{

    mt_ctrl_t * c = ( mt_ctrl_t * ) malloc ( sizeof ( mt_ctrl_t ) );

    pthread_mutex_init ( &c->m, NULL );
    pthread_cond_init ( &c->cv, NULL );
    c->cpu_usage_percentage = 0;

    pthread_t cpu_usage_controller, monitor_controller;//, pid_controller;
    pthread_create ( &cpu_usage_controller, NULL, (void*)control_cpu_usage, ( void * ) c );
    pthread_create ( &monitor_controller, NULL, (void*)monitor, ( void * ) c );

    pthread_join ( cpu_usage_controller, NULL );
    pthread_join ( monitor_controller, NULL );
    pthread_exit ( NULL );

    free ( c );
}
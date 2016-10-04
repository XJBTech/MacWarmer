#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#include "warmer.h"

void calculate_pi ()
{

}

void * control_cpu_usage ( mt_ctrl_t * c )
{

    while ( 1 )
    {
        pthread_mutex_lock ( &c->m );
        int cup = c->cpu_usage_percentage;
        pthread_mutex_unlock ( &c->m );

        int procs = ( ( ( cup + 100 ) / 100 ) - 1 );

        pid_t * pids = ( pid_t * ) malloc ( sizeof ( pid_t ) * procs );

        for ( int i = 0; i < procs; i++ )
        {

            pid_t pid = fork();

            if ( pid < 0 )
            {
                exit ( EXIT_FAILURE );
            }

            if ( pid == 0 )
            {
                clock_t st_time = clock();

                unsigned long idle_time = ( 100 - cup ) * 10,
                              busy_time = cup * 10;

                if(cup > 100) {
                	idle_time = 0;
                	busy_time = 100;
                }

                cup -= 100;

                while ( ( clock() - st_time ) * 1000 / CLOCKS_PER_SEC < busy_time );

                usleep ( idle_time * 1000 );
                exit ( EXIT_SUCCESS );
            }

            if ( pid > 0 )
            {
                pids[i] = pid;
            }

        }

        int status;

        for ( int i = 0; i < procs; i++ )
        {
            waitpid ( pids[i], &status, ( int ) NULL );
        }
    }

    return NULL;
}
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

        int procs = ( ( ( cup + 100 ) / 100 ) );

        pid_t * pids = ( pid_t * ) malloc ( sizeof ( pid_t ) * procs );

        for ( int i = 0; i < procs; i++ )
        {

            unsigned long busy_time = i == procs - 1 ? cup % 100 : 100,
                          idle_time = 100 - busy_time;

            // printf("\t%lu", busy_time);

            pid_t pid = fork();

            if ( pid < 0 )
            {
                exit ( EXIT_FAILURE );
            }

            if ( pid == 0 )
            {
                clock_t st_time = clock();

                while ( ( clock() - st_time ) * 100 / CLOCKS_PER_SEC < busy_time );

                usleep ( idle_time * 10 * 1000 );
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
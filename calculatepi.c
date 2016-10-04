#include <stdio.h>
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
    	pthread_mutex_lock(&c->m);
    	int cup = c->cpu_usage_percentage;
    	pthread_mutex_unlock(&c->m);

        clock_t st_time = clock();

        unsigned long idle_time = ( 100 - cup ) * 10,
                      busy_time = cup * 10;

        while ( ( clock() - st_time ) * 1000 / CLOCKS_PER_SEC < busy_time );

        usleep ( idle_time * 1000 );
    }

    return NULL;
}
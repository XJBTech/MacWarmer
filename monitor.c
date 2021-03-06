#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <IOKit/IOKitLib.h>

#include "warmer.h"
#include "smc.h"

double convertToFahrenheit ( double celsius )
{
    return ( celsius * ( 9.0 / 5.0 ) ) + 32.0;
}

void * monitor ( mt_ctrl_t * c )
{
    while ( 1 )
    {
        SMCOpen();
        double current_temp = SMCGetTemperature ( SMC_KEY_CPU_TEMP );
        SMCClose();
        pthread_mutex_lock ( &c->m );
        double set_temp = c->set_temp;
        c->current_temp = current_temp;
        int cpu_usage_percentage = c->cpu_usage_percentage;

        // if ( current_temp < set_temp )
        // {
        //     cpu_usage_percentage = 100;
        // }

        // c->cpu_usage_percentage = cpu_usage_percentage;
        pthread_cond_signal(&c->cv);
        pthread_mutex_unlock ( &c->m );
        // current_temp = convertToFahrenheit ( current_temp );
        fprintf ( stdout, "\r%0.1f° <= %0.1f° [set cpu_usage = %d%%]",
                  set_temp, current_temp, cpu_usage_percentage );
        fflush ( stdout );

        sleep ( 1 );
    }

    return NULL;
}
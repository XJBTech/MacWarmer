#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <IOKit/IOKitLib.h>

#include "warmer.h"
#include "smc.h"

void * monitor ( mt_ctrl_t * c )
{
    while ( 1 )
    {
        SMCOpen();
        double temperature = SMCGetTemperature ( SMC_KEY_CPU_TEMP );
        SMCClose();
        fprintf ( stdout, "\r%0.1f°", temperature );
        fflush(stdout);
        sleep ( 1 );
    }

    return NULL;
}
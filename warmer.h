#ifndef __WARMER_H__
#define __WARMER_H__

#include <pthread.h>

typedef struct mt_ctrl_t
{
	pthread_cond_t cv;
	pthread_mutex_t m;
	int cpu_usage_percentage;
	double set_temp, current_temp;
} mt_ctrl_t;

#endif
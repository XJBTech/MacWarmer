/*
 * Mac Warmer
 * warmer.c
 * author      Rijn
 */

#include "warmer.h"
#include "pid.h"
#include "smc.h"
#include "calculatepi.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
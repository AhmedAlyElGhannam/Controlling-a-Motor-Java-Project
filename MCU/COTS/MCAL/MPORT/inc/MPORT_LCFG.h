#ifndef MPORT_LCFG_H
#define MPORT_LCFG_H
#include "MPORT.h"

/* struct to store pin direction and mode */
typedef struct 
{
    MPORT_enuPortPinDir_t dir;
    MPORT_enuPortPinMode_t mode;
} MPORT_structPortPinDirAndMode_t;

/* macro specifies the max number of pins in mcu */
#define MPORT_NUM_OF_ALL_PINS   32

#endif
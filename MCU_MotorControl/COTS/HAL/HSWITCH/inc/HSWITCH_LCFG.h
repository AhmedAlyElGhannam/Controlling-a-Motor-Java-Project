#ifndef HSWITCH_LCFG_H
#define HSWITCH_LCFG_H

/* struct to define switch connection, port, and pin */
typedef struct 
{
    uint8_t portNum;
    uint8_t pinNum;
    HSWITCH_enuSwitchConnection_t connection;
} HSWITCH_structSwitchConfig_t;

/* macro to define the number of used switches */
#define NUM_OF_SWITCHES     2

/* macro to define the first defined switch */
#define FIRST_SWITCH        HSWITCH_RESET

#endif

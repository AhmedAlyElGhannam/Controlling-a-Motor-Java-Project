#ifndef HLED_LCFG_H
#define HLED_LCFG_H

/* struct to define LED connection, port, and pin */
typedef struct 
{
    uint8_t portNum;
    uint8_t pinNum;
    HLED_enuLEDConnection_t connection;
} HLED_structLEDConfig_t;

/* macro to define the first defined LED */
#define FIRST_LED       HLED_INVALID_ID

#endif

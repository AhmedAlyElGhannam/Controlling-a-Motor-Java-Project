#ifndef H7SEGMENT_LCFG_H
#define H7SEGMENT_LCFG_H

/* defining number of pins of used 7 segment display [MINUS GROUND] */
#define NUM_OF_PINS     8

/* struct to define LED connection, port, and pin */
typedef struct 
{
    uint8_t portNum;
    H7SEGMENT_enu7SegmentPinNames_t pinsConnection[NUM_OF_PINS];
    H7SEGMENT_enu7SegmentConnection_t connection;
} H7SEGMENT_struct7SegmentConfig_t;

/* macro to define the number of used LEDs */
#define NUM_OF_7SEG     2

/* macro to define the first defined LED */
#define FIRST_7SEG       H7SEGMENT_COUNTER_UNITS

#endif

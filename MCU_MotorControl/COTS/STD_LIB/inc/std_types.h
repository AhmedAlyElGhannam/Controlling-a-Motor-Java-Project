#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef unsigned char u8;   
typedef signed char s8;
typedef unsigned short int u16;
typedef signed short int s16;
typedef unsigned long int u32;
typedef signed long int s32;
typedef unsigned long long int u64;
typedef signed long long int s64;

typedef unsigned char uint8_t;
/*typedef unsigned short int uint16_t; **/
typedef unsigned long int uint32_t;

typedef signed char sint8_t;
typedef signed short int sint16_t;
typedef signed long int sint32_t;

typedef float float32_t;
typedef double float64_t;

#define NULL ((void*)0)
#define NULL_CHAR   ('\0')
#define F_CPU 	8000000UL

typedef enum
{
    true = 1U,
    false = !true,
} bool;

#endif
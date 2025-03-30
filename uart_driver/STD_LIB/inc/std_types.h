/**
 * @file std_types.h
 * @brief Standard type definitions for embedded systems
 * 
 * This header provides platform-independent type definitions commonly used in embedded systems programming.
 */

#ifndef STD_TYPES_H
#define STD_TYPES_H

/**
 * @defgroup StandardTypes Standard Data Types
 * @{
 */

/**
 * @brief Unsigned 8-bit integer type
 */
typedef unsigned char u8;

/**
 * @brief Unsigned 16-bit integer type
 */
typedef unsigned short int u16;

/**
 * @brief Unsigned 32-bit integer type
 */
typedef unsigned long int u32;

/**
 * @brief Signed 8-bit integer type
 */
typedef signed char s8;

/**
 * @brief Signed 16-bit integer type
 */
typedef signed short int s16;

/**
 * @brief Signed 32-bit integer type
 */
typedef signed long int s32;

/**
 * @brief 32-bit floating point type
 */
typedef float f32;

/**
 * @brief 64-bit floating point (double precision) type
 */
typedef double f64;

/**
 * @brief Null pointer definition
 */
#define NULL ((void *)0)

/**
 * @brief Null character definition
 */
#define NULL_CHAR ('\0')

/**
 * @brief Boolean type enumeration
 * 
 * Provides standard true/false values for boolean operations
 */
typedef enum
{
    true = 1,   /**< Boolean true value */
    false = 0   /**< Boolean false value */
} bool;

/** @} */ // end of StandardTypes group

#endif /* STD_TYPES_H */

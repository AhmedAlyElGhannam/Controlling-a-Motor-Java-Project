/**
 * @file bit_math.h
 * @brief Bit manipulation macros for embedded systems
 * 
 * This header provides a collection of macros for bit-level operations on registers,
 * including single-bit operations, nibble operations, and shift operations.
 */

#ifndef BIT_MATH_H_
#define BIT_MATH_H_

/**
 * @defgroup BitManipulation Bit Manipulation Macros
 * @{
 */

/* Single Bit Operations */

/**
 * @brief Set a specific bit in a register
 * @param REG Register to modify
 * @param BIT Bit position to set (0-7)
 */
#define SET_BIT(REG, BIT) ((REG) |= (1 << (BIT)))

/**
 * @brief Clear a specific bit in a register
 * @param REG Register to modify
 * @param BIT Bit position to clear (0-7)
 */
#define CLR_BIT(REG, BIT) ((REG) &= ~(1 << (BIT)))

/**
 * @brief Toggle a specific bit in a register
 * @param REG Register to modify
 * @param BIT Bit position to toggle (0-7)
 */
#define TGL_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))

/**
 * @brief Get the value of a specific bit in a register
 * @param REG Register to read
 * @param BIT Bit position to read (0-7)
 * @return 0 or 1 depending on bit state
 */
#define GET_BIT(REG, BIT) (((REG) >> (BIT)) & 1)

/* Nibble Operations (High) */

/**
 * @brief Set all bits in the high nibble (bits 4-7)
 * @param REG Register to modify
 */
#define SET_HIGH_NIB(REG) ((REG) |= 0xF0)

/**
 * @brief Clear all bits in the high nibble (bits 4-7)
 * @param REG Register to modify
 */
#define CLR_HIGH_NIB(REG) ((REG) &= 0x0F)

/**
 * @brief Get the value of the high nibble (bits 4-7)
 * @param REG Register to read
 * @return Value of high nibble (shifted right to bits 0-3)
 */
#define GET_HIGH_NIB(REG) (((REG) >> 4) & 0x0F)

/**
 * @brief Toggle all bits in the high nibble (bits 4-7)
 * @param REG Register to modify
 */
#define TGL_HIGH_NIB(REG) ((REG) ^= 0xF0)

/* Nibble Operations (Low) */

/**
 * @brief Set all bits in the low nibble (bits 0-3)
 * @param REG Register to modify
 */
#define SET_LOW_NIB(REG) ((REG) |= 0x0F)

/**
 * @brief Clear all bits in the low nibble (bits 0-3)
 * @param REG Register to modify
 */
#define CLR_LOW_NIB(REG) ((REG) &= 0xF0)

/**
 * @brief Get the value of the low nibble (bits 0-3)
 * @param REG Register to read
 * @return Value of low nibble (bits 0-3)
 */
#define GET_LOW_NIB(REG) ((REG) & 0x0F)

/**
 * @brief Toggle all bits in the low nibble (bits 0-3)
 * @param REG Register to modify
 */
#define TGL_LOW_NIB(REG) ((REG) ^= 0x0F)

/* Register Operations */

/**
 * @brief Assign a value to a whole register
 * @param REG Register to modify
 * @param VALUE Value to assign
 */
#define ASSIGN_REG(REG, VALUE) ((REG) = (VALUE))

/**
 * @brief Assign a value to the high nibble (bits 4-7)
 * @param REG Register to modify
 * @param VALUE 4-bit value to assign (will be shifted left)
 */
#define ASSIGN_HIGH_NIB(REG, VALUE) ((REG) = ((REG) & 0x0F) | ((VALUE) << 4))

/**
 * @brief Assign a value to the low nibble (bits 0-3)
 * @param REG Register to modify
 * @param VALUE 4-bit value to assign
 */
#define ASSIGN_LOW_NIB(REG, VALUE) ((REG) = ((REG) & 0xF0) | ((VALUE) & 0x0F))

/* Shift Operations */

/**
 * @brief Right shift a register by specified number of bits
 * @param REG Register to modify
 * @param NO Number of bits to shift (0-7)
 */
#define RSHFT_REG(REG, NO) ((REG) >>= (NO))

/**
 * @brief Left shift a register by specified number of bits
 * @param REG Register to modify
 * @param NO Number of bits to shift (0-7)
 */
#define LSHFT_REG(REG, NO) ((REG) <<= (NO))

/**
 * @brief Circular right shift a register by specified number of bits
 * @param REG Register to modify
 * @param NO Number of bits to rotate (0-7)
 */
#define CRSHFT_REG(REG, NO) (((REG) >> (NO)) | ((REG) << (8 - (NO))))

/**
 * @brief Circular left shift a register by specified number of bits
 * @param REG Register to modify
 * @param NO Number of bits to rotate (0-7)
 */
#define CLSHFT_REG(REG, NO) (((REG) << (NO)) | ((REG) >> (8 - (NO))))

/** @} */ // end of BitManipulation group

#endif /* BIT_MATH_H_ */

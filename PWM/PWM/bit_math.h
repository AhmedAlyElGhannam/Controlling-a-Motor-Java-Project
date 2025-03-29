#ifndef BIT_MATH_H
#define BIT_MATH_H

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

/* Clear a certain bit in any register */
#define CLR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

/* Get bit BIT in REG*/
#define GET_BIT(REG, BIT)      (((REG) >> (BIT)) & 1)

/* Shift left a certain bit in any register */
#define SHIFT_LEFT(REG,BIT) (REG=(REG << BIT))

/* Shift right a certain bit in any register */
#define SHIFT_RIGHT(REG,BIT) (REG=(REG >> BIT))

/* Check if a certain bit is set in any register */
#define IS_BIT_SET(REG,BIT) ((REG & (1<<BIT)) >> BIT)

/* Check if a certain bit is cleared in any register */
#define IS_BIT_CLEAR(REG,BIT) (!((REG & (1<<BIT)) >> BIT))

/* Mask a certain bit (set to 0 without affecting other bits) */
#define MASK_BIT(REG,BIT) (REG &= ~(1 << BIT))

/* Rotate left a certain number of times */
#define ROTATE_LEFT(REG,BIT) (REG=(REG << BIT) | (REG >> (sizeof(REG) * 8 - BIT)))

/* Rotate right a certain number of times */
#define ROTATE_RIGHT(REG,BIT) (REG=(REG >> BIT) | (REG << (sizeof(REG) * 8 - BIT)))

#endif /* BIT_MATH_H */
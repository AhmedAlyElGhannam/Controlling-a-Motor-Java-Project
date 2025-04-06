#ifndef BIT_MATH_H
#define BIT_MATH_H

#define SET_BIT(X, POS) ((X) |= (0x01 << (POS)))
#define CLR_BIT(X, POS) ((X) &= ~(0x01 << (POS)))
#define TOG_BIT(X, POS) ((X) ^= (0x01 << (POS)))
#define GET_BIT(X, POS) (((X) >> (POS)) & 0x01)

#define SET_HIGH_NIB(X) ((X) |= 0xF0)
#define CLR_HIGH_NIB(X) ((X) &= 0x0F)
#define GET_HIGH_NIB(X) (((X) >> 4) & 0x0F)
#define TOG_HIGH_NIB(X) ((X) ^= 0xF0)

#define SET_LOW_NIB(X) ((X) |= 0x0F)
#define CLR_LOW_NIB(X) ((X) &= 0xF0)
#define GET_LOW_NIB(X) ((X) & 0x0F)
#define TOG_LOW_NIB(X) ((X) ^= 0x0F)

#define SET_BYTE_TO_VAL(X, VAL)     ((X) = (VAL))
#define SET_HIGH_NIB_TO_VAL(X, VAL) ((X) = ((X) & 0x0F) | (((VAL) & 0x0F) << 4))
#define SET_LOW_NIB_TO_VAL(X, VAL)  ((X) = ((X) & 0xF0) | ((VAL) & 0x0F))

#define SHIFT_BYTE_RIGHT(X, STEP)   ((X) >>= (STEP))
#define SHIFT_BYTE_LEFT(X, STEP)    ((X) <<= (STEP))

#define CSHIFT_BYTE_RIGHT(X, STEP) \
    ((X) = (((X) >> (STEP)) | ((X) << (8 - (STEP)))) & 0xFF)
#define CSHIFT_BYTE_LEFT(X, STEP) \
    ((X) = (((X) << (STEP)) | ((X) >> (8 - (STEP)))) & 0xFF)

#endif
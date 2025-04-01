#ifndef H7SEGMENT_H
#define H7SEGMENT_H

/* enum for port and pin numbers --- common for all HAL so they must be defined once */
#ifndef HAL_PORT_AND_PIN_NAMES
#define HAL_PORT_AND_PIN_NAMES
typedef enum
{
    PORTA = 0x00,
    PORTB,
    PORTC,
    PORTD = 0x03,
} HAL_enuPortName_t;

typedef enum
{
    PIN0 = 0x00,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7 = 0x07,
} HAL_enuPinName_t;
#endif

/* enum to define 7 segment pin names (to make connection more clear) */
typedef enum
{
    H7SEGMENT_PIN_a = 0x00,
    H7SEGMENT_PIN_b,
    H7SEGMENT_PIN_c,
    H7SEGMENT_PIN_d,
    H7SEGMENT_PIN_e,
    H7SEGMENT_PIN_f,
    H7SEGMENT_PIN_g,
    H7SEGMENT_PIN_dot = 0x07,
} H7SEGMENT_enu7SegmentPinNames_t;

/* enum for encoded numbers */
typedef enum
{
	H7SEGMENT_ZERO = 0x3F,
	H7SEGMENT_ONE = 0x06,
	H7SEGMENT_TWO = 0x5B,
	H7SEGMENT_THREE = 0x4F,
	H7SEGMENT_FOUR = 0x66,
	H7SEGMENT_FIVE = 0x6D,
	H7SEGMENT_SIX = 0x7D,
	H7SEGMENT_SEVEN = 0x07,
	H7SEGMENT_EIGHT = 0x7F,
	H7SEGMENT_NINE = 0x6F,
    H7SEGMENT_DOT = 0x80,
} H7SEGMENT_enu7SegmentNumberValues_t;

/* enum for 7 segment connection (Common Anode vs Common Cathode) */
typedef enum
{
    H7SEGMENT_COMMON_ANODE,
    H7SEGMENT_COMMON_CATHODE,
} H7SEGMENT_enu7SegmentConnection_t;

/* enum for error status */
typedef enum
{
    H7SEGMENT_OK,
    H7SEGMENT_INVALID_7SEG_NAME,
    H7SEGMENT_LIMIT_ERROR,
    H7SEGMENT_INVALID_DIGIT,
    H7SEGMENT_NOK,
} H7SEGMENT_enuErrorStatus_t;

/* enum for connected 7 segment names [FILLED IN BY THE USER] */
/* value of first one must be 0x00 */
typedef enum
{
    H7SEGMENT_COUNTER_UNITS = 0x00,
    H7SEGMENT_COUNTER_TENS,
} H7SEGMENT_enu7SegNames_t;


/*
 * @brief	Initializes all 7 segments as per the configuration defined in the array located in LCFG.c
 *                   
 * @param	None
 *				
 * @return  None   
 */
void H7SEGMENT_voidInit(void);


/*
 * @brief	Writes a digit from 0 to 9 on 7 segment display
 *                   
 * @param (in) Copy_uint8_7SegmentName -> 7 segment name as defined by the user in H7SEGMENT_enu7SegNames_t enum
 * 
 * @param (in) Copy_uint8Digit -> number that will be displayed on 7 segment [0, 9].
 *				
 * @return H7SEGMENT_OK || H7SEGMENT_INVALID_7SEG_NAME || H7SEGMENT_INVALID_DIGIT || H7SEGMENT_NOK
 */
H7SEGMENT_enuErrorStatus_t H7SEGMENT_enuWriteDigit(uint8_t Copy_uint8_7SegmentName, uint8_t Copy_uint8Digit);


/*
 * @brief	Writes a multip-digit number on multiple 7 segment displays
 *                   
 * @param (in) Copy_uint32MultiDigitValue -> 7 segment name as defined by the user in H7SEGMENT_enu7SegNames_t enum
 *				
 * @return H7SEGMENT_OK || H7SEGMENT_LIMIT_ERROR || H7SEGMENT_NOK
 */
H7SEGMENT_enuErrorStatus_t H7SEGMENT_enuWriteMultiDigitValue(uint32_t Copy_uint32MultiDigitValue);


#endif

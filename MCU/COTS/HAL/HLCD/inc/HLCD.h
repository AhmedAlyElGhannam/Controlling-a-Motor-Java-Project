#ifndef HLCD_H
#define HLCD_H


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

typedef enum 
{
    HLCD_OK,
    HLCD_NOK,
    HLCD_NULL_PTR,
    HLCD_INVALID_PARAM,
    HLCD_INVALID_LINE,
    HLCD_INVALID_COL,
} HLCD_enuErrorStatus_t;

typedef enum 
{
    HLCD_LINE_01 = 0x80U,
    HLCD_LINE_02 = 0xC0U,
} HLCD_enuLinePosition_t;

typedef enum 
{
    HLCD_COL_00 = 0x00U,
    HLCD_COL_01,
    HLCD_COL_02,
    HLCD_COL_03,
    HLCD_COL_04,
    HLCD_COL_05,
    HLCD_COL_06,
    HLCD_COL_07,
    HLCD_COL_08,
    HLCD_COL_09,
    HLCD_COL_10,
    HLCD_COL_11,
    HLCD_COL_12,
    HLCD_COL_13,
    HLCD_COL_14,
    HLCD_COL_15 = 0x0FU,
} HLCD_enuColPosition_t;

typedef enum 
{
    HLCD_ENABLE = 1U,
    HLCD_DISABLE = 0U,
} HLCD_enuFeatureConfig_t;

typedef struct 
{
    HAL_enuPortName_t   HLCD_ConfigPort;
    HAL_enuPinName_t    HLCD_RS;
    HAL_enuPinName_t    HLCD_RW;
    HAL_enuPinName_t    HLCD_E;

    HAL_enuPortName_t   HLCD_DataPort;
    

    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    HAL_enuPinName_t    HLCD_DataPinsArr[4];
    #endif

} HLCD_structLCDObject_t;


typedef enum 
{
    HLCD_CMD01_CLR_DISPLAY = 0b00000001U,
    HLCD_CMD02_RET_HOME = 0b00000010U,
    HLCD_CMD03_CURSOR_MOVEMENT_DIR_INC = 0b00000110U,
    HLCD_CMD03_CURSOR_MOVEMENT_DIR_DEC = 0b00000100U,
    HLCD_CMD03_DISPLAY_SHIFT_ON = 0b00000101U,
    HLCD_CMD03_DISPLAY_SHIFT_OFF = 0b00000100U,
    HLCD_CMD04_DISPLAY_ON = 0b00001100U,
    HLCD_CMD04_DISPLAY_OFF = 0b00001000U,
    HLCD_CMD04_CURSOR_ON = 0b00001010U,
    HLCD_CMD04_CURSOR_OFF = 0b00001000U,
    HLCD_CMD04_CURSOR_BLINK_ON = 0b00001001U,
    HLCD_CMD04_CURSOR_BLINK_OFF = 0b00001000U,
    HLCD_CMD05_DISPLAY_SHIFT_SET = 0b00011000U,
    HLCD_CMD05_CURSOR_SHIFT_SET = 0b00010000U,
    HLCD_CMD05_SHIFT_RIGHT = 0b00010100U,
    HLCD_CMD05_SHIFT_LEFT = 0b00010000U,
    HLCD_CMD06_8_BIT_MODE = 0b00110000U,
    HLCD_CMD06_4_BIT_MODE = 0b00100000U,
    HLCD_CMD06_2_DISPLAY_LINE = 0b00101000U,
    HLCD_CMD06_1_DISPLAY_LINE = 0b00100000U,
    HLCD_CMD06_5x11_DOTS = 0b00100100U,
    HLCD_CMD06_5x8_DOTS = 0b00100000U,
} HLCD_enuLCDCommands_t;


extern HLCD_enuErrorStatus_t HLCD_enuInit(const HLCD_structLCDObject_t* const ptr_structLCDObject);
extern void HLCD_voidWriteCommand(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8Command);
extern HLCD_enuErrorStatus_t HLCD_enuWriteCharacter(const HLCD_structLCDObject_t* const ptr_structLCDObject, const char copy_charChar);
extern HLCD_enuErrorStatus_t HLCD_enuWriteString(const HLCD_structLCDObject_t* const ptr_structLCDObject, const char* const ptr_charString);
extern HLCD_enuErrorStatus_t HLCD_enuWriteInteger(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint32_t copt_uint32Number);
extern HLCD_enuErrorStatus_t HLCD_enuGoToXY(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8X, uint8_t Copy_uint8Y);
extern HLCD_enuErrorStatus_t HLCD_enuClrDisplay(const HLCD_structLCDObject_t* const ptr_structLCDObject);
extern HLCD_enuErrorStatus_t HLCD_enuStoreSpecialCharacter(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8BlockNumber, const uint8_t* const ptr_uint8Pattern);
extern HLCD_enuErrorStatus_t HLCD_enuDisplaySpecialCharacter(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8PatternNumber, uint8_t Copy_uint8Row, uint8_t Copy_uint8Column);
extern HLCD_enuErrorStatus_t HLCD_enuReturnHome(const HLCD_structLCDObject_t* const ptr_structLCDObject);
extern HLCD_enuErrorStatus_t HLCD_enuCursorBlinkOnOff(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8Config);
extern HLCD_enuErrorStatus_t HLCD_enuDisplayOnOff(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8Config);
extern HLCD_enuErrorStatus_t HLCD_enuCursorShiftLeft(const HLCD_structLCDObject_t* const ptr_structLCDObject);
extern HLCD_enuErrorStatus_t HLCD_enuCursorShiftRight(const HLCD_structLCDObject_t* const ptr_structLCDObject);
extern HLCD_enuErrorStatus_t HLCD_enuDisplayShiftLeft(const HLCD_structLCDObject_t* const ptr_structLCDObject);
extern HLCD_enuErrorStatus_t HLCD_enuDisplayShiftRight(const HLCD_structLCDObject_t* const ptr_structLCDObject);



#endif
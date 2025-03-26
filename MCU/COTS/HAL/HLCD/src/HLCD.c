#include "std_types.h"
#include "MDIO_PBCFG.h"
#include "MDIO.h"
// #include "MPORT_LCFG.h"
// #include "MPORT_PBCFG.h"
// #include "MPORT.h"
#include "HLCD_PBCFG.h"
#include "HLCD.h"
#include <util/delay.h>

#define IS_INVALID_PORT_NUM(X)			((((MDIO_enuPortNum_t)X) > MDIO_PORTD) || (((MDIO_enuPortNum_t)X) < MDIO_PORTA))
#define IS_INVALID_PIN_NUM(X)			((((MDIO_enuPinNum_t)X) > MDIO_PIN7) || (((MDIO_enuPinNum_t)X) < MDIO_PIN0))
#define IS_INVALID_LINE(LINE)           (((LINE) != HLCD_LINE_01) && ((LINE) != HLCD_LINE_02))
#define IS_INVALID_COL(COL)             (((COL) > HLCD_COL_15) || ((COL) < HLCD_COL_00))
#define IS_NULL_PTR(PTR)                ((PTR) == NULL)
#define IS_INVALID_CONFIG(CFG)          (((CFG) != HLCD_ENABLE) && ((CFG) != HLCD_DISABLE)) 

HLCD_enuErrorStatus_t HLCD_enuInit(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        #if (HLCD_MODE_OF_OPERATION == HLCD_8_BIT_MODE)
        _delay_us(50);
        /* function set */
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD06_8_BIT_MODE | HLCD_CMD06_5x8_DOTS | HLCD_CMD06_2_DISPLAY_LINE);
        _delay_ms(3);
        /* display control on/off */
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD04_DISPLAY_ON | HLCD_CMD04_CURSOR_ON | HLCD_CMD04_CURSOR_BLINK_ON);
        _delay_ms(3);
        /* display clear */
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD01_CLR_DISPLAY);
        _delay_ms(3);
        /* entry mode set */
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD03_CURSOR_MOVEMENT_DIR_INC | HLCD_CMD03_DISPLAY_SHIFT_OFF);
        _delay_us(50);
        #else
            _delay_us(50);
        /* function set */
        HLCD_voidWriteCommand(ptr_structLCDObject, (HLCD_CMD06_4_BIT_MODE >> 4));
        HLCD_voidWriteCommand(ptr_structLCDObject, (HLCD_CMD06_4_BIT_MODE >> 4));
        HLCD_voidWriteCommand(ptr_structLCDObject, ((HLCD_CMD06_5x8_DOTS | HLCD_CMD06_2_DISPLAY_LINE) >> 4));
        _delay_ms(3);
        /* display control on/off */
        HLCD_voidWriteCommand(ptr_structLCDObject, ((HLCD_CMD04_DISPLAY_ON | HLCD_CMD04_CURSOR_ON | HLCD_CMD04_CURSOR_BLINK_ON) >> 4));
        _delay_ms(3);
        /* display clear */
        HLCD_voidWriteCommand(ptr_structLCDObject, (HLCD_CMD01_CLR_DISPLAY >> 4));
        _delay_ms(3);
        /* entry mode set */
        HLCD_voidWriteCommand(ptr_structLCDObject, (HLCD_CMD03_CURSOR_MOVEMENT_DIR_INC | HLCD_CMD03_DISPLAY_SHIFT_OFF >> 4));
        _delay_us(50);
        #endif
        
    }

    return ret_enumStatus;
}

void HLCD_voidWriteCommand(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8Command)
{
    #if (HLCD_MODE_OF_OPERATION == HLCD_8_BIT_MODE)

    MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RS, MDIO_PIN_LOW);
    MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RW, MDIO_PIN_LOW);
    
    MDIO_enuSetPortValue(ptr_structLCDObject->HLCD_DataPort, Copy_uint8Command);

    #else

    uint8_t local_uint8Iter;

    MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RS, MDIO_PIN_LOW);
    MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RW, MDIO_PIN_LOW);

    for (local_uint8Iter = 0; local_uint8Iter < 4; local_uint8Iter++)
    {
        MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_DataPinsArr[local_uint8Iter], (1U & Copy_uint8Command));
        Copy_uint8Command = Copy_uint8Command >> 1;
    }

    #endif

    /* send a pulse on E pin for 1us */
    MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_E, MDIO_PIN_HIGH);
    _delay_us(1);
    MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_E, MDIO_PIN_LOW);

    _delay_ms(3);

    return;
}

HLCD_enuErrorStatus_t HLCD_enuWriteCharacter(const HLCD_structLCDObject_t* const ptr_structLCDObject, const char copy_charChar)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        #if (HLCD_MODE_OF_OPERATION == HLCD_8_BIT_MODE)

        MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RS, MDIO_PIN_HIGH);
        MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RW, MDIO_PIN_LOW);

        MDIO_enuSetPortValue(ptr_structLCDObject->HLCD_DataPort, copy_charChar);

        #else

        uint8_t local_uint8Iter;

        MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RS, MDIO_PIN_LOW);
        MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_RW, MDIO_PIN_LOW);

        for (local_uint8Iter = 0; local_uint8Iter < 4; local_uint8Iter++)
        {
            MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_DataPinsArr[local_uint8Iter], (1U & Copy_uint8Command));
            Copy_uint8Command = Copy_uint8Command >> 1;
        }

        #endif

        /* send a pulse on E pin for 1us */
        MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_E, MDIO_PIN_HIGH);
        _delay_us(1);
        MDIO_enuSetPinValue(ptr_structLCDObject->HLCD_ConfigPort, ptr_structLCDObject->HLCD_E, MDIO_PIN_LOW);

        _delay_ms(10);
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuWriteString(const HLCD_structLCDObject_t* const ptr_structLCDObject, const char* const ptr_charString)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;
    uint8_t local_uint8Iter;

    if (IS_NULL_PTR(ptr_structLCDObject) || IS_NULL_PTR(ptr_charString))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
	    for (local_uint8Iter = 0; ptr_charString[local_uint8Iter] != NULL_CHAR; local_uint8Iter++)
        {
            ret_enumStatus = HLCD_enuWriteCharacter(ptr_structLCDObject, ptr_charString[local_uint8Iter]);
            if (ret_enumStatus != HLCD_OK)
                break;
        }
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuWriteInteger(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint32_t copt_uint32Number)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        uint8_t local_uint8NumAsCharArr[10];
	    uint8_t local_uint8NumDigits = 0;
    
	    uint32_t local_uint32ReversedNum = 0;
    
	    uint8_t local_uint8Iter;
    
	    /* reversing the number so that it will be stored in order */
	    while (copt_uint32Number)
	    {
	    	local_uint32ReversedNum = (local_uint32ReversedNum * 10) + (copt_uint32Number % 10);
	    	copt_uint32Number /= 10;
	    	local_uint8NumDigits++;
	    }
    
	    for (local_uint8Iter = 0; local_uint8Iter < local_uint8NumDigits; local_uint8Iter++)
	    {
	    	local_uint8NumAsCharArr[local_uint8Iter] = (local_uint32ReversedNum % 10) + '0';
	    	local_uint32ReversedNum /= 10;
	    }
	    local_uint8NumAsCharArr[local_uint8NumDigits] = '\0';
    
	    ret_enumStatus = HLCD_enuWriteString(ptr_structLCDObject, local_uint8NumAsCharArr);

    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuGoToXY(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8X, uint8_t Copy_uint8Y)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else if (IS_INVALID_LINE(Copy_uint8Y))
    {
        ret_enumStatus = HLCD_INVALID_LINE;
    }
    else if (IS_INVALID_COL(Copy_uint8X))
    {
        ret_enumStatus = HLCD_INVALID_LINE;
    }
    else
    {
        HLCD_voidWriteCommand(ptr_structLCDObject, (Copy_uint8X + Copy_uint8Y));
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuClrDisplay(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD01_CLR_DISPLAY);
    }

    return ret_enumStatus;
}

static HLCD_enuErrorStatus_t HLCD_enuGoToCGRAM(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8BlockNumber)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        #if (HLCD_MODE_OF_OPERATION == HLCD_8_BIT_MODE)
        HLCD_voidWriteCommand(ptr_structLCDObject, (Copy_uint8BlockNumber * 8) + 64);
        #else 
        HLCD_voidWriteCommand(ptr_structLCDObject, (Copy_uint8BlockNumber * 8) + 64);
        #endif
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuStoreSpecialCharacter(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8BlockNumber, const uint8_t* const ptr_uint8Pattern)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;
    uint8_t local_uint8Iter;

    if (IS_NULL_PTR(ptr_structLCDObject) || IS_NULL_PTR(ptr_uint8Pattern))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        #if (HLCD_MODE_OF_OPERATION == HLCD_8_BIT_MODE)
        ret_enumStatus = HLCD_enuGoToCGRAM(ptr_structLCDObject, Copy_uint8BlockNumber);
        if (ret_enumStatus == HLCD_OK)
        {
            for (local_uint8Iter = 0; local_uint8Iter < 8; local_uint8Iter++)
            {
                ret_enumStatus = HLCD_enuWriteCharacter(ptr_structLCDObject, ptr_uint8Pattern[local_uint8Iter]);
                if (ret_enumStatus != HLCD_OK)
                    break;
            }
        }
        #else 
        4-bit code is not implemented yet
        #endif
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuDisplaySpecialCharacter(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8PatternNumber, uint8_t Copy_uint8Row, uint8_t Copy_uint8Column)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        HLCD_enuGoToXY(ptr_structLCDObject, Copy_uint8Column, Copy_uint8Row);
        HLCD_voidWriteCommand(ptr_structLCDObject, Copy_uint8PatternNumber);
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuReturnHome(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD02_RET_HOME);
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuCursorBlinkOnOff(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8Config)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_CONFIG(Copy_uint8Config))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        if (Copy_uint8Config == HLCD_ENABLE)
        {
            HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD04_CURSOR_BLINK_ON);
        }
        else
        {
            HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD04_CURSOR_BLINK_OFF);
        }
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuDisplayOnOff(const HLCD_structLCDObject_t* const ptr_structLCDObject, uint8_t Copy_uint8Config)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_CONFIG(Copy_uint8Config))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        if (Copy_uint8Config == HLCD_ENABLE)
        {
            HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD04_DISPLAY_ON);
        }
        else
        {
            HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD04_DISPLAY_OFF);
        }
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuCursorShiftLeft(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD05_CURSOR_SHIFT_SET | HLCD_CMD05_SHIFT_LEFT);

    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuCursorShiftRight(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD05_CURSOR_SHIFT_SET | HLCD_CMD05_SHIFT_RIGHT);
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuDisplayShiftLeft(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD05_DISPLAY_SHIFT_SET | HLCD_CMD05_SHIFT_LEFT);
    }

    return ret_enumStatus;
}

HLCD_enuErrorStatus_t HLCD_enuDisplayShiftRight(const HLCD_structLCDObject_t* const ptr_structLCDObject)
{
    HLCD_enuErrorStatus_t ret_enumStatus = HLCD_OK;

    if (IS_NULL_PTR(ptr_structLCDObject))
    {
        ret_enumStatus = HLCD_NULL_PTR;
    }
    else if (IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_ConfigPort) || IS_INVALID_PORT_NUM(ptr_structLCDObject->HLCD_DataPort))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    else if ( IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RS) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_RW) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_E))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #if (HLCD_MODE_OF_OPERATION == HLCD_4_BIT_MODE)
    else if (IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[3]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[2]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[1]) || IS_INVALID_PIN_NUM(ptr_structLCDObject->HLCD_DataPinsArr[0]))
    {
        ret_enumStatus = HLCD_INVALID_PARAM;
    }
    #endif
    else
    {
        HLCD_voidWriteCommand(ptr_structLCDObject, HLCD_CMD05_DISPLAY_SHIFT_SET | HLCD_CMD05_SHIFT_RIGHT);
    }

    return ret_enumStatus;
}

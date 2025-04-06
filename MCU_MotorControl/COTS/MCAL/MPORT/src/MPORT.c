#include "bit_math.h"
#ifdef UNIT_TESTING_MODE
    /* header files that are included only when in testing mode */
	#include <stdint.h>
	#include <stddef.h>
	#include <stdbool.h>
	#include "MDIO_MockReg.h"

	/* mock base addresses for ports using extern (only in testing mode) */
	extern MDIO_strPortRegElement_t MOCK_PORTA;
	extern MDIO_strPortRegElement_t MOCK_PORTB;
	extern MDIO_strPortRegElement_t MOCK_PORTC;
	extern MDIO_strPortRegElement_t MOCK_PORTD;
#else
    #include "std_types.h"
    #include "MDIO_Registers.h"
#endif
#include "MPORT_PBCFG.h"
#include "MPORT.h"
#include "MPORT_LCFG.h"


/* defining number of pins per port */
#define MPORT_NUM_OF_PINS   8

/* macros for checking function arguments */
#define IS_INVALID_PORT_NUM(X)			((((uint8_t)X) > 0x03) || (((uint8_t)X) < 0x00))
#define IS_INVALID_PIN_NUM(X)			((((uint8_t)X) > 0x07) || (((uint8_t)X) < 0x00))
#define IS_INVALID_PORT_PIN_DIR(X)		((((MPORT_enuPortPinDir_t)X) != MPORT_PORT_PIN_INPUT) && (((MPORT_enuPortPinDir_t)X) != MPORT_PORT_PIN_OUTPUT))
#define IS_INVALID_PORT_PIN_MODE(X)		((((MPORT_enuPortPinMode_t)X) != MPORT_PIN_MODE_INPUT_PULLUP) && (((MPORT_enuPortPinMode_t)X) != MPORT_PIN_MODE_INPUT_PULLDOWN) && (((MPORT_enuPortPinMode_t)X) != MPORT_PIN_MODE_UART))

/* accessing pin configuration array defined in LCFG.c file */
extern MPORT_structPortPinDirAndMode_t MPORT_enuArrPinConfig[MPORT_NUM_OF_ALL_PINS];

/* macro to combine pin && port numbers in required format */
#define COMBINE_PORT_AND_PIN(PORTx, PINn)   ((PORTx << 4) | (PINn))

/* macros to extract port && pin numbers from iterator */
#define GET_PORT_NUM(ITER)      ((ITER) / 8)
#define GET_PIN_NUM(ITER)       ((ITER) % 8)


/*
 * @brief	Initializes all pins direction and modes as specified in array defined in LCFG.c
 *                   
 * @param	None
 *				
 * @return  None   
 */
void MPORT_voidInit(void)
{
    /* defining variables to hold iterator, port number , pin number, combined port and pin number */
    uint8_t Local_uint8Iter;
    uint8_t Local_uint8PortNum;
    uint8_t Local_uint8PinNum;
    uint8_t Local_uint8CombinedPortAndPinNum;

	/* iterate over all pins */
    for (Local_uint8Iter = 0; Local_uint8Iter < (MPORT_NUM_OF_ALL_PINS); Local_uint8Iter++)
    {
        /* get port number from iterator and store it in variable */
        Local_uint8PortNum = GET_PORT_NUM(Local_uint8Iter);

        /* get pin number from iterator and store it in variable */
        Local_uint8PinNum = GET_PIN_NUM(Local_uint8Iter);
        
        /* combine port and pin numbers and store them in variable */
        Local_uint8CombinedPortAndPinNum = COMBINE_PORT_AND_PIN(Local_uint8PortNum, Local_uint8PinNum);
        
        /* call MPORT function to set specified port pin direction */
        MPORT_enuSetPinDirection(Local_uint8CombinedPortAndPinNum, MPORT_enuArrPinConfig[Local_uint8Iter].dir);
        
        /* call MPORT function to set specified port pin mode */
        if (MPORT_enuArrPinConfig[Local_uint8Iter].mode != MPORT_PIN_MODE_NONE)
            MPORT_enuSetPinMode(Local_uint8CombinedPortAndPinNum, MPORT_enuArrPinConfig[Local_uint8Iter].mode);
        else {} /* do nothing */
    }

    return;
}


/*
 * @brief	Set pin data direction
 *                   
 * @param	Copy_enuPortPinNum 	-> 	MPORT_PIN_A0 to MPORT_PIN_D7 (specified in MPORT.h) 
 * @param   Copy_enuPortPinDir	->	MPORT_PORT_PIN_INPUT, MPORT_PORT_PIN_OUTPUT
 *				
 * @return  MPORT_OK, MPORT_ERROR_INVALID_PIN,MPORT_ERROR_INVALID_DIRECTION
 */
MPORT_enuErrorStatus_t MPORT_enuSetPinDirection(MPORT_enuPortPin_t Copy_enuPortPinNum,  MPORT_enuPortPinDir_t  Copy_enuPortPinDir)
{
    /* defining a variable to store return address */
    MPORT_enuErrorStatus_t Ret_enuStatus = MPORT_OK;

    /* extracting port and pin numbers from passed argument*/
    uint8_t Local_uint8PortNum = GET_HIGH_NIB(Copy_enuPortPinNum);
    uint8_t Local_uint8PinNum = GET_LOW_NIB(Copy_enuPortPinNum);
    
    if (IS_INVALID_PIN_NUM(Local_uint8PinNum) || IS_INVALID_PORT_NUM(Local_uint8PortNum))
    {
        /* do not continue if passed argument contained either a wrong port number or pin number */
        Ret_enuStatus = MPORT_ERROR_INVALID_PIN;
    }
    else if (IS_INVALID_PORT_PIN_DIR(Copy_enuPortPinDir))
    {
        /* do not continue if passed direction is invalid */
        Ret_enuStatus = MPORT_ERROR_INVALID_DIRECTION;
    }
    else /* all arguments are valid */
    {
        /* get port base register based on passed port number */
        MDIO_strPortRegElement_t* Local_structPtrCurrPort = MDIO_GET_PORT_ADD(Local_uint8PortNum);

        /* set pin direction as specified in argument*/
        switch (Copy_enuPortPinDir)
        {
            case MPORT_PORT_PIN_OUTPUT:
                /* set bit in DDR corresponding to pin number */
                SET_BIT(Local_structPtrCurrPort->DDR, Local_uint8PinNum);
            break;

            case MPORT_PORT_PIN_INPUT:
                /* clear bit in DDR corresponding to pin number */
                CLR_BIT(Local_structPtrCurrPort->DDR, Local_uint8PinNum);
            break;
        }
    }
    
    return Ret_enuStatus;
}


/*
 * @brief	Set pin mode
 *                   
 * @param	Copy_enuPortPinNum 	-> 	MPORT_PIN_A0 to MPORT_PIN_D7 (specified in MPORT.h) 
 * @param   Copy_enuPortPinMode	->	MPORT_PIN_MODE_INPUT_PULLUP, MPORT_PIN_MODE_INPUT_PULLDOWN, MPORT_PIN_MODE_UART
 *				
 * @return  MPORT_OK, MPORT_ERROR_INVALID_PIN, MPORT_ERROR_INVALID_MODE, MPORT_ERROR_UNCHANGEABLE_MODE
 */
MPORT_enuErrorStatus_t MPORT_enuSetPinMode(MPORT_enuPortPin_t Copy_enuPortPinNum, MPORT_enuPortPinMode_t Copy_enuPortPinMode)
{
    /* defining a variable to store return address */
    MPORT_enuErrorStatus_t Ret_enuStatus = MPORT_OK;

    /* extracting port and pin numbers from passed argument*/
    uint8_t Local_uint8PortNum = GET_HIGH_NIB(Copy_enuPortPinNum);
    uint8_t Local_uint8PinNum = GET_LOW_NIB(Copy_enuPortPinNum);

    if (IS_INVALID_PIN_NUM(Local_uint8PinNum) || IS_INVALID_PORT_NUM(Local_uint8PortNum))
    {
        /* do not continue if passed argument contained either a wrong port number or pin number */
        Ret_enuStatus = MPORT_ERROR_INVALID_PIN;
    }
    else if (IS_INVALID_PORT_PIN_MODE(Copy_enuPortPinMode))
    {
        /* do not continue if passed mode is invalid */
        Ret_enuStatus = MPORT_ERROR_INVALID_MODE;
    }
    else /* all arguments are valid */
    {
        /* get port base register based on passed port number */
        MDIO_strPortRegElement_t* Local_structPtrCurrPort = MDIO_GET_PORT_ADD(Local_uint8PortNum);

        /* set pin mode as specified in argument*/
        switch (Copy_enuPortPinMode)
        {
            case MPORT_PIN_MODE_INPUT_PULLUP:
                /* pull up is activated on pin by setting corresponding bit in PORT to 1 */
                SET_BIT(Local_structPtrCurrPort->PORT, Local_uint8PinNum);
            break;

            case MPORT_PIN_MODE_INPUT_PULLDOWN:
                /* pull down is activated on pin by clearing corresponding bit in PORT*/
                CLR_BIT(Local_structPtrCurrPort->PORT, Local_uint8PinNum);
            break;

            case MPORT_PIN_MODE_UART:
                /* no knowledge at the moment */
            break;

            default:
                /* put this here cuz it will be used eventually */
                Ret_enuStatus = MPORT_ERROR_UNCHANGEABLE_MODE;
            break;
        }
    }

    return Ret_enuStatus;
}
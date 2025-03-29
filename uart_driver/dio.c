/**
 * @file dio.c
 * @brief Digital Input/Output (DIO) Driver Implementation for AVR
 * 
 * This file contains the implementation of GPIO functions for AVR microcontrollers,
 * including pin/port configuration and value manipulation.
 */

#include "std_types.h"
#include "bit_math.h"
#include "dio.h"

/**
 * @defgroup DIO_Private_Functions DIO Private Functions
 * @{
 */

/**
 * @brief Validate port and pin numbers
 * @param Port Port number to validate
 * @param Pin Pin number to validate
 * @return MDIO_enuErrorStatus_t Validation result
 * @retval MDIO_OK Port and pin are valid
 * @retval MDIO_NOK Invalid port or pin number
 */
static MDIO_enuErrorStatus_t MDIO_ValidatePortPin(MDIO_enuPortNum_t Port, MDIO_enuPinNum_t Pin)
{
    return (Port <= MDIO_PORTD && Pin <= MDIO_PIN7) ? MDIO_OK : MDIO_NOK;
}

/** @} */ // end of DIO_Private_Functions group

/**
 * @defgroup DIO_Public_Functions DIO Public Functions
 * @{
 */

/**
 * @brief Set the value of a specific GPIO pin
 * @param Copy_enuPortNum Target port (MDIO_PORTA to MDIO_PORTD)
 * @param Copy_enuPinNum Target pin (MDIO_PIN0 to MDIO_PIN7)
 * @param Copy_enuPinConfiguration Desired pin state (MDIO_HIGH or MDIO_LOW)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port/pin number or configuration
 * 
 * @note For input pins, this sets the pull-up resistor state when applicable
 */
MDIO_enuErrorStatus_t MDIO_enuSetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, 
                                         MDIO_enuPinNum_t Copy_enuPinNum, 
                                         u8 Copy_enuPinConfiguration)
{
    if (MDIO_ValidatePortPin(Copy_enuPortNum, Copy_enuPinNum) == MDIO_OK)
    {
        switch (Copy_enuPinConfiguration)
        {
        case MDIO_HIGH:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: SET_BIT(PORTA_REG, Copy_enuPinNum); break;
            case MDIO_PORTB: SET_BIT(PORTB_REG, Copy_enuPinNum); break;
            case MDIO_PORTC: SET_BIT(PORTC_REG, Copy_enuPinNum); break;
            case MDIO_PORTD: SET_BIT(PORTD_REG, Copy_enuPinNum); break;
            default: return MDIO_NOK;
            }
            break;

        case MDIO_LOW:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: CLR_BIT(PORTA_REG, Copy_enuPinNum); break;
            case MDIO_PORTB: CLR_BIT(PORTB_REG, Copy_enuPinNum); break;
            case MDIO_PORTC: CLR_BIT(PORTC_REG, Copy_enuPinNum); break;
            case MDIO_PORTD: CLR_BIT(PORTD_REG, Copy_enuPinNum); break;
            default: return MDIO_NOK;
            }
            break;

        default:
            return MDIO_NOK;
        }
        return MDIO_OK;
    }
    return MDIO_NOK;
}

/**
 * @brief Configure GPIO pin direction and pull-up settings
 * @param Copy_enuPortNum Target port (MDIO_PORTA to MDIO_PORTD)
 * @param Copy_enuPinNum Target pin (MDIO_PIN0 to MDIO_PIN7)
 * @param Copy_u8PinDir Pin configuration (MDIO_INPUT/OUTPUT/INPUT_PULLUP)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port/pin number or configuration
 * 
 * @note For input mode, automatically disables pull-up unless INPUT_PULLUP specified
 */
MDIO_enuErrorStatus_t MDIO_enuSetPinConfiguration(MDIO_enuPortNum_t Copy_enuPortNum, 
                                                 MDIO_enuPinNum_t Copy_enuPinNum, 
                                                 u8 Copy_u8PinDir)
{
    if (MDIO_ValidatePortPin(Copy_enuPortNum, Copy_enuPinNum) == MDIO_OK)
    {
        switch (Copy_u8PinDir)
        {
        case MDIO_OUTPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: SET_BIT(DDRA_REG, Copy_enuPinNum); break;
            case MDIO_PORTB: SET_BIT(DDRB_REG, Copy_enuPinNum); break;
            case MDIO_PORTC: SET_BIT(DDRC_REG, Copy_enuPinNum); break;
            case MDIO_PORTD: SET_BIT(DDRD_REG, Copy_enuPinNum); break;
            default: return MDIO_NOK;
            }
            break;

        case MDIO_INPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: 
                CLR_BIT(DDRA_REG, Copy_enuPinNum); 
                CLR_BIT(PORTA_REG, Copy_enuPinNum); 
                break;
            case MDIO_PORTB:
                CLR_BIT(DDRB_REG, Copy_enuPinNum);
                CLR_BIT(PORTB_REG, Copy_enuPinNum);
                break;
            case MDIO_PORTC:
                CLR_BIT(DDRC_REG, Copy_enuPinNum);
                CLR_BIT(PORTC_REG, Copy_enuPinNum);
                break;
            case MDIO_PORTD:
                CLR_BIT(DDRD_REG, Copy_enuPinNum);
                CLR_BIT(PORTD_REG, Copy_enuPinNum);
                break;
            default: return MDIO_NOK;
            }
            break;

        case MDIO_INPUT_PULLUP:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA:
                CLR_BIT(DDRA_REG, Copy_enuPinNum);
                SET_BIT(PORTA_REG, Copy_enuPinNum);
                break;
            case MDIO_PORTB:
                CLR_BIT(DDRB_REG, Copy_enuPinNum);
                SET_BIT(PORTB_REG, Copy_enuPinNum);
                break;
            case MDIO_PORTC:
                CLR_BIT(DDRC_REG, Copy_enuPinNum);
                SET_BIT(PORTC_REG, Copy_enuPinNum);
                break;
            case MDIO_PORTD:
                CLR_BIT(DDRD_REG, Copy_enuPinNum);
                SET_BIT(PORTD_REG, Copy_enuPinNum);
                break;
            default: return MDIO_NOK;
            }
            break;

        default:
            return MDIO_NOK;
        }
        return MDIO_OK;
    }
    return MDIO_NOK;
}

/**
 * @brief Set value for an entire GPIO port
 * @param Copy_enuPortNum Target port (MDIO_PORTA to MDIO_PORTD)
 * @param Copy_u8PortValue 8-bit value to write (0x00 to 0xFF)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port number
 */
MDIO_enuErrorStatus_t MDIO_enuSetPortValue(MDIO_enuPortNum_t Copy_enuPortNum, 
                                         u8 Copy_u8PortValue)
{
    if (Copy_enuPortNum <= MDIO_PORTD)
    {
        switch (Copy_enuPortNum)
        {
        case MDIO_PORTA: PORTA_REG = Copy_u8PortValue; break;
        case MDIO_PORTB: PORTB_REG = Copy_u8PortValue; break;
        case MDIO_PORTC: PORTC_REG = Copy_u8PortValue; break;
        case MDIO_PORTD: PORTD_REG = Copy_u8PortValue; break;
        default: return MDIO_NOK;
        }
        return MDIO_OK;
    }
    return MDIO_NOK;
}

/**
 * @brief Configure direction for an entire GPIO port
 * @param Copy_enuPortNum Target port (MDIO_PORTA to MDIO_PORTD)
 * @param Copy_enuPortConfiguration Port configuration (see MDIO_enuPortConfiguration_t)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port number or configuration
 * 
 * @note Supports mixed input/output configurations for nibbles
 */
MDIO_enuErrorStatus_t MDIO_enuSetPortDir(MDIO_enuPortNum_t Copy_enuPortNum, 
                                       MDIO_enuPortConfiguration_t Copy_enuPortConfiguration)
{
    if (Copy_enuPortNum <= MDIO_PORTD)
    {
        switch (Copy_enuPortConfiguration)
        {
        case MDIO_PORT_INPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: DDRA_REG = 0x00; break;
            case MDIO_PORTB: DDRB_REG = 0x00; break;
            case MDIO_PORTC: DDRC_REG = 0x00; break;
            case MDIO_PORTD: DDRD_REG = 0x00; break;
            default: return MDIO_NOK;
            }
            break;

        case MDIO_PORT_OUTPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: DDRA_REG = 0xFF; break;
            case MDIO_PORTB: DDRB_REG = 0xFF; break;
            case MDIO_PORTC: DDRC_REG = 0xFF; break;
            case MDIO_PORTD: DDRD_REG = 0xFF; break;
            default: return MDIO_NOK;
            }
            break;

        case MDIO_PORT_HIGH_NIBBLE_INPUP_PULL_UP_LOW_NIBBLE_OUTPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: DDRA_REG = 0x0F; PORTA_REG = 0xF0; break;
            case MDIO_PORTB: DDRB_REG = 0x0F; PORTB_REG = 0xF0; break;
            case MDIO_PORTC: DDRC_REG = 0x0F; PORTC_REG = 0xF0; break;
            case MDIO_PORTD: DDRD_REG = 0x0F; PORTD_REG = 0xF0; break;
            default: return MDIO_NOK;
            }
            break;

        case MDIO_PORT_LOW_NIBBLE_INPUT_PULL_UP_HIGH_NIBBLE_OUTPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA: DDRA_REG = 0xF0; PORTA_REG = 0x0F; break;
            case MDIO_PORTB: DDRB_REG = 0xF0; PORTB_REG = 0x0F; break;
            case MDIO_PORTC: DDRC_REG = 0xF0; PORTC_REG = 0x0F; break;
            case MDIO_PORTD: DDRD_REG = 0xF0; PORTD_REG = 0x0F; break;
            default: return MDIO_NOK;
            }
            break;
        }
        return MDIO_OK;
    }
    return MDIO_NOK;
}

/**
 * @brief Read the current value of a GPIO pin
 * @param Copy_enuPortNum Target port (MDIO_PORTA to MDIO_PORTD)
 * @param Copy_enuPinNum Target pin (MDIO_PIN0 to MDIO_PIN7)
 * @param Add_pu8PinValue Pointer to store the pin value (0 or 1)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port/pin number
 * @retval MDIO_NULL_PTR Null pointer provided for result
 * 
 * @note Always reads the pin register, regardless of direction setting
 */
MDIO_enuErrorStatus_t MDIO_enuGetPinValue(MDIO_enuPortNum_t Copy_enuPortNum,
                                         MDIO_enuPinNum_t Copy_enuPinNum,
                                         u8 *Add_pu8PinValue)
{
    if (Add_pu8PinValue == NULL) return MDIO_NULL_PTR;
    
    if (MDIO_ValidatePortPin(Copy_enuPortNum, Copy_enuPinNum) == MDIO_OK)
    {
        switch (Copy_enuPortNum)
        {
        case MDIO_PORTA: *Add_pu8PinValue = GET_BIT(PINA_REG, Copy_enuPinNum); break;
        case MDIO_PORTB: *Add_pu8PinValue = GET_BIT(PINB_REG, Copy_enuPinNum); break;
        case MDIO_PORTC: *Add_pu8PinValue = GET_BIT(PINC_REG, Copy_enuPinNum); break;
        case MDIO_PORTD: *Add_pu8PinValue = GET_BIT(PIND_REG, Copy_enuPinNum); break;
        default: return MDIO_NOK;
        }
        return MDIO_OK;
    }
    return MDIO_NOK;
}

/** @} */ // end of DIO_Public_Functions group

#include "bit_math.h"
#include "stdtypes.h"
#include "DIO.h"
#include "DIO_Private.h"

MDIO_enuErrorStatus_t MDIO_enuSetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, MDIO_enuPinConfiguration_t Copy_enuPinConfiguration)
{
    // Validate pin number
    if (Copy_enuPinNum > MDIO_PIN7)
    {
        return MDIO_INVALID_PIN; // Return error if pin number is invalid
    }

    // Validate port number
    if (Copy_enuPortNum > MDIO_PORTD)
    {
        return MDIO_INVALID_PORT; // Return error if port number is invalid
    }

    // Set or clear the pin based on the configuration
    switch (Copy_enuPinConfiguration)
    {
        case MDIO_HIGH:
            switch (Copy_enuPortNum)
            {
                case MDIO_PORTA: SET_BIT(PORTA_REG, Copy_enuPinNum); break;
                case MDIO_PORTB: SET_BIT(PORTB_REG, Copy_enuPinNum); break;
                case MDIO_PORTC: SET_BIT(PORTC_REG, Copy_enuPinNum); break;
                case MDIO_PORTD: SET_BIT(PORTD_REG, Copy_enuPinNum); break;
                default: return MDIO_NOK; // This should never happen due to the validation above
            }
            break;

        case MDIO_LOW:
            switch (Copy_enuPortNum)
            {
                case MDIO_PORTA: CLR_BIT(PORTA_REG, Copy_enuPinNum); break;
                case MDIO_PORTB: CLR_BIT(PORTB_REG, Copy_enuPinNum); break;
                case MDIO_PORTC: CLR_BIT(PORTC_REG, Copy_enuPinNum); break;
                case MDIO_PORTD: CLR_BIT(PORTD_REG, Copy_enuPinNum); break;
                default: return MDIO_NOK; // This should never happen due to the validation above
            }
            break;

        default:
            return MDIO_NOK; // Invalid configuration
    }

    return MDIO_OK; // Success
}

// Function to configure a pin direction
MDIO_enuErrorStatus_t MDIO_enuSetPinConfigration(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, u8 Copy_u8PinDir)
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
                    case MDIO_PORTA: CLR_BIT(DDRA_REG, Copy_enuPinNum); CLR_BIT(PORTA_REG, Copy_enuPinNum); break;
                    case MDIO_PORTB: CLR_BIT(DDRB_REG, Copy_enuPinNum); CLR_BIT(PORTB_REG, Copy_enuPinNum); break;
                    case MDIO_PORTC: CLR_BIT(DDRC_REG, Copy_enuPinNum); CLR_BIT(PORTC_REG, Copy_enuPinNum); break;
                    case MDIO_PORTD: CLR_BIT(DDRD_REG, Copy_enuPinNum); CLR_BIT(PORTD_REG, Copy_enuPinNum); break;
                    default: return MDIO_NOK;
                }
                break;

            case MDIO_INPUT_PULLUP:
                switch (Copy_enuPortNum)
                {
                    case MDIO_PORTA: CLR_BIT(DDRA_REG, Copy_enuPinNum); SET_BIT(PORTA_REG, Copy_enuPinNum); break;
                    case MDIO_PORTB: CLR_BIT(DDRB_REG, Copy_enuPinNum); SET_BIT(PORTB_REG, Copy_enuPinNum); break;
                    case MDIO_PORTC: CLR_BIT(DDRC_REG, Copy_enuPinNum); SET_BIT(PORTC_REG, Copy_enuPinNum); break;
                    case MDIO_PORTD: CLR_BIT(DDRD_REG, Copy_enuPinNum); SET_BIT(PORTD_REG, Copy_enuPinNum); break;
                    default: return MDIO_NOK;
                }
                break;

            default:
                return MDIO_NOK;
        }
        return MDIO_OK;
    
    return MDIO_NOK;
}

// Function to set the value of a whole port
MDIO_enuErrorStatus_t MDIO_enuSetPortValue(MDIO_enuPortNum_t Copy_enuPortNum, u8 Copy_u8PortValue)
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

// Function to configure the direction of a whole port
MDIO_enuErrorStatus_t MDIO_enuSetPortDir(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPortConfiguration_t Copy_enuPortConfiguration)
{
    // support input, output and input pull-up for the whole port or high/low nibble
    if (Copy_enuPortNum <= MDIO_PORTD)
    {
        switch (Copy_enuPortConfiguration)
        {
        case MDIO_PORT_INPUT:
            switch (Copy_enuPortNum)    
            {
            case MDIO_PORTA:
                DDRA_REG = 0x00;
                break;
            case MDIO_PORTB:
                DDRB_REG = 0x00;
                break;
            case MDIO_PORTC:
                DDRC_REG = 0x00;
                break;
            case MDIO_PORTD:
                DDRD_REG = 0x00;
                break;
            default:
                return MDIO_NOK;
            }
            break;

        case MDIO_PORT_OUTPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA:
                DDRA_REG = 0xFF;
                break;
            case MDIO_PORTB:
                DDRB_REG = 0xFF;
                break;
            case MDIO_PORTC:
                DDRC_REG = 0xFF;
                break;
            case MDIO_PORTD:
                DDRD_REG = 0xFF;
                break;
            default:
                return MDIO_NOK;
            }
            break;

        case MDIO_PORT_HIGH_NIBBLE_INPUP_PULL_UP_LOW_NIBBLE_OUTPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA:
                DDRA_REG = 0x0F;
                PORTA_REG = 0xF0;
                break;
            case MDIO_PORTB:
                DDRB_REG = 0x0F;
                PORTB_REG = 0xF0;
                break;
            case MDIO_PORTC:
                DDRC_REG = 0x0F;
                PORTC_REG = 0xF0;
                break;
            case MDIO_PORTD:
                DDRD_REG = 0x0F;
                PORTD_REG = 0xF0;
                break;
            default:
                return MDIO_NOK;
            }
            break;

        case MDIO_PORT_LOW_NIBBLE_INPUT_PULL_UP_HIGH_NIBBLE_OUTPUT:
            switch (Copy_enuPortNum)
            {
            case MDIO_PORTA:
                DDRA_REG = 0xF0;
                PORTA_REG = 0x0F;
                break;
            case MDIO_PORTB:
                DDRB_REG = 0xF0;
                PORTB_REG = 0x0F;
                break;
            case MDIO_PORTC:
                DDRC_REG = 0xF0;
                PORTC_REG = 0x0F;
                break;
            case MDIO_PORTD:
                DDRD_REG = 0xF0;
                PORTD_REG = 0x0F;
                break;
            default:
                return MDIO_NOK;
            }
            break;
        }
        return MDIO_OK;
    }
    return MDIO_NOK;
}


// Function to get the value of a pin
MDIO_enuErrorStatus_t MDIO_enuGetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, u8* Add_pu8PinValue)
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


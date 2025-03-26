# MCAL: DIO Module Library

## List of Supported Functions
```c
void MDIO_voidInit(void);
MDIO_enuErrorStatus_t MDIO_enuSetPinConfigration(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, MDIO_enuPinConfig_t Copy_enuPinConfigration);
MDIO_enuErrorStatus_t MDIO_enuSetPortConfigration(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPortConfig_t Copy_enuPortConfigration);
MDIO_enuErrorStatus_t MDIO_enuSetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, MDIO_enuPinState_t Copy_enuPinState);
MDIO_enuErrorStatus_t MDIO_enuSetPortValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPortState_t Copy_enuPortState);
MDIO_enuErrorStatus_t MDIO_enuGetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, MDIO_enuPinNum_t Copy_enuPinNum, uint8_t* Add_uint8PtrPinValue);
```

## Functions Sequence Diagrams

### `MDIO_voidInit` 
![text](SequenceDiagrams/MDIO_voidInit.png)

### `MDIO_enuSetPinConfigration` 
![text](SequenceDiagrams/MDIO_enuSetPinConfigration.png) 

### `MDIO_enuSetPortConfigration`
![text](SequenceDiagrams/MDIO_enuSetPortConfigration.png) 

### `MDIO_enuSetPinValue` 
![text](SequenceDiagrams/MDIO_enuSetPinValue.png) 

### `MDIO_enuSetPortValue` 
![text](SequenceDiagrams/MDIO_enuSetPortValue.png)

### `MDIO_enuGetPinValue`
![text](SequenceDiagrams/MDIO_enuGetPinValue.png) 


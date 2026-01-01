/**
 ****************************************************************************
 * @file		stm32_SX1276_spi.h
 * @author		
 * @brief		Header for SX1276 LoRa module driver.
 ****************************************************************************
 */


#ifndef SX1276_DRIVER_STM32_SX1276_SPI_H_
#define SX1276_DRIVER_STM32_SX1276_SPI_H_

/**
 * **************************************************
 * SX1276 Register Map
 * **************************************************
 */
#define SX1276_LoRa_REG_FIFO 					0x0
#define SX1276_LoRa_REG_OP_MODE 				0x1
#define SX1276_LoRa_REG_FR_MSB 					0x6
#define SX1276_LoRa_REG_FR_MID 					0x7
#define SX1276_LoRa_REG_FR_LSB 					0x8
#define SX1276_LoRa_REG_PA_CONFIG 				0x9
#define SX1276_LoRa_REG_PA_RAMP 				0xA
#define SX1276_LoRa_REG_OCP 					0xB
#define SX1276_LoRa_REG_LNA 					0xC
#define SX1276_LoRa_REG_FIFO_ADDR_PTR 			0xD
#define SX1276_LoRa_REG_FIFO_TX_BASE_ADDR 		0xE
#define SX1276_LoRa_REG_FIFO_RX_BASE_ADDR 		0xF
#define SX1276_LoRa_REG_FIFO_RX_CURRENT_ADDR 	0x10
#define SX1276_LoRa_REG_IRQ_FLAGS_MASK 			0x11
#define SX1276_LoRa_REG_IRQ_FLAGS 				0x12
#define SX1276_LoRa_REG_RX_NB_BYTES 			0x13
#define SX1276_LoRa_REG_RX_HEADER_CNT_VALUE_MSB	0x14
#define SX1276_LoRa_REG_RX_HEADER_CNT_VALUE_LSB	0x15
#define SX1276_LoRa_REG_RX_PACKET_CNT_VALUE_MSB	0x16
#define SX1276_LoRa_REG_RX_PACKET_CNT_VALUE_LSB	0x17
#define SX1276_LoRa_REG_MODEM_STAT 				0x18
#define SX1276_LoRa_REG_PKT_SNR_VALUE 			0x19
#define SX1276_LoRa_REG_PKT_RSSI_VALUE 			0x1A
#define SX1276_LoRa_REG_RSSI_VALUE 				0x1B
#define SX1276_LoRa_REG_HOP_CHANNEL 			0x1C
#define SX1276_LoRa_REG_MODEM_CONFIG_1 			0x1D
#define SX1276_LoRa_REG_MODEM_CONFIG_2 			0x1E
#define SX1276_LoRa_REG_SYMB_TIMEOUT_LSB 		0x1F
#define SX1276_LoRa_REG_SYMB_PREAMBLE_MSB 		0x20
#define SX1276_LoRa_REG_SYMB_PREAMBLE_LSB 		0x21
#define SX1276_LoRa_REG_PAYLOAD_LENGTH 			0x22
#define SX1276_LoRa_REG_MAX_PAYLOAD_LENGTH 		0x23
#define SX1276_LoRa_REG_HOP_PERIOD 				0x24
#define SX1276_LoRa_REG_FIFO_RX_BYTE_ADDR 		0x25
#define SX1276_LoRa_REG_MODEM_CONFIG_3 			0x26
#define SX1276_LoRa_REG_PPM_CORRECTION 			0x27
#define SX1276_LoRa_REG_FEI_MSB 				0x28
#define SX1276_LoRa_REG_FEI_MID 				0x29
#define SX1276_LoRa_REG_FEI_LSB 				0x2A
#define SX1276_LoRa_REG_RSSI_WIDEBAND 			0x2C
#define SX1276_LoRa_REG_DETECT_OPTIMIZE 		0x31
#define SX1276_LoRa_REG_INVERT_IQ 				0x33
#define SX1276_LoRa_REG_DETECTION_THRSEHOLD 	0x37
#define SX1276_LoRa_REG_SYNC_WORD 				0x39

/**
 * **************************************************
 * Register - RegOpMode (0x01)
 * --------------------------------------------------
 * @notes
 * 	- Defines are coded to run SX1276 in LoRa mode ONLY.
 * **************************************************
 */

#define SX1276_LoRa_REG_OP_MODE_HIGH_FREQUENCY_MODE		0x80	// High Frequency Mode (access to HF test registers)
#define SX1276_LoRa_REG_OP_MODE_LOW_FREQUENCY_MODE		0x88	// Low Frequency Mode (access to LF test registers)

#define SX1276_LoRa_REG_OP_MODE_SLEEP 					0x80
#define SX1276_LoRa_REG_OP_MODE_STDBY 					0x81
#define SX1276_LoRa_REG_OP_MODE_FSTX 					0x82
#define SX1276_LoRa_REG_OP_MODE_TX 						0x83
#define SX1276_LoRa_REG_OP_MODE_FSRX 					0x84
#define SX1276_LoRa_REG_OP_MODE_RXCONT 					0x85
#define SX1276_LoRa_REG_OP_MODE_RXSINGLE 				0x86
#define SX1276_LoRa_REG_OP_MODE_CAD 					0x87

/**
 * **************************************************
 * Register - RegPaConfig (0x09)
 * --------------------------------------------------
 * @notes
 * 	- Defines are coded with PaConfig = 1 (for NiceRF board)
 * **************************************************
 */

#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_2 			0xF0
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_3 			0xF1
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_4 			0xF2
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_5 			0xF3
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_6 			0xF4
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_7 			0xF5
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_8 			0xF6
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_9 			0xF7
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_11 			0xF9
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_12 			0xFA
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_13 			0xFB
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_14 			0xFC
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_15 			0xFD
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_16 			0xFE
#define SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17 			0xFF

/**
 * **************************************************
 * Register - RegModemConfig1 (0x1D)
 * --------------------------------------------------
 * @notes
 * 	- Defines are coded for Explicit Mode Only
 * **************************************************
 */
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_7_8_kHz 		0x0
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_10_4_kHz 		0x10
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_15_6_kHz 		0x20
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_20_8_kHz 		0x30
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_31_25_kHz 		0x40
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_41_7_kHz 		0x50
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_62_5_kHz 		0x60
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_125_kHz 		0x70
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_250_kHz 		0x80
#define SX1276_LoRa_REG_MODEM_CONFIG1_BW_500_kHz 		0x90

#define SX1276_LoRa_REG_MODEM_CONFIG1_CR_4_5 			0x2
#define SX1276_LoRa_REG_MODEM_CONFIG1_CR_4_6 			0x4
#define SX1276_LoRa_REG_MODEM_CONFIG1_CR_4_7 			0x6
#define SX1276_LoRa_REG_MODEM_CONFIG1_CR_4_8 			0x8

/**
 * **************************************************
 * Register - RegModemConfig1 (0x1D)
 * --------------------------------------------------
 * @notes
 * 	- Defines are coded for Explicit Mode Only
 * 	- Single packet mode only
 * 	- CRC is disabled
 * 	- RX timeout MSB is 0
 * **************************************************
 */

#define SX1276_LoRa_REG_MODEM_CONFIG2_SF6 				0x60
#define SX1276_LoRa_REG_MODEM_CONFIG2_SF7 				0x70
#define SX1276_LoRa_REG_MODEM_CONFIG2_SF8 				0x80
#define SX1276_LoRa_REG_MODEM_CONFIG2_SF9 				0x90
#define SX1276_LoRa_REG_MODEM_CONFIG2_SF10 				0xA0
#define SX1276_LoRa_REG_MODEM_CONFIG2_SF11 				0xB0
#define SX1276_LoRa_REG_MODEM_CONFIG2_SF12 				0xC0
/**
 * **************************************************
 * typedefs
 * **************************************************
 */
typedef enum
{
	SX1276_OK       			= 0x00,
	SX1276_ERROR    			= 0x01,
	SX1276_BUSY     			= 0x02,
	SX1276_TIMEOUT  			= 0x03,
	SX1276_PROGRAM_ERROR 		= 0x04,
	SX1276_SPI_ERROR			= 0x05,
	SX1276_RX_FLAG_CRC_ERROR	= 0x06,
	SX1276_RX_NOTHING_RECEIVED	= 0x07,
	SX1276_NC_ERROR				= 0x08
} SX1276_StatusTypeDef;

typedef enum
{
	CadDetected				= 0x00,
	FhssChangeChannel		= 0x01,
	CadDone					= 0x02,
	TxDone 					= 0x03,
	ValidHeader				= 0x04,
	PayloadCrcError     	= 0x05,
	RxDone		    		= 0x06,
	RxTimeout       		= 0x07
} SX1276_IRQFlags;

typedef enum
{
	NORMAL_MODE_MCU_STOP_MODE	= 0x00,
	SYNC_MODE_MCU_HAL_DELAY		= 0x01
} SX1276_DelayType;

/**
  * @brief  SPI Configuration Structure definition
  */
typedef struct
{
	SPI_HandleTypeDef *hspi;
	GPIO_TypeDef *NSS_GPIOx;
	uint16_t NSS_GPIO_Pin;
	RTC_HandleTypeDef *hrtc_l;
} SPI_SX1276_TypeDef;


/*
 * @brief	Initialization structure definition
 *
 */
typedef struct
{
	GPIO_TypeDef *NRESET_GPIOx;
	uint16_t NRESET_GPIO_Pin;
	SPI_SX1276_TypeDef SPI_SX1276;
	RTC_HandleTypeDef *hrtc_l;

} SX1276_Init_Param;

/*
 * @brief	Transmit Parameters definition
 */
typedef struct
{
	uint8_t Bandwidth;
	uint8_t CodingRate;
	uint8_t Spreadfactor;
	uint16_t PreambleLength;

}SX1276_CommParam;



extern void SystemClock_Config(void);


/**
 * **************************************************
 * Functions
 * **************************************************
 */
SX1276_StatusTypeDef SX1276_Init(SX1276_Init_Param h_SX1276);
SX1276_StatusTypeDef SX1276_SPI_Write(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pData, uint16_t DataLength);
SX1276_StatusTypeDef SX1276_SPI_Read(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pData, uint16_t DataLength);
SX1276_StatusTypeDef SX1276_SetOperationMode(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t Mode);
SX1276_StatusTypeDef SX1276_SetLoRaFrequency(SPI_SX1276_TypeDef hSPI_SX1276, uint32_t LoRaFrequnecy_KHz);
SX1276_StatusTypeDef SX1276_Set_FIFO_Buffer(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pDataTx, uint8_t TxDataLength);
SX1276_StatusTypeDef SX1276_Get_FIFO_Buffer(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pDataRx, uint8_t *ptr_vardat_SX1276_LoRa_REG_RX_NB_BYTES);

SX1276_StatusTypeDef SX1276_ClearIRQFlags(SPI_SX1276_TypeDef hSPI_SX1276, SX1276_IRQFlags Flag);
SX1276_StatusTypeDef SX1276_ClearAllIRQFlags(SPI_SX1276_TypeDef hSPI_SX1276);
SX1276_StatusTypeDef SX1276_GetIRQFlags(SPI_SX1276_TypeDef hSPI_SX1276, SX1276_IRQFlags Flag, uint8_t *ptrFlagData);
SX1276_StatusTypeDef SX1276_SetLoRaCommunicationParameter(SPI_SX1276_TypeDef hSPI_SX1276, SX1276_CommParam sCommParam);

SX1276_StatusTypeDef SX1276_Transmit(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataTx,
		uint8_t TxDataLength,
		uint32_t LoRaFrequnecy_KHz,
		uint8_t TxPower,
		SX1276_DelayType delay,
		SX1276_CommParam sCommParam);



SX1276_StatusTypeDef SX1276_ReceiveSingle(SPI_SX1276_TypeDef hSPI_SX1276,
		uint32_t LoRaFrequnecy_KHz,
		uint16_t Timeout_ms,
		SX1276_CommParam sCommParam,
		uint8_t *pDataRx,
		SX1276_DelayType delay,
		uint8_t *LengthOfDataReceived);

SX1276_StatusTypeDef SX1276_ReceiveContineousSingleUse(SPI_SX1276_TypeDef hSPI_SX1276,
		uint32_t LoRaFrequnecy_KHz,
		uint16_t Timeout_ms,
		SX1276_CommParam sCommParam,
		uint8_t *pDataRx,
		uint8_t *LengthOfDataReceived,
		SX1276_DelayType delay,
		uint32_t PostSleepDelay_ms);

/*Multi-LoRa transmit function*/
SX1276_StatusTypeDef SX1276_Multi_Transmitter_SetConfiguration(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataTx,
		uint8_t TxDataLength,
		uint32_t LoRaFrequnecy_KHz,
		uint8_t TxPower,
		SX1276_CommParam sCommParam);

SX1276_StatusTypeDef SX1276_Multi_Transmitter_Transmit(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t TxDataLength,
		SX1276_DelayType delay,
		SX1276_CommParam sCommParam);

SX1276_StatusTypeDef SX1276_Multi_Receiver_Contineous_SetConfiguration(SPI_SX1276_TypeDef hSPI_SX1276,
		uint32_t LoRaFrequnecy_KHz,
		SX1276_CommParam sCommParam);

SX1276_StatusTypeDef SX1276_Multi_Receiver_Contineous_Receive(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataRx,
		uint8_t *LengthOfDataReceived);

SX1276_StatusTypeDef SX1276_Transmitter_RunSyncSchedule(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataTx,
		uint8_t *pDataRx,
		uint8_t TxDataLength,
		uint8_t *LengthOfDataReceived,
		uint32_t LoRaFrequnecy_KHz,
		uint8_t TxPower,
		float *ConfigRxTime,
		float *ReceivePropDelay,
		SX1276_CommParam sCommParam,
		TIM_HandleTypeDef *htim,
		uint32_t TIM_PRESCALR);

SX1276_StatusTypeDef SX1276_Receiver_RunSyncSchedule(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataTx,
		uint8_t *pDataRx,
		uint8_t TxDataLength,
		uint8_t *LengthOfDataReceived,
		uint32_t LoRaFrequnecy_KHz,
		uint8_t TxPower,
		SX1276_CommParam sCommParam);


float getBandwidth(uint8_t regVal);
uint32_t ComputeToA(SX1276_CommParam sCommParam, uint8_t TxDataLength);
uint32_t GetTimerClkFromHW(void);
float measure_function_time_us(TIM_HandleTypeDef *htim, uint32_t TIM_PRESCALR);
void MCU_STOP_Delay_ms(uint32_t ms, RTC_HandleTypeDef *hrtc_loc);

SX1276_StatusTypeDef SX1276_SetRxTimeout(SPI_SX1276_TypeDef hSPI_SX1276, uint16_t Timeout_ms, SX1276_CommParam sCommParam);

#endif /* SX1276_DRIVER_STM32_SX1276_SPI_H_ */

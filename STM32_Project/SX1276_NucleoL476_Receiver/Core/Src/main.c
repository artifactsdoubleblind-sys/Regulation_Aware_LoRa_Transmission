/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32_SX1276_spi.h"
#include "string.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
# define NUMBER_OF_LORA_NODES			0x05

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t TIM16_PRESCALR=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */
	uint8_t buff[1000]={0};
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_SPI2_Init();
	MX_RTC_Init();
	MX_TIM16_Init();
	/* USER CODE BEGIN 2 */

	SPI_SX1276_TypeDef test, test1, test2, test3, test4;
	SX1276_Init_Param init, init1, init2, init3, init4;


	if(NUMBER_OF_LORA_NODES >=1)
	{
		test.NSS_GPIOx = SPI2_SX1276_NSS_S0_GPIO_Port;
		test.NSS_GPIO_Pin = SPI2_SX1276_NSS_S0_Pin;
		test.hspi = &hspi2;
		test.hrtc_l = &hrtc;

		init.NRESET_GPIOx = SX1276_NRESET_GPIO_Port;
		init.NRESET_GPIO_Pin = SX1276_NRESET_Pin;
		init.SPI_SX1276 = test;
		init.hrtc_l = &hrtc;
	}

	if(NUMBER_OF_LORA_NODES >=2)
	{
		test1.NSS_GPIOx = SPI2_SX1276_NSS_S1_GPIO_Port;
		test1.NSS_GPIO_Pin = SPI2_SX1276_NSS_S1_Pin;
		test1.hspi = &hspi2;
		test1.hrtc_l = &hrtc;

		init1.NRESET_GPIOx = SX1276_NRESET_GPIO_Port;
		init1.NRESET_GPIO_Pin = SX1276_NRESET_Pin;
		init1.SPI_SX1276 = test1;
		init1.hrtc_l = &hrtc;
	}

	if(NUMBER_OF_LORA_NODES >=3)
	{
		test2.NSS_GPIOx = SPI2_SX1276_NSS_S2_GPIO_Port;
		test2.NSS_GPIO_Pin = SPI2_SX1276_NSS_S2_Pin;
		test2.hspi = &hspi2;
		test2.hrtc_l = &hrtc;

		init2.NRESET_GPIOx = SX1276_NRESET_GPIO_Port;
		init2.NRESET_GPIO_Pin = SX1276_NRESET_Pin;
		init2.SPI_SX1276 = test2;
		init2.hrtc_l = &hrtc;
	}

	if(NUMBER_OF_LORA_NODES >=4)
	{
		test3.NSS_GPIOx = SPI2_SX1276_NSS_S3_GPIO_Port;
		test3.NSS_GPIO_Pin = SPI2_SX1276_NSS_S3_Pin;
		test3.hspi = &hspi2;
		test3.hrtc_l = &hrtc;

		init3.NRESET_GPIOx = SX1276_NRESET_GPIO_Port;
		init3.NRESET_GPIO_Pin = SX1276_NRESET_Pin;
		init3.SPI_SX1276 = test3;
		init3.hrtc_l = &hrtc;
	}

	if(NUMBER_OF_LORA_NODES >=5)
	{
		test4.NSS_GPIOx = SPI2_SX1276_NSS_S4_GPIO_Port;
		test4.NSS_GPIO_Pin = SPI2_SX1276_NSS_S4_Pin;
		test4.hspi = &hspi2;
		test4.hrtc_l = &hrtc;

		init4.NRESET_GPIOx = SX1276_NRESET_GPIO_Port;
		init4.NRESET_GPIO_Pin = SX1276_NRESET_Pin;
		init4.SPI_SX1276 = test4;
		init4.hrtc_l = &hrtc;
	}

	//---------------------------------------------
	SX1276_CommParam CommParam;
	CommParam.Bandwidth = SX1276_LoRa_REG_MODEM_CONFIG1_BW_125_kHz;
	CommParam.CodingRate= SX1276_LoRa_REG_MODEM_CONFIG1_CR_4_5;
	CommParam.Spreadfactor = SX1276_LoRa_REG_MODEM_CONFIG2_SF7;
	CommParam.PreambleLength = 8;

	uint8_t buffLoRa[255]={0};
	uint8_t buffLoRaR[255]={0};

	sprintf((char *)buffLoRa, " 123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.123456789.");
	buffLoRa[0] = 1;

	uint8_t count=0;

	uint8_t PacketSize_bytes=240;
	uint8_t PacketSize_bytesR=0;
	//HAL_Delay(1000);



	buffLoRa[1] = count;

	sprintf((char *)buff, "SX1276 Init \r\n");
	HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);



	uint8_t AvailableLoRaNodes=0xFF;



	/****************************************************
	 * Init Code
	 ****************************************************/
	// Node 0-----------------------------------------------------
	if(NUMBER_OF_LORA_NODES >=1)
	{
		if(SX1276_Init(init)!=SX1276_OK)
		{
			sprintf((char *)buff, "SX1276 Init 0: FAIL \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}
		else
		{
			AvailableLoRaNodes &= 0b11111110;
			sprintf((char *)buff, "SX1276 Init 0: OK \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

	}

	// Node 1-----------------------------------------------------
	if(NUMBER_OF_LORA_NODES >=2)
	{
		if(SX1276_Init(init1)!=SX1276_OK)
		{
			sprintf((char *)buff, "SX1276 Init 1: FAIL \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}
		else
		{
			AvailableLoRaNodes &= 0b11111101;
			sprintf((char *)buff, "SX1276 Init 1: OK \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

	}

	// Node 2-----------------------------------------------------
	if(NUMBER_OF_LORA_NODES >=3)
	{
		if(SX1276_Init(init2)!=SX1276_OK)
		{
			sprintf((char *)buff, "SX1276 Init 2: FAIL \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}
		else
		{
			AvailableLoRaNodes &= 0b11111011;
			sprintf((char *)buff, "SX1276 Init 2: OK \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

	}

	// Node 3-----------------------------------------------------
	if(NUMBER_OF_LORA_NODES >=4)
	{
		if(SX1276_Init(init3)!=SX1276_OK)
		{
			sprintf((char *)buff, "SX1276 Init 3: FAIL \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}
		else
		{
			AvailableLoRaNodes &= 0b11110111;
			sprintf((char *)buff, "SX1276 Init 3: OK \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

	}

	// Node 4-----------------------------------------------------
	if(NUMBER_OF_LORA_NODES >=5)
	{
		if(SX1276_Init(init4)!=SX1276_OK)
		{
			sprintf((char *)buff, "SX1276 Init 4: FAIL \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}
		else
		{
			AvailableLoRaNodes &= 0b11101111;
			sprintf((char *)buff, "SX1276 Init 4: OK \r\n");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

	}

	HAL_GPIO_WritePin(SX1276_NRESET_GPIO_Port, SX1276_NRESET_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(SX1276_NRESET_GPIO_Port, SX1276_NRESET_Pin, GPIO_PIN_SET);
	HAL_Delay(100);


	/****************************************************
	 * Dummy Transmit
	 ***************************************************/

	// Check Number of LoRa Nodes set and if the node is available
	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		sprintf((char *)buff, "SX1276 Dummy Node 0 Transmit : %u\r\n", SX1276_Transmit(test, buffLoRa, PacketSize_bytes, 906000, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, NORMAL_MODE_MCU_STOP_MODE, CommParam));
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		sprintf((char *)buff, "SX1276 Dummy Node 1 Transmit : %u\r\n", SX1276_Transmit(test1, buffLoRa, PacketSize_bytes, 906000, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, NORMAL_MODE_MCU_STOP_MODE, CommParam));
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		sprintf((char *)buff, "SX1276 Dummy Node 2 Transmit : %u\r\n", SX1276_Transmit(test2, buffLoRa, PacketSize_bytes, 906000, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, NORMAL_MODE_MCU_STOP_MODE, CommParam));
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		sprintf((char *)buff, "SX1276 Dummy Node 3 Transmit : %u\r\n", SX1276_Transmit(test3, buffLoRa, PacketSize_bytes, 906000, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, NORMAL_MODE_MCU_STOP_MODE, CommParam));
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		sprintf((char *)buff, "SX1276 Dummy Node 4 Transmit : %u\r\n", SX1276_Transmit(test4, buffLoRa, PacketSize_bytes, 906000, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, NORMAL_MODE_MCU_STOP_MODE, CommParam));
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}


	/*****************************************************************************************************
	 * Run Sync Schedule
	 * 	-	Set seperately from dummy code so that the dummy code can commented out easily.
	 *	-	The node availability check logic shall be the same.
	 *****************************************************************************************************/

	uint32_t LoRaFrequnecy_KHz = 915000;

	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{

		if(SX1276_Receiver_RunSyncSchedule(test, buffLoRa, buffLoRaR, PacketSize_bytes, &PacketSize_bytesR, LoRaFrequnecy_KHz, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, CommParam)!=SX1276_OK)
		{
			strcpy((char *)buff, "Run Sync Node 0; 1; failed");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

		if(SX1276_Receiver_RunSyncSchedule(test, buffLoRa, buffLoRaR, PacketSize_bytes, &PacketSize_bytesR, LoRaFrequnecy_KHz, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, CommParam)!=SX1276_OK)
		{
			strcpy((char *)buff, "Run Sync Node 0; 1; failed");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

		if(SX1276_Receiver_RunSyncSchedule(test, buffLoRa, buffLoRaR, PacketSize_bytes, &PacketSize_bytesR, LoRaFrequnecy_KHz, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, CommParam)!=SX1276_OK)
		{
			strcpy((char *)buff, "Run Sync Node 0; 1; failed");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

		if(SX1276_Receiver_RunSyncSchedule(test, buffLoRa, buffLoRaR, PacketSize_bytes, &PacketSize_bytesR, LoRaFrequnecy_KHz, SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17, CommParam)!=SX1276_OK)
		{
			strcpy((char *)buff, "Run Sync Node 0; 1; failed");
			HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		}

		sprintf((char *)buff, "SX1276 Node 0 Sync Schedule Complete.\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}


	/*********************************************************************************
	 * Receiver SetConfig
	 *********************************************************************************/
	LoRaFrequnecy_KHz = 905000;
	uint32_t  HoppFrequencyMultiplier = 250;	//relates to displacement of signal in Hz; Shall be fixed on the basis of bandwidth and spread factor
	uint8_t Status;

	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*0, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 0 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*1, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 1 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*2, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 2 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*3, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 3 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*4, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 4 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	/********************************************************
	 * Get data
	 ********************************************************/
	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 0 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);

	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test1, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 1 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test2, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 2 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test3, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 3 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test4, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 4 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	//Hopp-------------------------------------------------------------------------
	/*********************************************************************************
	 * Receiver SetConfig
	 *********************************************************************************/
	LoRaFrequnecy_KHz = 905125;

	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*0, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 0 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*1, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 1 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*2, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 2 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*3, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 3 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*4, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 4 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	/********************************************************
	 * Get data
	 ********************************************************/
	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 0 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);

	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test1, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 1 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test2, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 2 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test3, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 3 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test4, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 4 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}


	//Hopp-------------------------------------------------------------------------
	/*********************************************************************************
	 * Receiver SetConfig
	 *********************************************************************************/
	LoRaFrequnecy_KHz = 906250;

	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*0, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 0 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*1, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 1 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*2, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 2 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*3, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 3 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*4, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 4 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	/********************************************************
	 * Get data
	 ********************************************************/
	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 0 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);

	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test1, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 1 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test2, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 2 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test3, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 3 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test4, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 4 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}


	//Hopp-------------------------------------------------------------------------
	/*********************************************************************************
	 * Receiver SetConfig
	 *********************************************************************************/
	LoRaFrequnecy_KHz = 906375;

	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*0, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 0 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*1, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 1 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*2, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 2 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*3, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 3 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*4, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 4 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	/********************************************************
	 * Get data
	 ********************************************************/
	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 0 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);

	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test1, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 1 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test2, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 2 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test3, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 3 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test4, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 4 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}


	//Hopp-------------------------------------------------------------------------
	/*********************************************************************************
	 * Receiver SetConfig
	 *********************************************************************************/
	LoRaFrequnecy_KHz = 907500;

	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*0, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 0 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*1, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 1 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*2, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 2 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*3, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 3 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_SetConfiguration(test, LoRaFrequnecy_KHz+HoppFrequencyMultiplier*4, CommParam);
		sprintf((char *)buff, "SX1276 Receiver Node 4 SetConfig : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
	}

	/********************************************************
	 * Get data
	 ********************************************************/
	if( (NUMBER_OF_LORA_NODES >=1) && (AvailableLoRaNodes && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 0 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);

	}

	if( (NUMBER_OF_LORA_NODES >=2) && ((AvailableLoRaNodes>>1) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test1, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 1 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=3) && ((AvailableLoRaNodes>>2) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test2, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 2 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=4) && ((AvailableLoRaNodes>>3) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test3, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 3 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}

	if( (NUMBER_OF_LORA_NODES >=5) && ((AvailableLoRaNodes>>4) && 0b00000001) )
	{
		Status = SX1276_Multi_Receiver_Contineous_Receive(test4, buffLoRaR, &PacketSize_bytesR);
		sprintf((char *)buff, "SX1276 Receiver Node 4 Data Received : %u\r\n", Status);
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);

		// Print data
		strcpy((char *)buff, "\r\n");
		HAL_UART_Transmit(&huart2, buff, sizeof(buff), 100);
		HAL_UART_Transmit(&huart2, buffLoRaR, sizeof(buffLoRaR), 100);
	}


	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 10;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */

	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void)
{

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 7;
	hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}

/**
 * @brief TIM16 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM16_Init(void)
{

	/* USER CODE BEGIN TIM16_Init 0 */

	/* USER CODE END TIM16_Init 0 */

	/* USER CODE BEGIN TIM16_Init 1 */

	/* USER CODE END TIM16_Init 1 */
	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 4000;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = 65535;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM16_Init 2 */
	TIM16_PRESCALR = htim16.Init.Prescaler;
	/* USER CODE END TIM16_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, Temp_Pin_Pin|SPI2_SX1276_NSS_S4_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, SPI2_SX1276_NSS_S0_Pin|SX1276_NRESET_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, SPI2_SX1276_NSS_S1_Pin|SPI2_SX1276_NSS_S2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SPI2_SX1276_NSS_S3_GPIO_Port, SPI2_SX1276_NSS_S3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : Temp_Pin_Pin SPI2_SX1276_NSS_S4_Pin */
	GPIO_InitStruct.Pin = Temp_Pin_Pin|SPI2_SX1276_NSS_S4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : SPI2_SX1276_NSS_S0_Pin SPI2_SX1276_NSS_S1_Pin SPI2_SX1276_NSS_S2_Pin SX1276_NRESET_Pin */
	GPIO_InitStruct.Pin = SPI2_SX1276_NSS_S0_Pin|SPI2_SX1276_NSS_S1_Pin|SPI2_SX1276_NSS_S2_Pin|SX1276_NRESET_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI2_SX1276_NSS_S3_Pin */
	GPIO_InitStruct.Pin = SPI2_SX1276_NSS_S3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SPI2_SX1276_NSS_S3_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

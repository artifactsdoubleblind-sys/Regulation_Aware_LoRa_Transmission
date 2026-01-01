/**
 ****************************************************************************
 * @file		stm32_SX1276_spi.c
 * @author		
 * @brief		Driver for SX1276 LoRa module.
 ****************************************************************************
 * @notes
 * 				- SPI communication needs to be init in the main function.
 *
 */
#include "main.h"
#include "stdio.h"
#include "string.h"
#include <math.h>
#include "stm32_SX1276_spi.h"

SX1276_StatusTypeDef SX1276_Receiver_RunSyncSchedule(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataTx,
		uint8_t *pDataRx,
		uint8_t TxDataLength,
		uint8_t *LengthOfDataReceived,
		uint32_t LoRaFrequnecy_KHz,
		uint8_t TxPower,
		SX1276_CommParam sCommParam)
{


	SX1276_StatusTypeDef ret;

	// 1) Config Rx
	ret = SX1276_Multi_Receiver_Contineous_SetConfiguration(hSPI_SX1276, LoRaFrequnecy_KHz, sCommParam);

	if(ret !=SX1276_OK)
	{
		return ret;
	}


	// 2) Receive Data; Stay in receiving mode till you hear something
	ret = SX1276_Multi_Receiver_Contineous_Receive(hSPI_SX1276, pDataRx, LengthOfDataReceived);
	if(ret !=SX1276_OK)
	{
		return ret;
	}

	// 3) 100 msec delay
	HAL_Delay(100);

	// 4) Tx Config, ~90ms to execute
	ret = SX1276_Multi_Transmitter_SetConfiguration(hSPI_SX1276, pDataTx, TxDataLength, LoRaFrequnecy_KHz, TxPower, sCommParam);
	if(ret !=SX1276_OK)
	{
		return ret;
	}

	// 5) Transmit data ~funcToA to execute
	ret = SX1276_Multi_Transmitter_Transmit(hSPI_SX1276, TxDataLength, SYNC_MODE_MCU_HAL_DELAY, sCommParam);
	if(ret !=SX1276_OK)
	{
		return ret;
	}

	return SX1276_OK;
}


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
		uint32_t TIM_PRESCALR)
{
	SX1276_StatusTypeDef ret;
	// 1) Tx Config, ~90ms to execute
	ret = SX1276_Multi_Transmitter_SetConfiguration(hSPI_SX1276, pDataTx, TxDataLength, LoRaFrequnecy_KHz, TxPower, sCommParam);
	if(ret !=SX1276_OK)
	{
		return ret;
	}


	// 2) Wait for 100 ms
	MCU_STOP_Delay_ms(100, hSPI_SX1276.hrtc_l);

	// 3) Transmit data ~funcToA to execute
	ret = SX1276_Multi_Transmitter_Transmit(hSPI_SX1276, TxDataLength, SYNC_MODE_MCU_HAL_DELAY, sCommParam);
	if(ret !=SX1276_OK)
	{
		return ret;
	}

	/*
	 * Calculate Config Rx time
	 */
	// Reset counter to zero before starting for easier timing
	__HAL_TIM_SET_COUNTER(htim, 0);

	HAL_TIM_Base_Start(htim);

	ret = SX1276_Multi_Receiver_Contineous_SetConfiguration(hSPI_SX1276, LoRaFrequnecy_KHz, sCommParam);

	if(ret !=SX1276_OK)
	{
		return ret;
	}

	uint32_t end = __HAL_TIM_GET_COUNTER(htim);

	HAL_TIM_Base_Stop(htim);

	*ConfigRxTime= end*(TIM_PRESCALR*1.0*1e3/ GetTimerClkFromHW());
	end=0;

	/*
	 * Calculate Receiving+Prop+delay
	 */
	// Reset counter to zero before starting for easier timing
	__HAL_TIM_SET_COUNTER(htim, 0);

	HAL_TIM_Base_Start(htim);

	ret = SX1276_Multi_Receiver_Contineous_Receive(hSPI_SX1276, pDataRx, LengthOfDataReceived);

	if(ret !=SX1276_OK)
	{
		// Stop the timer and return to main function
		end = __HAL_TIM_GET_COUNTER(htim);

		HAL_TIM_Base_Stop(htim);

		*ReceivePropDelay= -1.0;

		// Deinit the timer
		HAL_TIM_Base_DeInit(htim);

		return ret;
	}

	end = __HAL_TIM_GET_COUNTER(htim);

	HAL_TIM_Base_Stop(htim);

	*ReceivePropDelay= end*(TIM_PRESCALR*1.0*1e3/ GetTimerClkFromHW());

	// Deinit the timer
	HAL_TIM_Base_DeInit(htim);
	return SX1276_OK;
}

SX1276_StatusTypeDef SX1276_Multi_Receiver_Contineous_Receive(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataRx,
		uint8_t *LengthOfDataReceived)
{
	SX1276_StatusTypeDef ret;
	uint32_t counterToTimeoutReceive=0;
	uint8_t pDataTemp[2]={0};
	uint8_t Flags=0;

	do
	{
		counterToTimeoutReceive++;
		pDataTemp[0] = SX1276_LoRa_REG_IRQ_FLAGS;
		pDataTemp[1] = 0;

		if(SX1276_SPI_Read(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
		{
			return SX1276_SPI_ERROR;
		}

		Flags = pDataTemp[1];
		HAL_Delay(1);

		// If the loop ran for more than 10000ms = 10s => timeout
		if(counterToTimeoutReceive>10000)
		{
			return SX1276_RX_NOTHING_RECEIVED;
		}

	}while(!(Flags>>RxDone & 0x01));



	if( 	(Flags>>ValidHeader & 0x01)	&&
			((Flags>>PayloadCrcError  & 0x01) == 0)		)
	{
		pDataTemp[0] = SX1276_LoRa_REG_OP_MODE;
		pDataTemp[1] = 0;


		// Go do the FIFO read thing
		ret = SX1276_Get_FIFO_Buffer(hSPI_SX1276, pDataRx, LengthOfDataReceived);
		if(ret != SX1276_OK)
		{
			return ret;
		}


		//Clear all flags
		ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
		if(ret!=SX1276_OK)
		{
			return ret;
		}

	}
	else
	{
		if(Flags>>PayloadCrcError  & 0x01)
		{
			//Clear all flags
			ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
			if(ret!=SX1276_OK)
			{
				return ret;
			}
			return SX1276_RX_FLAG_CRC_ERROR;
		}
		if(Flags == 0)
		{
			return SX1276_RX_NOTHING_RECEIVED;
		}
	}

	// 9) Set SX1276 to SLEEP
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_SLEEP)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	return SX1276_OK;
}

SX1276_StatusTypeDef SX1276_Multi_Receiver_Contineous_SetConfiguration(SPI_SX1276_TypeDef hSPI_SX1276,
		uint32_t LoRaFrequnecy_KHz,
		SX1276_CommParam sCommParam)
{
	SX1276_StatusTypeDef ret;
	uint8_t pDataTemp[2]={0};
	// 1) Set SX1276 RF Carrier frequency.
	if(SX1276_SetLoRaFrequency(hSPI_SX1276, LoRaFrequnecy_KHz) !=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 2) Set LoRa communication parameter
	ret = SX1276_SetLoRaCommunicationParameter(hSPI_SX1276, sCommParam);
	if(ret!=SX1276_OK)
	{
		return ret;
	}

	// 3) Set SX1276_LoRa_REG_FIFO_RX_BASE_ADDR (RegFifoRxBaseAddr) = 0x00.
	pDataTemp[0] = SX1276_LoRa_REG_FIFO_RX_BASE_ADDR;
	pDataTemp[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}


	// 4) Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
	pDataTemp[0] = SX1276_LoRa_REG_FIFO_ADDR_PTR;
	pDataTemp[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 5) Set SX1276 to RXCONT mode
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_RXCONT)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	return SX1276_OK;
}

/*
 * @brief Receive single data packet using LoRa Modem
 * @notes: As per Section 4.1.6 LoRa Modem State Machine Sequences -> Single Reception Operating Mode (Pg 40 of 132)
 * 			1) Set SX1276 RF Carrier frequency.
 * 			2) Set LoRa communication parameter
 * 			3) Set SX1276_LoRa_REG_FIFO_RX_BASE_ADDR (RegFifoRxBaseAddr) = 0x00.
 *			4) Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
 *			5) Set SX1276 to RXCONT mode
 *			6) Wait for Timeout_ms
 *			8) Logic for extracting data:
 * 				|--- Check ValidHeader==1, RxDone==1, RxTimeout==0, PayloadCrcError== 0
 * 					|---If yes, Read the data from buffer
 * 					|---If no, check RxTimeout==1, return SX1276_TIMEOUT; check PayloadCrcError==1, return SX1276_RX_FLAG_CRC_ERROR
 * 			9) Set SX1276 to SLEEP
 *			10) Wait for PostSleepDelay_ms
 *			## * Discard first byte in the array
 * @param
 * 			SX1276 handle
 * 			LoRa Receive frequency
 * 			Timeout in ms
 * 			Communication parameter struct
 * 			Rx Data buffer pointer
 * 			Length of Data received returned via pointer
 * 			Post sleep Delay in ms
 */
SX1276_StatusTypeDef SX1276_ReceiveContineousSingleUse(SPI_SX1276_TypeDef hSPI_SX1276,
		uint32_t LoRaFrequnecy_KHz,
		uint16_t Timeout_ms,
		SX1276_CommParam sCommParam,
		uint8_t *pDataRx,
		uint8_t *LengthOfDataReceived,
		SX1276_DelayType delay,
		uint32_t PostSleepDelay_ms)
{
	SX1276_StatusTypeDef ret;
	uint8_t pDataTemp[2]={0};
	// 1) Set SX1276 RF Carrier frequency.
	if(SX1276_SetLoRaFrequency(hSPI_SX1276, LoRaFrequnecy_KHz) !=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 2) Set LoRa communication parameter
	ret = SX1276_SetLoRaCommunicationParameter(hSPI_SX1276, sCommParam);
	if(ret!=SX1276_OK)
	{
		return ret;
	}

	// 3) Set SX1276_LoRa_REG_FIFO_RX_BASE_ADDR (RegFifoRxBaseAddr) = 0x00.
	pDataTemp[0] = SX1276_LoRa_REG_FIFO_RX_BASE_ADDR;
	pDataTemp[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}


	// 4) Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
	pDataTemp[0] = SX1276_LoRa_REG_FIFO_ADDR_PTR;
	pDataTemp[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}


	// 5) Set SX1276 to RXCONT mode
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_RXCONT)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}


	uint8_t Flags=0;
	// 7) Wait for Timeout_ms
	if(delay==SYNC_MODE_MCU_HAL_DELAY)
	{
		HAL_Delay(Timeout_ms);
	}
	else
	{
		MCU_STOP_Delay_ms(Timeout_ms, hSPI_SX1276.hrtc_l);
	}


	pDataTemp[0] = SX1276_LoRa_REG_IRQ_FLAGS;
	pDataTemp[1] = 0;

	if(SX1276_SPI_Read(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	Flags = pDataTemp[1];

	if( 	(Flags>>ValidHeader & 0x01)	&&
			(Flags>>RxDone & 0x01)		&&
			((Flags>>PayloadCrcError  & 0x01) == 0)		)
	{
		pDataTemp[0] = SX1276_LoRa_REG_OP_MODE;
		pDataTemp[1] = 0;


		// Go do the FIFO read thing
		ret = SX1276_Get_FIFO_Buffer(hSPI_SX1276, pDataRx, LengthOfDataReceived);
		if(ret != SX1276_OK)
		{
			return ret;
		}


		//Clear all flags
		ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
		if(ret!=SX1276_OK)
		{
			return ret;
		}

	}
	else
	{
		if(Flags>>PayloadCrcError  & 0x01)
		{
			//Clear all flags
			ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
			if(ret!=SX1276_OK)
			{
				return ret;
			}
			return SX1276_RX_FLAG_CRC_ERROR;
		}
		if(Flags == 0)
		{
			return SX1276_RX_NOTHING_RECEIVED;
		}
	}

	// 9) Set SX1276 to SLEEP
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_SLEEP)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 10) Wait for PostSleepDelay_ms
	if(delay==SYNC_MODE_MCU_HAL_DELAY)
	{
		HAL_Delay(PostSleepDelay_ms);
	}
	else
	{
		MCU_STOP_Delay_ms(PostSleepDelay_ms, hSPI_SX1276.hrtc_l);
	}

	return SX1276_OK;
}


/*
 * @brief Receive single data packet using LoRa Modem
 * @notes: As per Section 4.1.6 LoRa Modem State Machine Sequences -> Single Reception Operating Mode (Pg 40 of 132)
 * 			1) Set SX1276 RF Carrier frequency.
 * 			2) Set LoRa communication parameter
 * 			3) Set SX1276_LoRa_REG_FIFO_RX_BASE_ADDR (RegFifoRxBaseAddr) = 0x00.
 *			4) Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
 *			5) Set SX1276 RxTimeout
 *			6) Set SX1276 to RXSINGLE mode
 *			7) Wait for Timeout_ms
 *			8) Check the OP MODE => shall go back to STDBY
 *			9) Logic for extracting data:
 * 				+ Check the IRQ flag; if ValidHeader==1
 * 				|---If yes, check ValidHeader==1, RxDone==1, RxTimeout==0, PayloadCrcError== 0
 * 					|---If yes, Read the data from buffer
 * 					|---If no, check RxTimeout==1, return SX1276_TIMEOUT; check PayloadCrcError==1, return SX1276_RX_FLAG_CRC_ERROR
 * 				|---If no, check RxTimeout==1, return SX1276_TIMEOUT; check PayloadCrcError==1, return SX1276_RX_FLAG_CRC_ERROR
 * 			10) Set SX1276 to SLEEP
 *
 *			## * Discard first byte in the array
 * @param
 * 			SX1276 handle
 * 			LoRa Receive frequency
 * 			Timeout in ms
 * 			Communication parameter struct
 * 			Rx Data buffer pointer
 * 			Length of Data received returned via pointer
 */
SX1276_StatusTypeDef SX1276_ReceiveSingle(SPI_SX1276_TypeDef hSPI_SX1276,
		uint32_t LoRaFrequnecy_KHz,
		uint16_t Timeout_ms,
		SX1276_CommParam sCommParam,
		uint8_t *pDataRx,
		SX1276_DelayType delay,
		uint8_t *LengthOfDataReceived)
{
	SX1276_StatusTypeDef ret;
	uint8_t pDataTemp[2]={0};
	// 1) Set SX1276 RF Carrier frequency.
	if(SX1276_SetLoRaFrequency(hSPI_SX1276, LoRaFrequnecy_KHz) !=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	ret = SX1276_SetLoRaCommunicationParameter(hSPI_SX1276, sCommParam);
	if(ret!=SX1276_OK)
	{
		return ret;
	}

	// 2) Set SX1276_LoRa_REG_FIFO_RX_BASE_ADDR (RegFifoRxBaseAddr) = 0x00.
	pDataTemp[0] = SX1276_LoRa_REG_FIFO_RX_BASE_ADDR;
	pDataTemp[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}


	// 3) Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
	pDataTemp[0] = SX1276_LoRa_REG_FIFO_ADDR_PTR;
	pDataTemp[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 4) Set RxTimeout
	ret = SX1276_SetRxTimeout(hSPI_SX1276, Timeout_ms, sCommParam);
	if(ret != SX1276_OK)
	{
		return ret;
	}

	// 5) Set SX1276 to RXSINGLE mode
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_RXSINGLE)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 6) Wait for Timeout_ms
	if(delay==SYNC_MODE_MCU_HAL_DELAY)
	{
		HAL_Delay(ComputeToA(sCommParam, 240));
	}
	else
	{
		MCU_STOP_Delay_ms(ComputeToA(sCommParam, 240), hSPI_SX1276.hrtc_l);
	}

	pDataTemp[0] = SX1276_LoRa_REG_OP_MODE;
	pDataTemp[1] = 0;
	uint8_t temp1 = pDataTemp[1];

	// 7) Check the OP MODE => shall go back to STDBY
	while(temp1!= SX1276_LoRa_REG_OP_MODE_STDBY)
	{
		pDataTemp[0] = SX1276_LoRa_REG_OP_MODE;
		pDataTemp[1] = 0;
		if(SX1276_SPI_Read(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
		{
			return SX1276_SPI_ERROR;
		}
		temp1= pDataTemp[1];
	}

	/*
	 * 8) Logic for extracting data:
	 * 		+ Check the IRQ flag; if ValidHeader==1
	 * 		|---If yes, check ValidHeader==1, RxDone==1, RxTimeout==0, PayloadCrcError== 0
	 * 			|---If yes, Read the data from buffer
	 * 			|---If no, check RxTimeout==1, return SX1276_TIMEOUT; check PayloadCrcError==1, return SX1276_RX_FLAG_CRC_ERROR
	 * 		|---If no, check RxTimeout==1, return SX1276_TIMEOUT; check PayloadCrcError==1, return SX1276_RX_FLAG_CRC_ERROR
	 */
	pDataTemp[0] = SX1276_LoRa_REG_IRQ_FLAGS;
	pDataTemp[1] = 0;
	uint8_t Flags=0;


	if(SX1276_SPI_Read(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	Flags = pDataTemp[1];

	// Valid Reception

	if(Flags>>ValidHeader & 0x01)
	{
		/*
		 * Sometimes, it takes time for SX1276 to demodulate the signal; i.e.
		 * ValidHeader will be raised before RxDone.
		 * So, we check the IRQ flag again, we wait for sometime before checking the flag
		 * ## the delay is based on experimentations.
		 */
		if(delay==SYNC_MODE_MCU_HAL_DELAY)
		{
			HAL_Delay(2*ComputeToA(sCommParam, 240));
		}
		else
		{
			MCU_STOP_Delay_ms(2*ComputeToA(sCommParam, 240), hSPI_SX1276.hrtc_l);
		}


		pDataTemp[0] = SX1276_LoRa_REG_IRQ_FLAGS;
		pDataTemp[1] = 0;

		if(SX1276_SPI_Read(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
		{
			return SX1276_SPI_ERROR;
		}

		Flags = pDataTemp[1];

		if( 	(Flags>>ValidHeader & 0x01)	&&
				(Flags>>RxDone & 0x01)		&&
				((Flags>>RxTimeout  & 0x01) == 0) 	&&
				((Flags>>PayloadCrcError  & 0x01) == 0)		)
		{
			pDataTemp[0] = SX1276_LoRa_REG_OP_MODE;
			pDataTemp[1] = 0;
			uint8_t temp = pDataTemp[1];

			while(temp!= SX1276_LoRa_REG_OP_MODE_STDBY)
			{
				pDataTemp[0] = SX1276_LoRa_REG_OP_MODE;
				pDataTemp[1] = 0;
				if(SX1276_SPI_Read(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
				{
					return SX1276_SPI_ERROR;
				}
				temp = pDataTemp[1];
			}


			// Go do the FIFO read thing
			ret = SX1276_Get_FIFO_Buffer(hSPI_SX1276, pDataRx, LengthOfDataReceived);
			if(ret != SX1276_OK)
			{
				return ret;
			}

			//Clear all flags
			ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
			if(ret!=SX1276_OK)
			{
				return ret;
			}

		}
		else
		{
			if(Flags>>RxTimeout  & 0x01)
			{
				//Clear all flags
				ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
				if(ret!=SX1276_OK)
				{
					return ret;
				}
				//HAL_Delay(10);
				return SX1276_TIMEOUT;
			}
			if(Flags>>PayloadCrcError  & 0x01)
			{
				//Clear all flags
				ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
				if(ret!=SX1276_OK)
				{
					return ret;
				}
				return SX1276_RX_FLAG_CRC_ERROR;
			}
		}

	}
	else
	{
		if(Flags>>RxTimeout  & 0x01)
		{
			//Clear all flags
			ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
			if(ret!=SX1276_OK)
			{
				return ret;
			}
			//HAL_Delay(10);
			return SX1276_TIMEOUT;
		}
		if(Flags>>PayloadCrcError  & 0x01)
		{
			//Clear all flags
			ret = SX1276_ClearAllIRQFlags(hSPI_SX1276);
			if(ret!=SX1276_OK)
			{
				return ret;
			}
			return SX1276_RX_FLAG_CRC_ERROR;
		}
	}

	// 10) Set SX1276 to SLEEP
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_SLEEP)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	return SX1276_OK;
}


/*
 * @brief Read FIFO data buffer for data transmission
 * @notes As per Section 4.1.6 Payload Data Extraction from FIFO, Pg 41 of 132
 * 				i)		Check if ValidHeader, RxDone and PayloadCrcError interrupts in the status register RegIrqFlags are not asserted
 * 				ii) 	Set SX1276_LoRa_REG_FIFO_RX_BASE_ADDR (RegFifoRxBaseAddr) = 0x00.
 * 						This is done to so that full FIFO data buffer (256 Bytes) can be utilized.
 * 				iii)	Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
 * 						This is where we want to start entering data in the FIFO data buffer.
 * 				iv)		Read the SX1276_LoRa_REG_FIFO_RX_CURRENT_ADDR value.
 * 				v)		Read the FIFO address.
 *
 * @param
 * 			SX1276 handle
 * 			FIFO Addr + Data
 */
SX1276_StatusTypeDef SX1276_Get_FIFO_Buffer(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pDataRx, uint8_t *ptr_vardat_SX1276_LoRa_REG_RX_NB_BYTES)
{
	uint8_t pData[2] ={SX1276_LoRa_REG_RX_NB_BYTES, 0x00};


	// 2) Read SX1276_LoRa_REG_RX_NB_BYTES (RegRxNbBytes)
	uint8_t vardat_SX1276_LoRa_REG_RX_NB_BYTES=0;
	if(SX1276_SPI_Read(hSPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}
	vardat_SX1276_LoRa_REG_RX_NB_BYTES = pData[1];


	// 3) Read SX1276_LoRa_REG_FIFO_RX_CURRENT_ADDR (RegFifoRxCurrentAddr)
	pData[0] = SX1276_LoRa_REG_FIFO_RX_CURRENT_ADDR;
	pData[1] = 0;
	if(SX1276_SPI_Read(hSPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 4) Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = SX1276_LoRa_REG_FIFO_RX_CURRENT_ADDR (RegFifoRxCurrentAddr)
	pData[0] = SX1276_LoRa_REG_FIFO_ADDR_PTR;
	if(SX1276_SPI_Write(hSPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 5) Read the FIFO data
	pDataRx[0] = 0x00;
	if(SX1276_SPI_Read(hSPI_SX1276, pDataRx, vardat_SX1276_LoRa_REG_RX_NB_BYTES)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	*ptr_vardat_SX1276_LoRa_REG_RX_NB_BYTES = vardat_SX1276_LoRa_REG_RX_NB_BYTES;


	return SX1276_OK;
}

/*
 * @brief Sets RX Timeout register
 * @notes
 * 			Setting this register DOES NOT seem to do anything.
 * 			The flag is always raised at 32us (SPI running at 20 MBits/s)
 * 			Conclusion: Timeout duration cannot be changed.
 * @param
 * 		- SX1276 SPI handle
 * 		- Timeout in ms
 * 		- Communication Paramets struct
 *
 */
SX1276_StatusTypeDef SX1276_SetRxTimeout(SPI_SX1276_TypeDef hSPI_SX1276, uint16_t Timeout_ms, SX1276_CommParam sCommParam)
{
	// 1) Compute the Ts
	uint16_t temp = 1;
	temp = temp<<(sCommParam.Spreadfactor>>4);
	float Ts_ms =  (temp/getBandwidth(sCommParam.Bandwidth));

	uint16_t SymbTimeout = (uint16_t)(Timeout_ms/Ts_ms);

	// 2) Check if the values is actually less than OxFFF
	if(SymbTimeout > 0x3FF)
	{
		return SX1276_PROGRAM_ERROR;
	}

	// 3) Set the registers
	uint8_t pDataTemp[2]={SX1276_LoRa_REG_SYMB_TIMEOUT_LSB, (SymbTimeout & 0x00FF)};
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	pDataTemp[0] = SX1276_LoRa_REG_MODEM_CONFIG_2;
	pDataTemp[1] = 0;
	if(SX1276_SPI_Read(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	pDataTemp[1] &= 0b11111100;
	pDataTemp[1] |= (SymbTimeout>>8) & (0x03);

	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}



	return SX1276_OK;

}

/*
 * @brief Set config for data transmission
 * @notes	First part of SX1276_Transmit function
 * 			IMPORTANT: Takes ~90ms to execute for SPI @5MBits/s
 *
 * 			* Keep the first byte empty
 * @param
 * 			SX1276 handle
 * 			FIFO Addr + Data
 * 			Length of Tx data
 */
SX1276_StatusTypeDef SX1276_Multi_Transmitter_SetConfiguration(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataTx,
		uint8_t TxDataLength,
		uint32_t LoRaFrequnecy_KHz,
		uint8_t TxPower,
		SX1276_CommParam sCommParam)
{
	uint8_t pDataTemp[2]={0};
	// 1) Set SX1276 RF Carrier frequency.
	if(SX1276_SetLoRaFrequency(hSPI_SX1276, LoRaFrequnecy_KHz) !=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 2) Fill LoRa FIFO data buffer
	if(SX1276_Set_FIFO_Buffer(hSPI_SX1276, pDataTx, TxDataLength)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 3) Set PA Config to 1 (NiceRF requirement)
	pDataTemp[0] = SX1276_LoRa_REG_PA_CONFIG;
	pDataTemp[1] = TxPower;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 4) Set Communication Parameter
	if(SX1276_SetLoRaCommunicationParameter(hSPI_SX1276, sCommParam)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	return SX1276_OK;
}

/*
 * @brief Transmit Data using LoRa Modem after configuration is complete
 * @notes: 	Second part of SX1276_Transmit function
 *			Takes ToA
 * 			* Keep the first byte empty
 * @param
 * 			SX1276 handle
 * 			Length of Tx data
 */
SX1276_StatusTypeDef SX1276_Multi_Transmitter_Transmit(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t TxDataLength,
		SX1276_DelayType delay,
		SX1276_CommParam sCommParam)
{
	// 4) Transmit
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_TX)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// Wait for transmission to complete
	if(delay==SYNC_MODE_MCU_HAL_DELAY)
	{
		HAL_Delay(ComputeToA(sCommParam, TxDataLength));
	}
	else
	{
		MCU_STOP_Delay_ms(ComputeToA(sCommParam, TxDataLength), hSPI_SX1276.hrtc_l);
	}

	// 5) Clear TxDone IRQ flag
	if(SX1276_ClearIRQFlags(hSPI_SX1276, TxDone)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}


	// 6) Set SX1276 to Sleep
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_SLEEP)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}




	return SX1276_OK;
}


/*
 * @brief Transmit Data using LoRa Modem
 * @notes: As per Section 4.1.6 LoRa Modem State Machine Sequences -> Data Transmission Sequence (Pg 38 of 132)
 * 			1) Set SX1276 RF Carrier frequency.
 * 			2) Fill LoRa FIFO data buffer
 * 			3) Set Transmit Power
 * 			4) Set CommParam
 * 			5) Transmit
 * 			6) Put SX1276 in Sleep Mode (FIFO is cleared)
 *
 * 			* Keep the first byte empty
 * @param
 * 			SX1276 handle
 * 			FIFO Addr + Data
 * 			Length of Tx data
 */
SX1276_StatusTypeDef SX1276_Transmit(SPI_SX1276_TypeDef hSPI_SX1276,
		uint8_t *pDataTx,
		uint8_t TxDataLength,
		uint32_t LoRaFrequnecy_KHz,
		uint8_t TxPower,
		SX1276_DelayType delay,
		SX1276_CommParam sCommParam)
{
	uint8_t pDataTemp[2]={0};
	// 1) Set SX1276 RF Carrier frequency.
	if(SX1276_SetLoRaFrequency(hSPI_SX1276, LoRaFrequnecy_KHz) !=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 2) Fill LoRa FIFO data buffer
	if(SX1276_Set_FIFO_Buffer(hSPI_SX1276, pDataTx, TxDataLength)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 3) Set PA Config to 1 (NiceRF requirement)
	pDataTemp[0] = SX1276_LoRa_REG_PA_CONFIG;
	pDataTemp[1] = TxPower;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 4) Set Communication Parameter
	if(SX1276_SetLoRaCommunicationParameter(hSPI_SX1276, sCommParam)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}


	// 4) Transmit
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_TX)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// Wait for transmission to complete

	if(delay==SYNC_MODE_MCU_HAL_DELAY)
	{
		HAL_Delay(ComputeToA(sCommParam, TxDataLength));
	}
	else
	{
		MCU_STOP_Delay_ms(ComputeToA(sCommParam, TxDataLength), hSPI_SX1276.hrtc_l);
	}


	// 5) Clear TxDone IRQ flag
	if(SX1276_ClearIRQFlags(hSPI_SX1276, TxDone)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}


	// 6) Set SX1276 to Sleep
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_SLEEP)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}


	return SX1276_OK;
}

uint32_t ComputeToA(SX1276_CommParam sCommParam, uint8_t TxDataLength)
{
	uint16_t temp = 1;
	temp = temp<<(sCommParam.Spreadfactor>>4);
	float Ts_ms =  (temp/getBandwidth(sCommParam.Bandwidth));

	float Tpreamble_ms = (sCommParam.PreambleLength + 4.25)*Ts_ms;

	float n_Payload = ( 8*(TxDataLength-1) - 4*(sCommParam.Spreadfactor>>4) + 28 +16);
	n_Payload = ((float)n_Payload/(float)(4*(sCommParam.Spreadfactor>>4)));
	n_Payload = ceil(n_Payload) * (4 + (sCommParam.CodingRate>>1));
	n_Payload =  8 + fmax(n_Payload, 0.0);

	float Tpayload_ms = n_Payload * Ts_ms;

	uint32_t ToA_ms = ceil(Tpayload_ms + Tpreamble_ms);

	ToA_ms +=10;		// When making measurements, a discrepancy of +/- >10ms was found

	return ToA_ms;
}


float getBandwidth(uint8_t regVal)
{
	switch (regVal) {
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_7_8_kHz:    return 7.8;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_10_4_kHz:   return 10.4;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_15_6_kHz:   return 15.6;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_20_8_kHz:   return 20.8;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_31_25_kHz:  return 31.25;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_41_7_kHz:   return 41.7;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_62_5_kHz:   return 62.5;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_125_kHz:    return 125.0;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_250_kHz:    return 250.0;
	case SX1276_LoRa_REG_MODEM_CONFIG1_BW_500_kHz:    return 500.0;
	default:                                          return -1.0; // Unknown value
	}
}

uint32_t GetTimerClkFromHW(void)
{
	uint32_t hclk = HAL_RCC_GetHCLKFreq();
	uint32_t ppre2 = (RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos;

	uint32_t apb2_prescaler;

	if (ppre2 < 4)
		apb2_prescaler = 1;
	else {
		switch (ppre2) {
		case 4: apb2_prescaler = 2; break;
		case 5: apb2_prescaler = 4; break;
		case 6: apb2_prescaler = 8; break;
		case 7: apb2_prescaler = 16; break;
		default: apb2_prescaler = 1; break;
		}
	}

	uint32_t pclk2 = hclk / apb2_prescaler;

	if (apb2_prescaler == 1)
		return pclk2;
	else
		return 2 * pclk2;
}

float measure_function_time_us(TIM_HandleTypeDef *htim, uint32_t TIM_PRESCALR)
{
	uint16_t end;

	// Reset counter to zero before starting for easier timing
	__HAL_TIM_SET_COUNTER(htim, 0);

	HAL_TIM_Base_Start(htim);

	HAL_Delay(1000);
	//MCU_STOP_Delay_ms(100);

	end = __HAL_TIM_GET_COUNTER(htim);

	HAL_TIM_Base_Stop(htim);

	return end*(TIM_PRESCALR*1.0/ GetTimerClkFromHW());
}

void MCU_STOP_Delay_ms(uint32_t ms, RTC_HandleTypeDef *hrtc_loc)
{
	/*
	 * Ref: https://community.st.com/t5/stm32-mcus/how-to-configure-the-rtc-to-wake-up-the-stm32-periodically-from/ta-p/49836
	 */
	HAL_SuspendTick();
	/*
	 *  The RTC is fed via LSI (32MHz)
	 *  if ms*2 were = 1, then the interupt for waking up MCU would ben programmed for DIV16 (=16)
	 *   = 16/32 ms=0.5ms*ms*2=0.5ms * 1 = 0.5ms
	 */
	HAL_RTCEx_SetWakeUpTimer_IT(hrtc_loc, ms*2, RTC_WAKEUPCLOCK_RTCCLK_DIV16);

	/* Enter STOP 2 mode */
	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
	HAL_RTCEx_DeactivateWakeUpTimer(hrtc_loc);
	SystemClock_Config();
	HAL_ResumeTick();
}

/*
 * @brief Sets Bandwidth, Coding Rate and Spread Factor
 * @param
 * 	- SX1276 handle
 * 	- CommParam struct
 */
SX1276_StatusTypeDef SX1276_SetLoRaCommunicationParameter(SPI_SX1276_TypeDef hSPI_SX1276, SX1276_CommParam sCommParam)
{
	uint8_t pDataTemp[2] = {SX1276_LoRa_REG_MODEM_CONFIG_1, (sCommParam.Bandwidth | sCommParam.CodingRate)};
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	if(sCommParam.Spreadfactor == SX1276_LoRa_REG_MODEM_CONFIG2_SF6)
	{
		pDataTemp[0] = SX1276_LoRa_REG_DETECT_OPTIMIZE;
		pDataTemp[1] = 0x05;
		if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
		{
			return SX1276_SPI_ERROR;
		}

		pDataTemp[0] = SX1276_LoRa_REG_DETECTION_THRSEHOLD;
		pDataTemp[1] = 0x0C;
		if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
		{
			return SX1276_SPI_ERROR;
		}

	}
	else
	{
		pDataTemp[0] = SX1276_LoRa_REG_DETECT_OPTIMIZE;
		pDataTemp[1] = 0x03;
		if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
		{
			return SX1276_SPI_ERROR;
		}

		pDataTemp[0] = SX1276_LoRa_REG_DETECTION_THRSEHOLD;
		pDataTemp[1] = 0x0A;
		if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
		{
			return SX1276_SPI_ERROR;
		}
	}

	pDataTemp[0] = SX1276_LoRa_REG_MODEM_CONFIG_2;
	pDataTemp[1] = sCommParam.Spreadfactor;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	pDataTemp[0] = SX1276_LoRa_REG_SYMB_PREAMBLE_MSB;
	pDataTemp[1] = sCommParam.PreambleLength>>8;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	pDataTemp[0] = SX1276_LoRa_REG_SYMB_PREAMBLE_LSB;
	pDataTemp[1] = sCommParam.PreambleLength;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTemp, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}


	return SX1276_OK;

}

/*
 * @brief Get IRQ flag
 * @param
 * 		SX1276 handle
 * 		Send the flag that needs to be checked
 */
SX1276_StatusTypeDef SX1276_GetIRQFlags(SPI_SX1276_TypeDef hSPI_SX1276, SX1276_IRQFlags Flag, uint8_t *ptrFlagData)
{
	uint8_t pDataR[2]={SX1276_LoRa_REG_IRQ_FLAGS , 0x00};
	if(SX1276_SPI_Read(hSPI_SX1276, pDataR, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	*ptrFlagData = pDataR[1]>>Flag;

	return SX1276_OK;
}


/*
 * @brief Clears IRQ flag
 * @param
 * 		SX1276 handle
 * 		Send the flag that needs to be cleared
 */
SX1276_StatusTypeDef SX1276_ClearIRQFlags(SPI_SX1276_TypeDef hSPI_SX1276, SX1276_IRQFlags Flag)
{
	uint8_t pDataR[2]={SX1276_LoRa_REG_IRQ_FLAGS , 0x01<<Flag};
	return SX1276_SPI_Write(hSPI_SX1276, pDataR, 2);
}

/*
 * @brief Clears All IRQ flag
 * @param
 * 		SX1276 handle
 */
SX1276_StatusTypeDef SX1276_ClearAllIRQFlags(SPI_SX1276_TypeDef hSPI_SX1276)
{
	uint8_t pDataR[2]={SX1276_LoRa_REG_IRQ_FLAGS , 0xFF};
	return SX1276_SPI_Write(hSPI_SX1276, pDataR, 2);
}



/*
 * @brief Fill FIFO data buffer for data transmission
 * @notes As per Section 4.1.2.3 LoRa Mode FIFO Data Buffer, Pg 34 of 132
 * 				i)		Set SX1276 in Standby mode.
 * 				ii) 	Set SX1276_LoRa_REG_FIFO_TX_BASE_ADDR (RegFifoTxBaseAddr) = 0x00.
 * 						This is done to so that full FIFO data buffer (256 Bytes) can be utilized.
 * 				iii)	Set SX1276_LoRa_REG_PAYLOAD_LENGTH (RegPayloadLength) to size of data to be transmitted.
 * 				iv)		Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
 * 						This is where we want to start entering data in the FIFO data buffer.
 * 				v)		Send it to RegFifo
 * 						THE pDataTx first element shall be SX1276_LoRa_REG_FIFO (RegFifo).

 * @param
 * 			SX1276 handle
 * 			FIFO Addr + Data
 * 			Length of Tx data
 */
SX1276_StatusTypeDef SX1276_Set_FIFO_Buffer(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pDataTx, uint8_t TxDataLength)
{
	uint8_t pData[2] ={0};

	// 1) Set SX1276 in Standby mode
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_STDBY)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	// 2) Set SX1276_LoRa_REG_FIFO_TX_BASE_ADDR (RegFifoTxBaseAddr) = 0x00.
	pData[0] = SX1276_LoRa_REG_FIFO_TX_BASE_ADDR;
	pData[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 3) Set SX1276_LoRa_REG_PAYLOAD_LENGTH (RegPayloadLength) to size of data to be transmitted.
	pData[0] = SX1276_LoRa_REG_PAYLOAD_LENGTH;
	pData[1] = TxDataLength;
	if(SX1276_SPI_Write(hSPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 4) Set SX1276_LoRa_REG_FIFO_ADDR_PTR (RegFifoAddrPtr) = 0x00.
	pData[0] = SX1276_LoRa_REG_FIFO_ADDR_PTR;
	pData[1] = 0;
	if(SX1276_SPI_Write(hSPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	// 5) Write to FIFO data buffer
	pDataTx[0] = 0x00;
	if(SX1276_SPI_Write(hSPI_SX1276, pDataTx, TxDataLength)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	return SX1276_OK;
}

/*
 * @brief Set LoRa RF carrier frequency
 * @param
 * 			SX1276 handle
 * 			Frequency in KHz
 *
 */
SX1276_StatusTypeDef SX1276_SetLoRaFrequency(SPI_SX1276_TypeDef hSPI_SX1276, uint32_t LoRaFrequnecy_KHz)
{

	/*--Use this incase of custom crystal
	 * // For Now lets assume XOSC is 32 MHz
	 * uint32_t Fstep= 32000000;
	 * Fstep = (uint32_t)((float)(LoRaFrequnecy_KHz*1e3) / ((float)(Fstep>>19)));
	 */

	//For NiceRF
	uint32_t Fstep = (uint32_t)(LoRaFrequnecy_KHz*16.384);

	uint8_t pData[4]={0};
	pData[0] = SX1276_LoRa_REG_FR_MSB;
	pData[1] = (Fstep & 0xFF0000)>>16;
	pData[2] = (Fstep & 0xFF00)>>8;
	pData[3] = (Fstep & 0xFF);

	// Set SX1276 to Standby mode
	if(SX1276_SetOperationMode(hSPI_SX1276, SX1276_LoRa_REG_OP_MODE_STDBY)!=SX1276_OK)
	{
		return SX1276_ERROR;
	}

	if(SX1276_SPI_Write(hSPI_SX1276, pData, 4)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}
	return SX1276_OK;

}


/*
 * @brief Set SX1276 to a paricular mode
 *
 * @param
 * 		 SX1276 handle
 * 		 SX1276 Mode
 */
SX1276_StatusTypeDef SX1276_SetOperationMode(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t Mode)
{
	if((Mode & 0b00000111)>7)
	{
		return SX1276_PROGRAM_ERROR;
	}
	uint8_t pDataR[2]={0};

	pDataR[0] = SX1276_LoRa_REG_OP_MODE;
	pDataR[1] = Mode;

	if(SX1276_SPI_Write(hSPI_SX1276, pDataR, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}
	return SX1276_OK;


}
/**
 * @brief SX1276 Init function
 *  0)	Do manual reset
 * 	1)	Set the RegOpMode (0x01) Register
 * 			- In sleep mode
 * 			- Low frequency mode
 * 	2)	Read the RegOpMode (0x01) Register, check if the register content is same.
 * 	3) 	Set PA Config to 1 (NiceRF Requirement)
 * 	@param SX1276 handle:
 * 		- Chip select port
 * 		- Chip select pin
 * 		- SPI handle
 */
SX1276_StatusTypeDef SX1276_Init(SX1276_Init_Param h_SX1276)
{
	uint8_t pData[2]={0};
	uint8_t pDataR[2]={0};


//	// 0)	Write/read dummy data from reg to check if SX1276 is connected or not
//	pData[0] = SX1276_LoRa_REG_FIFO_ADDR_PTR;
//	pData[1] = 0x6F;
//	if(SX1276_SPI_Write(h_SX1276.SPI_SX1276, pData, 2)!=SX1276_OK)
//	{
//		return SX1276_SPI_ERROR;
//	}
//
//	pData[1] = 0x6F;
//	pDataR[0] = SX1276_LoRa_REG_FIFO_ADDR_PTR;
//
//	if( (SX1276_SPI_Read(h_SX1276.SPI_SX1276, pDataR, 2)!=SX1276_OK) || (pDataR[1] != pData[1]) )
//	{
//		return SX1276_NC_ERROR;
//	}


	// 1) Sets SX1276 register
	pData[0] = SX1276_LoRa_REG_OP_MODE;
	pData[1] = SX1276_LoRa_REG_OP_MODE_SLEEP;

	if(SX1276_SPI_Write(h_SX1276.SPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	//HAL_Delay(1000);

	// 2) Read SX1276 register
	pDataR[0] = SX1276_LoRa_REG_OP_MODE;

	if( (SX1276_SPI_Read(h_SX1276.SPI_SX1276, pDataR, 2)!=SX1276_OK) || ((pDataR[1] && 0b00000111) != (pData[1] && 0b00000111)) )
	{
		return SX1276_SPI_ERROR;
	}

	// 3) Set PA Config to 1 (NiceRF requirement)
	pData[0] = SX1276_LoRa_REG_PA_CONFIG;
	pData[1] = SX1276_LoRa_PA_CONFIG_OP_POWER_dBm_17;
	if(SX1276_SPI_Write(h_SX1276.SPI_SX1276, pData, 2)!=SX1276_OK)
	{
		return SX1276_SPI_ERROR;
	}

	return SX1276_OK;
}
/*
 * @brief SX1276 wrap for SPI Write
 * @notes
 * 		-	pData[0] shall be the SX1276 register address
 * 		-	DataLength shall be number of register writes + 1 or  sizeof(pData)
 * @param
 * 	- SX1276 handle
 * 	- Data register location
 * 	- Length of data
 */

SX1276_StatusTypeDef SX1276_SPI_Write(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pData, uint16_t DataLength)
{
	HAL_StatusTypeDef ret;

	pData[0] |= 0b10000000;										// As per data sheet, write bit needs to be set
	HAL_GPIO_WritePin(hSPI_SX1276.NSS_GPIOx, hSPI_SX1276.NSS_GPIO_Pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit(hSPI_SX1276.hspi, pData, DataLength, 10);
	HAL_GPIO_WritePin(hSPI_SX1276.NSS_GPIOx, hSPI_SX1276.NSS_GPIO_Pin, GPIO_PIN_SET);

	if(ret!=HAL_OK)
	{
		return SX1276_SPI_ERROR;
	}

	return SX1276_OK;
}

/*
 * @brief SX1276 wrap for SPI Read
 * @notes
 * 		-	pData[0] shall be the SX1276 register address
 * 		-	DataLength shall be number of register read + 1 or  sizeof(pData)
 * @param
 * 	- SX1276 handle
 * 	- Data register location
 * 	- Length of data
 */
SX1276_StatusTypeDef SX1276_SPI_Read(SPI_SX1276_TypeDef hSPI_SX1276, uint8_t *pData, uint16_t DataLength)
{
	HAL_StatusTypeDef ret;

	pData[0] &= 0b01111111;										// As per data sheet, read bit needs to be set
	HAL_GPIO_WritePin(hSPI_SX1276.NSS_GPIOx, hSPI_SX1276.NSS_GPIO_Pin, GPIO_PIN_RESET);
	ret = HAL_SPI_Receive(hSPI_SX1276.hspi, pData, DataLength, 10);
	HAL_GPIO_WritePin(hSPI_SX1276.NSS_GPIOx, hSPI_SX1276.NSS_GPIO_Pin, GPIO_PIN_SET);

	if(ret!=HAL_OK)
	{
		return SX1276_SPI_ERROR;
	}

	return SX1276_OK;
}

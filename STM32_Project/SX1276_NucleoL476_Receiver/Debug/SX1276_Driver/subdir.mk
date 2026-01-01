################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
./SX1276_Driver/stm32_SX1276_spi.o 

C_DEPS += \
./SX1276_Driver/stm32_SX1276_spi.d 


# Each subdirectory must supply rules for building sources it contributes
SX1276_Driver/stm32_SX1276_spi.o: D:/OneDrive/OneDrive\ -\ Norfolk\ State\ University/MastersWork/21_LORA_STM_ULP_JournelPaper/GitHub/STM32_Project/SX1276_NucleoL476/Drivers/SX1276_Driver/stm32_SX1276_spi.c SX1276_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SX1276_Driver

clean-SX1276_Driver:
	-$(RM) ./SX1276_Driver/stm32_SX1276_spi.cyclo ./SX1276_Driver/stm32_SX1276_spi.d ./SX1276_Driver/stm32_SX1276_spi.o ./SX1276_Driver/stm32_SX1276_spi.su

.PHONY: clean-SX1276_Driver


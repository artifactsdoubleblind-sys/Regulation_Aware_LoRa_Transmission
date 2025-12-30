################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SX1276_Driver/stm32_SX1276_spi.c 

OBJS += \
./Drivers/SX1276_Driver/stm32_SX1276_spi.o 

C_DEPS += \
./Drivers/SX1276_Driver/stm32_SX1276_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SX1276_Driver/%.o Drivers/SX1276_Driver/%.su Drivers/SX1276_Driver/%.cyclo: ../Drivers/SX1276_Driver/%.c Drivers/SX1276_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SX1276_Driver

clean-Drivers-2f-SX1276_Driver:
	-$(RM) ./Drivers/SX1276_Driver/stm32_SX1276_spi.cyclo ./Drivers/SX1276_Driver/stm32_SX1276_spi.d ./Drivers/SX1276_Driver/stm32_SX1276_spi.o ./Drivers/SX1276_Driver/stm32_SX1276_spi.su

.PHONY: clean-Drivers-2f-SX1276_Driver


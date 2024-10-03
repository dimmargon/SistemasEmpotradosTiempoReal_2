################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Components/ft3x67/ft3x67.c 

OBJS += \
./Components/ft3x67/ft3x67.o 

C_DEPS += \
./Components/ft3x67/ft3x67.d 


# Each subdirectory must supply rules for building sources it contributes
Components/ft3x67/%.o Components/ft3x67/%.su Components/ft3x67/%.cyclo: ../Components/ft3x67/%.c Components/ft3x67/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../BSP -I../Components -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components-2f-ft3x67

clean-Components-2f-ft3x67:
	-$(RM) ./Components/ft3x67/ft3x67.cyclo ./Components/ft3x67/ft3x67.d ./Components/ft3x67/ft3x67.o ./Components/ft3x67/ft3x67.su

.PHONY: clean-Components-2f-ft3x67


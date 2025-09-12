################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Lib/fonts.c \
../Core/Lib/ssh1106.c \
../Core/Lib/ui.c 

OBJS += \
./Core/Lib/fonts.o \
./Core/Lib/ssh1106.o \
./Core/Lib/ui.o 

C_DEPS += \
./Core/Lib/fonts.d \
./Core/Lib/ssh1106.d \
./Core/Lib/ui.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Lib/%.o Core/Lib/%.su Core/Lib/%.cyclo: ../Core/Lib/%.c Core/Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/USER/STM32CubeIDE/workspace_1.19.0/PMT_TEST/Core/Lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Lib

clean-Core-2f-Lib:
	-$(RM) ./Core/Lib/fonts.cyclo ./Core/Lib/fonts.d ./Core/Lib/fonts.o ./Core/Lib/fonts.su ./Core/Lib/ssh1106.cyclo ./Core/Lib/ssh1106.d ./Core/Lib/ssh1106.o ./Core/Lib/ssh1106.su ./Core/Lib/ui.cyclo ./Core/Lib/ui.d ./Core/Lib/ui.o ./Core/Lib/ui.su

.PHONY: clean-Core-2f-Lib


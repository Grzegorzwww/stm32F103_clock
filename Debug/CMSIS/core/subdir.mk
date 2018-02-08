################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/core/core_cm3.c 

OBJS += \
./CMSIS/core/core_cm3.o 

C_DEPS += \
./CMSIS/core/core_cm3.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/core/%.o: ../CMSIS/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F102C8Tx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/gwarchol/STMworkspace/clock/StdPeriph_Driver/inc" -I"/home/gwarchol/STMworkspace/clock/inc" -I"/home/gwarchol/STMworkspace/clock/CMSIS/device" -I"/home/gwarchol/STMworkspace/clock/CMSIS/core" -O1 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



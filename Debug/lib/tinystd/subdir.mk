################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/tinystd/itoa.c 

OBJS += \
./lib/tinystd/itoa.o 

C_DEPS += \
./lib/tinystd/itoa.d 


# Each subdirectory must supply rules for building sources it contributes
lib/tinystd/%.o: ../lib/tinystd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103RBTx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/gwarchol/STMworkspace/clock_digital/StdPeriph_Driver/inc" -I"/home/gwarchol/STMworkspace/clock_digital/inc" -I"/home/gwarchol/STMworkspace/clock_digital/CMSIS/device" -I"/home/gwarchol/STMworkspace/clock_digital/CMSIS/core" -I"/home/gwarchol/STMworkspace/clock_digital/lib/ILI9341" -I"/home/gwarchol/STMworkspace/clock_digital/lib/tinystd" -Og -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



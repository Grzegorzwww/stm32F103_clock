################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/ILI9341/core.c \
../lib/ILI9341/delay.c \
../lib/ILI9341/dma.c \
../lib/ILI9341/graph.c \
../lib/ILI9341/text.c 

OBJS += \
./lib/ILI9341/core.o \
./lib/ILI9341/delay.o \
./lib/ILI9341/dma.o \
./lib/ILI9341/graph.o \
./lib/ILI9341/text.o 

C_DEPS += \
./lib/ILI9341/core.d \
./lib/ILI9341/delay.d \
./lib/ILI9341/dma.d \
./lib/ILI9341/graph.d \
./lib/ILI9341/text.d 


# Each subdirectory must supply rules for building sources it contributes
lib/ILI9341/%.o: ../lib/ILI9341/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103RBTx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/gwarchol/STMworkspace/clock_digital/StdPeriph_Driver/inc" -I"/home/gwarchol/STMworkspace/clock_digital/inc" -I"/home/gwarchol/STMworkspace/clock_digital/CMSIS/device" -I"/home/gwarchol/STMworkspace/clock_digital/CMSIS/core" -I"/home/gwarchol/STMworkspace/clock_digital/lib/ILI9341" -I"/home/gwarchol/STMworkspace/clock_digital/lib/tinystd" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



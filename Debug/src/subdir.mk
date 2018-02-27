################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buffors.c \
../src/device_config.c \
../src/main.c \
../src/rtc.c \
../src/scan_timer.c \
../src/system_stm32f10x.c \
../src/timers.c \
../src/touch_screen.c \
../src/uart_interrupt.c \
../src/usart_dma.c 

OBJS += \
./src/buffors.o \
./src/device_config.o \
./src/main.o \
./src/rtc.o \
./src/scan_timer.o \
./src/system_stm32f10x.o \
./src/timers.o \
./src/touch_screen.o \
./src/uart_interrupt.o \
./src/usart_dma.o 

C_DEPS += \
./src/buffors.d \
./src/device_config.d \
./src/main.d \
./src/rtc.d \
./src/scan_timer.d \
./src/system_stm32f10x.d \
./src/timers.d \
./src/touch_screen.d \
./src/uart_interrupt.d \
./src/usart_dma.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103RBTx -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/gwarchol/STMworkspace/clock_digital/StdPeriph_Driver/inc" -I"/home/gwarchol/STMworkspace/clock_digital/inc" -I"/home/gwarchol/STMworkspace/clock_digital/CMSIS/device" -I"/home/gwarchol/STMworkspace/clock_digital/CMSIS/core" -I"/home/gwarchol/STMworkspace/clock_digital/lib/ILI9341" -I"/home/gwarchol/STMworkspace/clock_digital/lib/tinystd" -Og -g -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



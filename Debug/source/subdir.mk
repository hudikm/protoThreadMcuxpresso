################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/LedFlasher.cpp \
../source/MKL25Z128xxx4_ProtoThreads.cpp \
../source/SwTimer.cpp \
../source/UartThread.cpp \
../source/cpp_config.cpp 

C_SRCS += \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/LedFlasher.o \
./source/MKL25Z128xxx4_ProtoThreads.o \
./source/SwTimer.o \
./source/UartThread.o \
./source/cpp_config.o \
./source/mtb.o \
./source/semihost_hardfault.o 

CPP_DEPS += \
./source/LedFlasher.d \
./source/MKL25Z128xxx4_ProtoThreads.d \
./source/SwTimer.d \
./source/UartThread.d \
./source/cpp_config.d 

C_DEPS += \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=gnu++14 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -DPROFILE_GCC_GENERIC -DSDK_DEBUGCONSOLE=1 -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -I"/home/martin/Documents/MCUXpresso_11.0.1_2563/workspace/MKL25Z128xxx4_ProtoThreads/etl" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-rtti -fno-exceptions -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu11 -D__NEWLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DPROFILE_GCC_GENERIC -DSDK_DEBUGCONSOLE=1 -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -I"/home/martin/Documents/MCUXpresso_11.0.1_2563/workspace/MKL25Z128xxx4_ProtoThreads/etl" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



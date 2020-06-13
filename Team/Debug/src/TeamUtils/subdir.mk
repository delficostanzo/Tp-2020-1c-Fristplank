################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TeamUtils/LogManager.c 

OBJS += \
./src/TeamUtils/LogManager.o 

C_DEPS += \
./src/TeamUtils/LogManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/TeamUtils/%.o: ../src/TeamUtils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-Fritsplank/Library/Library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



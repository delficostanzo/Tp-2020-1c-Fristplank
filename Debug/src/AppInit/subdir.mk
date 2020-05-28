################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AppInit/ConfigFunctions.c \
../src/AppInit/EntrenadoresInit.c \
../src/AppInit/HilosEntrenadores.c 

OBJS += \
./src/AppInit/ConfigFunctions.o \
./src/AppInit/EntrenadoresInit.o \
./src/AppInit/HilosEntrenadores.o 

C_DEPS += \
./src/AppInit/ConfigFunctions.d \
./src/AppInit/EntrenadoresInit.d \
./src/AppInit/HilosEntrenadores.d 


# Each subdirectory must supply rules for building sources it contributes
src/AppInit/%.o: ../src/AppInit/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-Fritsplank/Library/Library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



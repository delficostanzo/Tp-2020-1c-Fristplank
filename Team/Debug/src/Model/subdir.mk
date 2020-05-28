################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Model/Casillero.c \
../src/Model/Mapa.c \
../src/Model/Pokemon.c \
../src/Model/Trainer.c 

OBJS += \
./src/Model/Casillero.o \
./src/Model/Mapa.o \
./src/Model/Pokemon.o \
./src/Model/Trainer.o 

C_DEPS += \
./src/Model/Casillero.d \
./src/Model/Mapa.d \
./src/Model/Pokemon.d \
./src/Model/Trainer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Model/%.o: ../src/Model/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-Fritsplank/Library/Library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Broker.c \
../src/buddy.c \
../src/colas.c \
../src/conexionBroker.c \
../src/memoriaCache.c \
../src/particiones.c \
../src/repositorioSuscriptores.c \
../src/variables.c 

OBJS += \
./src/Broker.o \
./src/buddy.o \
./src/colas.o \
./src/conexionBroker.o \
./src/memoriaCache.o \
./src/particiones.o \
./src/repositorioSuscriptores.o \
./src/variables.o 

C_DEPS += \
./src/Broker.d \
./src/buddy.d \
./src/colas.d \
./src/conexionBroker.d \
./src/memoriaCache.d \
./src/particiones.d \
./src/repositorioSuscriptores.d \
./src/variables.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-Fritsplank/Library/Library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



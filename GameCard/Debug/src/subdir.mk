################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/GameCard.c \
../src/archivos.c \
../src/bitmap.c \
../src/blocks.c \
../src/conexiones.c \
../src/procesar.c \
../src/utils.c 

OBJS += \
./src/GameCard.o \
./src/archivos.o \
./src/bitmap.o \
./src/blocks.o \
./src/conexiones.o \
./src/procesar.o \
./src/utils.o 

C_DEPS += \
./src/GameCard.d \
./src/archivos.d \
./src/bitmap.d \
./src/blocks.d \
./src/conexiones.d \
./src/procesar.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-Fritsplank/Library/Library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



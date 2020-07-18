################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../conexion.c \
../desserializadores.c \
../serializadores.c \
../utils.c 

OBJS += \
./conexion.o \
./desserializadores.o \
./serializadores.o \
./utils.o 

C_DEPS += \
./conexion.d \
./desserializadores.d \
./serializadores.d \
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



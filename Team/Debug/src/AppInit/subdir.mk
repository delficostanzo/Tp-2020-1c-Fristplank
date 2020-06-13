################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/AppInit/Conexion.c \
../src/AppInit/ConfigFunctions.c \
../src/AppInit/CreacionEstructurasMsj.c \
../src/AppInit/EntrenadoresInit.c \
../src/AppInit/EnvioMensajes.c \
../src/AppInit/HilosEntrenadores.c \
../src/AppInit/LogFunctions.c \
../src/AppInit/Planificador.c 

OBJS += \
./src/AppInit/Conexion.o \
./src/AppInit/ConfigFunctions.o \
./src/AppInit/CreacionEstructurasMsj.o \
./src/AppInit/EntrenadoresInit.o \
./src/AppInit/EnvioMensajes.o \
./src/AppInit/HilosEntrenadores.o \
./src/AppInit/LogFunctions.o \
./src/AppInit/Planificador.o 

C_DEPS += \
./src/AppInit/Conexion.d \
./src/AppInit/ConfigFunctions.d \
./src/AppInit/CreacionEstructurasMsj.d \
./src/AppInit/EntrenadoresInit.d \
./src/AppInit/EnvioMensajes.d \
./src/AppInit/HilosEntrenadores.d \
./src/AppInit/LogFunctions.d \
./src/AppInit/Planificador.d 


# Each subdirectory must supply rules for building sources it contributes
src/AppInit/%.o: ../src/AppInit/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/tp-2020-1c-Fritsplank/Library/Library" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



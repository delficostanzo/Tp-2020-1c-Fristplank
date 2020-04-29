################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include Default/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Broker

dependents:
	-cd /home/utnso/Escritorio/tp-2020-1c-Fritsplank/Library/Library/Debug && $(MAKE) all

# Tool invocations
Broker: $(OBJS) $(USER_OBJS) /home/utnso/Escritorio/tp-2020-1c-Fritsplank/Library/Library/Debug/libLibrary.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L"/home/utnso/Escritorio/tp-2020-1c-Fritsplank/Library/Library/Debug" -o "Broker" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) Broker
	-@echo ' '

.PHONY: all clean dependents
/home/utnso/Escritorio/tp-2020-1c-Fritsplank/Library/Library/Debug/libLibrary.so:

-include ../makefile.targets

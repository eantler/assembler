################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../symbolsTable/symbolsTable.c 

OBJS += \
./symbolsTable/symbolsTable.o 

C_DEPS += \
./symbolsTable/symbolsTable.d 


# Each subdirectory must supply rules for building sources it contributes
symbolsTable/%.o: ../symbolsTable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -fPIC -std=c99 -O0 -g -pedantic -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



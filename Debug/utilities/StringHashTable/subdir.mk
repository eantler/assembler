################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/StringHashTable/StringHashTable.c 

OBJS += \
./utilities/StringHashTable/StringHashTable.o 

C_DEPS += \
./utilities/StringHashTable/StringHashTable.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/StringHashTable/%.o: ../utilities/StringHashTable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -fPIC -std=c99 -O0 -g -pedantic -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



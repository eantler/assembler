################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/StringHashTable/StringHashTable.c \
../utilities/StringHashTable/stringHashTableTester.c 

OBJS += \
./utilities/StringHashTable/StringHashTable.o \
./utilities/StringHashTable/stringHashTableTester.o 

C_DEPS += \
./utilities/StringHashTable/StringHashTable.d \
./utilities/StringHashTable/stringHashTableTester.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/StringHashTable/%.o: ../utilities/StringHashTable/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



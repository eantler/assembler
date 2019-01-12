################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/LinkedList/LinkedList.c \
../utilities/LinkedList/linkedListTester.c 

OBJS += \
./utilities/LinkedList/LinkedList.o \
./utilities/LinkedList/linkedListTester.o 

C_DEPS += \
./utilities/LinkedList/LinkedList.d \
./utilities/LinkedList/linkedListTester.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/LinkedList/%.o: ../utilities/LinkedList/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -fPIC -std=c99 -O0 -g -pedantic -Wall -c -fmessage-length=0 -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



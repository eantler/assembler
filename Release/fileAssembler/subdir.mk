################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fileAssembler/encoder.c \
../fileAssembler/fileAssembler.c 

OBJS += \
./fileAssembler/encoder.o \
./fileAssembler/fileAssembler.o 

C_DEPS += \
./fileAssembler/encoder.d \
./fileAssembler/fileAssembler.d 


# Each subdirectory must supply rules for building sources it contributes
fileAssembler/%.o: ../fileAssembler/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -pedantic -ansi -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



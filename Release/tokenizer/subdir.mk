################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tokenizer/tokenizer.c 

OBJS += \
./tokenizer/tokenizer.o 

C_DEPS += \
./tokenizer/tokenizer.d 


# Each subdirectory must supply rules for building sources it contributes
tokenizer/%.o: ../tokenizer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



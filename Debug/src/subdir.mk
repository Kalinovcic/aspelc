################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/expression.c \
../src/function.c \
../src/identifier.c \
../src/label.c \
../src/output.c \
../src/program.c \
../src/scanner.c \
../src/statement.c \
../src/struct.c \
../src/token.c \
../src/type.c \
../src/var.c 

OBJS += \
./src/expression.o \
./src/function.o \
./src/identifier.o \
./src/label.o \
./src/output.o \
./src/program.o \
./src/scanner.o \
./src/statement.o \
./src/struct.o \
./src/token.o \
./src/type.o \
./src/var.o 

C_DEPS += \
./src/expression.d \
./src/function.d \
./src/identifier.d \
./src/label.d \
./src/output.d \
./src/program.d \
./src/scanner.d \
./src/statement.d \
./src/struct.d \
./src/token.d \
./src/type.d \
./src/var.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/kalinovcic/workspace/aspel/aspelc/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



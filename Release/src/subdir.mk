################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/genetico.c \
../src/main.c \
../src/mem_structs.c \
../src/problema.c \
../src/rand.c 

OBJS += \
./src/genetico.o \
./src/main.o \
./src/mem_structs.o \
./src/problema.o \
./src/rand.o 

C_DEPS += \
./src/genetico.d \
./src/main.d \
./src/mem_structs.d \
./src/problema.d \
./src/rand.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/rand.o: ../src/rand.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -Im -O2 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/rand.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../my_drivers/Src/bno085.c \
../my_drivers/Src/debug_port.c 

OBJS += \
./my_drivers/Src/bno085.o \
./my_drivers/Src/debug_port.o 

C_DEPS += \
./my_drivers/Src/bno085.d \
./my_drivers/Src/debug_port.d 


# Each subdirectory must supply rules for building sources it contributes
my_drivers/Src/%.o my_drivers/Src/%.su my_drivers/Src/%.cyclo: ../my_drivers/Src/%.c my_drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Inc -I"/home/furkan/Projects/KuartisV1/KuartisHomeworkV1/my_drivers/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../ThreadSafe -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-my_drivers-2f-Src

clean-my_drivers-2f-Src:
	-$(RM) ./my_drivers/Src/bno085.cyclo ./my_drivers/Src/bno085.d ./my_drivers/Src/bno085.o ./my_drivers/Src/bno085.su ./my_drivers/Src/debug_port.cyclo ./my_drivers/Src/debug_port.d ./my_drivers/Src/debug_port.o ./my_drivers/Src/debug_port.su

.PHONY: clean-my_drivers-2f-Src


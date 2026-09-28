################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThreadSafe/newlib_lock_glue.c 

OBJS += \
./ThreadSafe/newlib_lock_glue.o 

C_DEPS += \
./ThreadSafe/newlib_lock_glue.d 


# Each subdirectory must supply rules for building sources it contributes
ThreadSafe/%.o ThreadSafe/%.su ThreadSafe/%.cyclo: ../ThreadSafe/%.c ThreadSafe/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F767xx -DSTM32_THREAD_SAFE_STRATEGY=4 -c -I../Inc -I"/home/furkan/Projects/KuartisV1/KuartisHomeworkV1/App/Inc" -I"/home/furkan/Projects/KuartisV1/KuartisHomeworkV1/my_drivers/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../ThreadSafe -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThreadSafe

clean-ThreadSafe:
	-$(RM) ./ThreadSafe/newlib_lock_glue.cyclo ./ThreadSafe/newlib_lock_glue.d ./ThreadSafe/newlib_lock_glue.o ./ThreadSafe/newlib_lock_glue.su

.PHONY: clean-ThreadSafe


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
WeatherStation.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
WeatherStation.list \

OBJCOPY_HEX += \
WeatherStation.hex \

OBJCOPY_BIN += \
WeatherStation.bin \


# All Target
all: WeatherStation.elf secondary-outputs

# Tool invocations
WeatherStation.elf: $(OBJS) $(USER_OBJS) C:\Users\vladyslavN\Documents\Projects\KievWeather\WeatherStationSTM32IDE\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "WeatherStation.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\vladyslavN\Documents\Projects\KievWeather\WeatherStationSTM32IDE\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="WeatherStation.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

WeatherStation.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "WeatherStation.list"
	@echo 'Finished building: $@'
	@echo ' '

WeatherStation.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "WeatherStation.hex"
	@echo 'Finished building: $@'
	@echo ' '

WeatherStation.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "WeatherStation.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

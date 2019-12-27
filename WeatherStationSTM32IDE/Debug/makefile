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
WeatherStationSTM32IDE.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
WeatherStationSTM32IDE.list \

OBJCOPY_HEX += \
WeatherStationSTM32IDE.hex \

OBJCOPY_BIN += \
WeatherStationSTM32IDE.bin \


# All Target
all: WeatherStationSTM32IDE.elf secondary-outputs

# Tool invocations
WeatherStationSTM32IDE.elf: $(OBJS) $(USER_OBJS) C:\Users\vladyslavN\Documents\Projects\KievWeather\WeatherStationSTM32IDE\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "WeatherStationSTM32IDE.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\vladyslavN\Documents\Projects\KievWeather\WeatherStationSTM32IDE\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="WeatherStationSTM32IDE.map" -Wl,--gc-sections -static -u _printf_float --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

WeatherStationSTM32IDE.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "WeatherStationSTM32IDE.list"
	@echo 'Finished building: $@'
	@echo ' '

WeatherStationSTM32IDE.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "WeatherStationSTM32IDE.hex"
	@echo 'Finished building: $@'
	@echo ' '

WeatherStationSTM32IDE.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "WeatherStationSTM32IDE.bin"
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

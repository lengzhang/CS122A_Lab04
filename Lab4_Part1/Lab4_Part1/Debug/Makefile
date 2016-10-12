################################################################################
# Automatically-generated file. Do not edit!
################################################################################


RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../croutine.c \
../heap_1.c \
../list.c \
../main.c \
../port.c \
../queue.c \
../tasks.c \
../timers.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
croutine.o \
heap_1.o \
list.o \
main.o \
port.o \
queue.o \
tasks.o \
timers.o

OBJS_AS_ARGS +=  \
croutine.o \
heap_1.o \
list.o \
main.o \
port.o \
queue.o \
tasks.o \
timers.o

C_DEPS +=  \
croutine.d \
heap_1.d \
list.d \
main.d \
port.d \
queue.d \
tasks.d \
timers.d

C_DEPS_AS_ARGS +=  \
croutine.d \
heap_1.d \
list.d \
main.d \
port.d \
queue.d \
tasks.d \
timers.d

OUTPUT_FILE_PATH +=Lab4_Part1.elf

OUTPUT_FILE_PATH_AS_ARGS +=Lab4_Part1.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler

















./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Users\user\Documents\Software\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Users\user\Documents\Software\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\include" -I"\\Mac\Dropbox\UCRiverside\2016Fall\CS122A\FreeRTOS_Lab\FreeRTOS\Source" -I"\\Mac\Dropbox\UCRiverside\2016Fall\CS122A\FreeRTOS_Lab\FreeRTOS\Source\include" -I"\\Mac\Dropbox\UCRiverside\2016Fall\CS122A\FreeRTOS_Lab\FreeRTOS\Source\portable\GCC\ATMega323"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega1284 -B "C:\Users\user\Documents\Software\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega1284" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Users\user\Documents\Software\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Lab4_Part1.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atmega1284 -B "C:\Users\user\Documents\Software\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.1.130\gcc\dev\atmega1284"  
	@echo Finished building target: $@
	"C:\Users\user\Documents\Software\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Lab4_Part1.elf" "Lab4_Part1.hex"
	"C:\Users\user\Documents\Software\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Lab4_Part1.elf" "Lab4_Part1.eep" || exit 0
	"C:\Users\user\Documents\Software\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "Lab4_Part1.elf" > "Lab4_Part1.lss"
	"C:\Users\user\Documents\Software\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Lab4_Part1.elf" "Lab4_Part1.srec"
	"C:\Users\user\Documents\Software\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "Lab4_Part1.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Lab4_Part1.elf" "Lab4_Part1.a" "Lab4_Part1.hex" "Lab4_Part1.lss" "Lab4_Part1.eep" "Lab4_Part1.map" "Lab4_Part1.srec" "Lab4_Part1.usersignatures"
	
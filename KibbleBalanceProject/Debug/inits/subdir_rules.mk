################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
inits/inits.obj: ../inits/inits.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.6.LTS/include" --include_path="C:/ti/TivaWare_C_Series-2.1.2.111" -g --gcc --define=ccs="ccs" --define=UART_BUFFERED --define=PART_TM4C123GH6PM --display_error_number --diag_wrap=off --diag_warning=225 --abi=eabi --preproc_with_compile --preproc_dependency="inits/inits.d_raw" --obj_directory="inits" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



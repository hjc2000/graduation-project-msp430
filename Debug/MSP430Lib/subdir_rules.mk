################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
MSP430Lib/%.obj: ../MSP430Lib/%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="C:/ti/ccs1100/ccs/ccs_base/msp430/include" --include_path="D:/my_files/workspace/CCSworkspace/GraduateProject" --include_path="D:/my_files/workspace/CCSworkspace/GraduateProject/driverlib/MSP430F5xx_6xx" --include_path="C:/ti/ccs1100/ccs/tools/compiler/ti-cgt-msp430_21.6.0.LTS/include" --advice:power="none" --define=__MSP430F5529__ --define=DEPRECATED -g --c11 --rtti --cpp_default --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="MSP430Lib/$(basename $(<F)).d_raw" --obj_directory="MSP430Lib" --exceptions $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct F:\FPGA_Project\Vitis_Project\Sonar_SV_SSD\platform.tcl
# 
# OR launch xsct and run below command.
# source F:\FPGA_Project\Vitis_Project\Sonar_SV_SSD\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {Sonar_SV_SSD}\
-hw {F:\FPGA_Project\SonarProjectSV\SonarProjectSV_top.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {F:/FPGA_Project/Vitis_Project}

platform write
platform generate -domains 
platform active {Sonar_SV_SSD}
platform generate

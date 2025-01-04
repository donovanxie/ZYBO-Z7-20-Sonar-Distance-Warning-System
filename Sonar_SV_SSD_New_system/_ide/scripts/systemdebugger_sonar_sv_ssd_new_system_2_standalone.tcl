# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: F:\FPGA_Project\Vitis_Project\Sonar_SV_SSD_New_system\_ide\scripts\systemdebugger_sonar_sv_ssd_new_system_2_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source F:\FPGA_Project\Vitis_Project\Sonar_SV_SSD_New_system\_ide\scripts\systemdebugger_sonar_sv_ssd_new_system_2_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351B482F4A" && level==0 && jtag_device_ctx=="jsn-Zybo Z7-210351B482F4A-23727093-0"}
fpga -file F:/FPGA_Project/Vitis_Project/Sonar_SV_SSD_New/_ide/bitstream/SonarProjectSV_top.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw F:/FPGA_Project/Vitis_Project/Sonar_SV_SSD/export/Sonar_SV_SSD/hw/SonarProjectSV_top.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source F:/FPGA_Project/Vitis_Project/Sonar_SV_SSD_New/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow F:/FPGA_Project/Vitis_Project/Sonar_SV_SSD_New/Debug/Sonar_SV_SSD_New.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con

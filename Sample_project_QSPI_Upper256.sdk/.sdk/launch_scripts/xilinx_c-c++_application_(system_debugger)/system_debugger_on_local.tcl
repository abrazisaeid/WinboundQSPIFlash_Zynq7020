connect -url tcp:127.0.0.1:3121
source D:/All/Software/ZZ-Hardware_Old/Sample_Code_Alghasi/Sample_project_QSPI_Upper256/Sample_project_QSPI_Upper256.sdk/hw_wrapper_hw_platform_0/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-SMT1 210203859289A"} -index 0
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent JTAG-SMT1 210203859289A" && level==0} -index 1
fpga -file D:/All/Software/ZZ-Hardware_Old/Sample_Code_Alghasi/Sample_project_QSPI_Upper256/Sample_project_QSPI_Upper256.sdk/hw_wrapper_hw_platform_0/hw_wrapper.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-SMT1 210203859289A"} -index 0
loadhw -hw D:/All/Software/ZZ-Hardware_Old/Sample_Code_Alghasi/Sample_project_QSPI_Upper256/Sample_project_QSPI_Upper256.sdk/hw_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent JTAG-SMT1 210203859289A"} -index 0
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent JTAG-SMT1 210203859289A"} -index 0
dow D:/All/Software/ZZ-Hardware_Old/Sample_Code_Alghasi/Sample_project_QSPI_Upper256/Sample_project_QSPI_Upper256.sdk/QSPI_Test/Debug/QSPI_Test.elf
configparams force-mem-access 0
bpadd -addr &main

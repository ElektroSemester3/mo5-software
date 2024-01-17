# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\lscho\OneDrive\Documents\GitHub\mo5-software\custom_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\lscho\OneDrive\Documents\GitHub\mo5-software\custom_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {custom_wrapper}\
-hw {C:\Users\lscho\OneDrive\Documents\GitHub\mo5-software-wrapper\custom_wrapper.xsa}\
-out {C:/Users/lscho/OneDrive/Documents/GitHub/mo5-software}

platform write
domain create -name {standalone_ps7_cortexa9_0} -display-name {standalone_ps7_cortexa9_0} -os {standalone} -proc {ps7_cortexa9_0} -runtime {cpp} -arch {32-bit} -support-app {hello_world}
platform generate -domains 
platform active {custom_wrapper}
domain active {zynq_fsbl}
domain active {standalone_ps7_cortexa9_0}
platform generate -quick
platform generate
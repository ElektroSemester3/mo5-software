# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\lscho\OneDrive\Documents\GitHub\mo5-software\system_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\lscho\OneDrive\Documents\GitHub\mo5-software\system_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {system_wrapper}\
-hw {C:\Users\lscho\OneDrive\Documents\GitHub\mo5-software-wrapper\custom_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -out {C:/Users/lscho/OneDrive/Documents/GitHub/mo5-software}

platform write
platform generate -domains 
platform active {system_wrapper}
platform generate
catch {platform remove system_wrapper}
platform write

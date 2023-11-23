# Init the project
1. Open Vitis
2. Create a new Workspace
3. Go to File -> Import -> Import projects from Git -> Existing local repository -> Add -> Browse -> Select the folder where you cloned the project 
4. Import existing Eclipse projects -> Select only `selfdriving-car` -> Next
5. Select `selfdriving-car` -> Finish
6. Go to File -> New -> Platform Project -> Name it `wrapper` -> In 'Hardware Specification' select 'Browse' -> Select `system_wrapper/hw/system_wrapper.xsa` wrapper file -> Finish
7. Go to `selfdriving-car_system/selfdriving-car.prj`
8. Edit the platform to `wrapper`

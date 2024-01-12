# Init the project
1. Open Vitis
2. Create a new Workspace
3. Go to File -> Import -> Import projects from Git -> Existing local repository -> Add -> Browse -> Select the folder where you cloned the project 
4. Import existing Eclipse projects -> Select only `selfdriving-car` -> Next
5. Select `selfdriving-car` -> Finish
6. Go to File -> New -> Platform Project -> Name it `system_wrapper` -> In 'Hardware Specification' select 'Browse' -> Select `custom_wrapper.xsa` wrapper file -> Finish
7. Go to `selfdriving-car_system/selfdriving-car.prj`
8. Edit the platform to `system_wrapper`

# Assigned hardware pins
|IO | Arduino header pin | Description |
| --- | --- | --- |
| Y11 | A0 | Motor driver output 0 |
| Y12 | A1 | Motor driver output 1 |
| W11 | A2 | Motor driver output 2 |
| V11 | A3 | Motor driver output 3 |
| T14 | AR0 | Lijn herkenning input 0 |
| U12 | AR1 | Lijn herkenning input 1 |
| U13 | AR2 | Lijn herkenning input 2 |
| V13 | AR3 | Lijn herkenning input 3 |
| V15 | AR4 | Lijn herkenning input 4 |
| T15 | AR5 | Lijn herkenning input 5 |
| R16 | AR6 | PWM Motor Right |
| U17 | AR7 | PWM Motor Left |
| V17 | AR8 | Speed sensor input 0 |
| V18 | AR9 | Speed sensor input 1 |
| T16 | AR10 | Speed sensor input 2 |
| R17 | AR11 | Speed sensor input 3 |
| P18 | AR12 | Obstakeldetectie input 0 |
| N17 | AR13 | Obstakeldetectie input 1 |
| P15 | SCL | Obstakeldetectie I2C SCL |
| P16 | SDA | Obstakeldetectie I2C SDA |
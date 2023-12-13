# Init the project
1. Open Vitis
2. Create a new Workspace
3. Go to File -> Import -> Import projects from Git -> Existing local repository -> Add -> Browse -> Select the folder where you cloned the project 
4. Import existing Eclipse projects -> Select only `selfdriving-car` -> Next
5. Select `selfdriving-car` -> Finish
6. Go to File -> New -> Platform Project -> Name it `wrapper` -> In 'Hardware Specification' select 'Browse' -> Select `custom_wrapper.xsa` wrapper file -> Finish
7. Go to `selfdriving-car_system/selfdriving-car.prj`
8. Edit the platform to `wrapper`


# Assigned hardware pins
| Arduino header pin | Description |
| --- | --- |
| AR0 | Motor driver output 0 |
| AR1 | Motor driver output 1 |
| AR2 | Motor driver output 2 |
| AR3 | Motor driver output 3 |
| AR4 | PWM Motor Right |
| AR5 | PWM Motor Left |
| A0 | Lijn herkenning input 0 |
| A1 | Lijn herkenning input 1 |
| A2 | Lijn herkenning input 2 |
| A3 | Lijn herkenning input 3 |
| A4 | Lijn herkenning input 4 |
| A5 | Lijn herkenning input 5 |
| AR12 | Obstakeldetectie input 0 |
| AR13 | Obstakeldetectie input 1 |
| SCL | Obstakeldetectie I2C SCL |
| SDA | Obstakeldetectie I2C SDA |
| AR8 | Speed sensor input 0 |
| AR9 | Speed sensor input 1 |
| AR10 | Speed sensor input 2 |
| AR11 | Speed sensor input 3 |
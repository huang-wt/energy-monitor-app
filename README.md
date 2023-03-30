# COMP0016 2022-23 Project: Linux Energy Monitor

## Project Introduction
It is an Industry Exchange Network project in partnership with Intel, aiming to develop a Linux energy monitor application that enables users to view real-time system information and energy consumption while taking advantages of Intel's 12th Gen processor architecture.

## Project Tree
```txt
.
├── src/        # The project source code
├── include/    # Headers
├── forms/      # UI forms
├── docs/       # Documentation files
├── resources/  # Styling sheets and icons
└──...
```

## Deployment Manual
### Prerequisites
#### Technologies
- Git
- Linux
- C++ 17 (and onwards)
- Qt (include qmake and Charts module) 
- 12th Gen Intel Processor
#### Qt Installation (take Ubuntu for example)
1. Update the package list by running the following command:
```shell
sudo apt-get update
```
2. install the Qt development libraries by running the following command
```shell
sudo apt-get install qt5-default
```
3. Update the package list by running the following command:
```shell
sudo apt-get install libqt5charts5 libqt5charts5-dev
```
4. Verify the installation by running the following command:
```shell
qmake -v
```
This should display the version of qmake and the version of Qt being used.

#### System Permission
Unlock permissions for accessing the energy counter
```shell
sudo chmod 777 /sys/class/powercap/intel-rapl/intel-rapl:1/energy_uj
```

### Installation
```shell
git clone https://github.com/huang-wt/energy-monitor-app.git
cd energy-monitor-app
```

### Build and Run
```shell
mkdir build && cd build
qmake ..
make
./energy_monitor
```

### Test

## Showcase

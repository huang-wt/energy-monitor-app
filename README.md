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
- Qt
- CMake
- 12th Gen Intel Processor
#### System Permission
```shell
# Unlock permissions for reading and writing the following files
sudo chmod 777 /sys/class/powercap/intel-rapl/intel-rapl:1/energy_uj
sudo chmod 777 /sys/class/powercap/intel-rapl/intel-rapl:1/max_energy_range_uj
```

### Installation
```shell
git clone https://github.com/huang-wt/energy-monitor-app.git
cd energy-monitor-app
```

### Build and Run
```shell
mkdir build && cd build
cmake ..
make
./energy_monitor
```

### Test

## Showcase

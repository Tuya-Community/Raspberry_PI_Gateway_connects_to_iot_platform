## Overview

Raspberry Pi gateway can collect status data of RS-485 temperature and humidity sensors and door sensors and report data to the cloud. After receiving commands from the cloud, Raspberry Pi gateway will control the relay action accordingly. After command execution, the relay will return the current state to the cloud through the gateway.

## Get started

### Prerequisites

Ubuntu and Debian

```sh
sudo apt-get install make cmake libqrencode-dev
```

### Compile the code

```sh
mkdir build && cd build
cmake ..
make
```

### Run the demo

```sh
./bin/data_model_basic_demo
```

## Other

Two demos are used to build a composite Raspberry Pi gateway. One demo (in C) works for interaction with the [Tuya IoT Development Platform](https://iot.tuya.com/). The other demo (in Python) runs the program used to collect data from the RS-485 devices.

Download the demo (in C) at: https://github.com/Tuya-Community/Raspberry_PI_Gateway_connects_to_iot_platform.git

Download the demo (in Python) at: https://github.com/Tuya-Community/485_sub-devices_connect_to_the_Raspberry_PI_gateway.git



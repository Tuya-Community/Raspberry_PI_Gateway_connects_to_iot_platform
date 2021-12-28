
## Overview
The status data of 485 temperature and humidity sensor and 485 door status sensor are collected by the raspberry PI gateway and reported to the cloud;  At the same time, after receiving the command from the cloud,raspberry PI gateway will control the relay action, and the relay will return the current state to the cloud.  


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

## other

```c
The demo(C) interacts with the Internet of Things platform, and the other demo(Python) is the 485 subdevice acquisition program. Together, the two demos are used to build the Raspberry PI gateway.  
This demo download address:https：//github.com/Tuya-Community/Raspberry_PI_Gateway_connects_to_iot_platform.git
Another demo that communicates with 485 sub-devices (Python) download address：https://github.com/Tuya-Community/485_sub-devices_connect_to_the_Raspberry_PI_gateway.git
Reference Document Address：https://blog.csdn.net/sandwich_iot/article/details/122194669
```


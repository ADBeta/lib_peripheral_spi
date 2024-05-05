# lib_peripheral_i2c
This library is part of the `lib_peripheral_` series of libraries.  
It provides a highly simplified API to use basic SPI functionality for any
device supported in esp-idf.  

This Library is not very fully integrated yet, and has a lot of improvment
to be done

----
## TODO
* Automatically switch DMA mode if the in/out data is small enough
* Clean up code drastically
* Add more support for address/command bytes
* Add support for selecting half/full duplex in one driver (ESP driver is fucked)
* Add support for callbacks

----
This project is under the GPL-2.0 License, see LICENSE for more information.  
ADBeta (c) 2024

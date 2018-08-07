# Introduction

The USB-C Explorer is a development board with everything needed to start working with USB Type-C. It contains a USB-C port controller and Power Delivery PHY chip, a microcontroller, and several options for user interaction. Here are the hardware features.

* [SAMD21E17A](https://www.microchip.com/wwwproducts/en/ATSAMD21E17) microcontroller
* [FUSB302B](http://www.onsemi.com/PowerSolutions/product.do?id=FUSB302B) USB PD PHY
* OLED Display
* Capacitive-sense slider
* Power header (up to 5 Amps)
* Debug port (UART and/or GPIO)
* SWD connector (Tag-Connect)
* Reset and User Buttons
* Onboard regulator, input up to 20 V.

# More Detail

The default firmware will identify as a USB Power Delivery Sink and list out all power capabilities from a corresponding Source. It will then request the highest power option. This voltage will be available on the power header. 

The default firmware also includes a USB bootloader. If you hold the User Button during reset, it will show up as a USB Mass Storage Device with a single file "FLASH.BIN". Copy the file to inspect the application firmware. Delete it and replace it with a new .bin file to rewrite the firmware. No programming cable is required.

To use the Single Wire Debug (SWD) function for live debugging, you will need a Tag-Connect TC2050-IDC-NL "plug of nails" cable and clip. You can then use an adapter to something like a Segger or J-Link.

This design is open source under an MIT license. All hardware designs, documentation, and firmware are freely available. You can find all design files at this github repository. I welcome feedback, so please let me know what you think. In particular, help with firmware would be appreciated. 

# Where to Buy

You can purchase a board [here](https://www.tindie.com/products/ReclaimerLabs/usb-c-explorer/). 

# Questions, Comments, and Contributions

Pull requests are welcome. If you have questions or comments, you can email me directly at jason@reclaimerlabs.com. 

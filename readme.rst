Overview
========




Howto
=====

On Linux
--------

Tested on Linux Mint 18.2


Preparing the environment
~~~~~~~~~~~~~~~~~~~~~~~~~

#. ``sudo apt-get install gcc-avr binutils-avr avr-libc dfu-programmer``
#. ``wget http://www.github.com/abcminiuser/lufa/archive/LUFA-120219.zip``  Note that there are newer versions too, but the latest one, ``https://codeload.github.com/abcminiuser/lufa/zip/LUFA-170418``, did *not* compile successfully!
#. Extract the archive somewhere and get into that directory, then

	- run ``make`` and wait until everything is compiled, it will take a while.
	- ``cd Demos/Device`` and run ``make`` again; this will compile a lot of firmware samples, producing many ``.hex`` files to play with

Building
~~~~~~~~

TODO

Deploying
~~~~~~~~~

Assuming you have a ``.hex`` file at hand, upload it to the board using the following steps:

#. press ``RST`` and ``HWB`` simultaneously on the board
#. release ``RST`` then release ``HWB``
#. run ``lsusb`` and ensure that it is listed as ``Bus 001 Device 008: ID 03eb:2ffb Atmel Corp. at90usb AVR DFU bootloader`` - if that is the case, you can upload the firmware itself
#. ``sudo dfu-programmer at90usb1287 erase``
#. ``sudo dfu-programmer at90usb1287 flash bin/usbone.hex`` - substitute the path to the firmware with the file you want to upload. ``usbone.hex`` was generated using ATMEL Studio, see the Windows approach below.
#. ``sudo dfu-programmer at90usb1287 reset``
#. press ``RST``, this will reboot the board using the new firmware. Depending on the firmware, it may or may not show up in ``lsusb``, keep that in mind.




On Windows, with ATMEL Studio
-----------------------------

#. Start ``ATMEL Studio``
#. ``New\Project\GCC Executable``
#. Select the board type using the filter, best guess = ``AT90USB1287``



Building and deploying
~~~~~~~~~~~~~~~~~~~~~~

First, you have to produce the binary itself:

#. Hit ``F7`` to build it and
#. Get the binary from ``usbone/Debug/usbone.hex`` - this is the resulting firmware

Then it has to be uploaded onto the board:

#. ``./dfu-programmer at90usb1287 get bootloader-version`` (doesn't work yet)



Questions
=========

Dead ends
---------

This is a set of questions that I asked myself and had no meaningful answer to.

#. What toolset to retrieve from this site? http://www.atmel.com/tools/ATMELAVRTOOLCHAINFORWINDOWS.aspx
#. Is it 8-bit or 32-bit? (appears to be 8-bit according to https://en.wikipedia.org/wiki/Atmel_AVR#AT90USBKey)
#. Which Atmel Studio template to use (see this list http://start.atmel.com/#project)?




References
==========

- http://mcqn.com/posts/running-lufa-on-an-at90usbkey2-from-ubuntu/
- http://www.ssalewski.de/AT90USB_firmware.html.en
- http://www.avrfreaks.net/forum/at90usb-microcontroller-and-avr-studio-6-questions - practical tip on how to get started using ATMEL Studio for this particular type of board
- http://www.fourwalledcubicle.com/files/LUFA/Doc/120219/html/_page__getting_started.html - LUFA, getting started
- http://therandombit.blogspot.se/2009/08/hello-world-getting-started-with-atmel.html - another LUFA basic tutorial
- https://github.com/theRandomBit/trb-at90usb - very basic projects related to the tutorials from the site above
- http://www.technoburst.net/2013/04/atmel-dfu-programmer-tutorial-for-linux.html - ``dfu-programmer`` sample calls
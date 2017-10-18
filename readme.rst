Overview
========




Howto
=====

On Windows, with ATMEL Studio
-----------------------------

#. Start ``ATMEL Studio``
#. ``New\Project\GCC Executable``
#. Select the board type using the filter, best guess = ``AT90USB1287``


Building instructions
---------------------



Uploading the firmware to the board
-----------------------------------




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
Overview
========

Simple application for the AT90USBKEY2 board, it is meant to be a USB device with a custom set of commands, for subsequent use as target hardware for a Linux driver.


Proposed features
=================

- read the temperature
- control each of the 2 LEDs on the board


Architecture
------------

The device operates in a single *configuration*, that exposes two *interfaces*, one for the temperature sensor and one for the LEDs (both of them). Note that it is also technically possible to have each of the LEDs assigned to a separate endpoint, and have a common interface for them. However, at this stage it seems that there would be no benefits to that, so we take the approach that involves juggling with fewer entities (i.e. 2 interfaces and 2 endpoints vs 2 interfaces and 3 endpoints).


::

	+---------------+------------------------+
	|   Endpoint 0  |                        |
	|   (standard)  |  USB                   |
	+---------------+  device                |
	                |                        |
	+---------------+      +---------------+ |
	|   Endpoint 1  <------|   Temperature | |
	|   (read temp) |      |     sensor    | |
	+---------------+      +---------------+ |
	                |                        |
	+---------------+            +------+    |
	|   Endpoint 2  <------------> LED1 |    |
	|   (LED ctrl)  <-------+    |      |    |
	+---------------+       |    +------+    |
	                |       |                |
	                |       |    +------+    |
	                |       +----> LED2 |    |
	                |            +------+    |
	                +------------------------+



+-------------+----------------------+-------------+----------------------------------------+
| Endpoint #  | Purpose              | Type        | Reasoning                              |
+=============+======================+=============+========================================+
| 0           | Standard             |             | It has to be there by definition;it is |
|             |                      |             | outside of scope.                      |
+-------------+----------------------+-------------+----------------------------------------+
| 1           | Temperature readings | Isochronous | The payload is ``<1024 bytes`` and we  |
|             |                      |             | don't care if every now and then a     |
|             |                      |             | temperature reading is lost. (I see    |
|             |                      |             | this as ``UDP``, at least for now).    |
+-------------+----------------------+-------------+----------------------------------------+
| 2           | LED control          | Control     | Appropriate for commands issued every  |
|             |                      |             | now and then.                          |
+-------------+----------------------+-------------+----------------------------------------+

*Interrupt* and *bulk* endpoints are not necessary for now, as there are no device-initiated actions, nor large data transfers. However, they may be added in the future.


Howto
=====

On Linux
--------

Tested on Linux Mint 18.2


Preparing the environment for LUFA
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. ``sudo apt-get install gcc-avr binutils-avr avr-libc dfu-programmer``
#. ``git clone git@github.com:abcminiuser/lufa.git``
#. ``cd lufa/Demos/Device`` then ``make`` to build the firmware samples. It will take a while.

As a result you get many ``.hex`` files to play with.

.. NOTE::

	I first tried it with ``wget http://www.github.com/abcminiuser/lufa/archive/LUFA-120219.zip``, running ``make`` inside this directory (after unzipping) successfully builds something else (not just the demos). But it is not yet clear whether this is relevant or not.



Building
~~~~~~~~

The path of minimum resistance is to do this within the ``LUFA`` directory structure, after cloning the repository. Make a copy of the ``BulkVendor`` directory and do your changes there. Run ``make`` to produce new firmware.

PID/VID for device identification
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

These are defined in ``Descriptors.c``, line 57 and 58.



Building without LUFA
~~~~~~~~~~~~~~~~~~~~~

If USB-related functionality is not desired, a minimalist project can be taken from https://github.com/theRandomBit/trb-at90usb/blob/master/1.light.

Assuming you've done the first step from the LUFA environment section, you'll be able to ``make`` this project and produce a firmware binary. It is the same as the one produced by ATMEL Studio, but without all the bells-and-whistles/overhead of the IDE for Windows.


Deploying
~~~~~~~~~

Assuming you have a ``.hex`` file at hand, upload it to the board using the following steps:

#. press ``RST`` and ``HWB`` simultaneously on the board
#. release ``RST`` then release ``HWB``
#. run ``lsusb`` and ensure that it is listed as ``Bus 001 Device 008: ID 03eb:2ffb Atmel Corp. at90usb AVR DFU bootloader`` - if that is the case, you can upload the firmware itself
#. ``sudo dfu-programmer at90usb1287 erase``
#. ``sudo dfu-programmer at90usb1287 flash bin/usbone.hex`` - substitute the path to the firmware with the file you want to upload. ``usbone.hex`` was generated using ATMEL Studio, see the Windows approach below.
#. ``sudo dfu-programmer at90usb1287 reset`` to reset the device and make it load the new firmware.
#. OR press ``RST`` on the board itself, to achieve the same effect.

.. NOTE::
	Depending on the firmware, it may or may not show up in ``lsusb``, keep that in mind.


Testing
~~~~~~~

For the ``BulkVendor`` demo, the USB package for Python ``sudo apt-get install python-usb`` then run ``python test_bulk_vendor.py`` (tested, but fails for now).



On Windows, with ATMEL Studio
-----------------------------

#. Start ``ATMEL Studio``
#. ``New\Project\GCC Executable``
#. Select the board type using the filter, choose ``AT90USB1287``



Building and deploying
~~~~~~~~~~~~~~~~~~~~~~

First, you have to produce the binary itself:

#. Hit ``F7`` to build it and
#. Get the binary from ``usbone/Debug/usbone.hex`` - this is the resulting firmware

Then it has to be uploaded onto the board. This was only done and successfully tested on Linux. The procedure should be the same, as long as ``dfu-programmer`` works.



Questions
=========

#. Is ``transfer`` the same as ``endpoint``?
#. "When a device is enumerated, the host reads the device descriptors and can make a decision of which configuration to enable. It can only enable one configuration at a time." Should one then map all features to a single endpoint? Or is it irrelevant, because we can put them all under different *interface descriptors*, of which more than 1 can be active at once.
#. Why disable the watchdog? What does the watchdog watch? (line 77 in the BulkVendor sample)

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



Other remarks
=============

- Data on the USBus is transmitted LSB first.




Kernel debugging
================

This section explains how to attach ``gdb`` to a target system, via a serial port, in order to debug kernel code running on it.

Tested on:

- host = Linux Mint 18.2 x64, running on Surface Pro 3 with 8GB RAM and a core i7 CPU
- target = Debian 9 x64

Ingredients
-----------

- *host* - a Linux machine, with Virtualbox, this is where our debugger is running.
- *target* - a Linux virtual machine running inside the host, this is the debuggee, i.e. we attach to it with the debugger running on the host.


Prerequisites
-------------

Guest
~~~~~

Install *Virtualbox guest additions* into the target VM.

#. Start VirtualBox and run the guest
#. Devices\Insert Guest Additions CD Image
#. Mount the CD-ROM ``mount /dev/cdrom /media/cdrom``
#. ``apt-get install dkms build-essential linux-headers-$(uname -r)``
#. Run ``/media/cdrom/VBoxLinuxAdditions.run``

The next phase is to build the kernel from source. These instructions are derived from the Debian handbook: https://kernel-handbook.alioth.debian.org/ch-common-tasks.html#s-common-official

#. Run ``uname -a`` and store the current output, e.g. ``Linux panic 4.9.0-4-amd64 #1 SMP Debian 4.9.51-1 (2017-09-28) x86_64 GNU/Linux``, you'll refer to this later to compare what kernel was loaded before and after all the steps below.
#. ``apt-get install libncurses5-dev bc devscripts``
#. ``apt-get source linux``
#. ``cd <directory where the kernel is>``, e.g. ``cd linux-4.9.51``
#. ``make localyesconfig`` and use the default settings by pressing Enter, if prompted. This ensures that you build the kernel only with the modules that are actually used by your system (thus minimizing build time)
#. ``make nconfig``
#. In ``kernel hacking`` enable ``KGDB`` and ``debug boot parameters``. Note that building the kernel with debug symbols is also required, but this option is enabled by default in *Compiler settings\compile time checks and compiler options*
#. It may help if you look around the sections and check what other modules are enabled, as ``make localyesconfig`` is not perfect. For example, I found *Graphic cards\Radeon* was enabled, although I don't have such a card, so I disabled it. Beware that ``localyesconfig`` hinders replicability, as it will detect different modules on different systems, so YMMV.
#. Press ``F6`` to save the configuration (leave the default name as ``.config``)
#. ``make clean``
#. Before you run the next step, **make sure you have around 20 GB of free space**. If you're running on battery power or have some commitments, be aware that the process will take **several hours**, depending on your hardware!
#. If you're using external drives, especially USB-powered ones, make sure your host OS will not spin them down after the screensaver kicks in. Basically, you have to anticipate what might interrupt the build process, and ensure that won't happen.
#. ``make -j8 deb-pkg`` - this initiates the kernel build process, it will take a *long time*. As a result you'll get a ``.deb`` that can be installed. Note the ``-j8`` parameter, it indicates that the build process is done by 8 processes in parallel, adjust this as you see fit, depending on your hardware capabilities (this should speed up the process a bit). The value could be chosen as the number of cores times ``1.5``; you can play with it and see empirically what works best for you.
#. ``dpkg -i <name of output>.deb`` to install the freshly built kernel, it is stored in the home directory, e.g. ``dpkg -i ../*.deb``

During the installation, ``grub``'s configuration is also updated, and at the next reboot the new kernel will be used. You can check that with ``uname -a``, the output will include a fresh timestamp that reflects the time when the loaded kernel was built, e.g. ``Linux panic 4.9.51 #2 SMP Wed Dec 6 11:45:26 CET 2017 x86_64 GNU/Linux`` (compare it with the one you got at the first step)

Host
~~~~

For convenience, if you use a NAT network for the VM (which is so by default), add a port-forwarding rule, so you can connect to the SSH server on the guest from your host. This is done via the ``Settings`` menu of your VM, go to ``Networking\Advanced`` and add a rule that forwards ``host:2222 -> guest:22``, the IP of the guest can be obtained from within the VM itself with ``ip address``. After that is done, use an SSH client, e.g. ``ssh developer@127.0.0.1 -p 2222 -o pubkeyauthentication=false`` to get inside.


#. Install the following packages, e.g. ``sudo apt-get install socat``
#. Create a new VM and install Debian in it, this is a typical installation, there are no special steps here, however:

	- you need to make sure you have plenty of free space, because the kernel building process will consume a lot of space. 25..30 GB should be enough.
	- TODO determine if 64-bit targets will work or not

#. Configure the VM to have a serial port

	- set ``Port mode`` = ``Host pipe``
	- check ``Create pipe``
	- set the path to ``/tmp/kerneldebug`` (or whatever you prefer, as long as you remember it later)
	- make sure ``Connect to existing pipe/socket`` is unchecked, i.e. Virtualbox will create it automatically

#. UNNECESSARY Start ``socat`` before powering up the virtual machine (otherwise it will complain that the pipe doesn't exist yet) as follows: ``socat -d -d -d /tmp/kerneldebug pty,wait-slave,link=/dev/hz``
#. Change the permissions to ``/tmp/kerneldebug`` such that the user who runs virtualbox can interact with it.


The VM settings used in this experiment were:

- Chipset set to ``PIIX3``
- 4 GB RAM
- 2 CPUs with a 95% execution cap
- VT-x/AMD-V enabled
- ``Nested paging`` enabled
- 13MB of video memory
- 40 GB virtual hard drive (stored on an external drive, connected via USB3 to the host OS)

The build process took under 1h.



An actual debugging session
---------------------------

Star the guest and boot it as usual.

You need access to the kernel sources that are inside the VM. An easy way to do that is via ``sshfs``, mounting the remote drive to your local file system. You can install the tool with ``sudo apt-get install sshfs``. Once it is done::

	mkdir /tmp/kernelsrc
	sudo sshfs -o allow_other developer@127.0.0.1:/ /tmp/kernelsrc -p 2222

This will mount the remote file system to a local mount point. Adjust paths and port numbers accordingly (consider using an *SSH config* file to make this easier). Note that you can unmount the volume using ``sudo umount /tmp/kernelsrc``.


#. Run socat on the host: ``sudo socat -d -d /tmp/kerneldebug pty`` and leave it running in the background. Note that the supplied path, ``/tmp/kerneldebug`` must match the path given in the properties of the virtual machine. Once started, pay attention to the path of the created device file, in the output below it is ``/dev/pts/3``::

	2017/12/13 22:35:46 socat[17218] N opening connection to AF=1 "/tmp/kerneldebug"
	2017/12/13 22:35:46 socat[17218] N successfully connected from local address AF=1 "\xEE\xEE\xEE\xEE\xEE\xEE"
	2017/12/13 22:35:46 socat[17218] N successfully connected via <anon>
	2017/12/13 22:35:46 socat[17218] N PTY is /dev/pts/3
	2017/12/13 22:35:46 socat[17218] N starting data transfer loop with FDs [5,5] and [6,6]
 
#. Run ``sudo chmod 666 /dev/pts/3`` to ensure that your user will be able to access this file (you can also take ownership, or run ``gdb`` as root, it is up to you)
#. Go to ``/tmp/kernelsrc/home/developer/linux-4.9.51`` (adjust the path accordingly)
#. Start ``gdb ./vmlinux``, it will load debug symbols, it may take a few seconds
#. Run ``target remote /dev/pts/3`` (substitute the path to the one you got from ``socat`` earlier
#. On the guest, run as root ``echo "ttyS0,115200" >/sys/module/kgdboc/parameters/kgdboc``
#. Then run ``echo g > /proc/sysrq-trigger`` to trigger a breakpoint (or press ``Alt-SysRq-g`` if you have that on the keyboard). You will notice that the console says ``KGDB: Entering KGDB``, it is a good sign. Also note that at this stage your CPU will most likely run at 100%.

At this point, you are ready to debug.




References
==========

- http://opensourceforu.com/2011/03/kgdb-with-virtualbox-debug-live-kernel/

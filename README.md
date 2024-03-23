# PiMusiic: Interactive Music Instrument with Joy-IT Kit and Raspberry Pi

PiMusiic is a music instrument project developed in C language, using the Joy-IT kit. This project aims to create an immersive and interactive musical experience, allowing users to compose, load, and play music with ease.
This repo includes a server with a custom protocol created especialy for the PiMusiic.

## Features:
- **User-friendly Interface:** Implemented using the ncurses library for intuitive navigation and interaction.
- **Music Creation:** Enables users to create new music by specifying the beats per minute (BPM), with a sequencer for composing melodies.
- **Music Loading:** Allows users to connect to the Pi2serv server using RFID cards and select music to load or modify.
- **Sound Generation:** Utilizes multiple channels for sound generation, including two audio channels from the RPI sound card and a special channel for playing sounds from the stepper motor.
- **Pi2iserv Communication:** Facilitates communication with the Pi2iserv server for user authentication and music management operations (List/Add/Modify/Delete).

## Installation:
1. Clone the repository: `git clone https://github.com/your_username/PiMusiic.git`
2. Set up environment variables for cross-compilation (optional):
~~~bash
# IP address of your Raspberry Pi
export IP_RPI=...
# Path to folder containing include and lib directories
export TARGET_FAKEROOT_RPI=...
# Path to cross compiler 
export CCC=...
~~~
3. Compile the project: `make`
4. Install it on your RPI: `make install`
5. Run the executable: `./bin-pi/PiMusiic`

## Usage:
- Follow the on-screen instructions to navigate the menu, create music, load music, and play music. 

## Requirements:
- Raspberry Pi with Joy-IT kit
- Raspberry Pi development environment set up
- Cross-compiler binaries installed (if compiling for Raspberry Pi)
- WiringPi library installed
- ALSA library installed
- ncurses library installed (for user interface)
- BCM2835 library installed (for spi communication)
- RFID library installed (for RFID card communication)

## About RFID library :

The rfid library is not really a library but a program made by paulvha. I modified it to remove all the printouts to compile it as a library

You can find the original program here :
[rfid-rc522](https://github.com/paulvha/rfid-rc522 "Paulvha's rfid-rc522 program")

You can suppress the printouts by changing the p_printf function in the main.c file

Then you can compile and install it as a library with the following makefile :

```makefile
# Path to cross-compiler binaries
PATH_CC_BINS?=/home/lukas/LE3/OCC/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin
# Default target fake root directory
TARGET_FAKEROOT_RPI?=/home/lukas/LE3/OCC/fakeroot
# Compiler command
CCC?=$(PATH_CC_BINS)/arm-linux-gnueabihf-gcc-4.8.3
LDD?=$(PATH_CC_BINS)/arm-linux-gnueabihf-ld

all: librfid.a install

%.o: %.c %.h
	$(CCC) -o $@ -c $< 

librfid.a: config.o rc522.o rfid.o value.o main.o
	ar rcs $@ $^ 
	
install:
	mkdir -p $(TARGET_FAKEROOT_RPI)/include/rfid
	cp librfid.a $(TARGET_FAKEROOT_RPI)/lib
	cp main.h config.h rc522.h rfid.h value.h $(TARGET_FAKEROOT_RPI)/include/rfid
```

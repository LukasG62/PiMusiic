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

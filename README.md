<p align="center">

<img src="https://lamphysics.github.io/logo.png" >

</p>

LAMPh — acronym from Laboratory for Advanced Materials Physics. LAMPh is an interesting computer program. His experimental tasks help to better understand the physical processes.

# BIO

The program includes a set of laboratory experiments that cover the following topics:
- Microwave absorption (MWA)
- Electron spin resonance (ESR)
- Resistance (AC/DC)
- Susceptibilit
- Measurement and change of system temperature, magnetic field and many other things..

Main fields of research: Study of physical properties of materials with nonstandard conducting and magnetic characteristics like those of high-temperature superconductors and strongly correlated electronic systems; multilayered systems with alternating superconducting and magnetic layers.

# Use

Try to start `bin/release/LAMPh.exe` (only for Windows) or compile the project for you system 

# Installation

- `qmake qwt.pro` The generated makefile includes all paths related to the chosen Qt version.
- `mingw32-make` For MinGW builds the name of the make tool is "mingw32-make"
- Finally you have to install everything below the directories you have specified in qwtconfig.pri.
- `mingw32-make install`
- After that you can work with LAMPh (see `src/LAMPh/LAMPh.pro` and include `qwtWidgetLibrary.pri, common.pri, app.pri`)

# Useful

The project uses a modular system. To connect new devices, you need to write only one library (the template is available).
This makes it possible to work in the same way with all devices, regardless of how they are connected, via RS232, USB or another method.
Inside the program, everything is configured in the form of a constructor.
In real time, you can draw, edit, scale, control devices.

# Screenshots

LAMPhPlot:
![1](https://github.com/llnoor/PROJECT_LAMPh/blob/master/img/1.jpg?raw=true)

LAMPhDevices:
![2](https://github.com/llnoor/PROJECT_LAMPh/blob/master/img/2.jpg?raw=true)

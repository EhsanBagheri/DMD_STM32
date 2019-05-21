# DMD_STM32 with Unicode fonts support 
The library is STM32 port of Freetronics DMD library (https://github.com/freetronics/DMD) and designed to make it easy to display graphics and scrolling text on p10 DMD 32x16 matrix display. Its fundamental difference from the original DMD library is support of Adafruit GFX format fonts: https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts. Using Adafruit `fontconvert` utility allows users to convert and display on DMD matrix Truetype fonts, including Unicode fonts with national characters of almost any language.

STM32 specific code of the library based on DMDSTM by Evgen Mozok: https://github.com/mozok/DMDSTM

Notice
------
This software is experimental and a work in progress. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.

What works and not?
---------- 

* All examples adopted from original DMD library are works
* Simple brightness control included (STM32 only)
* On the "bluepill" board can be used two independent DMD instances simultaneously - on SPI(1) and SPI(2) (see double_dmd example)
* Adafruit_GFX fonts can be used on drawString() and drawMarquee() routines (see dmd_cyr_chars example)
* Using more than one P10 matrix on channel are **not tested** yet.

Versions
---------
For version history see [Changes.txt](Changes.txt)

Compatible IDE
----------
This library works with Arduino IDE 1.8, other versions are **not tested**

Compatible boards
-----------------

* STM32 - only STM32F103C8TB (bluepill) board tested !
* AVR - Atmega328

Connections
-----------

| DMD Signal | STM32 Pin | Comments |
| ---------- | --------- | -------- |
| A, B | PB6/PB7/PB10/PB11... | User adjustable, almost any digital pin, see exclusions |
| nOE | PA8/PB1... | User adjustable, should be PWM pin, see exclusions |
| CLK | PA5/PB13 for SPI(1)/SPI(2) | Predefined by SPI |
| SCLK | PB0/PB8... |  User adjustable, almost any digital pin, see exclusions |
| R_DATA | PA7/PB15 for SPI(1)/SPI(2) |  Predefined by SPI |

* **Exclusions:** Do not use these pins: PB3/PB4 (JTAG), PA11/PA12 (USB D+ D-) and PA15 - Timer(2) output
* For tested pin combinations see examples.
* Pulldown resistor 3-10K between nOE and GND is recommended.

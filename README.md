Spanish word clock with Arduino
=

![alt tag](https://raw.githubusercontent.com/conejoninja/WordClock/master/working.jpg)

This is my Word clock in Spanish using an Arduino in a 10x10 matrix, I did it some years ago, and was hanging powered off on a wall of my kitchen for the same time due to some Arduino problems. I used an original Arduino (first model), and those had a small problem when they are powered by an external battery, making it implossible to boot propertly. Recently I decided to ditch out the old Arduino and replace it with a new nano model and power the whole thing with a USB wall adapter.


I tried to recover all the information I wrote done when I first build this, I got most important information but some details and pictures are missing.



Bill of materials
=
* 1 Arduino nano 9-10€ (original was about 20€), there a lot of different models and places to buy it
* 1 Macetech ChronoDot + Shipping 21.61€
* 1 MAX7219 chip 1.5-2€? (can't remember the price, but you could buy 10 at ebay for 14€ + free shipping)
* 104 LEDS + resistors (5mm 26000mcd) 11.95€ (+5€ shipping) = 16.95
* 2 Push buttons around 2€ ?
* 1 Plywood 800x400x5mm 5.25€
* 1 Glass 400x400x3mm 6€
* 1 Vynil 400x400mm 25€
* Nails, screws, glue,.. less than 2-3€

**Aproximate cost: 90-100€**

Original project had a 9V battery and 9V battery connector, add 7-8€ to the cost.


**Retail value** of the clock, at the time, 899€. I would assume that since now they're more common, the price of one of them should be lot cheaper.



Words
=
I used a 10x10 matrix for the characters/words, it's really easy to fit them all in a 10x11 matrix, but I wanted a 10x10 one. I use 4 additional leds on the corners to show the exact minute. 11 spots in the matrix are unused, I placed random characters on them and put leds behind.


![alt tag](https://raw.githubusercontent.com/conejoninja/WordClock/master/LED.groups.png)

Since some of the characters of the 10x10 matrix could be lit at the same time you could avoid using two MAX7219, the following led-grouping could be optimized even more, but I'm happy with it. Minute-led are in groups 38-41, all extra characters are in group 42 (the last one)


To light up words, we need to put ON the next groups:

* ES: 1,2
* SON: 2,3
* LA: 4
* LAS: 4,5
* UNA: 6
* DOS: 7
* OCHO: 8,9
* ONCE: 9,10
* TRES: 11
* CUATRO: 12
* DIEZ: 13
* NUEVE: 14,15,16
* CINCO: 17
* SIETE: 18,19,20
* DOCE: 21
* SEIS: 22,23,24
* Y: 25
* MENOS: 26,27,28
* CUARTO: 29,30,31
* VEINTE: 32
* DIEZ: 33,34,35
* MEDIA: 36
* CINCO: 37
* CEINTICINCO: 15,19,23,27,30,34,37



Coding Max7219
=
You could use MAX7219 or MAX7221. In this code, to light up the led groups you need to follow the next table:

![alt tag](https://raw.githubusercontent.com/conejoninja/WordClock/master/relation.led.variable.png)

For example, to light up the group #1, variable 'a1' needs to have a value of 128. To light up group #2 and #3, variable 'a1' next to be 96 (= 64+32).



Schematic
=

![alt tag](https://raw.githubusercontent.com/conejoninja/WordClock/master/CLOCK.png)

First time using Fritzing or any other schematic editor, I know it's a mess and ugly, not sure how to resize the MAX7219 as it looks like the scale isn't right, but it's my first time! RTC clock (Macetech Chronodot) is a DS3132 but the connection is the same so I used the one it came with Fritxing. I also used two overlapping 5x7 led matrices to create the 6x7 I used, so numbers of input are wrong, but I thing you could get the picture.


Licenses
=
This is distributed under the Apache License v2.0 unless some library or code isn't compatible ):

Copyright 2014 Daniel Esteban  -  conejo@conejo.me

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

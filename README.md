Spanish word clock with Arduino
=

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

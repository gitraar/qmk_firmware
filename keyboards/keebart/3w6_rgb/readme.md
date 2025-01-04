# 3w6 RGB

The 3w6 RGB is a hotswap, RP2040, low profile, split keyboard with 36 keys with per-key RGB LEDs, modified from the 3w6hs from beekeeb, which is based on the 3w6 keyboard by weteor.

* Keyboard Maintainer: [Keebart](https://github.com/Keebart)
* Hardware Supported: RP2040
* Hardware Availability: [https://keebart.com/](https://keebart.com/products/3w6)

## Bootloader

To enter the bootloader, follow these steps:

* Disconnect the keyboard from the computer.
* Press the "Q" button, which is the top-left button on the left side of the keyboard directly below the USB connector.
* While holding down the "Q" button, connect the keyboard back to the computer.

## Flashing example

Flashing examples for this keyboard (after setting up your build environment):

    make keebart/3w6_rgb:default
    make keebart/3w6_rgb:vial

#!/usr/bin/python

import time
import RPi.GPIO as GPIO

GPIO_PIR = 11


def detectedAndPrint():
    GPIO.setup(GPIO_PIR, GPIO.IN)
    print "Nothing"
    while isDetected():
        time.sleep(0.2)
        print "PIR sensor detected some stuff"


def isDetected():
    while GPIO.input(GPIO_PIR) == GPIO.LOW:
        return False

    while GPIO.input(GPIO_PIR) == GPIO.HIGH:
        return True


if __name__ == '__main__':
    # rpi board gpio or bcm gpio
    GPIO.setmode(GPIO.BOARD)

    # loop method
    detectedAndPrint()

#!/usr/bin/python

import time
import RPi.GPIO as GPIO

GPIO_RELAY = 11

def relay():
    #GPIO.cleanup()
    GPIO.setup(GPIO_RELAY, GPIO.OUT)
    while True:
        GPIO.output(GPIO_RELAY, GPIO.HIGH)
        #time.sleep(1)
        time.sleep(0.5)
        GPIO.output(GPIO_RELAY, GPIO.LOW)
        time.sleep(0.5)

if __name__ == '__main__':


    # rpi board gpio or bcm gpio
    GPIO.setmode(GPIO.BOARD)

    # loop method
    relay()

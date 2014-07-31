#!/usr/bin/python
"""
 * relay.py
 * A library for relay at RP
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : seeed fellow
 * Create Time:
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
"""
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

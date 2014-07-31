/*
 * dht22.c
 * A library for Grove - Temperature and Humidity Sensor Pro at RP
 *
 * Copyright (c) 2012 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : technion@lolware.net, yexiaobo@seeedstudio.com
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
 */


#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include "locking.h"

#define MAXTIMINGS 85
static int dht22_dat[5] = {0,0,0,0,0};

static void PrintUsage()
{
	printf ("ReadDHT22 <pin> <mode>\n");
	printf ("  pin  : GPIO No (0-7)\n");
	printf ("  mode : V - verbose output\n");
	printf ("         S - simple output.\n");
	printf ("             [-]HHH\n");
	printf ("             [-]TTT\n");
	printf ("             Values are signed 16 bit resolution and need dividing by 10\n");
}

static uint8_t sizecvt(const int read)
{
    /* digitalRead() and friends from wiringpi are defined as returning a value
       < 256. However, they are returned as int() types. This is a safety function */

    if (read > 255 || read < 0)
    {
        printf("Invalid data from wiringPi library\n");
        exit(EXIT_FAILURE);
    }
    return (uint8_t)read;
}

static int read_dht22_dat(int iPin, int* piHumidity, int* piTemp)
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;

    dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

    // pull pin down for 18 milliseconds
    pinMode(iPin, OUTPUT);
    digitalWrite(iPin, LOW);
    delay(18);

    // then pull it up for 40 microseconds
    digitalWrite(iPin, HIGH);
    delayMicroseconds(40);

	// prepare to read the pin
    pinMode(iPin, INPUT);

    // detect change and read data
    for ( i=0; i< MAXTIMINGS; i++)
	{
        counter = 0;
        while (sizecvt(digitalRead(iPin)) == laststate)
		{
            counter++;
            delayMicroseconds(1);
            if (counter == 255)
			{
                break;
            }
        }
        laststate = sizecvt(digitalRead(iPin));

        if (counter == 255) break;

        // ignore first 3 transitions
        if ((i >= 4) && (i%2 == 0))
		{
            // shove each bit into the storage bytes
            dht22_dat[j/8] <<= 1;
            if (counter > 16)
                dht22_dat[j/8] |= 1;
            j++;
        }
    }

    // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
    // print it out if data is good
    if ((j >= 40) && (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) )
	{
		*piHumidity = dht22_dat[0] * 256 + dht22_dat[1];
		*piTemp = (dht22_dat[2] & 0x7F)* 256 + dht22_dat[3];
        if ((dht22_dat[2] & 0x80) != 0)
			*piTemp *= -1;

		return 1;
    }
    else
    {
        return 0;
    }
}

int main( int argc, char * argv[])
{
    int lockfd;
    int iPin = 0;
    int iErr = 0;
    char cMode = 'V';
	int iReturnCode = 0;

    if ( argc !=3 )
	{
		PrintUsage();
		return -1;
	}

	if (strlen(argv[1]) != 1 || argv[1][0] < '0' || argv[1][0] > '7')
	{
		PrintUsage();
	    printf ("Invalid Pin Value [%s]\n", argv[1]);
		return -1;
	}
	if (strlen(argv[2]) != 1 || (argv[2][0] != 'V' && argv[2][0] != 'S'))
	{
		PrintUsage();
	    printf ("Invalid Mode Value [%s]\n", argv[2]);
		return -1;
	}

	iPin = atoi(argv[1]);
	cMode = argv[2][0];

	if (cMode == 'V')
	{
		printf ("Raspberry Pi wiringPi DHT22 reader\n");
	    printf ("   Reading data from pin %d\n", iPin);
	}

    lockfd = open_lockfile(LOCKFILE);

	iErr = wiringPiSetup ();
    if (iErr == -1)
	{
		if (cMode == 'V')
			printf ("ERROR : Failed to init WiringPi %d\n", iErr);
        iReturnCode = -1;
	}
	else
	{
		if (setuid(getuid()) < 0)
		{
			perror("Dropping privileges failed\n");
			iReturnCode = -1;
		}
		else
		{
			int iHumidity = -1;
			int iTemp = -1;

			for(int i = 0; i<10; i++)
			{
				// read_dht22_dat(iPin);
				if (read_dht22_dat(iPin, &iHumidity, &iTemp) == 1)
				{
					if (cMode == 'V')
						printf("    Humidity = %.2f %% Temperature = %.2f *C \n", (float)(iHumidity/10.0), (float)(iTemp/10.0) );
					else
						printf("%d\n%d\n", iHumidity, iTemp);
					iReturnCode = 0;
					break;
				}
				else
				{
					if (cMode == 'V')
					{
						printf("    Data not good, skip\n");
						iReturnCode = -1;
					}
				}
			}
		}
	}

    close_lockfile(lockfd);

    return iReturnCode;
}



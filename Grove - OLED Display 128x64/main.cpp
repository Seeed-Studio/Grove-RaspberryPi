/*
 * main.cpp
 * A library for OLED Display 128*64 at RP
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
 */
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "SeeedOLED.h"

using namespace std;

SeeedOLED oled = SeeedOLED(0x3c);

string GetStdoutFromCommand(string cmd) {
    int len;
    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream)) {
            if (fgets(buffer, max_buffer, stream) != NULL) {
                data.append(buffer);
            }
        }
        pclose(stream);
    }
    len = data.size() - 1;
    data = data.substr(0, len);
    return data;
}

int main ()
{
    while(1) {
        oled.writeString(0, 0, " Seeed Studio Pi ");
        string line2 = GetStdoutFromCommand("awk '{print $3}' /proc/version"); // kernel release
        oled.writeString(1, 0, (char*)line2.c_str());

        string line3 = "Temp: ";
        line3.append(GetStdoutFromCommand("awk '{printf \"%.1f\\n\", $1 / 1000}' /sys/class/thermal/thermal_zone0/temp")); // GPU temperature
        line3.append("oC");
        oled.writeString(2, 0, (char*)line3.c_str());

        string line4 = GetStdoutFromCommand("date +\"%H:%M %Y-%m-%d\""); // time and date
        oled.writeString(3, 0, (char*)line4.c_str());

        string line5 = "Uptime: ";
        line5.append(GetStdoutFromCommand("awk '{printf \"%.1f\\n\", $1 / 3600}' /proc/uptime")); // uptime
        line5.append("h");
        oled.writeString(4, 0, (char*)line5.c_str());

        string line6 = " ";
        line6.append(GetStdoutFromCommand("awk '{print $1\"/\"$2\"/\"$3}' /proc/loadavg")); // CPU usage
        oled.writeString(5, 0, (char*)line6.c_str());

        string line7 = "RAM: ";
        line7.append(GetStdoutFromCommand("sed -n '1p' /proc/meminfo | awk '{printf \"%.0f\\n\", $2 / 1024}'")); // RAM total
        line7.append("M/");
        line7.append(GetStdoutFromCommand("sed -n '2p' /proc/meminfo | awk '{printf \"%.0f\\n\", $2/ 1024}'")); // RAM free
        line7.append("M");
        oled.writeString(6, 0, (char*)line7.c_str());

        string line8 = "HDD: ";
        line8.append(GetStdoutFromCommand("df -m | grep \"rootfs\" | awk '{print $2}'")); // HDD total
        line8.append("M/");
        line8.append(GetStdoutFromCommand("df -m | grep \"rootfs\" | awk '{print $4}'")); // HDD total
        line8.append("M");
        oled.writeString(7, 0, (char*)line8.c_str());
        sleep(3);
    }

    return 0;
}

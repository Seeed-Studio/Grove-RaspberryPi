/*
 * SeeedOLED.cpp
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
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>

#include "SeeedOLED.h"
#include "font_8x8.h"
#include "font_16x24.h"

SeeedOLED::SeeedOLED(uint8_t deviceAddress)
{
    _writeOpcode = deviceAddress & 0xFE; // low order bit = 0 for write
    _readOpcode  = deviceAddress | 0x01; // low order bit = 1 for read

    initialize();
}

SeeedOLED::~SeeedOLED()
{
    //dtor
}

void SeeedOLED::initialize()
{
    setHorizontalAddressingMode();
    clearDisplay();
    setInverted(false);
    setDisplayOn();
}

void SeeedOLED::clearDisplay()
{
    //setDisplayOff();
    setPageAddress(0, MAX_PAGE);  // all pages
    setColumnAddress(0, MAX_COL); // all columns
    for (uint8_t page = 0; page < PAGES; page++) {
        for (uint8_t col = 0; col < COLUMNS; col++) {
            _sendData(0x00);
        }
    }
    //setDisplayOn();
}

void SeeedOLED::fillDisplay(uint8_t pattern, uint8_t start_page, uint8_t end_page, uint8_t start_col, uint8_t end_col)
{
    int count = (end_page - start_page + 1) * (end_col - start_col + 1);

    //setDisplayOff();
    setPageAddress(start_page, end_page);  // set page window
    setColumnAddress(start_col, end_col);  // set column window

    int file;
    char buf[1024] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x40;
    for (int i=1; i<count; i++) {
        buf[i] = pattern;
    }
    write(file,buf,count);
    close(file);

    //setDisplayOn();
}

void SeeedOLED::writeBitmap(uint8_t* data, uint8_t start_page, uint8_t end_page, uint8_t start_col, uint8_t end_col)
{
    int count = (end_page - start_page + 1) * (end_col - start_col + 1);

    //setDisplayOff();
    setPageAddress(start_page, end_page);  // set page window
    setColumnAddress(start_col, end_col);  // set column window

    int file;
    char buf[1024] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x40;
    for (int i=1; i<count; i++) {
        buf[i] = data[i];
    }
    write(file,buf,count);
    close(file);

    //setDisplayOn();
}

#define PRG_MAX_SCALE     50
#define PRG_LEFT_EDGE   0xFF
#define PRG_RIGHT_EDGE  0xFF
#define PRG_ACTIVE      0xFF
//#define PRG_ACTIVE      0xBD
#define PRG_NOT_ACTIVE  0x81

void SeeedOLED::writeProgressBar(uint8_t page, uint8_t col, int percentage)
{
    uint8_t scale_value = 0;

    if(percentage <= 0) {
        scale_value = 0;
    } else if (percentage >= 100) {
        scale_value = PRG_MAX_SCALE - 1 ;
    } else {
        scale_value = (percentage * PRG_MAX_SCALE) / 100;
    }

    //setDisplayOff();
    setPageAddress(page, page);
    setColumnAddress(col, MAX_COL);

    //  _i2c.start();
    //  _i2c.write(_writeOpcode);
    //  _i2c.write(DATA_MODE);
    //
    //  _i2c.write(PRG_LEFT_EDGE);  // Write Data
    //
    for (uint8_t col = 0; col < scale_value; col++) {
    //     _i2c.write(PRG_ACTIVE);  // Write Data
    }
    //
    //  _i2c.write(PRG_ACTIVE);  // Write Data
    //
    for (uint8_t col = (scale_value+1); col < PRG_MAX_SCALE; col++) {
    //     _i2c.write(PRG_NOT_ACTIVE);  // Write Data
    }
    //
    //  _i2c.write(PRG_RIGHT_EDGE);  // Write Data
    //
    //  _i2c.stop();

    //setDisplayOn();
}

void SeeedOLED::writeLevelBar(uint8_t page, uint8_t col, int percentage)
{
    uint8_t scale_value;

    if (percentage <= 0) {
        scale_value = 0;
    } else if (percentage >= 100) {
        scale_value = PRG_MAX_SCALE - 1;
    } else {
        scale_value = (percentage * PRG_MAX_SCALE) / 100;
    }

    //setDisplayOff();
    setPageAddress(page, page);
    setColumnAddress(col, MAX_COL);

    //  _i2c.start();
    //  _i2c.write(_writeOpcode);
    //  _i2c.write(DATA_MODE);
    //
    //  _i2c.write(PRG_LEFT_EDGE);  // Write Data
    //
    for (uint8_t col = 0; col < scale_value; col++) {
    //     _i2c.write(PRG_NOT_ACTIVE);  // Write Data
    }
    //
    //  _i2c.write(PRG_ACTIVE);  // Write Data at active meterlevel
    //
    for (uint8_t col = scale_value+1; col < PRG_MAX_SCALE; col++) {
    //     _i2c.write(PRG_NOT_ACTIVE);  // Write Data
    }
    //
    //  _i2c.write(PRG_RIGHT_EDGE);  // Write Data
    //
    //  _i2c.stop();

    //setDisplayOn();
}

void SeeedOLED::setHorizontalAddressingMode()
{
    setMemoryAddressingMode(HORIZONTAL_ADDRESSING_MODE);
}

void SeeedOLED::setVerticalAddressingMode()
{
    setMemoryAddressingMode(VERTICAL_ADDRESSING_MODE);
}

void SeeedOLED::setPageAddressingMode()
{
    setMemoryAddressingMode(PAGE_ADDRESSING_MODE);
}

void SeeedOLED::setMemoryAddressingMode(uint8_t mode)
{
    _sendCommand(SET_MEMORY_ADDRESSING_MODE, mode);
}

void SeeedOLED::setPageAddress(uint8_t start, uint8_t end)
{
    _sendCommand(SET_PAGE_ADDRESS, start, end);
}

void SeeedOLED::setColumnAddress(uint8_t start, uint8_t end)
{
    _sendCommand(SET_COLUMN_ADDRESS, start, end);
}

void SeeedOLED::setDisplayStartLine(uint8_t line)
{
    line = line & MAX_ROW;
    _sendCommand(SET_DISPLAY_START_LINE | line);
}

void SeeedOLED::setColumnStartForPageAddressingMode(uint8_t column)
{
    column = column & MAX_COL;
    _sendCommand(SET_LOWER_COLUMN  | ( column     & 0x0F));  // lower nibble
    _sendCommand(SET_HIGHER_COLUMN | ((column>>4) & 0x0F));  // higher nibble
}

void SeeedOLED::setPageStartForPageAddressingMode(uint8_t page)
{
    page = page & MAX_PAGE;
    _sendCommand(SET_PAGE_START_ADDRESS | page);
}

void SeeedOLED::setContrastControl(uint8_t contrast)
{
    _sendCommand(SET_CONTRAST, contrast);
}

void SeeedOLED::setDisplayOn()
{
    _sendCommand(SET_DISPLAY_POWER_ON);
}

void SeeedOLED::setDisplayOff()
{
    _sendCommand(SET_DISPLAY_POWER_OFF);
}

void SeeedOLED::setDisplayPower(bool on)
{
    if (on) {
        setDisplayOn();
    } else {
        setDisplayOff();
    }
}

void SeeedOLED::setDisplayNormal()
{
    _sendCommand(SET_NORMAL_DISPLAY);
}

void SeeedOLED::setDisplayInverse()
{
    _sendCommand(SET_INVERSE_DISPLAY);
}

void SeeedOLED::setDisplayBlink(bool on){
  if (on) {
    _sendCommand(SET_FADE_BLINK, (BLINK_ENABLE | FADE_INTERVAL_128_FRAMES));
  }
  else {
    _sendCommand(SET_FADE_BLINK, FADE_BLINK_DISABLE);
  }
}

void SeeedOLED::setDisplayFade(bool on)
{
  if (on) {
    _sendCommand(SET_FADE_BLINK, (FADE_OUT_ENABLE | FADE_INTERVAL_128_FRAMES));
  }
  else {
    _sendCommand(SET_FADE_BLINK, FADE_BLINK_DISABLE);
  }
}

void SeeedOLED::setDisplayFlip(bool left, bool down) {
  if (left) {
    // column address   0 is mapped to SEG0 (Reset)
    _sendCommand(SET_SEGMENT_REMAP_0);
  }
  else {
    // column address 127 is mapped to SEG0
    _sendCommand(SET_SEGMENT_REMAP_127);
  }

  if (down) {
    // Reset mode
    _sendCommand(SET_COMMON_REMAP_0);
  }
  else {
    // Flip Up/Down (Need to rewrite display before H effect shows)
    _sendCommand(SET_COMMON_REMAP_63);
  }

}

void SeeedOLED::setInternalIref()
{
    //  uint8_t cmds[2] = {SET_IREF_SELECTION, INTERNAL_IREF};
    //  _sendCommands(2, cmds);
    _sendCommand(SET_IREF_SELECTION, INTERNAL_IREF);
}

void SeeedOLED::setExternalIref()
{
    //  uint8_t cmds[2] = {SET_IREF_SELECTION, EXTERNAL_IREF};
    //  _sendCommands(2, cmds);
    _sendCommand(SET_IREF_SELECTION, EXTERNAL_IREF);
}

void SeeedOLED::setEntireDisplayOn()
{
    _sendCommand(SET_ENTIRE_DISPLAY_ON);
}

void SeeedOLED::setEntireDisplayRAM()
{
    _sendCommand(SET_DISPLAY_GDDRAM);
}

void SeeedOLED::setEntireDisplay(bool on)
{
    if (on) {
        setEntireDisplayOn();  // All Pixels on
    }
    else {
        setEntireDisplayRAM(); // Pixels are RAM content
    }
}

void SeeedOLED::setContinuousHorizontalScroll(bool left, uint8_t start_page, uint8_t end_page, uint8_t interval)
{
    if (left) {
        _sendCommand(SET_LEFT_HOR_SCROLL, 0x00, start_page, interval, end_page, 0x00, 0xFF);  // Scroll Left
    }
    else {
        _sendCommand(SET_RIGHT_HOR_SCROLL, 0x00, start_page, interval, end_page, 0x00, 0xFF); // Scroll Right
    }
}

void SeeedOLED::setContinuousVerticalAndHorizontalScroll(bool left, uint8_t start_page, uint8_t end_page, uint8_t offset, uint8_t interval)
{
    if (left) {
        _sendCommand(SET_VERT_LEFT_HOR_SCROLL, 0x00, start_page, interval, end_page, offset);  // Scroll Left
    }
    else {
        _sendCommand(SET_VERT_RIGHT_HOR_SCROLL, 0x00, start_page, interval, end_page, offset); // Scroll Right
    }
}

void SeeedOLED::setVerticalScrollArea(uint8_t topRowsFixed, uint8_t scrollRows)
{
  if((topRowsFixed + scrollRows) > ROWS) {
     scrollRows = ROWS - topRowsFixed;
  }
  _sendCommand(SET_VERTICAL_SCROLL_AREA, topRowsFixed, scrollRows);
}

void SeeedOLED::setDisplayScroll(bool on)
{
    if (on) {
        _sendCommand(SET_ACTIVATE_SCROLL);   // Scroll on
    }
    else {
        _sendCommand(SET_DEACTIVATE_SCROLL); // Scroll off
    }
}

void SeeedOLED::writeChar(char chr)
{
    const uint8_t char_index = chr - 0x20;

    for(uint8_t i = 0; i < 8; i++) {
        if(_inverted) {
            _sendData( ~font_8x8[char_index][i] );
        } else {
            _sendData( font_8x8[char_index][i] );
        }
    }
}

void SeeedOLED::writeString(uint8_t row, uint8_t col, const char * txt)
{
    uint16_t index = 0;
    uint16_t len = strlen(txt);

    setPageAddress(row, MAX_PAGE);
    const uint8_t col_addr = FONT8x8_WIDTH*col;
    setColumnAddress(col_addr, MAX_COL);

    while((col+index) < CHARS && (index < len)) {
        // write first line, starting at given position
        writeChar(txt[index++]);
    }

    // write remaining lines
    // write until the end of memory
    // then wrap around again from the top.
    if(index + 1 < len) {
        setPageAddress(row + 1, MAX_PAGE);
        setColumnAddress(0, MAX_COL);
        bool wrapEntireScreen = false;
        while(index + 1 < len) {
            writeChar(txt[index++]);
            // if we've written the last character space on the screen,
            // reset the page and column address so that it wraps around from the top again
            if(!wrapEntireScreen && (row*CHARS + col + index) > 127) {
                setPageAddress(0, MAX_PAGE);
                setColumnAddress(0, MAX_COL);
                wrapEntireScreen = true;
            }
        }
    }
}

void SeeedOLED::writeBigChar(uint8_t row, uint8_t col, char chr)
{
    writeBitmap((uint8_t*) font_16x24[int(chr) - FONT16x24_START], row, (row + FONT16x24_BYTES - 1), col, (col + FONT16x24_WIDTH - 1));
}

void SeeedOLED::_sendCommand(uint8_t command)
{
    int file;
    char buf[2] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x80;
    buf[1] = command;
    write(file,buf,2);
    close(file);
}

void SeeedOLED::_sendCommand(uint8_t command, uint8_t param1)
{
    int file;
    char buf[4] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x80;
    buf[1] = command;
    buf[2] = 0x80;
    buf[3] = param1;
    write(file,buf,4);
    close(file);
}

void SeeedOLED::_sendCommand(uint8_t command, uint8_t param1, uint8_t param2)
{
    int file;
    char buf[6] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x80;
    buf[1] = command;
    buf[2] = 0x80;
    buf[3] = param1;
    buf[4] = 0x80;
    buf[5] = param2;
    write(file,buf,6);
    close(file);
}

void SeeedOLED::_sendCommand(uint8_t command, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4, uint8_t param5)
{
    int file;
    char buf[12] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x80;
    buf[1] = command;
    buf[2] = 0x80;
    buf[3] = param1;
    buf[4] = 0x80;
    buf[5] = param2;
    buf[6] = 0x80;
    buf[7] = param3;
    buf[8] = 0x80;
    buf[9] = param4;
    buf[10] = 0x80;
    buf[11] = param5;
    write(file,buf,12);
    close(file);
}

void SeeedOLED::_sendCommand(uint8_t command, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4, uint8_t param5, uint8_t param6)
{
    int file;
    char buf[14] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x80;
    buf[1] = command;
    buf[2] = 0x80;
    buf[3] = param1;
    buf[4] = 0x80;
    buf[5] = param2;
    buf[6] = 0x80;
    buf[7] = param3;
    buf[8] = 0x80;
    buf[9] = param4;
    buf[10] = 0x80;
    buf[11] = param5;
    buf[12] = 0x80;
    buf[13] = param6;
    write(file,buf,14);
    close(file);
}

void SeeedOLED::_sendData(uint8_t data)
{
    int file;
    char buf[2] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    buf[0] = 0x40;
    buf[1] = data;
    write(file,buf,2);
    close(file);
}

void SeeedOLED::_sendData(uint8_t len, uint8_t* data)
{
    int file;
    char buf[64] = {0};

    if((file = open("/dev/i2c-1",O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    }

    if(ioctl(file,I2C_SLAVE,_writeOpcode) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
    }
    for(int i=0; i<len; i++) {
        buf[i] = data[i];
    }
    write(file,buf,len);
    close(file);
}

void SeeedOLED::_init()
{
    _sendCommand(SET_DISPLAY_POWER_OFF);      // 0xAE

    // column address   0 is mapped to SEG0 (Reset)
    // row address   0 is mapped to COM0 (Reset)
    _sendCommand(SET_SEGMENT_REMAP_0);        // 0xA0 (Reset)
    _sendCommand(SET_COMMON_REMAP_0);         // 0xC0 (Reset)

    setDisplayStartLine(0);                   // 0x40 (Reset)

    _sendCommand(SET_COMMON_CONF, COMMON_BASE | COMMON_ALTERNATIVE | COMMON_LEFTRIGHT_NORMAL); // 0xDA, 0x12 (Reset)

    // Pagemode or Horizontal mode
    //  setPageAddressingMode();                  // 0x20, 0x02 (Reset)
    //  setColumnStartForPageAddressingMode(0);   // 0x00, 0x10 (Reset = Column 0)
    //  setPageStartForPageAddressingMode(PAGE_0);// 0xBO       (Reset = Page 0)
    setHorizontalAddressingMode();            // 0x20, 0x00 (Non-Reset)
    setColumnAddress(0, MAX_COL);             // 0x21, 0x00, 0x37 (Reset)
    setPageAddress(0, MAX_PAGE);              // 0x22, 0x00, 0x07 (Reset)

    setExternalIref();                        // 0xAD, 0x10 (Reset)

    _sendCommand(SET_DISPLAY_CLOCK, 0x70);    // 0xD5, 0x70 (Reset = 0x80)
    _sendCommand(SET_PRECHARGE_TIME, 0x21);   // 0xD9, 0x21 (Reset = 0x22)
    _sendCommand(SET_VCOMH_DESELECT_LEVEL, 0x30); // 0xDB, 0x30 (Reset = 0x20)
    _sendCommand(SET_MULTIPLEX_RATIO, 0x3F);  // 0xA8, 0x3F (Reset)
    _sendCommand(SET_DISPLAY_OFFSET, 0x00);   // 0xD3, 0x00 (Reset)

    _sendCommand(SET_CONTRAST, 0x7F);         // 0x81, 0x7F (Reset)

    _sendCommand(SET_NORMAL_DISPLAY);         // 0xA6 (Reset)

    setEntireDisplayRAM();                    // 0xA4 (Reset)
    setDisplayScroll(false);

    clearDisplay();

    _sendCommand(SET_DISPLAY_POWER_ON);       // 0xAF
}

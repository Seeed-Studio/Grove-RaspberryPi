/*
 * SeeedOLED.h
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
#ifndef SEEEDOLED_H
#define SEEEDOLED_H

#include <inttypes.h>

// This is the I2C address (8 bit)
//  There are two possible addresses: with D/C# (pin 13) grounded, the address is 0x78,
//  with D/C# tied high it is 0x7A. Assume grounded by default.

// I2C Slave Address bit "0111 10 SA0 R/W" --> "0x7x"
#define SSD1308_SA0                0x3C // 0011 1100
#define SSD1308_SA1                0x3D // 0011 1101
#define SSD1308_DEF_SA             SSD1308_SA0

// Display dimensions
#define ROWS                       64 // Row of the OLED
#define COLUMNS                    128 // Colume of the OLED
#define PAGES                      (ROWS / 8) // 0-7 pages
#define MAX_PAGE                   (PAGES - 1)
#define MAX_ROW                    (ROWS - 1) //0-63
#define MAX_COL                    (COLUMNS - 1) // 0-127

// Character dimensions 8x8 font
#define CHARS                      (COLUMNS / FONT8x8_WIDTH)

// Command and Datamode
#define COMMAND_MODE               0x80 // continuation bit is set!
#define DATA_MODE                  0x40

// Commands and Parameter defines
#define SET_LOWER_COLUMN           0x00 // | with lower nibble  (Page mode only)
#define SET_HIGHER_COLUMN          0x10 // | with higher nibble (Page mode only)

#define HORIZONTAL_ADDRESSING_MODE 0x00
#define VERTICAL_ADDRESSING_MODE   0x01
#define PAGE_ADDRESSING_MODE       0x02
#define SET_MEMORY_ADDRESSING_MODE 0x20 // takes one byte as given above

#define SET_COLUMN_ADDRESS         0x21 // takes two bytes, start address and end address of display data RAM
#define SET_PAGE_ADDRESS           0x22 // takes two bytes, start address and end address of display data RAM

// Command maybe unsupported by SSD1308
#define FADE_INTERVAL_8_FRAMES     0x00
#define FADE_INTERVAL_16_FRAMES    0x01
#define FADE_INTERVAL_24_FRAMES    0x02
#define FADE_INTERVAL_32_FRAMES    0x03
#define FADE_INTERVAL_64_FRAMES    0x07
#define FADE_INTERVAL_128_FRAMES   0x0F
#define FADE_BLINK_DISABLE         0x00
#define FADE_OUT_ENABLE            0x20
#define BLINK_ENABLE               0x30
#define SET_FADE_BLINK             0x23 // takes one byte
                                        //  bit5-4 = 0, fade/blink mode
                                        //  bit3-0 = Time interval in frames

#define SET_DISPLAY_START_LINE     0x40 // | with a row number 0-63 to set start row. (Reset = 0)

#define SET_CONTRAST               0x81 // takes one byte, 0x00 - 0xFF

#define SET_SEGMENT_REMAP_0        0xA0 // column address 0 is mapped to SEG0 (Reset)
#define SET_SEGMENT_REMAP_127      0xA1 // column address 127 is mapped to SEG0

#define SET_DISPLAY_GDDRAM         0xA4 // restores display to contents of RAM
#define SET_ENTIRE_DISPLAY_ON      0xA5 // turns all pixels on, does not affect RAM

#define SET_NORMAL_DISPLAY         0xA6 // a databit of 1 indicates pixel 'ON'
#define SET_INVERSE_DISPLAY        0xA7 // a databit of 1 indicates pixel 'OFF'

#define SET_MULTIPLEX_RATIO        0xA8 // takes one byte, from 16xMUX to 64xMUX (MUX Ratio = byte+1; Default 64)

#define EXTERNAL_IREF              0x10
#define INTERNAL_IREF              0x00
#define SET_IREF_SELECTION         0xAD // sets internal or external Iref

#define SET_DISPLAY_POWER_OFF      0xAE
#define SET_DISPLAY_POWER_ON       0xAF

#define PAGE0                      0x00
#define PAGE1                      0x01
#define PAGE2                      0x02
#define PAGE3                      0x03
#define PAGE4                      0x04
#define PAGE5                      0x05
#define PAGE6                      0x06
#define PAGE7                      0x07
#define SET_PAGE_START_ADDRESS     0xB0 // | with a page number to get start address (Page mode only)

#define SET_COMMON_REMAP_0         0xC0 // row address  0 is mapped to COM0 (Reset)
#define SET_COMMON_REMAP_63        0xC8 // row address 63 is mapped to COM0

#define SET_DISPLAY_OFFSET         0xD3 // takes one byte from 0-63 for vertical shift, Reset = 0

#define SET_DISPLAY_CLOCK          0xD5 // takes one byte
                                        //  bit7-4 = Osc Freq DCLK (Reset = 1000b)
                                        //  bit3-0 = Divide ration (Reset = oooob, Ratio = 1)

#define SET_PRECHARGE_TIME         0xD9 // takes one byte
                                        //  bit7-4 = Phase2, upto 15 DCLKs (Reset = 0010b)
                                        //  bit3-0 = Phase1, upto 15 DCLKs (Reset = 0010b)


#define COMMON_BASE                0x02 //
#define COMMON_SEQUENTIAL          0x00 // Sequential common pins config
#define COMMON_ALTERNATIVE         0x10 // Odd/Even common pins config (Reset)
#define COMMON_LEFTRIGHT_NORMAL    0x00 // LeftRight Normal (Reset)
#define COMMON_LEFTRIGHT_FLIP      0x20 // LeftRight Flip
#define SET_COMMON_CONF            0xDA // takes one byte as given above


#define VCOMH_DESELECT_0_65_CODE   0x00
#define VCOMH_DESELECT_0_77_CODE   0x20
#define VCOMH_DESELECT_0_83_CODE   0x30
#define SET_VCOMH_DESELECT_LEVEL   0xDB // takes one byte as given above

#define NOP                        0xE3

#define SCROLL_INTERVAL_5_FRAMES   0x00
#define SCROLL_INTERVAL_64_FRAMES  0x01
#define SCROLL_INTERVAL_128_FRAMES 0x02
#define SCROLL_INTERVAL_256_FRAMES 0x03
#define SCROLL_INTERVAL_3_FRAMES   0x04
#define SCROLL_INTERVAL_4_FRAMES   0x05
#define SCROLL_INTERVAL_25_FRAMES  0x06
#define SCROLL_INTERVAL_2_FRAMES   0x07

#define SET_RIGHT_HOR_SCROLL       0x26 // takes 6 bytes: 0x00, PageStart, Scroll_Interval, PageEnd, 0x00, 0xFF
#define SET_LEFT_HOR_SCROLL        0x27 // takes 6 bytes: 0x00, PageStart, Scroll_Interval, PageEnd, 0x00, 0xFF

#define SET_VERT_RIGHT_HOR_SCROLL  0x29 // takes 5 bytes: 0x00, PageStart, Scroll_Interval, PageEnd, VertOffset
#define SET_VERT_LEFT_HOR_SCROLL   0x2A // takes 5 bytes: 0x00, PageStart, Scroll_Interval, PageEnd, VertOffset

#define SET_DEACTIVATE_SCROLL      0x2E
#define SET_ACTIVATE_SCROLL        0x2F

#define SET_VERTICAL_SCROLL_AREA   0xA3 // takes 2 bytes: Rows in Top Area (Reset=0), Rows in Scroll Area (Reset=64)

class SeeedOLED
{
    public:
        SeeedOLED(uint8_t deviceAddress);
        virtual ~SeeedOLED();
        void initialize();
        void clearDisplay();
        void fillDisplay(uint8_t pattern = 0x00, uint8_t start_page=0, uint8_t end_page=MAX_PAGE, uint8_t start_col=0, uint8_t end_col=MAX_COL);
        void writeBitmap(uint8_t* data, uint8_t start_page=0, uint8_t end_page=MAX_PAGE, uint8_t start_col=0, uint8_t end_col=MAX_COL);
        void writeProgressBar(uint8_t page, uint8_t col, int percentage);
        void writeLevelBar(uint8_t page, uint8_t col, int percentage);
        //void setXY(uint8_t, uint8_t y);
        void setInverted(bool inverted) { _inverted = inverted; };
        void writeChar(char chr);
        void writeBigChar(uint8_t row, uint8_t col, char chr);
        void writeString(uint8_t row, uint8_t col, const char* txt);
        virtual int _putc(int value) { writeChar(value); return 1; };
        virtual int _getc() { return -1; };
        void setHorizontalAddressingMode();
        void setVerticalAddressingMode();
        void setPageAddressingMode();
        void setMemoryAddressingMode(uint8_t mode);
        void setColumnStartForPageAddressingMode(uint8_t column);
        void setPageStartForPageAddressingMode(uint8_t page);
        void setColumnAddress(uint8_t start, uint8_t end);
        void setPageAddress(uint8_t start, uint8_t end);
        void setDisplayStartLine(uint8_t line);
        void setContrastControl(uint8_t contrast);
        void setEntireDisplayOn();
        void setEntireDisplayRAM();
        void setEntireDisplay(bool on);
        void setMultiplexRatio(uint8_t lines);
        void setInternalIref();
        void setExternalIref();
        void setDisplayOn();
        void setDisplayOff();
        void setDisplayPower(bool on);
        void setDisplayNormal();
        void setDisplayInverse();
        void setDisplayBlink(bool on);
        void setDisplayFade(bool on);
        void setDisplayFlip(bool left, bool down);
        void setDisplayOffset(uint8_t offset);
        void setDisplayClock(uint8_t divideRatio, uint8_t oscFreq);
        void setPrechargePeriod(uint8_t phase1, uint8_t phase2);
        void setVcomhDeselectLevel(uint8_t level);
        void nop();
        void setContinuousHorizontalScroll(bool left, uint8_t start_page, uint8_t end_page, uint8_t interval);
        void setContinuousVerticalAndHorizontalScroll(bool left, uint8_t start_page, uint8_t end_page,
                                                          uint8_t offset, uint8_t interval);
        void setDisplayScroll(bool on);
        void setVerticalScrollArea(uint8_t topRowsFixed, uint8_t scrollRows);
    private:
        void _sendCommand(uint8_t command);
        void _sendCommand(uint8_t command, uint8_t param1);
        void _sendCommand(uint8_t command, uint8_t param1, uint8_t param2);
        //void sendCommands(uint8_t len, uint8_t* buf);
        void _sendCommand(uint8_t command, uint8_t param1, uint8_t param2,
                                               uint8_t param3, uint8_t param4,
                                               uint8_t param5);
        void _sendCommand(uint8_t command, uint8_t param1, uint8_t param2,
                                               uint8_t param3, uint8_t param4,
                                               uint8_t param5, uint8_t param6);
        void _sendData(uint8_t data);
        void _sendData(uint8_t len, uint8_t* data);
        void _init();
        uint8_t _readOpcode;   // contains the I2C address of the device
        uint8_t _writeOpcode;  // contains the I2C address of the device
        bool _inverted;        // inverted or normal text
};

#endif // SEEEDOLED_H

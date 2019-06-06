/*
 * Copyright (c) 2019, Vincent Hervieux vincent.hervieux@gmail.com
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the author Vincent Hervieux, nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef Adafruit_GFX_dummy_display_h
#define Adafruit_GFX_dummy_display_h

#include "Arduino.h"
#include "Adafruit_GFX.h"




class Adafruit_GFX_dummy_display : public Adafruit_GFX
{
    public:
        Adafruit_GFX_dummy_display(uint8_t w, uint8_t h); // Constructor
	    ~Adafruit_GFX_dummy_display();
	    void display(void);
        void clearDisplay(void);
	    void drawPixel(int16_t x, int16_t y, uint16_t color);
        void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
        void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
        void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        void fillScreen(uint16_t color);
        /* to match graphics.h */
        const static uint16_t BLACK = 15;
        const static uint16_t WHITE = 15;
    private:
        void initDisplay(uint8_t w, uint8_t h);

};

#endif

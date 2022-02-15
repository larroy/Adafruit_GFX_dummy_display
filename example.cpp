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
#include "Adafruit_GFX_dummy_display.h"
#include <unistd.h>
#include <cstdio>

#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "emenu.h"
#include "adafruit_gfx_renderer.h"

/* A rectangle representing the screen area will be displayed */
#define HEIGHT 32
#define WIDTH 128
/* If the display is small, this will increase it by ZOOM on the display */
#define ZOOM 5
/* Some monochrome display are blue when calling WHITE, this can be simulated by such a macro */
//#define WHITE Adafruit_GFX_dummy_display::WHITE
#define WHITE Adafruit_GFX_dummy_display::YELLOW

/* Here is the dummy display instanciation */
Adafruit_GFX_dummy_display display(WIDTH, HEIGHT, ZOOM);

class GUI
{
public:
    GUI(Adafruit_GFX_dummy_display *display) : _display(display),
                                               ada_gfx_r(display),
                                               gui(&ada_gfx_r, 0, 0, 128, 32),
                                               label(&gui, 0, 0, 120, 50,
                                                     static_cast<emenu::Label::fn_refresh_t>(std::bind(&GUI::label_refresh, this))),
                                               dvalue(&gui, 60, 0, 120, 50,
                                                      static_cast<emenu::DValue::fn_refresh_t>(std::bind(&GUI::dval_refresh, this)), 12, 2),
                                               ivalueinput(&gui, 30, 16, 20, 10,
                                                           nullptr,
                                                           static_cast<emenu::IValueInput<uint16_t>::fn_set_t>(std::bind(&GUI::ival_set, this, std::placeholders::_1)), 2),

                                               fmt_buff()
    {
        fmt_buff.resize(64);
        gui.addChild(&label);
        gui.addChild(&dvalue);
        display->setTextSize(1);
        // display->clearDisplay();
    }

    char *label_refresh()
    {
        static int i = 0;
        snprintf(fmt_buff.data(), fmt_buff.size(), "%d", i++);
        return fmt_buff.data();
    }

    double dval_refresh()
    {
        return 12.2345;
    }

    void ival_set(uint16_t x)
    {
        printf("ival_set(%d)\n", x);
    }

    void redraw()
    {
        _display->clearDisplay();
        gui.redraw();
        _display->display();
        // ESP_LOGD(LOG_TAG, "display");
    }

    void handleKey(emenu::Key key)
    {
        gui.handleKey(key);
    }

    Adafruit_GFX_dummy_display *_display;
    emenu::AdaGFXRenderer ada_gfx_r;
    emenu::GUI gui;
    emenu::Label label;
    emenu::DValue dvalue;
    emenu::IValueInput<uint16_t> ivalueinput;
    std::vector<char> fmt_buff;
};

/* Your code under test should be called in those Arduino like functions: */
GUI gui(&display);

void setup()
{
    display.setTextColor(WHITE);
    display.print("Welcome!");
    display.display();
    sleep(1);
}

void loop()
{
    gui.redraw();
    SDL_Delay(150);
    printf("loop\n");
}

/* This is emulating Arduino Behaviour, and should probably most of the time be only copy/pasted unless you really know what your are doing */

int main()
{
    int running = 1;
    /* Calling Arduino like setup() */
    setup();
    while (running)
    {
        SDL_Event event;
        /* Calling Arduino loop() forever until user presses the quit arrow */
        loop();
        while (SDL_PollEvent(&event) == 1)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    gui.handleKey(emenu::Key::UP);
                    break;
                case SDLK_DOWN:
                    gui.handleKey(emenu::Key::DOWN);
                    break;
                case SDLK_RETURN:
                    gui.handleKey(emenu::Key::ENTER);
                    break;
                case SDLK_ESCAPE:
                    running = 0;
                    break;
                default:
                    break;
                }
                break;

            default:
                /* running */
                break;
            }
        }
    }
    return 0;
}

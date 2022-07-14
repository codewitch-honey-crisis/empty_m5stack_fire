#include <Arduino.h>
#include <SPIFFS.h>
#include <SD.h>
#include <mpu6886.hpp>
#include <tft_io.hpp>
#include <ili9341.hpp>
#include <w2812.hpp>
#include <htcw_button.hpp>
#include <gfx.hpp>
// font for example
// not necessary
#include "Ubuntu.hpp"
using namespace arduino;
using namespace gfx;

// pin assignments
constexpr static const uint8_t spi_host = VSPI;
constexpr static const int8_t lcd_pin_bl = 32;
constexpr static const int8_t lcd_pin_dc = 27;
constexpr static const int8_t lcd_pin_rst = 33;
constexpr static const int8_t lcd_pin_cs = 14;
constexpr static const int8_t sd_pin_cs = 4;
constexpr static const int8_t speaker_pin_cs = 25;
constexpr static const int8_t mic_pin_cs = 34;
constexpr static const int8_t button_a_pin = 39;
constexpr static const int8_t button_b_pin = 38;
constexpr static const int8_t button_c_pin = 37;
constexpr static const int8_t led_pin = 15;
constexpr static const int8_t spi_pin_mosi = 23;
constexpr static const int8_t spi_pin_clk = 18;
constexpr static const int8_t spi_pin_miso = 19;

using bus_t = tft_spi_ex<spi_host, 
                        lcd_pin_cs, 
                        spi_pin_mosi, 
                        -1, 
                        spi_pin_clk, 
                        SPI_MODE0,
                        true, 
                        320 * 240 * 2 + 8, 2>;

using lcd_t = ili9342c<lcd_pin_dc, 
                      lcd_pin_rst, 
                      lcd_pin_bl, 
                      bus_t, 
                      1, 
                      true, 
                      400, 
                      200>;

// lcd colors
using color_t = color<typename lcd_t::pixel_type>;
// led strip colors
using lscolor_t = color<typename w2812::pixel_type>;

lcd_t lcd;

// declare the MPU6886 that's attached
// to the first I2C host
mpu6886 gyro(i2c_container<0>::instance());
// the following is equiv at least on the ESP32
// mpu6886 mpu(Wire);

w2812 led_strips({5,2},led_pin,NEO_GBR);

button<button_a_pin,10,true> button_a;
button<button_b_pin,10,true> button_b;
button<button_c_pin,10,true> button_c;

// initialize M5 Stack Fire peripherals/features
void initialize_m5stack_fire() {
    Serial.begin(115200);
    SPIFFS.begin(false);
    SD.begin(4,spi_container<spi_host>::instance());
    lcd.initialize();
    led_strips.fill(led_strips.bounds(),lscolor_t::purple);
    lcd.fill(lcd.bounds(),color_t::purple);
    rect16 rect(0,0,64,64);
    rect.center_inplace(lcd.bounds());
    lcd.fill(rect,color_t::white);
    lcd.fill(rect.inflate(-8,-8),color_t::purple);
    gyro.initialize();
    // see https://github.com/m5stack/m5-docs/blob/master/docs/en/core/fire.md
    pinMode(led_pin, OUTPUT_OPEN_DRAIN);
    led_strips.initialize();
    button_a.initialize();
    button_b.initialize();
    button_c.initialize();
}

// for the button callbacks
char button_states[3];
void buttons_callback(bool pressed, void* state) {
    Serial.printf("Button %c %s\n",*(char*)state,pressed?"pressed":"released");
}
void setup() {
    initialize_m5stack_fire();
    
    // setup the button callbacks (optional)
    button_states[0]='a';
    button_states[1]='b';
    button_states[2]='c';
    button_a.callback(buttons_callback,button_states);
    button_b.callback(buttons_callback,button_states+1);
    button_c.callback(buttons_callback,button_states+2);
    
    // your code here

    
    // example - go ahead and delete
    lcd.fill(lcd.bounds(),color_t::black);
    const char* m5_text = "M5Stack";
    constexpr static const uint16_t text_height = 80;
    srect16 text_rect;
    open_text_info text_draw_info;
    const open_font &text_font = Ubuntu;
    
    text_draw_info.text = m5_text;
    text_draw_info.font = &text_font;
    text_draw_info.scale = text_font.scale(text_height);
    text_draw_info.transparent_background = false;
    text_rect = text_font.measure_text(ssize16::max(),spoint16::zero(),m5_text,text_draw_info.scale).bounds().center((srect16)lcd.bounds()).offset(0,-text_height/2);
    draw::text(lcd,text_rect,text_draw_info,color_t::gray);
    draw::line(lcd,srect16(text_rect.x1,text_rect.y1,text_rect.x1,text_rect.y2).offset(80,0),color_t::white);
    const char* fire_text = "Fire";
    text_draw_info.text = fire_text;
    text_rect = text_font.measure_text(ssize16::max(),spoint16::zero(),fire_text,text_draw_info.scale).bounds().center((srect16)lcd.bounds()).offset(0,text_height/2);
    draw::text(lcd,text_rect,text_draw_info,color_t::red);
    led_strips.fill({0,0,4,0},lscolor_t::red);
    led_strips.fill({0,1,4,1},lscolor_t::blue);
}
void loop() {
    // pump the buttons to make sure
    // their callbacks (if any) get
    // fired
    button_a.update();
    button_b.update();
    button_c.update();
}
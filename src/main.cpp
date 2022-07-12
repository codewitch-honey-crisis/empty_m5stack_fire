#include <Arduino.h>
#include <SPIFFS.h>
#include <ili9341.hpp>
#include <tft_io.hpp>
#include <gfx.hpp>
#include <mpu6886.hpp>
using namespace arduino;
using namespace gfx;

constexpr static const uint8_t spi_host = VSPI;
constexpr static const int8_t lcd_pin_bl = 32;
constexpr static const int8_t lcd_pin_dc = 27;
constexpr static const int8_t lcd_pin_rst = 33;
constexpr static const int8_t lcd_pin_cs = 14;
constexpr static const int8_t spi_pin_mosi = 23;
constexpr static const int8_t spi_pin_clk = 18;
constexpr static const int8_t spi_pin_miso = 19;

using bus_t = tft_spi_ex<spi_host, 
                        lcd_pin_cs, 
                        spi_pin_mosi, 
                        spi_pin_miso, 
                        spi_pin_clk, 
                        SPI_MODE0,
                        false, 
                        320 * 240 * 2 + 8, 2>;
// can be simplified since the M5 Stack
// uses the default VSPI pins:
// using bus_t = tft_spi<spi_host,lcd_pin_cs,SPI_MODE0,320*240*2+8,2>;

using lcd_t = ili9342c<lcd_pin_dc, 
                      lcd_pin_rst, 
                      lcd_pin_bl, 
                      bus_t, 
                      1, 
                      true, 
                      400, 
                      200>;

using color_t = color<typename lcd_t::pixel_type>;

lcd_t lcd;

mpu6886 mpu(i2c_container<0>::instance());

void setup() {
    Serial.begin(115200);
    SPIFFS.begin(false);
    // your code here
}
void loop() {
    // your code here
}
#pragma once
#include <Arduino.h>

class m5fire_lcd_dimmer final {
    bool m_initialized;
    uint32_t m_timeout_ms;
    uint32_t m_timeout_ts;
    uint32_t m_fade_ts;
    uint32_t m_fade_step_ms;
    bool m_dimmed;
    int m_dim_count;
    void do_fade();
public:
    constexpr static const int8_t pin_bl = 32;
    m5fire_lcd_dimmer();
    bool initialize();
    inline bool initialized() const { return m_initialized; }
    inline uint32_t timeout() const { return m_timeout_ms; }
    void timeout(uint32_t milliseconds);
    inline uint32_t fade_step() const { return m_fade_step_ms; }
    void fade_step(uint32_t milliseconds);
    inline bool dimmed() const { return m_dimmed; }
    bool update();
    bool wake();
    bool sleep();

};
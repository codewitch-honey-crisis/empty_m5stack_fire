#include <m5fire_lcd_dimmer.hpp>
m5fire_lcd_dimmer::m5fire_lcd_dimmer() : m_initialized(false), m_timeout_ms(10*1000),m_timeout_ts(0),m_fade_ts(0),m_fade_step_ms(10),m_dimmed(false) {
}
void m5fire_lcd_dimmer::do_fade() {
    uint32_t ms = millis();
    if(ms>=m_fade_ts) {
        m_fade_ts = ms+m_fade_step_ms;
        --m_dim_count;
        ledcWrite(0,m_dim_count);
    }    
}
bool m5fire_lcd_dimmer::initialize() {
    if(!m_initialized) {
        ledcAttachPin(pin_bl,0);
        ledcSetup(0,5000,8);
        ledcWrite(0,255);
        m_dimmed = false;
        m_timeout_ts = millis()+m_timeout_ms;
        m_initialized = true;
        m_dim_count = 0;
    }
    return true;
}
void m5fire_lcd_dimmer::timeout(uint32_t milliseconds) {
    m_timeout_ms = milliseconds;
    m_timeout_ts = millis()+m_timeout_ms;
}
void m5fire_lcd_dimmer::fade_step(uint32_t milliseconds) {
    m_fade_step_ms = milliseconds;
}
bool m5fire_lcd_dimmer::update() {
    if(!initialize()) {return false;}
    uint32_t ms = millis();
    if(!m_dimmed && ms>=m_timeout_ts) {
        m_dimmed = true;
        m_dim_count = 255;
        m_fade_ts = ms+m_fade_step_ms;
    }
    if(m_dimmed && m_dim_count) {
        do_fade();
    } 
    return true;
}
bool m5fire_lcd_dimmer::wake() {
    if(!initialize()) {return false;}
    if(!m_dimmed) {return true;}
    m_timeout_ts = millis()+m_timeout_ms;
    m_dimmed = false;
    m_dim_count = 0;
    ledcWrite(0,255);
    return true;
}
bool m5fire_lcd_dimmer::sleep() {
    if(m_dimmed) { return true;}
    if(!initialize()) {return false;}
    m_dim_count =255;
    m_dimmed = true;
    m_fade_ts = millis()+m_fade_step_ms;
    do_fade();
    return true;
}
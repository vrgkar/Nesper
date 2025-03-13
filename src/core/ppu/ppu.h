#pragma once

#include "common/pch.h"
#include "common/component.h"
#include "common/defs.h"

#include "bus/bus.h"

class PPU final : public Component
{
public:
    uint8_t read(uint16_t addr) override;
    void write(uint8_t byte, uint16_t addr) override;

    void step();

    void connect(Bus *bus) { m_bus = bus; m_bus->connect(this); }

    std::string_view get_id() const override { return "PPU"; }
    bool frame_ready() { return m_cycle == 340 && m_scanline == -1; }

    unsigned char* framebuffer() { return &m_framebuffer[0][0][0]; }

private:

    enum State
    {
        PRERENDER,
        RENDER,
        POSTRENDER
    } m_state = RENDER;

    Bus *m_bus = nullptr;

    int m_cycle = 0;
    int m_scanline = 0;

    bool m_reset = true;
    bool m_odd_frame = false;

    std::array<uint8_t, 256> m_oam = {};
    std::array<uint8_t, 32> m_secoam = {};
    
    std::array<uint8_t, 32> m_palette_ram = {};
    std::array<std::array<std::array<uint8_t, 3>, 256>, 240> m_framebuffer = {};

    /* Pallete Map */
    std::array<uint8_t, 192> m_palette_map = 
    {
        84u, 84u, 84u,
        0u, 30u, 116u,
        8u, 16u, 144u,
        48u, 0u, 136u,
        68u, 0u, 100u,
        92u, 0u, 48u,
        84u, 4u, 0u,
        60u, 24u, 0u,
        32u, 42u, 0u,
        8u, 58u, 0u,
        0u, 64u, 0u,
        0u, 60u, 0u,
        0u, 50u, 60u,
        0u, 0u, 0u,
        0u, 0u, 0u,
        0u, 0u, 0u,
        152u, 150u, 152u,
        8u, 76u, 196u,
        48u, 50u, 236u,
        92u, 30u, 228u,
        136u, 20u, 176u,
        160u, 20u, 100u,
        152u, 34u, 32u,
        120u, 60u, 0u,
        84u, 90u, 0u,
        40u, 114u, 0u,
        8u, 124u, 0u,
        0u, 118u, 40u,
        0u, 102u, 120u,
        0u, 0u, 0u,
        0u, 0u, 0u,
        0u, 0u, 0u,
        236u, 238u, 236u,
        76u, 154u, 236u,
        120u, 124u, 236u,
        176u, 98u, 236u,
        228u, 84u, 236u,
        236u, 88u, 180u,
        236u, 106u, 100u,
        212u, 136u, 32u,
        160u, 170u, 0u,
        116u, 196u, 0u,
        76u, 208u, 32u,
        56u, 204u, 108u,
        56u, 180u, 204u,
        60u, 60u, 60u,
        0u, 0u, 0u,
        0u, 0u, 0u,
        236u, 238u, 236u,
        168u, 204u, 236u,
        188u, 188u, 236u,
        212u, 178u, 236u,
        236u, 174u, 236u,
        236u, 174u, 212u,
        236u, 180u, 176u,
        228u, 196u, 144u,
        204u, 210u, 120u,
        180u, 222u, 120u,
        168u, 226u, 144u,
        152u, 226u, 180u,
        160u, 214u, 228u,
        160u, 162u, 160u,
        0u, 0u, 0u,
        0u, 0u, 0u,
    };

    int m_nmi_enable = 0;
    int m_master_slave = 0;
    int m_sprite_height = 0;
    int m_bg_table = 0;
    int m_sprite_table = 0;
    int m_inc_mode = 0;

    int m_sprite_enable = 0;
    int m_bg_enable = 0;
    int m_sprite_left_enable = 0;
    int m_bg_left_enable = 0;
    int m_grayscale_enable = 0;

    int m_vblank_mode = 0;
    int m_sprite_zero_hit = 0;
    int m_sprite_overflow = 0;
    
    int m_oamaddr = 0;
    int m_oamdata = 0;

    int m_ppulatch = 0;
    int m_ppubuffer = 0;
    
    int m_v = 0;
    int m_t = 0;
    int m_x = 0;
    int m_w = 0;

    int m_nt = 0;
    int m_at = 0;
    int m_pt_lo = 0;
    int m_pt_hi = 0;       

    int m_at_lsb = 0;
    int m_at_msb = 0;
    int m_pt_lsb = 0;
    int m_pt_msb = 0;

    int m_bg_pal = 0;
    int m_sprite_pal = 0;

    struct Sprite
    {
        int y;
        int tile;

        bool flip_v;
        bool flip_h;
        bool priority;

        int pal_hi;

        int x;
    };

    std::array<Sprite, 8> m_sprites = {};
    int m_sprite_idx = 8;
private:

    void update_shifters()                  
    { 
        if (m_bg_enable)
            m_at_lsb <<= 1, m_at_msb <<= 1, m_pt_lsb <<= 1, m_pt_msb <<= 1;
    }

    void reload_shifters()
    {
        m_at_lsb = (m_at_lsb & 0xFF00) | ((m_at & 1) ? 0x00FF : 0);
        m_at_msb = (m_at_msb & 0xFF00) | ((m_at & 2) ? 0x00FF : 0);
        m_pt_lsb = (m_pt_lsb & 0xFF00) | m_pt_lo;
        m_pt_msb = (m_pt_msb & 0xFF00) | m_pt_hi;
    }

    void increment_scroll_x()
    {   
        if (m_sprite_enable || m_bg_enable)
        {
            if ((m_v & 0x001F) == 31)
            {
                m_v &= 0x7FE0;
                m_v ^= (1 << 10);
            }
            else
                ++m_v;
        }
    }

    void increment_scroll_y()
    {
        if (m_sprite_enable || m_bg_enable)
        {
            if ((m_v >> 12) < 7)
                m_v += 0x1000;
            else
            {
                int y = (m_v >> 5) & 0x1F;

                if (y == 29)
                {
                    y = 0;
                    m_v ^= (1 << 11);
                }
                else if (y == 31)
                    y = 0;
                else
                    ++y;

                m_v = (m_v & 0x0C1F) | (y << 5);
            }
        }
    }

    void transfer_x()
    {
        if (m_sprite_enable || m_bg_enable)
            m_v = (m_v & 0x7BE0) | (m_t & 0x041F);
    }

    void transfer_y()
    {
        if (m_sprite_enable || m_bg_enable)
            m_v = (m_v & 0x041F) | (m_t & 0x7BE0);
    }

private:

    uint8_t read_palette_ram(uint16_t addr);
    void write_palette_ram(uint8_t byte, uint16_t addr);

    void clock();

    void update();
    void update_prerender();
    void update_render();
    void update_postrender();

    void render();

    void background_fetch();
    void background_render();

    void secondary_oam_clear();
    bool sprite_evaluation();
    void sprite_fetch();
    void sprite_render();


};
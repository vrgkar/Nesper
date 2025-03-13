#include "ppu.h"

uint8_t PPU::read(uint16_t addr)
{
    uint8_t byte = 0;
    switch (addr % 8)
    {
    /* PPUCTRL */
    case 0:
        byte = m_ppulatch;
        break;
    /* PPUMASK */
    case 1:
        byte = m_ppulatch;
        break;
    /* PPUSTATUS */
    case 2:
        byte = (m_vblank_mode << 7) | (m_sprite_zero_hit << 6) | (m_sprite_overflow << 5);

        /* Clear VBlank on read */
        m_vblank_mode = 0;

        /* Clear w on read */
        m_w = 0;

        m_ppulatch = byte;
        break;
    /* OAMADDR */
    case 3:
        byte = m_ppulatch;
        break;
    /* OAMDATA */
    case 4:
        /* This happens *only* during VBlank, but it's good enough for now */
        byte = m_oam[m_oamaddr];

        m_ppulatch = byte;
        break;
    /* PPUSCROLL */
    case 5:
        byte = m_ppulatch;
        break;
    /* PPUADDR */
    case 6:
        byte = m_ppulatch;
        break;
    /* PPUDATA */
    case 7:
        /*  This only applies OUTSIDE of rendering */

        byte = m_ppubuffer;
        if (m_v < 0x3F00)
            m_ppubuffer = m_bus->read_ppu(m_v);
        else
            byte = m_ppubuffer = read_palette_ram(m_v);

        m_v += m_inc_mode ? 32 : 1;
        m_ppulatch = byte;
        break;
    }

    return byte;
}

void PPU::write(uint8_t byte, uint16_t addr)
{
    /* Writes to any PPU port will also fill the bits  in the latch */

    m_ppulatch = byte;
    
    switch (addr % 8)
    {
    case 0:                                           /* PPUCTRL */
    /* Turning NMI enable on while the vblank is 1 triggers NMI */
        if (!m_reset)
        {
            m_nmi_enable            = (byte & 0x80) > 0;
            m_master_slave          = (byte & 0x40) > 0;
            m_sprite_height         = (byte & 0x20) > 0;
            m_bg_table              = (byte & 0x10) > 0;
            m_sprite_table          = (byte & 0x08) > 0;
            m_inc_mode              = (byte & 0x04) > 0;

            m_t = (m_t & 0x73FF) | ((byte & 0x03) << 10);
        }
        break;
    /* PPUMASK */
    case 1:
        /* Toggling rendering takes 3-4 cycles after write. */
        if (!m_reset)
        {
            m_sprite_enable         = (byte & 0x10) > 0;
            m_bg_enable             = (byte & 0x08) > 0;
            m_sprite_left_enable    = (byte & 0x04) > 0;
            m_bg_left_enable        = (byte & 0x02) > 0;
            m_grayscale_enable      = (byte & 0x01) > 0;
        }
        break;

    case 2:                                       /* PPUSTATUS */
        break;

    case 3:                                       /* OAMADDR */
        m_oamaddr = byte;
        break;
    case 4:                                       /* OAMDATA */
        if (m_scanline > 239 || m_scanline == -1)  
        {
            m_oam[m_oamaddr] = byte;
            if (++m_oamaddr == 256)
                m_oamaddr = 0;
        }

        break;
    case 5:                                       /* PPUSCROLL */
        if (m_w)
        {
            m_t &= 0x0C1F;
            m_t |= (byte & 0x07) << 12;
            m_t |= (byte & 0xF8) << 2;
            m_w = 0;
        }
        else
        {
            m_t = (m_t & 0x7FE0) | (byte >> 3);
            m_x = byte & 0x07;
            m_w = 1;
        }
        break;
    case 6:                                        /* PPUADDR */
        if (m_w)
        {
            m_t = (m_t & 0x7F00) | byte;
            m_v = m_t;
            m_w = 0;
        }
        else
        {
            m_t = (m_t & 0x00FF) | ((byte & 0x3F) << 8);
            m_w = 1;
        }
        break;
    case 7:
        if (m_v < 0x3F00u)
            m_bus->write_ppu(byte, m_v);          /* This only applies OUTSIDE of rendering */
        else
            write_palette_ram(byte, m_v);
        m_v += m_inc_mode ? 32 : 1;
        break;
    }

}

uint8_t PPU::read_palette_ram(uint16_t addr)
{
    addr &= 0x001F;
            
    if (addr == 0x0010) addr = 0x0000;
    if (addr == 0x0014) addr = 0x0004;
    if (addr == 0x0018) addr = 0x0008;
    if (addr == 0x001C) addr = 0x000C;

    return m_palette_ram[addr] & (m_grayscale_enable ? 0x30 : 0x3F);    
}

void PPU::write_palette_ram(uint8_t byte, uint16_t addr)
{
    addr &= 0x001F;
            
    if (addr == 0x0010) addr = 0x0000;
    if (addr == 0x0014) addr = 0x0004;
    if (addr == 0x0018) addr = 0x0008;
    if (addr == 0x001C) addr = 0x000C;

    m_palette_ram[addr] = byte; 
}

void PPU::step()
{
    update();

    if (m_scanline == -1)
    {
        update_prerender();
        
        background_fetch();
    }
    else if (m_scanline < 240)
    {
        update_render();

        background_fetch();

        secondary_oam_clear();
        sprite_evaluation();
        sprite_fetch();

        background_render();
        sprite_render();
        
        render();
    }
    else
        update_postrender();
    
    clock();
}

void PPU::update()
{}

void PPU::update_prerender()
{
    m_reset = false;

    if (m_cycle == 1)
        m_vblank_mode = m_sprite_zero_hit = m_sprite_overflow = 0;

    else if (279 < m_cycle && m_cycle < 305)
        transfer_y();

    else if (m_cycle == 340)
        m_odd_frame = !m_odd_frame;

}

void PPU::update_render()
{
    if (m_bg_enable && m_cycle == 0 && m_scanline == 0 && m_odd_frame)
        m_cycle = 1;
    
    if (m_cycle == 338 || m_cycle == 340)
      m_nt = m_bus->read_ppu(0x2000 | (m_v & 0x0FFF));
}

void PPU::update_postrender()
{
    if (m_cycle == 1 && m_scanline == 241)
    {
        m_vblank_mode = 1;
        if (m_nmi_enable)
            m_bus->broadcast(this, Event::NMI);
    }    
}

void PPU::clock()
{
    if (++m_cycle == 341)
    {
        m_cycle = 0;
        
        if (++m_scanline == 261)
            m_scanline = -1;
    }
}

void PPU::background_fetch()
{
    if (!((m_cycle > 0 && m_cycle < 258) || (m_cycle > 320 && m_cycle < 338)))
        return;

    update_shifters();

    int addr = 0;
        
    switch ((m_cycle - 1) % 8)
    {
    case 0:
        reload_shifters();
        addr = 0x2000 | (m_v & 0x0FFF);
        m_nt = m_bus->read_ppu(addr);
        break;
    
    case 2:
        addr = 0x23C0 | (m_v & 0x0C00) | ((m_v >> 4) & 0x38) | ((m_v >> 2) & 0x07);        
        m_at = m_bus->read_ppu(addr);
    
        if (m_v & 0x02)
            m_at >>= 2;
        if ((m_v >> 5) & 0x02)
            m_at >>= 4;

        m_at &= 0x03;
        break;

    case 4:
        addr = (m_bg_table << 12) + (m_nt << 4) + (m_v >> 12);
        m_pt_lo = m_bus->read_ppu(addr);
        break;

    case 6:
        addr = (m_bg_table << 12) + (m_nt << 4) + (m_v >> 12) + 8;
        m_pt_hi = m_bus->read_ppu(addr);
        break;
        
    case 7:
        increment_scroll_x();
        break;
    }

    if (m_cycle == 256)
        increment_scroll_y();

    if (m_cycle == 257)
    {
        reload_shifters();
        transfer_x();
    }
}

void PPU::secondary_oam_clear()
{
    if (0 < m_cycle && m_cycle < 65)
        m_secoam[m_cycle % m_secoam.size()] = m_oamdata = 0xFF;
}

bool PPU::sprite_evaluation()
{
    if (m_cycle < 65 || m_cycle > 256)
        return false;

    INIT_SEGMENTS

    static int n = 0, m = 0;    /* Primary OAM Indices */
    static int k = 0;           /* Secondary OAM Index */

    if (m_cycle % 2)
    {
        m_oamdata = m_oam[4*n+m];
        return false;
    }
    else
    {
        START_SEGMENT(1)

        m_secoam[k % m_secoam.size()] = m_oamdata;
        
        if (m_oamdata <= m_scanline && m_scanline < m_oamdata + (m_sprite_height ? 16 : 8))
        {
            m = (m+1) % 4;
            ++k;
        }
        else
        {
            n = (n+1) % 64;
            
            if (n == 0)
                GOTO_SEGMENT(5)
            else
                REPEAT_SEGMENT
        }
        
        NEXT_SEGMENT(2)
        
        m_secoam[k % m_secoam.size()] = m_oamdata;

        ++k;
        m = (m+1) % 4;

        if (m == 0)
        {
            n = (n+1) % 64;

            if (n == 0)
                GOTO_SEGMENT(5)
            
            else if (k < m_secoam.size())
                GOTO_SEGMENT(1)
        }
        else
            REPEAT_SEGMENT;

        NEXT_SEGMENT(3)

        if (!(m_oamdata <= m_scanline && m_scanline < m_oamdata + (m_sprite_height ? 16 : 8)))
        {
            m = (m+1) % 4;
            n = (n+1) % 64;

            if (n == 0)
                GOTO_SEGMENT(5)
            else
                REPEAT_SEGMENT
        }
        else
        {
            m_sprite_overflow = 1;
            m = (m+1) % 4;
        }

        NEXT_SEGMENT(4)

        m = (m+1) % 4;

        if (m == 0)
        {
            n = (n+1) % 64;
            if (n != 0)
                GOTO_SEGMENT(3)
        }
        else
            REPEAT_SEGMENT

        END_SEGMENT

        n = (n+1) % 64;

        if (m_cycle < 256)
            return false;
    }

    n = m = k = 0;
    FREE_SEGMENTS
    return true;
}

void PPU::sprite_fetch()
{
    if (m_cycle == 320)
    {
        for (int i = 0; i < 8; ++i)
        {
            m_sprites[i].y = m_secoam[4*i]+1;
            m_sprites[i].tile = m_secoam[4*i+1];
            m_sprites[i].flip_v = m_secoam[4*i+2] & 0x80;
            m_sprites[i].flip_h = m_secoam[4*i+2] & 0x40;
            m_sprites[i].priority = m_secoam[4*i+2] & 0x20;
            m_sprites[i].pal_hi = m_secoam[4*i+2] & 0x03;
            m_sprites[i].x = m_secoam[4*i+3];
        }
    }
}

void PPU::background_render()
{
    m_bg_pal = 0;

    if (!m_bg_enable || m_cycle < (m_bg_left_enable ? 1 : 8) || m_cycle > 256)
        return;

    auto mux = 0x8000 >> m_x;

    auto bg_pal_lo_lsb = (m_pt_lsb & mux) ? 1 : 0;
    auto bg_pal_lo_msb = (m_pt_msb & mux) ? 1 : 0;

    auto bg_pal_lo = (bg_pal_lo_msb << 1) | bg_pal_lo_lsb;

    auto bg_pal_hi_lsb = (m_at_lsb & mux) ? 1 : 0;
    auto bg_pal_hi_msb = (m_at_msb & mux) ? 1 : 0;

    auto bg_pal_hi = (bg_pal_hi_msb << 1) | bg_pal_hi_lsb;

    m_bg_pal = (bg_pal_hi << 2) | bg_pal_lo;

    m_bg_pal &= 0x001F;

    if (m_bg_pal == 0x0010) m_bg_pal = 0x0000;
    if (m_bg_pal == 0x0014) m_bg_pal = 0x0004;
    if (m_bg_pal == 0x0018) m_bg_pal = 0x0008;
    if (m_bg_pal == 0x001C) m_bg_pal = 0x000C;

}

void PPU::sprite_render()
{
    m_sprite_pal = 0;

    if (m_sprite_enable && m_scanline != 0)
    {
        for (m_sprite_idx = 0; m_sprite_idx < 8; ++m_sprite_idx)
        {
            const auto &sprite = m_sprites[m_sprite_idx];

            if (sprite.y <= m_scanline && m_scanline < sprite.y + (m_sprite_height ? 16 : 8))
                if (sprite.x <= m_cycle && m_cycle < sprite.x + 8)
                    break;
        }
        
        if (m_sprite_idx < 8)
        {
            const auto &sprite = m_sprites[m_sprite_idx];

            auto addr = 0;

            if (m_sprite_height == 0)
            {
                if (sprite.flip_v)
                    addr = (m_sprite_table << 12) | (sprite.tile << 4) | (7 - (m_scanline - sprite.y));
                else
                    addr = (m_sprite_table << 12) | (sprite.tile << 4) | (m_scanline - sprite.y);

            }
            else
            {
                if (sprite.flip_v)
                {
                    if (m_scanline - sprite.y < 8)
                        addr = ((sprite.tile & 1) << 12) | (sprite.tile | 0x01) << 4 | (7 - (m_scanline - sprite.y));                    
                    else
                        addr = ((sprite.tile & 1) << 12) | (sprite.tile & 0xFE) << 4 | (7 - (m_scanline - sprite.y - 8));
                }
                else
                {
                    if (m_scanline - sprite.y < 8)
                        addr = ((sprite.tile & 1) << 12) | (sprite.tile & 0xFE) << 4 | (m_scanline - sprite.y);
                    else
                        addr = ((sprite.tile & 1) << 12) | (sprite.tile | 0x01) << 4 | (m_scanline - sprite.y - 8);
                }
            }

            auto spr_pt_lo = m_bus->read_ppu(addr);
            auto spr_pt_hi = m_bus->read_ppu(addr + 8);

            auto mux = 0x80;

            if (sprite.flip_h)
                mux >>= 7 - (m_cycle - sprite.x);
            else
                mux >>= m_cycle - sprite.x;

            auto spr_pal_lo_lsb = (spr_pt_lo & mux) ? 1 : 0;
            auto spr_pal_lo_msb = (spr_pt_hi & mux) ? 1 : 0;

            auto spr_pal_lo = (spr_pal_lo_msb << 1) | spr_pal_lo_lsb;

            m_sprite_pal = 0x10 | (sprite.pal_hi << 2) | spr_pal_lo;

            m_sprite_pal &= 0x001F;
        
            if (m_sprite_pal == 0x0010) m_sprite_pal = 0x0000;
            if (m_sprite_pal == 0x0014) m_sprite_pal = 0x0004;
            if (m_sprite_pal == 0x0018) m_sprite_pal = 0x0008;
            if (m_sprite_pal == 0x001C) m_sprite_pal = 0x000C;
        }
    }
    
    if (m_sprite_idx == 0 && m_sprite_enable && m_bg_enable && 
        (m_sprite_left_enable ? 0 : 8) < m_cycle && m_cycle < 256)
    {
        m_sprite_zero_hit |= ((m_sprite_pal & 0x0F) % 4) && ((m_bg_pal & 0x0F) % 4);
    }
}

void PPU::render()
{
    if (!(0 < m_cycle && m_cycle < 257))
        return;

    auto bg_col = read_palette_ram(m_bg_pal);
    auto spr_col = read_palette_ram(m_sprite_pal);

    auto col = read_palette_ram(0);

    if ((m_bg_pal & 0x0F) % 4 && (m_sprite_pal & 0x0F) % 4)
    {
        if (m_sprites[m_sprite_idx].priority)
            col = bg_col;
        else
            col = spr_col;
    }
    else if ((m_bg_pal & 0x0F) % 4)
        col = bg_col;

    else if ((m_sprite_pal & 0x0F) % 4)
        col = spr_col;

    for (int i = 0; i < 3; ++i)
        m_framebuffer[m_scanline][m_cycle-1][i] = m_palette_map[3*col+i];
}
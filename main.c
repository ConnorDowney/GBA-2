#include "gba.h"

void sync()
{
    while(REG_DISPLAY_VCOUNT >= 160);
    while(REG_DISPLAY_VCOUNT < 160);
}

void drawRect(struct Rect r, uint16 color)
{
    for(int i=0;i<r.h;i++)
    {
        for(int j=0;j<r.w;j++)
        {
            SCREENBUFFER[SCREEN_WIDTH * (i+r.y) + (j+r.x)] = color;
        }
    }
}

void init7seg()
{
    a.w = 16;
    a.h = 4;
    f.w = 4;
    f.h = 16;
    b = c = e = f;
    d = g = a;

    a.x = d.x = e.x = f.x = g.x = SCREEN_WIDTH/2;
    b.x = c.x = a.x+a.w;

    a.y = b.y = f.y = 0;
    c.y = e.y = g.y = b.y+b.h-a.h;
    d.y = g.y+b.h-a.h;
}

void clear7seg()
{
    drawRect(a, 0x0000);
    drawRect(b, 0x0000);
    drawRect(c, 0x0000);
    drawRect(d, 0x0000);
    drawRect(e, 0x0000);
    drawRect(f, 0x0000);
    drawRect(g, 0x0000);
}

void draw7seg(uint8 num)
{
    clear7seg();

    uint8 w, x, y, z;

    w = num & 8;
    x = num & 4;
    y = num & 2;
    z = num & 1;

    if(y || w || (x && z) || (!x && !z))
    {
        drawRect(a, 0x001f);
    }
    if(!x || (!y && !z) || (y && z))
    {
        drawRect(b, 0x001f);
    }
    if(!y || x || z)
    {
        drawRect(c, 0x001f);
    }
    if(w || (!x && !z) || (!x && y) || (y && !z) || (x && !y && z))
    {
        drawRect(d, 0x001f);
    }
    if((!x && !z) || (y && !z))
    {
        drawRect(e, 0x001f);
    }
    if(w || (!y && !z) || (x && !y) || (x && !z))
    {
        drawRect(f, 0x001f);
    }
    if(w || (!x && y) || (y && !z) || (x && !y))
    {
        drawRect(g, 0x001f);
    }
}

int main()
{
    REG_DISPLAY = VIDEOMODE | BGMODE;

    init7seg();

    uint8 counter = 0;
    uint16 c = 0x001f;

    uint8 down = 0;

    while(1)
    {
        sync();
        
        if(!(REG_DISPLAY_INPUT & A) && !down)
        {
            down = 1;
            counter++;
            if(counter>9)
            {
                counter = 0;
            }
        }
        else if(A & REG_DISPLAY_INPUT)
        {
            down = 0;
        }

        draw7seg(counter);
    }
    return 0;
}
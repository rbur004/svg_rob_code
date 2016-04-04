/* Not sure when this was created, or even if it was a working version */
/* It is for Mac OS, based on the Quickdraw calls, and the disk I found it on */
/* It does have a better approach than the SVG version, which I created from memory */
/*It assumes all bytes are written in order, so there is no need to calculate the byte position the hard way. */

#include <Types.h>
#include <Memory.h>
#include <Quickdraw.h>
#include <Fonts.h>
#include <Events.h>
#include <Menus.h>
#include <Windows.h>
#include <TextEdit.h>
#include <Dialogs.h>
#include <Sound.h>
#include <ToolUtils.h>
#include <Processes.h>
#include <math.h>

#define CENTER_H 200
#define CENTER_V 200
#define RING_WIDTH 20
#define LABELSIZE 12

Rect    windowRect;

void Initialize(void);

short bytesInRing(short ring, short robOrder);
    short bytesInRingLinear(short ring, short robOrder);
    short bytesInRingExp(short ring, short robOrder);
    
void drawRoBCode(short h, short v, short width, short robOrder, char *s);

void drawRingSegment(short h, short v, short innerRadius, short width, float startAngle, float wedgeAngle);
    void drawRingSegment1(short h, short v, short innerRadius, short width, float startAngle, float wedgeAngle);
    void drawRingSegment2(short h, short v, short innerRadius, short width, float startAngle, float wedgeAngle);


long main(void)
{
    Initialize();
    drawRoBCode(CENTER_H, CENTER_V, RING_WIDTH, 2, "Hello World\n");
    while(!Button);
    return 0;
}

/*init Mac window stuff*/
void Initialize(void)
{
    WindowPtr   windowPtr;
    /* Initialize all the needed managers. */
    InitGraf(&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nil);
    InitCursor();

    windowRect = qd.screenBits.bounds;
    InsetRect(&windowRect, 50, 50);
    windowPtr = NewCWindow(nil, &windowRect, "\pRoBCode", true, documentProc, (WindowPtr) -1, false, 0);

    SetPort(windowPtr); /* set window to current graf port */
    TextSize(LABELSIZE);
}

short bytesInRing(short ring, short robOrder)
{
    return bytesInRingLinear(ring,robOrder);
}

short bytesInRingLinear(short ring, short robOrder)
{
    return ring * robOrder;
}

short bytesInRingExp(short ring, short robOrder)
{
    return (short) powf(robOrder, ring);
}

void drawRoBCode(short h, short v, short width, short robOrder, char *s)
{
short ring = 1;
short byteIndex = 0;
short bytesInRing_v = bytesInRing(ring,robOrder);
float wedgeAngle = 360 / (bytesInRing_v * 9);
float startAngle;
char *p;
long i,b;
long parity = 0;

    drawInnerCircle(h,v,width); /*Central Circle*/
    
    for(p = s; *p != 0; p++) /*for each string char*/
    {   
        for(i=0, b = ((long)*p)&0xFF; i < 8; i++) /*for each bit in string char*/
        {
            if(b & 1)
            {   parity ^= 1;
                startAngle = wedgeAngle * (byteIndex * 9 + i);
                drawRingSegment(h, v, ring * width, width, startAngle, wedgeAngle);
            }
            b >>= 1; /*Shift to get next bit*/
        }
        if(parity & 1)
        {   
            startAngle = wedgeAngle * (byteIndex * 9 + 8);
            drawRingSegment(h, v, ring * width, width, startAngle, wedgeAngle);
        }
        if(++byteIndex == bytesInRing_v)
        {   /*Move to next ring*/
            ring++;
            byteIndex = 0;
            bytesInRing_v = bytesInRing(ring,robOrder);
            wedgeAngle = 360 / (bytesInRing_v * 9);
        }
    }
}

void drawRingSegment(short h, short v, short innerRadius, short width, float startAngle, float wedgeAngle) 
{
    drawRingSegment2(h, v, innerRadius, width, startAngle, wedgeAngle);
}

/*Draw arc using pen width the same as the ring width*/
void drawRingSegment1(short h, short v, short innerRadius, short width, float startAngle, float wedgeAngle) 
{
Rect    r;
short outerRadius = innerRadius + width;
short i_startAngle = (short)(startAngle + 0.5);
short arc_angle = (short)(wedgeAngle + 0.5);

    MoveTo(h-outer_radius, v+outer_radius); /*Move pen to top left of the circle*/
    PenNormal(); /*Quick and dirty way to get pen back to state we want*/

    /*Short angles is going to have some overlap issues. */
    /*Alternative is to code arc drawing ourselves! */
    SetRect(&r, h-outer_radius, v+outer_radius, h+outer_radius, v-outer_radius); /*bound circle*/
    PenSize(width, width); /*Thick pen to fill wedge*/
    FrameArc(&r, i_startAngle, arc_angle); /*Draw the wedge*/
}

/*Alternate drawRingSegment, drawing multiple arcs of 1 pixel width*/
void drawRingSegment2(short h, short v, short innerRadius, short width, float startAngle, float wedgeAngle) 
{
Rect r;
long i;
short outerRadius = innerRadius + width;
short i_startAngle = (short)(startAngle + 0.5);
short arc_angle = (short)(wedgeAngle + 0.5)

    PenNormal(); /*Quick and dirty way to get pen back to state we want*/
    /*Short angles is going to have some overlap issues. */
    /*Draw several Arcs, to fill space*/
    for(i = innerRadius; i < outerRadius )
    {
      SetRect(&r, (short)(h-i), (short)(v+i), (short)(h+i), (short)(v-i)); /*bound circle*/
      MoveTo((short)(h-i),(short)(v+i)); /*Move pen to top left of the circle*/
      FrameArc(&r, i_startAngle, arc_angle); /*Draw the wedge*/
    }
}

void drawInnerCircle(short h, short v, short radius)
{
Rect r;
    PenNormal(); /*Quick and dirty way to get pen back to state we want*/
    MoveTo((short)(h-i), (short)(v+i));
    SetRect(&r, (short)(h-radius), (short)(v+rodius), (short)(h+radius), (short)(v-radius)); /*bound circle with square*/
    PaintOval (&r);
}

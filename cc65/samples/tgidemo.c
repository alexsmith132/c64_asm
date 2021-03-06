#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <modload.h>
#include <tgi.h>





/*****************************************************************************/
/*     	      	    	  	     Data				     */
/*****************************************************************************/



static const unsigned char SinusTable[] = {
    0x64,0x63,0x61,0x5F,0x5D,0x5B,0x59,0x57,0x55,0x54,
    0x52,0x50,0x4E,0x4C,0x4A,0x49,0x47,0x45,0x43,0x42,
    0x40,0x3E,0x3C,0x3B,0x39,0x38,0x36,0x34,0x33,0x31,
    0x30,0x2E,0x2D,0x2B,0x2A,0x28,0x27,0x26,0x24,0x23,
    0x22,0x20,0x1F,0x1E,0x1D,0x1C,0x1B,0x1A,0x19,0x18,
    0x17,0x16,0x15,0x14,0x13,0x12,0x12,0x11,0x10,0x10,
    0x0F,0x0E,0x0E,0x0D,0x0D,0x0C,0x0C,0x0C,0x0B,0x0B,
    0x0B,0x0B,0x0B,0x0B,0x0B,0x0A,0x0B,0x0B,0x0B,0x0B,
    0x0B,0x0B,0x0B,0x0C,0x0C,0x0C,0x0D,0x0D,0x0E,0x0E,
    0x0F,0x10,0x10,0x11,0x12,0x12,0x13,0x14,0x15,0x16,
    0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,
    0x22,0x23,0x24,0x26,0x27,0x28,0x2A,0x2B,0x2D,0x2E,
    0x30,0x31,0x33,0x34,0x36,0x38,0x39,0x3B,0x3C,0x3E,
    0x40,0x42,0x43,0x45,0x47,0x49,0x4A,0x4C,0x4E,0x50,
    0x52,0x54,0x55,0x57,0x59,0x5B,0x5D,0x5F,0x61,0x63,
    0x64,0x65,0x67,0x69,0x6B,0x6D,0x6F,0x71,0x73,0x74,
    0x76,0x78,0x7A,0x7C,0x7E,0x7F,0x81,0x83,0x85,0x86,
    0x88,0x8A,0x8C,0x8D,0x8F,0x91,0x92,0x94,0x95,0x97,
    0x98,0x9A,0x9B,0x9D,0x9E,0xA0,0xA1,0xA2,0xA4,0xA5,
    0xA6,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,
    0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB6,0xB7,0xB8,0xB8,
    0xB9,0xBA,0xBA,0xBB,0xBB,0xBC,0xBC,0xBC,0xBD,0xBD,
    0xBD,0xBD,0xBD,0xBD,0xBD,0xBE,0xBD,0xBD,0xBD,0xBD,
    0xBD,0xBD,0xBD,0xBC,0xBC,0xBC,0xBB,0xBB,0xBA,0xBA,
    0xB9,0xB8,0xB8,0xB7,0xB6,0xB6,0xB5,0xB4,0xB3,0xB2,
    0xB1,0xB0,0xAF,0xAE,0xAD,0xAC,0xAB,0xAA,0xA9,0xA8,
    0xA6,0xA5,0xA4,0xA2,0xA1,0xA0,0x9E,0x9D,0x9B,0x9A,
    0x98,0x97,0x95,0x94,0x92,0x91,0x8F,0x8D,0x8C,0x8A,
    0x88,0x86,0x85,0x83,0x81,0x7F,0x7E,0x7C,0x7A,0x78,
    0x76,0x74,0x73,0x71,0x6F,0x6D,0x6B,0x69,0x67,0x65
};



/* Driver stuff */
static unsigned XRes;
static unsigned YRes;



/*****************************************************************************/
/*     	      	    	    	     Code				     */
/*****************************************************************************/



static void CheckError (const char* S)
{
    unsigned char Error = tgi_geterror ();
    if (Error != TGI_ERR_OK) {
        printf ("%s: %d\n", S, Error);
        exit (EXIT_FAILURE);
    }
}



static void DoWarning (void)
/* Warn the user that the TGI driver is needed for this program */
{
    printf ("Warning: This program needs the TGI\n"
            "driver on disk! Press 'y' if you have\n"
            "it - any other key exits.\n");
    if (cgetc () != 'y') {
        exit (EXIT_SUCCESS);
    }
    printf ("Ok. Please wait patiently...\n");
}



static void DoCircles (void)
{
    static const unsigned char Palette[2] = { COLOR_WHITE, COLOR_LIGHTRED };
    unsigned char I;
    unsigned char Color = 1;
    unsigned X = XRes / 2;
    unsigned Y = YRes / 2;

    tgi_setpalette (Palette);
    while (!kbhit ()) {
        tgi_setcolor (1);
        tgi_line (0, 0, XRes-1, YRes-1);
        tgi_line (0, YRes-1, XRes-1, 0);
        tgi_setcolor (Color);
        for (I = 10; I < 240; I += 10) {
            tgi_circle (X, Y, I);
        }
        Color ^= 0x01;
    }

    cgetc ();
    tgi_clear ();
}



static void DoCheckerboard (void)
{
    static const unsigned char Palette[2] = { COLOR_WHITE, COLOR_BLACK };
    unsigned X, Y;
    unsigned char Color;

    tgi_setpalette (Palette);
    Color = 0;
    while (1) {
        for (Y = 0; Y < YRes; Y += 10) {
            for (X = 0; X < XRes; X += 10) {
                tgi_setcolor (Color);
                tgi_bar (X, Y, X+9, Y+9);
                Color ^= 0x01;
                if (kbhit ()) {
                    cgetc ();
                    tgi_clear ();
                    return;
                }
            }
            Color ^= 0x01;
        }
        Color ^= 0x01;
    }
}



static void DoDiagram (void)
{
    static const unsigned char Palette[2] = { COLOR_WHITE, COLOR_BLACK };
    unsigned X, I;

    tgi_setpalette (Palette);
    tgi_setcolor (1);
    tgi_line (10, 10, 10, YRes-10);
    tgi_lineto (XRes-10, YRes-10);
    tgi_line (8, 12, 10, 10);
    tgi_lineto (12, 12);
    tgi_line (XRes-12, YRes-12, XRes-10, YRes-10);
    tgi_lineto (XRes-12, YRes-8);
    for (I = 0, X = 10; X < XRes-10; ++X) {
        tgi_setpixel (X, SinusTable[I]);
	if (++I >= sizeof (SinusTable)) {
	    I = 0;
	}
    }

    cgetc ();
    tgi_clear ();
}



static void DoLines (void)
{
    static const unsigned char Palette[2] = { COLOR_WHITE, COLOR_BLACK };
    unsigned X;

    tgi_setpalette (Palette);
    tgi_setcolor (1);

    for	(X = 0; X < YRes; X+=10) {
	tgi_line (0, 0, YRes, X);
	tgi_line (0, 0, X, YRes);
	tgi_line (YRes, YRes, 0, YRes-X);
	tgi_line (YRes, YRes, YRes-X, 0);
    }

    cgetc ();
    tgi_clear ();
}



int main (void)
{
    unsigned char Border;

    /* Warn the user that the tgi driver is needed */
    DoWarning ();

    /* Load and initialize the driver */
    tgi_load (TGI_MODE_320_200_2);
    CheckError ("tgi_load");
    tgi_init ();
    CheckError ("tgi_init");

    /* Get stuff from the driver */
    XRes = tgi_getxres ();
    YRes = tgi_getyres ();

    /* Set the palette, set the border color */
    Border = bordercolor (COLOR_BLACK);

    /* Do graphics stuff */
    DoCircles ();
    DoCheckerboard ();
    DoDiagram ();
    DoLines ();

    /* Unload the driver */
    tgi_unload ();

    /* Reset the border */
    bordercolor (Border);

    /* Done */
    printf ("Done\n");
    return EXIT_SUCCESS;
}



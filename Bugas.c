// Bugas intro for Revision 2021
// Amiga Skool
// Code & gfx: Balrog Soft
// Music: Samplemind


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "custom_defines.h"

#include "ptplayer.h"

#include "c2p_6b.h"

#include "System.h"

#include "Math.h"

#include "clist.h"

#include "Renderer.h"

#include "Rotoscoping.h"

#include "resources.h"

ULONG fps = 0;

UWORD copwait[] = {0xffdf, 0xfffe};

UWORD copend[] = {0xffff, 0xfffe,
                  0xffff, 0xfffe};
                  
UWORD copcol[] = {0x5229, 0xfffe,
                  0x0106, 0x0000,
                  0x0182, 0x0000,
                  0x0106, 0x0200,
                  0x0182, 0x0000};

UBYTE *greetings[] = {
                    "GREETINGS",
                    "BATMAN GROUP",
                    "CAPSULE",
                    "GOBLINS",
                    "SOFTWARE FAILURE"};

                  
                  
UBYTE *credits[] = {"BUGAS IS THE",
                    "AMIGA SKOOL", 
                    "CONTRIBUTION",
                    "FOR REVISION 2021",
                    "",
                    "A TRIBUTE TO THE",
                    "GREAT GARY NUMAN",
                    "",
                    "MUSIC:",
                    "    SAMPLEMIND",
                    "CODE AND GFX:",
                    "    BALROG SOFT",
                    "",
                    "BUGAS IS A POPULAR",
                    "SPANISH WORD USED",
                    "IN THE 80'S THAT",
                    "MEANS CARS.", 
                    "",
                    "SPECIAL THANKS TO",
                    "ESTRAYK, IOLO,",
                    "SKYNET, WINDEN,",
                    "AND THE BROTH3RS,",
                    "",
                    "AND GREETINGS TO",
                    "ALL THE PEOPLE IN",
                    "THE AMIGA TELEGRAM",
                    "GROUPS FROM SPAIN,", 
                    "YOU ARE THE BEST!"};

UBYTE *cars_lyrics_1[] = {
                        "HERE IN MY CAR",
                        "I FEEL SAFEST OF ALL",
                        "I CAN LOCK ALL MY DOORS",
                        "IT'S THE ONLY WAY TO LIVE"};

UBYTE cars_lyrics_pos_1[] = {90,58,38,35};

WORD cars_lyrics_cvb_1[] = {2315,2390,2480,2570,2683};

UBYTE *cars_lyrics_2[] = {
                          "HERE IN MY CAR",
                          "I CAN ONLY RECEIVE",
                          "I CAN LISTEN TO YOU",
                          "IT KEEPS ME STABLE FOR DAYS"};
                          
UBYTE cars_lyrics_pos_2[] = {90,68,65,18};

WORD cars_lyrics_cvb_2[] = {2885,2965,3055,3145,3258};

void drawCarScene(RotoscopingScene *road, RotoscopingScene *car, WORD x, WORD y, WORD roady, UBYTE rnd) {
    UBYTE l = 0;
    if (rnd == 1)
        l = ((random()&7) == 0 ? 1 : 0);
    if (roady >= 0)
        renderScene(0, roady, road, chunky, TRUE, TRUE);
    drawVectors(x, y, car->scene[0].nvectors, car->scene[0].vectors, car->scene[0].keyframes, chunky, rowchunky);
    drawVectors(x, y+1+l, car->scene[1].nvectors, car->scene[1].vectors, car->scene[1].keyframes, chunky, rowchunky);
    drawVectors(x, y+1, car->scene[2].nvectors, car->scene[2].vectors, car->scene[2].keyframes, chunky, rowchunky);
}


int main()
{
    ULONG *wireback = NULL;
    ULONG *boingback = NULL;
    ULONG *carback = NULL;
    ULONG *colorMap = NULL;
    ULONG *origColorMap = NULL;
    UBYTE *delorean = NULL;
    UBYTE *delorean_mask = NULL;
    UBYTE *ateam = NULL;
    UBYTE *ateam_mask = NULL;
    
    UWORD *alphaCop = NULL;
    UWORD *textCop = NULL;

    WireObject *dmc = NULL;
    WireObject *dogcookie = NULL;
    WireObject *doghead = NULL;
    WireObject *dogbody = NULL;
    WireObject *gmc = NULL;
    WireObject *donpino = NULL;
    WireObject *bugas = NULL;
    WireObject *cassette = NULL;
    WireObject *player = NULL;
    WireObject *speaker = NULL;
    WireObject *wheel = NULL;
    WireObject *glasses = NULL;
    WireObject *boing = NULL;
    WireObject *city = NULL;
    WireObject *gary1 = NULL;
    WireObject *gary2 = NULL;
    WireObject *gary3 = NULL;
    
    RotoscopingScene road;
    RotoscopingScene car2d;
    RotoscopingScene dmc2d;
    RotoscopingScene gmc2d;
    RotoscopingScene kitt2d;
    RotoscopingScene kraftwerk;
    
    BitmapFont *bfont = NULL;
    BitmapFont *sfont = NULL;
    
    struct ColorMap* cm;
    UWORD *cl;
    UBYTE *ptr;
    UBYTE *ptr10 = NULL, *ptr11, *ptr12, *ptr13, *ptr14, *ptr15;
    UBYTE *ptr20 = NULL, *ptr21, *ptr22, *ptr23, *ptr24, *ptr25;
    ULONG *ptrl, *ptrlwire;
    
    ULONG *skyrgb;
    UWORD *skyhi, *skylo;
    UWORD coplistSize;
    
    LONG alphaBlendPalette[64];
    
    LONG sky[5] = {0x261f6e, 
                   0xca2a2b,
                   0xd6ce4d,
                   0x673064,
                   0x3a223a};

    WORD credits_pos[5][2] = {{33,21},{114,40},{31,60},{95,45},{65,65}};
                   
    LONG redfade[180], greenfade[180], bluefade[180];

    WORD i, j, l, m;
    WORD checkboard_z[8];
    UWORD wireframe_inc;
    WORD scroller;
    LONG k;
    UBYTE iscene = 0;
    
    UWORD lcol;
    
    UBYTE path[16] = {0,7,15,21,25,28,29,31,31,30,28,25,22,17,12,5};
    UBYTE cpath = 0;
          
    WORD gradient[4] = {180, 280, 286, 360};
    WORD lines[15][6] = {{0, 31, 319, 31, 0x2f, 2},
                        {159, 37, 159-55, 102, 0x3f, 3},
                        {159, 37, 159+55, 102, 0x3f, 3},
                        {158, 37, 159-150, 102, 0x3f, 3},
                        {160, 37, 159+150, 102, 0x3f, 3},
                        {157, 37, 1, 75, 0x3f, 3},
                        {161, 37, 319, 75, 0x3f, 3},
                        {156, 37, 1, 60, 0x3f, 3},
                        {162, 37, 319, 60, 0x3f, 3},
                        {155, 37, 1, 52, 0x3f, 3},
                        {163, 37, 319, 52, 0x3f, 3},
                        {154, 37, 1, 45, 0x3f, 3},
                        {164, 37, 319, 45, 0x3f, 3},
                        {154, 37, 1, 39, 0x3f, 3},
                        {164, 37, 319, 39, 0x3f, 3}};
        
	UWORD cars_palette_hi[] = {0x000,
                               0x555,
                               0x888,
                               0xAA9,
                               0xFFF,
                               0xF10};
                            
	UWORD cars_palette_lo[] = {0x000,
                               0xDE8,
                               0x796,
                               0x02F,
                               0xCFB,
                               0xCB2};
    
    
    WORD delorean_bg_points[] = {194<<FP_2D,0,
                                 320<<FP_2D,0,
                                 320<<FP_2D,41<<FP_2D,
                                 
                                 0,0,
                                 0,65<<FP_2D,
                                 320<<FP_2D,68<<FP_2D,
                                 
                                 0,108<<FP_2D,
                                 320<<FP_2D,85<<FP_2D,
                                 
                                 
                                 0,180<<FP_2D,
                                 320<<FP_2D,115<<FP_2D,
                                 
                                 8<<FP_2D,180<<FP_2D,
                                 
                                 
                                 320<<FP_2D,149<<FP_2D,
                                 247<<FP_2D,180<<FP_2D,
                                 
                                 286<<FP_2D,180<<FP_2D,
                                 320<<FP_2D,162<<FP_2D,
                                 
                                 320<<FP_2D,180<<FP_2D
                                };
                               
    WORD delorean_bg_vectors[] = {0,1,2,0x3737,0x3737,
                                  3,0,2,0x2f2f,0x2f2f,
                                  3,2,5,0x2f2f,0x2f2f,
                                  3,5,4,0x2f2f,0x2f2f,
                                  
                                  
                                  4,5,7,0x2626,0x2626,
                                  4,7,6,0x2626,0x2626,
                                    
                                  6,7,8,0x1717,0x1717,
                                  7,8,9,0x1717,0x1717,
                                  8,9,10,0x1717,0x1717,
                                  
                                  9,10,11,0x0f0f,0x0f0f,
                                  10,11,12,0x0f0f,0x0f0f,
                                  
                                  11,12,13,0x2626,0x2626,
                                  11,13,14,0x2626,0x2626,
                                  
                                  13,14,15,0x2f2f,0x2f2f
    };
    
    
    
    // Open libs
    initMathLibs();
    IntuitionBase = (struct IntuitionBase*)OldOpenLibrary("intuition.library");
    
   	// open gfx lib and save original copperlist
  	GfxBase = (struct GfxBase*)OldOpenLibrary("graphics.library");
   	
    oldCopinit = GfxBase->copinit;
   	oldview = GfxBase->ActiView;
    oldViewPort = &IntuitionBase->ActiveScreen->ViewPort;
    cm = oldViewPort->ColorMap;
    
    // Alloc memory
    
    colorMap = AllocMem(((cm->Count*3)*4)+4, MEMF_CHIP);
    origColorMap = AllocMem(((cm->Count*3)*4)+4, MEMF_CHIP);
    wireback = (ULONG*)AllocMem(320*110, MEMF_ANY | MEMF_CLEAR);
    boingback = (ULONG*)AllocMem(320*180, MEMF_ANY | MEMF_CLEAR);
    carback = (ULONG*)AllocMem(320*180, MEMF_ANY | MEMF_CLEAR);
    
    delorean = (UBYTE*)AllocMem(208*122, MEMF_ANY | MEMF_CLEAR);
    delorean_mask = (UBYTE*)AllocMem((208>>2)*122, MEMF_ANY | MEMF_CLEAR);
    ateam = (UBYTE*)AllocMem(176*106, MEMF_ANY | MEMF_CLEAR);
    ateam_mask = (UBYTE*)AllocMem((176>>2)*106, MEMF_ANY | MEMF_CLEAR);
    chunky = (UBYTE*)AllocMem(62400, MEMF_ANY | MEMF_CLEAR);
        
    textCop = (UWORD*)AllocMem(908, MEMF_ANY);
    alphaCop = (UWORD*)AllocMem(524, MEMF_ANY);
    skyrgb = (ULONG*)AllocMem(180*4, MEMF_ANY);
    skyhi = (UWORD*)AllocMem(360*2, MEMF_ANY);
    skylo = (UWORD*)AllocMem(360*2, MEMF_ANY);
    cop1 = (UWORD*)AllocMem(sizeof(coplist)+7200, MEMF_CHIP | MEMF_CLEAR);
    cop2 = (UWORD*)AllocMem(sizeof(coplist)+7200, MEMF_CHIP | MEMF_CLEAR);
    
    
    ptr10 = AllocMem(((WIDTH * HEIGHT) >> 3) * 6, MEMF_CHIP | MEMF_CLEAR);
    ptr11 = ptr10 + 7200;
    ptr12 = ptr11 + 7200;
    ptr13 = ptr12 + 7200;
    ptr14 = ptr13 + 7200;
    ptr15 = ptr14 + 7200;
    
    ptr20 = AllocMem(((WIDTH * HEIGHT) >> 3) * 6, MEMF_CHIP | MEMF_CLEAR);
    ptr21 = ptr20 + 7200;
    ptr22 = ptr21 + 7200;
    ptr23 = ptr22 + 7200;
    ptr24 = ptr23 + 7200;
    ptr25 = ptr24 + 7200;
    
    if (wireback      != NULL && 
        boingback     != NULL &&
        carback       != NULL &&
        delorean      != NULL &&
        delorean_mask != NULL &&
        ateam         != NULL &&
        ateam_mask    != NULL &&
        chunky        != NULL &&
        textCop       != NULL &&
        alphaCop      != NULL &&
        skyrgb        != NULL &&
        skyhi         != NULL &&
        skylo         != NULL &&
        cop1          != NULL &&
        cop2          != NULL && 
        ptr10         != NULL && 
        ptr20         != NULL) {
            
        generateSineCosTable();

        wireframe_inc = 2;
        for (i = 0; i < 8; i++) {
            checkboard_z[i] =  ((i+8)<<FP_2D);

            checkboard_z[i] += wireframe_inc<<1;
            if (checkboard_z[i]>0)
                checkboard_z[i] -= 1024;
        }

        for (i = 0; i < 151; i++) {
            city_mat[i] = i>=104?1:0;
        }
        k = 0;
        for (i = 0; i < 190; i++) {
            rowchunky[i] = k;
            k+=320;
        }
        for (i = 0; i < 180; i++) {
            redfade[i] = 0;
            greenfade[i] = 0;
            bluefade[i] = 0;
        }

        // Initialize resources
        
        unpackImage((UBYTE*)delorean_data, delorean, delorean_mask, 2767, 208, 122);
        unpackImage((UBYTE*)ateam_data, ateam, ateam_mask, 3140, 176, 106);
  
        bfont = createFont((UBYTE*)font, 3584, fontw, fontt, fonth, 27, 55);
        sfont = createFont((UBYTE*)font2, 2304, sfontw, sfontt, sfonth, 18, 55);
        
        dmc       = createWireObject(dmc_npoints, dmc_nedges, dmc_points, dmc_edges, NULL);
        dogcookie = createWireObject(dogcookie_npoints, dogcookie_nedges, dogcookie_points, dogcookie_edges, NULL);
        doghead   = createWireObject(doghead_npoints, doghead_nedges, doghead_points, doghead_edges, NULL);
        dogbody   = createWireObject(dogbody_npoints, dogbody_nedges, dogbody_points, dogbody_edges, NULL);
        gmc       = createWireObject(gmc_npoints, gmc_nedges, gmc_points, gmc_edges, NULL);
        donpino   = createWireObject(donpino_npoints, donpino_nedges, donpino_points, donpino_edges, NULL);
        glasses   = createWireObject(glasses_npoints, glasses_nedges, glasses_points, glasses_edges, NULL);
        gary1     = createWireObject(gary1_npoints, gary1_nedges, gary1_points, gary1_edges, NULL);
        gary2     = createWireObject(gary2_npoints, gary2_nedges, gary2_points, gary2_edges, NULL);
        gary3     = createWireObject(gary3_npoints, gary3_nedges, gary3_points, gary3_edges, NULL);
        wheel     = createWireObject(wheel_npoints, wheel_nedges, wheel_points, wheel_edges, NULL);
        cassette  = createWireObject(cassette_npoints, cassette_nedges, cassette_points, cassette_edges, NULL);
        player    = createWireObject(player_npoints, player_nedges, player_points, player_edges, NULL);
        speaker   = createWireObject(speaker_npoints, speaker_nedges, speaker_points, speaker_edges, NULL);
        boing     = createWireObject(boing_npoints, boing_nedges, boing_points, boing_edges, NULL);
        bugas     = createWireObject(bugas_npoints, bugas_nedges, bugas_points, bugas_edges, NULL);
        city      = createWireObject(city_npoints, city_nedges, city_points, city_edges, city_mat);
     
        createRotoFrame(&dmc2d, dmc2d_data);
        createRotoFrame(&gmc2d, gmc2d_data);
        createRotoFrame(&kitt2d, kitt2d_data);
        createRotoFrame(&car2d, car2d_data);
        
        createRotoScene(&road, road_data);
        createRotoScene(&kraftwerk, kraftwerk_data);

        CopyMem(coplist, cop1, sizeof(coplist));
        CopyMem(coplist, cop2, sizeof(coplist));
        
        coplistSize = (sizeof(coplist)>>1)-4;
        
        // Generate palettes and copper lists
        
        generateAlphaBlendingPalette(alphaBlendPalette, alphaCop);
        
        i = 0;
        
        for (k = 0; k < 360; k++) {
            UBYTE lred, lgreen, lblue;
            for (j = 0; j < 4; j++) {
                if (k < gradient[j]) {
                    WORD gradientLen = gradient[j] - (j>0?gradient[j-1]:0);
                    WORD gradientPos = k - (j>0?gradient[j-1]:0);
                    lred   = ((gradientPos*(sky[j+1]>>16))       + 
                            ((gradientLen-gradientPos)*(sky[j]>>16)))/gradientLen;
                    lgreen = ((gradientPos*((sky[j+1]>>8)&0xff)) + 
                            ((gradientLen-gradientPos)*((sky[j]>>8)&0xff)))/gradientLen;
                    lblue  = ((gradientPos*(sky[j+1]&0xff))      + 
                            ((gradientLen-gradientPos)*(sky[j]&0xff)))/gradientLen;
                    if (j == 0)
                        skyrgb[k] = (lred<<16) | (lgreen<<8) | lblue;
                    break;
                }
            }
            skyhi[k] = ((lred&0xf0)<<4) | (lgreen&0xf0) | ((lblue&0xf0)>>4);
            skylo[k] = ((lred&0x0f)<<8) | ((lgreen&0x0f)<<4) | (lblue&0x0f);
        }
        
        
        for (k = 0; k < 180; k++) {
            if (k == 174) {
                CopyMem(copwait, cop1+coplistSize+i, 4);
                CopyMem(copwait, cop2+coplistSize+i, 4);
            }
            copcol[0] = (((0x52+k)<<8)&0xff00) + (copcol[0]&0xff);

            CopyMem(copcol, cop1+coplistSize+i+(k>=174?2:0), 20);
            CopyMem(copcol, cop2+coplistSize+i+(k>=174?2:0), 20);
            cop1[coplistSize+i+5+(k>=174?2:0)] = skyhi[k];
            cop2[coplistSize+i+5+(k>=174?2:0)] = skyhi[k];
            cop1[coplistSize+i+9+(k>=174?2:0)] = skylo[k];
            cop2[coplistSize+i+9+(k>=174?2:0)] = skylo[k];
            i += 10;   
        }
        CopyMem(copend, cop1+coplistSize+1800+2, 8);
        CopyMem(copend, cop2+coplistSize+1800+2, 8);
        
        i = 0;
        lcol = 4;
        copcol[4] = 0x184;
        copcol[8] = 0x184;
        for (k = 0; k < 41; k++) {
            if ((k&1)==0) {
                if (k < 20) {
                    lcol += 1;
                    copcol[5] = ((lcol&0xf)<<4) | (lcol&0xf);
                    copcol[9] = ((lcol&0xf)<<4) | (lcol&0xf);
                }
                else if (k == 40) {
                    copcol[5] = 0;
                    copcol[9] = 0;
                }
                else {
                    if (k == 20)
                        lcol = 15;
                    copcol[5] = ((lcol&0xf)<<8) | (lcol&0xf);
                    copcol[9] = ((lcol&0xf)<<8) | (lcol&0xf);
                    lcol -= 1;
                }
        
                copcol[0] = (((0x75+k)<<8)&0xff00) + (copcol[0]&0xff);

                CopyMem(copcol, textCop+i, 20);
                i += 10;   
            }
        }
        
        CopyMem(copend, textCop+210, 8);
        
        
        for (k = 0; k < 14400; k++) {
            ((ULONG*)chunky)[k] = 0x01010101;
        }

        for (i = 0; i < 20; i+=3) {
            drawGlowLine((UBYTE*)wireback, 0, 15+i, 319, 15+i, i < 10 ? 0x0f : 0x1f, 2, rowchunky);
            if (i>5)
                drawGlowLine((UBYTE*)wireback, 0, 15+i+1, 319, 15+i+1, 0x0f, 1, rowchunky);
        }
        
        for (i = 0; i < 15; i++) {
            drawGlowLine((UBYTE*)wireback, lines[i][0], lines[i][1], lines[i][2], lines[i][3], lines[i][4], lines[i][5], rowchunky);
        }
    
        for (i = 0; i < 160; i+=32) {
            drawGlowLine((UBYTE*)boingback,20+i, 15+(i>64?(i>>2):0), 20+i, 150, 0x3f, 2, rowchunky);
            drawGlowLine((UBYTE*)boingback, 15, 20+i, 210-((180-i)>>1), 20+i, 0x3f, 1, rowchunky);
            drawGlowLine((UBYTE*)boingback,20+i, 150, 20+i-16, 170, 0x3f, 2, rowchunky);
        }
        drawGlowLine((UBYTE*)boingback, 8, 160, 170, 160, 0x3f, 1, rowchunky);
    
        for (j = 0; j < BUFFER2D_LEN; j++) {
            object2d.points[0][j] = 0;
        }
        
        
        palflag = SysBase->PowerSupplyFrequency < 59;
    
        vbr = (SysBase->AttnFlags & AFF_68010) ?
                (void *)Supervisor((void *)GetVBR) : NULL;

        GetRGB32(cm, 0, cm->Count, &colorMap[1]);
        GetRGB32(cm, 0, cm->Count, &origColorMap[1]);
        
        origColorMap[0] = cm->Count<<16;
        origColorMap[cm->Count*3+1] = 0;
        
        colorMap[0] = cm->Count<<16;
        colorMap[cm->Count*3+1] = 0;
        
        // Fade out wb
        for (i = 0; i < 85; i++) {
            WaitTOF();
            for (j = 1; j < cm->Count*3; j++) {
                WORD color;
                color = colorMap[j]>>24;
                if (color - 0x03 > 0)
                    colorMap[j] = colorMap[j] - 0x03000000;
                else
                    colorMap[j] = 0;

                LoadRGB32(oldViewPort, colorMap);
            }
        }
        
        LoadView(NULL);
        WaitTOF();
        WaitTOF();
        
        // Install input handler
        installInputHandler();
        
        Forbid();
        
        // Save interrupts and DMA
        oldInt = custom->intenar;
        oldDMA = custom->dmaconr;

        
        // disable all interrupts and DMA
        custom->intena = 0x7fff;
        custom->intreq = 0x7fff; 
        custom->intreq = 0x7fff;
        custom->dmacon = 0x7fff;

        custom->dmacon = DMAF_SETCLR | DMAF_MASTER | DMAF_RASTER | DMAF_BLITTER  | DMAF_COPPER;

        
        bplptr[0] = ptr20;
        bplptr[1] = ptr10;

        clist[0] = (ULONG)cop1;
        clist[1] = (ULONG)cop2;


        cop1[BPL0]     = (ULONG)ptr10 >> 16;
        cop1[BPL0 + 2] = (ULONG)ptr10 & 0xffff;
        cop1[BPL1]     = (ULONG)ptr11 >> 16;
        cop1[BPL1 + 2] = (ULONG)ptr11 & 0xffff;
        cop1[BPL2]     = (ULONG)ptr12 >> 16;
        cop1[BPL2 + 2] = (ULONG)ptr12 & 0xffff;
        cop1[BPL3]     = (ULONG)ptr13 >> 16;
        cop1[BPL3 + 2] = (ULONG)ptr13 & 0xffff;
        cop1[BPL4]     = (ULONG)ptr14 >> 16;
        cop1[BPL4 + 2] = (ULONG)ptr14 & 0xffff;
        cop1[BPL5]     = (ULONG)ptr15 >> 16;
        cop1[BPL5 + 2] = (ULONG)ptr15 & 0xffff;
        
        cop2[BPL0]     = (ULONG)ptr20 >> 16;
        cop2[BPL0 + 2] = (ULONG)ptr20 & 0xffff;
        cop2[BPL1]     = (ULONG)ptr21 >> 16;
        cop2[BPL1 + 2] = (ULONG)ptr21 & 0xffff;
        cop2[BPL2]     = (ULONG)ptr22 >> 16;
        cop2[BPL2 + 2] = (ULONG)ptr22 & 0xffff;
        cop2[BPL3]     = (ULONG)ptr23 >> 16;
        cop2[BPL3 + 2] = (ULONG)ptr23 & 0xffff;
        cop2[BPL4]     = (ULONG)ptr24 >> 16;
        cop2[BPL4 + 2] = (ULONG)ptr24 & 0xffff;
        cop2[BPL5]     = (ULONG)ptr25 >> 16;
        cop2[BPL5 + 2] = (ULONG)ptr25 & 0xffff;
        
        for (k = 0; k < car2d.palette.ncolors; k++) {
            frame = 1;
            setColor(k, car2d.palette.hicolor[k], car2d.palette.locolor[k]);
            frame = 0;
            setColor(k, car2d.palette.hicolor[k], car2d.palette.locolor[k]);
        }
    
        mt_install_cia(custom, vbr, palflag);
        mt_init(custom, mod, NULL, 0);
        mt_musicmask(custom, 0xf);
        
        c2p1x1_6_c5_030_init(320, 180, 0, 0, 40, 7200, 0);

        custom->cop1lc = (ULONG)cop1;
        ptr = ptr20;

        scroller = 0;
        clearColor = 0x01010101;
        
        installVBInterrupt();
   
        mt_Enable = 1;
        
        while ((custom->potinp&0x400) && (cia->ciapra & CIAF_GAMEPORT0)) {

            switch (iscene) {
                case 0:
                    if (scroller < 480) {
                        LONG sine = (fpsin[((scroller-127)<<9)/720]*180)>>10;
                        cl = (UWORD*) clist[frame^1];

                        for (k = 0, i = 0; k < 180; k++, i += 10) {
                            j = coplistSize+i+5+(k>=174?2:0);
                            if (scroller < 128) {
                                ULONG lcol  = skyrgb[k];
                                UWORD red   = (lcol>>16)&0xff;
                                UWORD green = (lcol>>8)&0xff;
                                UWORD blue  = lcol&0xff;
                                UWORD lrgb, hrgb;
                            
                                if (scroller == 0) {
                                    redfade[k]   = red;
                                    greenfade[k] = green;
                                    bluefade[k]  = blue;
                                } 
                                else {
                                    for (l = 0; l < cvbinc; l++) {
                                        redfade[k]   += red;
                                        greenfade[k] += green;
                                        bluefade[k]  += blue;
                                    }
                                }
                                red   = redfade[k]>>7;
                                green = greenfade[k]>>7;
                                blue  = bluefade[k]>>7;
                            
                                hrgb = ((red&0xf0)<<4) |  (green&0xf0)     | (blue>>4);
                                lrgb = ((red&0x0f)<<8) | ((green&0x0f)<<4) | (blue&0x0f);
                            
                                cl[j] = hrgb;
                                cl[j+4] = lrgb;
                    
                            }
                            else { 
                                cl[j] = skyhi[sine+k];
                                cl[j+4] = skylo[sine+k];
                            }
                        }
                    
                        drawCarScene(&road, &car2d, 123-(scroller>>2), 134-(scroller>>2), (scroller > 160+127?180-sine:-1), 0);
                        dbcount = 2;
                    }
                    else {
                        if (dbcount > 0) {
                            cl = (UWORD*) clist[frame^1];
                            j = coplistSize + 5;
                            for (k = 0, i = 0; k < 180; k++, i += 10) {
                                cl[j+i+(k>=174?2:0)] = skyhi[(359>>1)+k];
                                cl[j+i+4+(k>=174?2:0)] = skylo[(359>>1)+k];
                            }
                            dbcount--;
                        }
        
                        drawCarScene(&road, &car2d, 4, 15, 0, 1);
                    }
                     
                    if (cvb >= 575) 
                        iscene = 1;
                    break;
                    
                case 1:
                    if (dbcount > 0) {
                        cl = (UWORD*) clist[frame^1];
                        j = coplistSize + 5;
                        for (i = 0, k = 0; k < 180; k++, i += 10) {
                            cl[j+i+(k>=174?2:0)] = skyhi[(359>>1)+k];
                            cl[j+i+4+(k>=174?2:0)] = skylo[(359>>1)+k];
                        }
                        dbcount--;
                    }
                    
                    k = 1023-fpcos[((cvb-575)<<8)/383];
                    drawWireObjectS(bugas, 500, 475, 1200, k, k, 0, 2, 50, 0);
                    drawCarScene(&road, &car2d, 4, 15, 0, 1);
                
                    if (cvb >= 958)
                        iscene = 2;
                    break;
            
                case 2:
                    drawCarScene(&road, &car2d, 4, 15, 0, 1);
            
                    drawWireObject(bugas, 500, 475, 1200, 2, 50, 0);
                    
                    drawString(chunky,sfont,"THE MOST TRENDY", 10, 120, 0x16161616);
                    drawString(chunky,sfont,"INTRO, EVER!", 40, 150, 0x16161616);
                    
                    if (cvb >= 1118) {
                        iscene = 3;
                        clearColor = 0;
                        dbcount = 2;
                    }
                    break;
                    
                case 3:
                    if (dbcount > 0) {
                        cl = (UWORD*) clist[frame^1];
                        
                        cl[coplistSize] = 0xffff;
                        cl[coplistSize+1] = 0xfffe;
                        cl[coplistSize+2] = 0xffff;
                        cl[coplistSize+3] = 0xfffe;
                        
                        setColor(0, 0xffff, 0xffff);
                        setColor(1, 0, 0);
                        
                        dbcount --;
                    }
                    drawVectors(0, 4, dmc2d.scene[0].nvectors, dmc2d.scene[0].vectors, dmc2d.scene[0].keyframes, chunky, rowchunky);
          
                    if (cvb >= 1123) {
                        iscene = 4;
                        dbcount = 2;
                    }
                    break;
                    
                case 4:
                    if (dbcount > 0) {
                        
                        cl = (UWORD*) clist[frame^1];
                        
                        cl[coplistSize] = 0xffff;
                        cl[coplistSize+1] = 0xfffe;
                        cl[coplistSize+2] = 0xffff;
                        cl[coplistSize+3] = 0xfffe;
                        
                        
                        setColor(0, 0, 0);
                        setColor(1, 0xffff, 0xffff);
                        dbcount --;
                    }
                    drawVectors(0, 4, gmc2d.scene[0].nvectors, gmc2d.scene[0].vectors, gmc2d.scene[0].keyframes, chunky, rowchunky);
               
                    if (cvb >= 1128) {
                        iscene = 5;
                        dbcount = 2;
                    }
                    break;
                    
                case 5:
                    if (dbcount > 0) {
                        setColor(0, 0xffff, 0xffff);
                        setColor(1, 0, 0);
                        dbcount --;
                    }
                    drawVectors(0, 4, kitt2d.scene[0].nvectors, kitt2d.scene[0].vectors, kitt2d.scene[0].keyframes, chunky, rowchunky);
                 
                    if (cvb >= 1133) {
                        iscene = 6;
                        dbcount = 2;
                    }
                    break;
                    
                case 6:
                    if (dbcount > 0) {
                        setColor(0, 0, 0);
                        setColor(1, 0xffff, 0xffff);
                        dbcount --;
                    }
                    drawVectors(0, 4, dmc2d.scene[0].nvectors, dmc2d.scene[0].vectors, dmc2d.scene[0].keyframes, chunky, rowchunky);

                    if (cvb >= 1138) {
                        iscene = 7;
                        dbcount = 2;
                    }
                    break;
                
                case 7:
                    if (dbcount > 0) {
                        setColor(0, 0xffff, 0xffff);
                        setColor(1, 0, 0);
                        dbcount --;
                    }
                
                    drawVectors(0, 4, gmc2d.scene[0].nvectors, gmc2d.scene[0].vectors, gmc2d.scene[0].keyframes, chunky, rowchunky);

                    if (cvb >= 1143) {
                        iscene = 8;
                        dbcount = 2;
                    }
                    break;
                    
                case 8:
                    if (dbcount > 0) {
                        setColor(0, 0, 0);
                        setColor(1, 0xffff, 0xffff);
                        dbcount --;
                    }
                    drawVectors(0, 4, kitt2d.scene[0].nvectors, kitt2d.scene[0].vectors, kitt2d.scene[0].keyframes, chunky, rowchunky);
                    if (cvb >= 1148) {
                        iscene = 9;
                        scroller = 0;
                        clearColor = 0x03030303;
                        dbcount = 2;
                    }
                    break;
                    
                case 9:
                case 11:
                    if (dbcount > 0) {
                        if (dbcount == 2) {
                            drawVectors(0, 0, 14, delorean_bg_vectors, delorean_bg_points, (UBYTE*)carback, rowchunky);                        
                        }
                        cl = (UWORD*) clist[frame^1];
                        if (iscene == 9)
                            CopyMem(alphaCop, cl+78, 524);
                        
                        setColor(0, 0, 0);
                        for (i = 0; i < 6; i++) {
                            setColor(i+1, cars_palette_hi[i], cars_palette_lo[i]);
                        }
                        
                        cl[coplistSize] = 0xffff;
                        cl[coplistSize+1] = 0xfffe;
                        cl[coplistSize+2] = 0xffff;
                        cl[coplistSize+3] = 0xfffe;
                        dbcount--;
                    }

                    if (iscene == 9) {
                        i = 0;
                        j = (scroller<<2)&1023;
                        l = (1024+(fpcos[(scroller<<2)&1023]>>5))&1023;
                        drawWireObjectXYZ(dmc, 200, 50, 600, i, j, l, 2, 0x38, 2);
                        drawString(chunky,sfont,"DELOREAN DMC-12, 1981", 5, 8, 0x16161616);
                        drawString(chunky,sfont,"FLUX CAPACITOR INCLUDED, BUT", 5, 28, 0x16161616);
                        drawString(chunky,sfont,"PLZ DONT COME BACK TO 2020", 5, 48, 0x16161616);
                        j = 0;
                        i = 0;
                        if (scroller < 128) {
                            k = fpsin[scroller<<1];
                            j = 208 - ((k * 208) >> FP);
                            i = -72 + ((k *  72) >> FP);
                        }
                    }
                    else {
                        i = 960+(fpsin[(scroller<<4)&1023]>>5);
                        j = 512+(fpsin[(scroller<<4)&1023]>>4);
                        l = (fpsin[(scroller<<4)&1023]>>6);
                        drawWireObjectXYZ(dogcookie, 0, 125, 1000, i, j, 992, 2, 0x38, 2);
                        drawWireObject(dogbody, 525, -50, 650, 2, 0x38, 2);
                        drawWireObject(doghead, 450+l, 105+l, 650, 2, 0x38, 2);
                        drawString(chunky,sfont,"ACCESORIES: IT COMES",5, 8, 0x16161616);
                        drawString(chunky,sfont,"WITH A MOVING HEAD DOGGIE", 5, 28, 0x16161616);
                        drawString(chunky,sfont,"NO, HE IS NOT EINSTEIN.", 5, 48, 0x16161616);
                        j = 0;
                        i = 0;
                        if (scroller > 256) {
                            k = fpcos[((scroller - 256) << 1)];
                            j =  ((k * 340) >> FP) - 340;
                            i = -((k * 102) >> FP) +  102;
                        }
                    } 
                    
                    drawImage((ULONG*)delorean, delorean_mask, 114+j, 63+i, 208, 122);
                    
                    if (iscene == 9 && cvb >= 1533) {
                        scroller = 0;
                        iscene = 10;
                        clearColor = 0;
                        dbcount = 2;
                    }
                    else if (iscene == 11 && cvb >= 2108) {
                        scroller = 0;
                        iscene = 12;
                        clearColor = 0;
                        dbcount = 2;
                    }
                    break;
                    
                case 10:
                case 12:
                case 13:
                case 15:
                case 17:
                case 21:
                case 23:
                case 24:
                    if (dbcount > 0) {
                        cl = (UWORD*) clist[frame^1];
                        if (iscene == 24) {
                            cl[coplistSize] = 0xffff;
                            cl[coplistSize+1] = 0xfffe;
                            cl[coplistSize+2] = 0xffff;
                            cl[coplistSize+3] = 0xfffe;
                        }
                        else {
                            CopyMem(alphaCop, cl+78, 524);
                            CopyMem(textCop, cl+coplistSize, 908);
                        }
                        dbcount --;
                    }
                    
                    for (i = 0; i < 8; i++) {
                        LONG ywire = (105<<FP_2D) + (-409600/checkboard_z[i]); 
                        WORD ywire2d = ywire>>FP_2D;
                        if (checkboard_z[i] < 0 && ywire2d < 184) {
                            drawGlowLine(chunky, 0, ywire2d, 319, ywire2d, checkboard_z[i] > -224 ? 0x3f : 0x2f, checkboard_z[i] > -224 ? 1: 0, rowchunky);
                        }
                    }
                    
                    if (iscene == 13) {
                        drawWireObjectZ(player, 0, (scroller < 256 ? 50+((1024-fpsin[scroller])>>2) : 50), 1000-(scroller < 500 ? scroller : 500), 0, 2, (scroller < 96 ? (scroller>>4)<<3 : 0x38), 3);
                        if (scroller > 512)
                            scroller = 512;
                        i = (768+(scroller>>1))&1023;
                        if (cvb >= 2478) {
                            j = cvb - 2478;
                            j = (j<<2) + j + (j>>2);
                        }
                        else 
                            j = 0;
                        
                        j = 1000-(abs(fpcos[j&255])>>6);
                        
                        drawWireObjectY(speaker, 1600-(scroller<<1), 550, j, (-i)&1023, 3, 0x38, 3);
                        
                        drawWireObjectY(speaker, -1600+(scroller<<1), 550, j, i&1023, 3, 0x38, 3);
                        
                        for (i = 0; i < 4; i++) {
                            if (cvb >= cars_lyrics_cvb_1[i] && cvb < cars_lyrics_cvb_1[i+1]) {
                                drawString(chunky,sfont,cars_lyrics_1[i],cars_lyrics_pos_1[i],160, 0x30303030);
                                break;                        
                            }
                        }
                        
                    }
                    else if (iscene == 15) {
                        drawWireObject(player, 0, 50, 500, 2, 0x38, 3);
            
                        if (scroller > 1024)
                            scroller = 1024;
                        
                        if (cvb >= 2896) {
                            j = cvb - 2896;
                            j = (j<<2) + j + (j>>2);
                        }
                        else 
                            j = 0;
                        
                        j = 1000-(abs(fpcos[j&255])>>6);
                        
                        drawWireObjectY(speaker,  576, 550, j, 0, 3, 0x38, 3);
                        
                        drawWireObjectY(speaker, -576, 550, j, 0, 3, 0x38, 3);
                        
                        drawWireObjectXYZ(cassette, -1000 + scroller, 50, 500 + (scroller>>1), 1024 - (scroller>>2), 0, scroller>>2, 2, 0x38, 3);

                        for (i = 0; i < 4; i++) {
                            if (cvb >= cars_lyrics_cvb_2[i] && cvb < cars_lyrics_cvb_2[i+1]) {
                                drawString(chunky, sfont, cars_lyrics_2[i], cars_lyrics_pos_2[i], 160, 0x30303030);
                                break;                        
                            }
                        }
                    }
                    else if (iscene == 23) {
                        if ( ((cvb - 5175) >> 5) < 12) {
                            j = ((cvb - 5175) >> 5)%6;
                            i = ((cvb - 5175) - (j<<5))&31;
                            
                            if (j > 0)
                                drawString(chunky, bfont, greetings[j-1], credits_pos[j-1][0], credits_pos[j-1][1], 0x38383838-(0x08080808*((i*7)>>5)));
                            if (j < 5)
                                drawString(chunky, bfont, greetings[j], credits_pos[j][0], credits_pos[j][1], 0x08080808+(0x08080808*((i*7)>>5)));
                        }
                    }
                    else if (iscene == 24) {
                        WORD intensity = 0;
                        
                        if (cvb >= 5845 && cvb <= 5864)
                            intensity = random()&3;
                        
                        drawWireObject(bugas, 0, 110, 1000, 2, 0x38-(intensity<<4), 3-intensity);
                        
                        if (cvb >= 5945) {
                            cl = (UWORD*) clist[frame^1];
                            for (k = 0; k < 64; k++) {
                                ULONG lcol = alphaBlendPalette[k];
                                UWORD red = ((lcol>>16)&0xff)<<1;
                                UWORD green = ((lcol>>8)&0xff)<<1;
                                UWORD blue = (lcol&0xff)<<1;
                                WORD p1 = 79+(k<<1)+(k>31?2:0);
                                WORD p2 = 211+(k<<1)+(k>31?2:0);
                        
                                for (l = 0; l < cvbinc; l++) {
                                    redfade[k] -= red;
                                    greenfade[k] -= green;
                                    bluefade[k] -= blue;
                                }
                                
                                if (redfade[k] < 0)
                                    redfade[k] = 0;
                                
                                if (greenfade[k] < 0)
                                    greenfade[k] = 0;
                                
                                if (bluefade[k] < 0)
                                    bluefade[k] = 0;
                                
                                red = redfade[k]>>7;
                                green = greenfade[k]>>7;
                                blue = bluefade[k]>>7;
                            
                                cl[p1] = ((red&0xf0)<<4)|(green&0xf0)|(blue>>4);
                                cl[p2] = ((red&0x0f)<<8)|((green&0x0f)<<4)|(blue&0x0f);
                            }
                        }
                    }
                    else if (iscene == 17) {
                        drawString(chunky, bfont, "AND DONT FORGET YOUR SUN GLASSES!", 317-(scroller)-(fpsin[scroller>>2]>>3), 40+4, 0x02020202);
                        drawWireObjectXYZ(glasses, -400+scroller+((fpsin[scroller>>2]>>2)), 0, 400, (-scroller>>4)&1023, (scroller>>2)&1023, 0, 3, 0x38, 3);
                    }
                    else if (iscene < 23) {
                        drawString(chunky, bfont, "OH MAN, WE LOVE BUGAS!", 317-(scroller)-(fpsin[scroller>>2]>>3), 40+4, 0x02020202);
                        drawWireObjectXYZ(wheel, 650-(scroller)-(fpsin[scroller>>2]), 0, 400, 0, (scroller>>2)&1023,  (-scroller<<1)&1023, 2, 0x38, 3);
                       
                    }
                    switch (iscene) {
                        case 10:
                            if (cvb >= 1725) {
                                iscene = 11;
                                scroller = 0;
                                clearColor = 0x03030303;
                                dbcount = 2;
                            }
                            break;
                        case 13:
                            if (cvb >= 2683) {
                                iscene = 14;
                                scroller = 0;
                            }
                            break;
                        case 15:
                            if (cvb >= 3258) {
                                iscene = 16;
                                scroller = 0;
                            }
                            break;
                        case 12:
                            if (cvb >= 2300) {
                                iscene = 13;
                                scroller = 0;
                            }
                            break;
                        case  17:
                            if (cvb >= 3737) {
                                scroller = -80;
                                iscene = 18;
                                dbcount = 2;
                                
                            }
                            break;
                        case 21:
                            if (cvb >= 4791) {
                                iscene = 22;
                                clearColor = 0x03030303;
                                scroller = 0;
                                dbcount = 2;
                            }
                            break;
                        case 23:
                            if (cvb >= 5558+4) {
                                iscene = 24;
                                                    
                                for (i = 0; i < 64; i++) {
                                    ULONG lcol  = alphaBlendPalette[i];
                                    UWORD red   = (lcol>>16)&0xff;
                                    UWORD green = (lcol>>8)&0xff;
                                    UWORD blue  = lcol&0xff;
                                    
                                    redfade[i]   = red<<7;
                                    greenfade[i] = green<<7;
                                    bluefade[i]  = blue<<7;
                                }
                                dbcount = 2;
                            }
                            break;
                        case 24:
                            if (cvb >= 6014) {
                                iscene = 25;
                                scroller = 360+8;
                                
                                clearColor = 0;
                                
                                ptrl = ((ULONG*)chunky);
                                for (i = 0; i < 14400; i++) {
                                    *ptrl++ = clearColor;
                                }
                                dbcount = 2;
                                k = 0;
                            }
                            break;
                    }
                    break;
        
                case 14:
                case 16:
                    if ((iscene == 14 && cvb < 2710) || (iscene == 16 && cvb < 3285)) {
                        drawWireObjectZ(bugas, -900-(scroller>>2), 225+(scroller>>1), 1500, (64+(scroller>>2))&1023, 2, 0x3f, 2);
                        
                        drawWireObject(gary1, 0, 0, 200, 2, 0x38, 3);
                    }
                    else if ((iscene == 14 && cvb < 2730) || (iscene == 16 && cvb < 3305)) {
                        drawWireObjectZ(bugas, 1300-(scroller>>2), 550+(scroller>>1), 1900, (-120-(scroller>>2))&1023, 2, 0x3f, 2);
                    
                        drawWireObject(gary2, 0, 0, 200, 2, 0x38, 3);
                    }
                    else if ((iscene == 14 && cvb >= 2730) || (iscene == 16 && cvb >= 3305)) {

                        if ((iscene == 14 && cvb >= 2845) || (iscene == 16 && cvb >= 3420)) {
                            if (iscene == 14)  {
                                k = cvb-2845;
                                j = (k>>3);
                            }
                            else { 
                                k = cvb-3420;
                                j = (k>>3);
                            }
                            if (j > 4)
                                j = 4;
                            drawWireObjectZ(bugas, -900-(scroller>>2), 225+(scroller>>1), 1500, (128-(scroller>>2))&1023, 2, 0x38+(scroller&7), 2);

                            for (i = 0; i < j; i++) {
                                drawWireObjectZ(gary3, -i<<7, i<<6, 400+(i<<8), ((k>>2)+((i*i)<<4))&1023, 2, 0x38, 3);
                            }
                            
                            drawWireObjectZ(gary3, 0, 0, 200, (k>>2)&1023, 2, 0x38, 3);
                        }
                        else {
                            drawWireObjectZ(bugas, -900-(scroller>>2), 225+(scroller>>1), 1500, (128-(scroller>>2))&1023, 2, 0x3f, 2);
                        
                            drawWireObject(gary3, 0, 0, 200,2, 0x38, 3);
                        }
                    }
                    if (iscene == 14 && cvb >= 2875) {
                        iscene = 15;
                        clearColor = 0;
                        scroller = 0;
                    }
                    if (iscene == 16 && cvb >= 3450) {
                        iscene = 17;
                        scroller = 0;
                        clearColor = 0;
                        wireframe_inc = 2;
                        for (i = 0; i < 8; i++) {
                            checkboard_z[i] =  ((i+8)<<FP_2D);
                            checkboard_z[i] += wireframe_inc<<1;
                            if (checkboard_z[i]>0)
                                checkboard_z[i] -= 1024;
                        }
                        dbcount = 2;
                    }
                    break;
            
                    
                case 18:
                    if (dbcount > 0) {
                        for (k = 0; k < 8; k++) {
                            setColor(k, kraftwerk.palette.hicolor[k], kraftwerk.palette.locolor[k]);
                        }
                        
                        cl = (UWORD*) clist[frame^1];
                        cl[coplistSize] = 0xffff;
                        cl[coplistSize+1] = 0xfffe;
                        cl[coplistSize+2] = 0xffff;
                        cl[coplistSize+3] = 0xfffe;
                        dbcount --;
                    }
                    
                    drawWireObjectY(boing, 700-(scroller<<4), (abs(fpsin[(scroller<<2)&1023])<<9)>>FP, 3000, (-scroller<<2)&1023, 3, 0x3f, 2);

                    drawWireObjectY(boing, 700-(scroller<<4), (abs(fpsin[(scroller<<2)&1023])<<9)>>FP, 3000, (85-(scroller<<2))&1023, 3, 0x38, 2);

                    renderScene(50, 4, &kraftwerk, chunky, FALSE, cvb>3804?TRUE:FALSE);
                    
                    if (cvb >= 3833) {
                        iscene = 19;
                        wireframe_inc = 2;
                        for (i = 0; i < 5; i++) {
                            checkboard_z[i] =  ((i+8)<<FP_2D);

                            checkboard_z[i] += wireframe_inc<<1;
                            if (checkboard_z[i]>0)
                                checkboard_z[i] -= 1024;
                        }
                        dbcount = 2;
                    }
                    break;
                    
                case 19:
                    if (dbcount > 0) {
                        for (i = 0; i < 8; i++) {
                            setColor(i, 0, 0);
                        } 
                        dbcount --;
                    }
                    drawGlowLine(chunky, 0, 129+4, 319, 133-(city->rZ>>10), 0x1f, 0, rowchunky);
                    for (i = 0; i < 5; i++) {
                        LONG ywire = (104<<FP_2D) + (-1000000/checkboard_z[i]); 
                        if (checkboard_z[i] < 0 && (ywire>>FP_2D) < 186) {
                            drawGlowLine(chunky, 0, ywire>>FP_2D, 319, (ywire>>FP_2D)-(city->rZ>>10),
                                        checkboard_z[i] > -424 ? 0x3f : ( checkboard_z[i] > -500 ? 0x2f : 0x1f),
                                        checkboard_z[i] > -324 ? 2: 1,
                                        rowchunky); 
                        }
                    }  
                    
                    if ((ZCAM)<0) {
                        LONG ipath = -(ZCAM)&511;
                        cpath = -(ZCAM)>>9;
                        k = ((ipath*path[(cpath+1)&0x1f])+((511-ipath)*path[cpath&0x1f]));
    
                        city->rY += (k>>9)-city->rY;
                        city->rZ =  (k>>1)-city->rZ;
                        city->tX = (((ipath<<3)*-path[(cpath+1)&0x1f])+(((511-ipath)<<3)*-path[cpath&0x1f]))>>9;
                    }
                    city->tY = -300;
                    city->tZ = ZCAM;
                    matrixIdentity(mt);
                    matrixXYZRotation(mt, 1014, city->rY&1023, 0);
                    updateWireObject(city, 3);
                    renderWireObject(city, chunky, -1, -1);
                    ptrl = (ULONG*)chunky;
                    for (i = 4; i < 184; i++) {
                        j = rowchunky[i]>>2;
                        ptrl[j] = 0;
                        ptrl[79+j] = 0;
                    }
                    if (cvb >= 4192) {
                        j = (cvb - 4184)>>2;
                        if ((j&1) == 1)
                            setColor(0, 0xfff, 0xfff);
                        else
                            setColor(0, 0, 0);
                    }
                    if (cvb >= 4216) {
                        iscene = 20;
                        clearColor = 0x03030303;
                        scroller = 0;
                        for (i = 0; i < 16; i++) {
                            delorean_bg_points[i<<1] = (320<<FP_2D)-delorean_bg_points[i<<1];
                        }
                        dbcount = 2;
                    }
                    break;
                    
                case 20:
                case 22:
                    if (dbcount > 0) {
                        if (dbcount == 2) {
                            drawVectors(0,0,14,delorean_bg_vectors,delorean_bg_points,(UBYTE*)carback,rowchunky);                        
                        }
                        cl = (UWORD*) clist[frame^1];
                        setColor(0, 0, 0);
                        for (i = 0; i < 6; i++) {
                            setColor(i+1, cars_palette_hi[i], cars_palette_lo[i]);
                        }
                        cl[coplistSize] = 0xffff;
                        cl[coplistSize+1] = 0xfffe;
                        cl[coplistSize+2] = 0xffff;
                        cl[coplistSize+3] = 0xfffe;
                        
                        dbcount --;
                    }
                    if (iscene == 20)  {
                        i = 50;
                        l = (scroller<<2)&1023;
                        m = (1024-(fpcos[(scroller<<2)&1023]>>5))&1023;
                        drawWireObjectXYZ(gmc, -250, i, 500, 0, l, m, 2, 0x38, 2);
                        drawString(chunky,sfont,"GMC VANDURA G SERIES, 1983",5,4+4, 0x16161616);
                        drawString(chunky,sfont,"DAMN! THAT'S A VEHICLE, I BET" ,5,24+4, 0x16161616);
                        drawString(chunky,sfont,"IT CAN DESTROY A DELOREAN.",5,44+4, 0x16161616);
                        j = 0;
                        i = 0;
                        if (scroller < 128) {
                            k = fpsin[scroller<<1];
                            j = ((k * 176) >> FP)-176;
                            i = -56 + ((k *  56) >> FP);
                        }
                    }
                    else {
                        j = (fpsin[(scroller<<4)&1023]>>5)&1023;
                        l = 512 + (fpsin[(scroller<<4)&1023]>>5);
                        drawWireObjectXYZ(donpino, -400, 250, 500, j, l, 32, 2, 0x38, 2);
                        drawString(chunky,sfont,"ACCESORIES:",5,4+4, 0x16161616);
                        drawString(chunky,sfont,"IT COMES WITH A BUNCH OF" ,5,24+4, 0x16161616);
                        drawString(chunky,sfont,"PISSED-OFF MERCENARIES",5,44+4, 0x16161616);
                        drawString(chunky,sfont,"AND A CAR",200,140, 0x36363636);
                        drawString(chunky,sfont,"FRESHENER!",195,160, 0x36363636);
                        j = 0;
                        i = 0;
                        if (scroller > 256) { 
                            k = fpcos[((scroller - 256) << 1)];
                            j = -((k * 340) >> FP) + 340;
                            i = -((k *  96) >> FP) + 96;
                        }
                    }
                    
                    drawImage((ULONG*)ateam, ateam_mask, 4+j, 76+i, 176, 106);
                    
                    if (iscene == 20 && cvb >= 4600) {
                        scroller = 0;
                        iscene = 21;
                        clearColor = 0;
                        wireframe_inc = 2;
                        for (i = 0; i < 8; i++) {
                            checkboard_z[i] =  ((i+8)<<FP_2D);
                            checkboard_z[i] += wireframe_inc<<1;
                            if (checkboard_z[i]>0)
                                checkboard_z[i] -= 1024;
                        }
                        dbcount = 2;
                    }
                    else if (iscene == 22 && cvb >= 5175) {
                        scroller = 0;
                        iscene = 23;
                        clearColor = 0;
                        dbcount = 2;
                    }
                    break;
                    
                case 25:
                    if (dbcount > 0) {
                        cl = (UWORD*) clist[frame^1];
                        CopyMem(alphaCop, cl+78, 524);
                        setColor(2, 0xffff, 0xffff);
                        dbcount --;
                    }
                    j = k&511;
                    drawWireObjectZ(wheel, 550-(fpsin[j&1023]>>2), -400+(j<<1), 500, (j<<1)&1023, 2, 0x38, 3);
                    j = (k+180)&511;
                    drawWireObjectZ(cassette, 550+(fpsin[j&1023]>>2), -675+(j<<2), 1000, (-j)&1023, 2, 0x38, 3);
                    j = (k+270)&511;
                    drawWireObjectZ(glasses, 250+(fpcos[j&1023]>>2), -600+(j<<2), 500, (-j)&1023, 3, 0x38, 3);
                    
                    for (i = 0; i < 28; i++) {
                        WORD pos = (scroller>>1)+(i<<5);
                        if (pos > -36 && pos < 184)
                            drawString(chunky, sfont, credits[i], 120, pos, 0x02020202);
                    }
                    if (-(scroller>>1) > 896)
                        scroller = 368;
                    break;
            }
            
            c2p1x1_6_c5_030(&chunky[1280], ptr);
            
            waitVBlank();
            
            frame ^= 1;
            custom->cop1lc = clist[frame];
            ptr = bplptr[frame];
            
            fps++;
            
            cvb = vbcounter;
            cvbinc = vbinc;
            vbinc = 0;
            
            if (cvbinc>0) {
                switch (iscene) {
                    case 0:
                    case 9:
                    case 11:
                    case 14:
                    case 16:
                    case 18:
                    case 20:
                    case 22:
                        scroller += cvbinc;
                        break;
                    
                    case 19:
                        scroller += cvbinc;
                        ZCAM -= cvbinc<<4;
                    
                        for (i = 0; i < 5; i++) {
                            checkboard_z[i] += cvbinc;
                            if (checkboard_z[i]>-200)
                                checkboard_z[i] -= 328;
                        }
                        break;
                        
                    case 10:
                    case 12:
                    case 13:
                    case 15:
                    case 17:
                    case 21:
                        scroller += cvbinc*3;
                        wireframe_inc = cvbinc << 1;
                        for (i = 0; i < 8; i++) {
                            checkboard_z[i] += cvbinc;
                            if (checkboard_z[i] >= 0)
                                checkboard_z[i] -= 512;
                        }
                        break;
                        
                    case 23:
                        scroller += cvbinc<<2;
                        j = cvb-5434;
                        if (j > 0) {
                            if ((fps&(j>>4)) == 0)
                                wireframe_inc = cvbinc;
                        
                            else
                                wireframe_inc = 0;
                        }
                        else 
                            wireframe_inc = cvbinc;
                        
                        for (i = 0; i < 8; i++) {
                            checkboard_z[i] += wireframe_inc;
                            if (checkboard_z[i] >= 0)
                                checkboard_z[i] -= 512;
                        }
                        break;
                        
                    case 25:
                        scroller -= cvbinc;
                        k += cvbinc;
                        break;
                }
            }
            switch (iscene) {
                case 9:
                case 11:
                case 20:
                case 22:
                    ptrl = ((ULONG*)chunky)+320;
                    ptrlwire = carback;
                    for (i = 0; i < 14400; i++) {
                        *ptrl++ = *ptrlwire++;
                    }
                    break;
                    
                case 18:
                case 25:
                    ptrl = ((ULONG*)chunky)+320;
                    ptrlwire = boingback;
                    for (i = 0; i < 14400; i++) {
                        *ptrl++ = *ptrlwire++;
                    }
                    break;

                case 10:
                case 12:
                case 13:
                case 15:
                case 17:
                case 21:
                case 23:
                case 24:
                    ptrl = ((ULONG*)chunky)+320;
                    for (i = 0; i < 6720; i++) {
                        *ptrl++ = clearColor;
                    }
                    
                    ptrl = ((ULONG*)chunky)+6720;
                    ptrlwire = wireback;
                    for (k = 0; k < 8000; k++) {
                        *ptrl++ = *ptrlwire++;
                    }
                    break;
                    
                default:
                    ptrl = ((ULONG*)chunky)+320;
                    for (i = 0; i < 14400; i++) {
                        *ptrl++ = clearColor;
                    }
                    break;
            }
        }

    
        removeVBInterrupt();

        mt_Enable = 0;
        mt_end(custom);
        mt_remove_cia(custom);

    
        // restore DMA
        custom->dmacon = 0x7fff;
        custom->dmacon = oldDMA | DMAF_SETCLR | DMAF_MASTER;


        // restore original copper
        custom->cop1lc = (ULONG) oldCopinit;

        // restore interrupts
        custom->intena = oldInt | 0xc000;
        
        Permit();

        LoadRGB32(oldViewPort, origColorMap);
        
        // Remove input handler
        removeInputHandler();
        
        LoadView(oldview);
        WaitTOF();
        WaitTOF();
       
    
        freeRotoScene(&road);
        freeRotoScene(&kraftwerk);
        freeWireObject(dmc);
        freeWireObject(dogcookie);
        freeWireObject(doghead);
        freeWireObject(dogbody);
        freeWireObject(gmc);
        freeWireObject(donpino);
        freeWireObject(glasses);
        freeWireObject(gary1);
        freeWireObject(gary2);
        freeWireObject(gary3);
        freeWireObject(wheel);
        freeWireObject(cassette);
        freeWireObject(player);
        freeWireObject(speaker);
        freeWireObject(boing);
        freeWireObject(bugas);
        freeWireObject(city);
        
        
        freeFont(sfont);
        freeFont(bfont);
        
    }
    else
        printf("Not enough memory\n");
        
    
    FreeMem(colorMap, (cm->Count*3*4)+4);
    FreeMem(origColorMap, (cm->Count*3*4)+4);
    FreeMem(ptr10, ((WIDTH * HEIGHT) >> 3) * 6);
    FreeMem(ptr20, ((WIDTH * HEIGHT) >> 3) * 6);
    FreeMem(chunky, 62400);
    FreeMem(wireback, 320*110);
    FreeMem(boingback, 320*180);
    FreeMem(carback, 320*180);
    FreeMem(cop1, sizeof(coplist)+7200);
    FreeMem(cop2, sizeof(coplist)+7200);
    FreeMem(alphaCop, 524);
    FreeMem(textCop, 908);
    FreeMem(skyrgb, 180*4);
    FreeMem(skyhi, 360*2);
    FreeMem(skylo, 360*2);
    FreeMem(delorean, 208*122);
    FreeMem(ateam, 176*106);
    FreeMem(delorean_mask, (208>>2)*122);
    FreeMem(ateam_mask, (176>>2)*106);

    closeMathLibs();
    CloseLibrary((struct Library *)GfxBase);
    CloseLibrary((struct Library *)IntuitionBase);


   return 0;
}

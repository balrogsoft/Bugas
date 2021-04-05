
#define ZBUFFER_LEN             1100
#define BUFFER2D_LEN            1600

#define WIDTH_FP                20480
#define HEIGHT_FP               11520


typedef struct {
    UBYTE height;
    UBYTE nchars;
    UBYTE *cfont;
    UBYTE *dfont;
    UBYTE *bfont;
    UBYTE *tfont;
} BitmapFont;

typedef struct {
    LONG points[1][BUFFER2D_LEN];
} Object2D;

typedef struct {
    UWORD npoints;
    UWORD nedges;
    WORD *points;
    WORD *edges;
    UBYTE *material;
    
    UWORD *rowoffset;
    
    WORD rX;
    WORD rY;
    WORD rZ;
    WORD tX;
    WORD tY;
    WORD tZ;
    
    WORD dirty[4];
    
    WORD npoints_size;
    WORD nedges_size;
} WireObject;

ULONG fontMask[] = {0x00000000,
                    0x000000FF,
                    0x0000FF00,
                    0x0000FFFF,
                    0x00FF0000,
                    0x00FF00FF,
                    0x00FFFF00,
                    0x00FFFFFF,
                    0xFF000000,
                    0xFF0000FF,
                    0xFF00FF00,
                    0xFF00FFFF,
                    0xFFFF0000,
                    0xFFFF00FF,
                    0xFFFFFF00,
                    0xFFFFFFFF};

UBYTE imask[] =     {0,
                    1,
                    2,
                    4,
                    8,
                    3,
                    6,
                    12,
                    5,
                    10,
                    9,
                    14,
                    13,
                    11,
                    7,
                    15};
                    
                    
UBYTE frame = 0;
Object2D object2d;
    
UBYTE *chunky;
ULONG rowchunky[HEIGHT+10];
UWORD medlight[800];
UBYTE cpoint[800];

FP_TYPE wpoints[1600];

FP_TYPE mt[4][4];

LONG ZCAM = 0;

void __asm drawGlowLine(register __a0 UBYTE *chunky_buffer, 
                    register __d0 UWORD x1,
                    register __d1 UWORD y1,
                    register __d2 UWORD x2,
                    register __d3 UWORD y2,
                    register __d4 UWORD color,
                    register __d5 UBYTE intensity,
                    register __a1 ULONG *rowlen
                   );

void __asm drawVectors(register __d1 WORD x,
                       register __d2 WORD y,
                       register __d0 UWORD npoly,
                       register __a1 WORD *vectors,
                       register __a2 WORD *points,
                       register __a0 UBYTE *chunky,
                       register __a3 ULONG *rowlen);

WireObject* createWireObject(UWORD npoints, UWORD nedges, WORD *points, WORD *edges, UBYTE *mat) {
    WireObject *obj = AllocMem(sizeof(WireObject), MEMF_ANY|MEMF_CLEAR);
    if (obj) {
        obj->npoints = npoints;
        obj->nedges = nedges;
        obj->points = points;
        obj->edges = edges;
        
        if (mat != NULL)
            obj->material = mat;
        
        obj->rowoffset = (UWORD*)AllocMem(obj->npoints*2, MEMF_ANY);
                    
        if (obj->rowoffset) {
            WORD i, j;
            j = 0;
            for (i = 0; i < obj->npoints; i++) {
                obj->rowoffset[i] = j;
                j += obj->npoints;
            }
        }
    }
    return obj;
}

void freeWireObject(WireObject *obj) {
    if (obj) {
        if (obj->rowoffset)
            FreeMem(obj->rowoffset, obj->npoints*2);
   
        FreeMem(obj, sizeof(WireObject));
    }
}

void updateWireObject(WireObject *wireobj, UBYTE ndimensions) {
    WORD npoints = wireobj->npoints;
    WORD *points = wireobj->points;
    LONG *points2d = object2d.points[0];
    WORD npoints_size = npoints<<1;
    WORD i, j;
    LONG tpoints[4];
    if (ndimensions == 3)
        npoints_size += npoints;
   
    for (i = 0, j = 0; i < npoints_size; i += ndimensions, j+=2) {
        tpoints[0] = tpoints[1] = tpoints[2] = tpoints[3] = 0;
        if (points[i] != 32767 && points[i+1] != -32768) {
            tpoints[0] = points[i]+wireobj->tX;
            tpoints[1] = points[i+1]+wireobj->tY;
            tpoints[2] = ndimensions==3?points[i+2]+wireobj->tZ:wireobj->tZ;
            tpoints[3] = FP_UNIT;
            
            matrixMultiply1(tpoints, mt);
            
            wpoints[i]   = tpoints[0];
            wpoints[i+1] = tpoints[1];
            wpoints[i+2] = tpoints[2];
                    
            if ((tpoints[2]) > 50) {
                points2d[j]   = (CWIDTH2<<FP_2D)  + ((-tpoints[0]<<13) / (tpoints[2]));
                points2d[j+1] = (122<<FP_2D) + ((-tpoints[1]<<13) / (tpoints[2]));
            }
            else {
                points[i] = 32767;
                points[i+1] = -32768;
                points2d[j] = 32767;
            }
        }
        else {
            points2d[j] = points[i];
            points2d[j+1] = points[i+1];
        }
    }
}

void renderWireObject(WireObject *wireobj, UBYTE *chunky, BYTE color, BYTE intensity) {
    WORD  i, j;
    LONG *points = object2d.points[0];
    WORD col = color; 
    WORD glow = intensity;
    for (i = 0, j = 0; i < wireobj->nedges; i++, j+= 2) {
        WORD a = wireobj->edges[j]<<1,
             b = wireobj->edges[j+1]<<1,
             c = a + wireobj->edges[j];
        UBYTE mat = wireobj->material[i];

        if (points[a] != 32767 && points[b] !=32767) {
            if (wpoints[c+2] < 14000) {
                LONG x0 = points[a]>>FP_2D,
                     y0 = points[a+1]>>FP_2D,
                     x1 = points[b]>>FP_2D,
                     y1 = points[b+1]>>FP_2D;
                if (intensity == -1) {
                    if (wpoints[c+2] < 6000)
                        glow = 3;
                    else if (wpoints[c+2] < 8000)
                        glow = 2;
                    else if (wpoints[c+2] < 10000)
                        glow = 1;
                }
                if (color == -1) {
                    col = (mat==0?0x38:0x3f) - ((((-1500+wpoints[c+2]))>>11)<<3);
                    if (col > 0x3f)
                        col -= 8;
                }
                if (((x0 > 0 && x0 < 319) || (x1 > 0 && x1 < 319)) || ((y0 > 0 && y0 < 179) || (y1 > 0 && y1 < 179))) {
                    drawGlowLine(chunky, x0, y0, x1, y1, col, glow, rowchunky);
                }
            }
        }
    }
}

void generateAlphaBlendingPalette(LONG *colors, UWORD *coplist) {
    UBYTE i, j, k;
    ULONG rgb1[8], rgb2[8];
    
    for (i = 0; i < 4; i++) { 
        ULONG lev = (i) * 255 / 4;
        ULONG rgb = (lev<<16)|lev;
        rgb1[i] = rgb;
    }
    for (i = 0; i < 4; i++) { 
        ULONG lev = (i) * 255 / 4;
        ULONG rgb = 0xff00ff|(lev<<8);
        rgb1[i+4] = rgb;
    }

    for (i = 0; i < 4; i++) { 
        ULONG lev = (i) * 255 / 5;
        ULONG rgb = (lev<<8)|lev;
        rgb2[i] = rgb;
    }
    
    for (i = 0; i < 4; i++) { 
        ULONG lev = (i) * 255 / 4;
        ULONG rgb = 0xffff|lev<<16;
        rgb2[i+4] = rgb;
    }
    CopyMem(cop1+78, coplist, 524);
    
    k = 0;
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            UBYTE lred = (((i+1)*(rgb1[j]>>16))+((7-i)*(rgb2[j]>>16)))/9;
            UBYTE lgreen = (((i+1)*((rgb1[j]>>8)&0xff))+((7-i)*((rgb2[j]>>8)&0xff)))/9;
            UBYTE lblue = (((i+1)*(rgb1[j]&0xff))+((7-i)*(rgb2[j]&0xff)))/9;
            colors[k] = (lred << 16) | (lgreen << 8) | lblue;
            coplist[(k<<1)+(k>31?2:0)+1] = ((lred&0xf0)<<4) | (lgreen&0xf0)      | ((lblue&0xf0)>>4);
            coplist[(k<<1)+(k>31?2:0)+133] = ((lred&0x0f)<<8) | ((lgreen&0x0f)<<4) | (lblue&0x0f);
            k++;
        }
    }
    
}
    
BitmapFont* createFont(UBYTE *font, UWORD size, UBYTE *fontw, UBYTE *fontt, UBYTE *fontb, UBYTE fonth, UBYTE nchars) {
    BitmapFont *bf = AllocMem(sizeof(BitmapFont), MEMF_ANY|MEMF_CLEAR);
    UBYTE *dfont = AllocMem(nchars*4, MEMF_ANY|MEMF_CLEAR);
    if (bf != NULL && dfont != NULL) {
        UWORD i, tw, th;
        bf->height = fonth;
        bf->nchars = nchars;
        bf->dfont = dfont;
        bf->bfont = font;
        bf->tfont = fontt;

        tw = 0;
        th = 0;
        for (i = 0; i < nchars; i++) {
            UBYTE w = fontw[i];
            UBYTE t = fontt[i];
            UBYTE h = fontb[i];
            UBYTE nw = ((w + 7) & (-8));
            WORD j = i<<2;
            dfont[j] = tw;
            dfont[j+1] = th+t;
            dfont[j+2] = tw+w;
            dfont[j+3] = th+h;
            if (tw + nw > 255) {
                tw = 0;
                th += fonth;
                dfont[j] = tw;
                dfont[j+1] = th+t;
                dfont[j+2] = tw+w;
                dfont[j+3] = th+h;
            }
            w = nw;
            tw += w;
        }
    }
    return bf;
}

void freeFont(BitmapFont* font) {
    if (font != NULL) {
        if (font->dfont)
            FreeMem(font->dfont, font->nchars*4);
        FreeMem(font, sizeof(BitmapFont));
    }
}

void drawChar(UBYTE *ptr, BitmapFont *bfont, WORD x, WORD y, UBYTE ch, ULONG color) {
    UBYTE i,j,l;
    UBYTE tf, xf1,yf1,xf2,yf2,lenx,leny;
    ULONG *sptr = NULL; 
    UBYTE *bptrl = NULL;
    UBYTE *dptr;
    WORD k, xk;
    if (ch < 60)
        ch = ch - 33;
    else 
        ch = ch - 65 + 29;
    tf = bfont->tfont[ch];
    ch <<= 2;
    dptr = bfont->dfont+ch;
    xf1 = *dptr++;
    yf1 = *dptr++;
    xf2 = *dptr++;
    yf2 = *dptr++;
    
    lenx = (xf2-xf1);
    leny = (yf2-yf1);
    
    if (((xf2-xf1) & 0xf) != 0)
        lenx = (lenx>>2) + 1;
    else
        lenx = lenx>>2;
    
    for (i = 0; i <= leny; i++) {
        if (y+tf+i >= 4 && y+tf+i < 184) {
            if (sptr == NULL) {
                sptr = (ULONG*)&ptr[x+rowchunky[y+tf+i]];
                bptrl = &bfont->bfont[(xf1>>3)+((i+yf1)<<5)];
            }
            k = 0;
            for (j = 0; j < lenx; j++, k+=4) {
                UBYTE nmask = bptrl[k>>3];
                if (nmask != 0) {
                    if ((j&1)==0)
                        nmask >>= 4;
                    else 
                        nmask &= 0x0f;  
                    xk = x+k;
                    if (xk >= 4 && xk < 316) {
                        *sptr = (*sptr&fontMask[15-nmask]) | (color&fontMask[nmask]);
                    }
                    else {
                        UBYTE *sptrb = (UBYTE*)sptr;
                        for (l = 0; l < 4; l++) {
                            WORD xp = xk+l;
                            if (xp >= 4 && xp < 316 && ((nmask>>(3-l))&1) != 0) {
                                *sptrb++ = color;
                            }
                            else {
                                sptrb++;
                            }
                        }
                    }
                }
                sptr++;
           }
        }
        if (sptr != NULL) {
            sptr += 80-lenx;
            bptrl += 32;
        }
    }
}

void drawString(UBYTE *ptr, BitmapFont *bfont, char* str, WORD x, WORD y, ULONG color) { 
    WORD w = 0;
    UBYTE ch,ch_w;
    while (*str != 0) {
        if (*str > 32) {
            if (*str < 60)
                ch = *str - 33;
            else 
                ch = *str - 65 + 29;
            ch = ch << 2;
            ch_w = bfont->dfont[ch+2]-bfont->dfont[ch];
            if (x+w+ch_w >= 0 && x < 320 ) { 
                drawChar(ptr, bfont, x+w, y, *str, color);
            }
            w+=ch_w;
        }
        else
            w+=bfont->height>>1;
        
        str++;
    };
}

void drawWireObject(WireObject *wireObj, WORD x, WORD y, WORD z, UBYTE ndimensions, WORD col, WORD intensity) {
    matrixIdentity(mt);
    matrixTranslation(mt, x, y, z);
    updateWireObject(wireObj, ndimensions);
    renderWireObject(wireObj, chunky, col, intensity);
}

void drawWireObjectS(WireObject *wireObj, WORD x, WORD y, WORD z, WORD sx, WORD sy, WORD sz, UBYTE ndimensions, WORD col, WORD intensity) {
    matrixScale(mt, sx, sy, sz);
    matrixTranslation(mt, x, y, z);
    updateWireObject(wireObj, ndimensions);
    renderWireObject(wireObj, chunky, col, intensity);
}


void drawWireObjectZ(WireObject *wireObj, WORD x, WORD y, WORD z, WORD rz, UBYTE ndimensions, WORD col, WORD intensity) {
    matrixIdentity(mt);
    matrixTranslation(mt, x, y, z);
    matrixZRotation(mt, rz);
    updateWireObject(wireObj, ndimensions);
    renderWireObject(wireObj, chunky, col, intensity);
}


void drawWireObjectY(WireObject *wireObj, WORD x, WORD y, WORD z, WORD ry, UBYTE ndimensions, WORD col, WORD intensity) {
    matrixIdentity(mt);
    matrixTranslation(mt, x, y, z);
    matrixYRotation(mt, ry);
    updateWireObject(wireObj, ndimensions);
    renderWireObject(wireObj, chunky, col, intensity);
}


void drawWireObjectXYZ(WireObject *wireObj, WORD x, WORD y, WORD z, WORD rx, WORD ry, WORD rz, UBYTE ndimensions, WORD col, WORD intensity) {
    matrixIdentity(mt);
    matrixXYZRotation(mt, rx, ry, rz);
    matrixTranslation(mt, x, y, z);
    updateWireObject(wireObj, ndimensions);
    renderWireObject(wireObj, chunky, col, intensity);
}

void unpackImage(UBYTE *orig, UBYTE *dest, UBYTE *mask, UWORD size, UWORD width, UWORD height) {
    UWORD  j, y = 0;
    UBYTE *to = orig+size;
    UBYTE *destPtr = dest;
    ULONG *ptrl = (ULONG*)dest;
    while (orig < to) {
        UBYTE col = *orig++;
        if (col == 0xff) {
            UBYTE rep = *orig++;
            dest = destPtr + (y*width) + rep;
            y++;
        }
        else if (col >= 0x80) {
            col &= 0x7f;
            *dest++ = col;
        }
        else {
            UBYTE rep = *orig++;
            for (j = 0; j < rep; j++) { 
                *dest++ = col;
            }
        }
    }
    
     size = (width>>2)*height;
     for (j = 0; j < size; j++) {
        ULONG pixels = *ptrl++;
        if (pixels == 0) {
            if ((j&1) == 1)
                mask++;
        }
        else {
            for (y = 0; y < 16; y++) {
                if ((pixels&fontMask[imask[y]]) == pixels) {
                    if ((j&1) == 0) {
                        *mask = imask[y]<<4;
                        break;
                    }
                    else {
                        *mask |= imask[y];
                        mask++;
                        break;
                    }       
                }
            }
        }
    }
}

void drawImage(ULONG *data, UBYTE *mask, WORD x, WORD y, WORD width, WORD height) {
    WORD i, j, l;
    ULONG *ptrl = (ULONG*)&chunky[rowchunky[y<0?0:y]+x];
    UBYTE *ptrm = mask;
    UBYTE bwidth = width >> 3;
    WORD startx = 0, starty = 0, lenx = width>>2, leny = height;
    width = lenx;
    if (x < 0) 
        startx = -x>>2;
    if (x + width > 319)
        lenx = ((320 - x)>>2)+(((320 - x)&2)>0?1:0);
    if (y < 0)
        starty = -y;
    if (y + height > 183)
        leny = 184 - y;
    
    data += starty*width;
    ptrm += starty*bwidth;
    for (i = starty; i < leny; i++) {
        for (j = startx; j < lenx; j++) {
            WORD xj = x+(j<<2);
            if (xj >= 0 && xj < 317) {
                UBYTE nmask = ptrm[j>>1];
                if ((j&1)==0)
                    nmask >>= 4;
                else 
                    nmask &= 0x0f;  
                ptrl[j] = (ptrl[j]&fontMask[15-nmask]) | (data[j]&fontMask[nmask]);
            }
            
            else if (xj >= -3 && xj < 320) {
                UBYTE *ptrb = (UBYTE*)&ptrl[j];
                UBYTE *datab = (UBYTE*)&data[j];
                for (l = 0; l < 4; l++) {
                    if (xj+l >= 0 && xj+l < 320) {
                        if (*datab != 0)
                            *ptrb++ = *datab++;
                    }
                    else {
                        ptrb++;
                        datab++;
                    }
                }
            }
        }
        ptrl += 80;
        ptrm += bwidth;
        data += width;
    }
}

void setColor(WORD index, UWORD hicolor, UWORD locolor) {
    UWORD *cl = (UWORD*)clist[frame^1];
    UWORD offset = (index<<1)+(index>31?2:0);
    cl[79+offset] = hicolor;
    cl[211+offset] = locolor;
}

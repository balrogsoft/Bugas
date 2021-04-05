
typedef struct {
    WORD nvectors;
    WORD *vectors;
    WORD nkeyframes;
    WORD nkeyframes_points;
    WORD keyframe_pointer;
    WORD *keyframes;
    WORD *keyframes_int;
    WORD *ikeyframes;
    LONG *color;
    WORD color_pointer;
} Rotoscoping;

typedef struct {
    WORD ncolors;
    UWORD *hicolor;
    UWORD *locolor;
} RotoscopingPalette;

typedef struct {
    Rotoscoping scene[4];
    WORD *data;
    UWORD data_size;
    RotoscopingPalette palette;
} RotoscopingScene;

Rotoscoping *cscene;
UWORD rotoFrame;
WORD l[30];


void createRotoFrame(RotoscopingScene *rotoScene, WORD *data) { 
    UWORD i, j;
    ULONG k = 0;
    rotoScene->data = data;
    
	for (i = 0; i < 4; i++) {
        rotoScene->scene[i].nkeyframes_points = data[k];
        rotoScene->scene[i].keyframes         = &data[k + 1];
        j = rotoScene->scene[i].nkeyframes_points*2;
        rotoScene->scene[i].nvectors          = data[k + j + 1];
        rotoScene->scene[i].vectors           = &data[k + j + 2];
        j += rotoScene->scene[i].nvectors*5;
        k += j + 2;
    }

    
    rotoScene->palette.ncolors = data[k++];
    rotoScene->palette.hicolor = &data[k];
    rotoScene->palette.locolor = &data[k+rotoScene->palette.ncolors];
}

void createRotoScene(RotoscopingScene *rotoScene, WORD *data) {
    UWORD i, j;
    ULONG k = 0;
    WORD m;
    rotoScene->data = data;
	for (i = 0; i < 4; i++) {
        rotoScene->scene[i].nvectors          = data[k];
        rotoScene->scene[i].vectors           = &data[k + 1];
        
        j = rotoScene->scene[i].nvectors*5;
        
        rotoScene->scene[i].nkeyframes        = data[k + j + 1];
        rotoScene->scene[i].nkeyframes_points = data[k + j + 2];
        rotoScene->scene[i].keyframe_pointer  = 0;
        
        rotoScene->scene[i].keyframes         = &data[k + j + 3];
        rotoScene->scene[i].keyframes_int     = &data[k + j + ((rotoScene->scene[i].nkeyframes * rotoScene->scene[i].nkeyframes_points) << 1) + 3];
        if (rotoScene->scene[i].nkeyframes > 0) {
            rotoScene->scene[i].ikeyframes = (WORD*) AllocMem(rotoScene->scene[i].nkeyframes_points << 2, MEMF_ANY);
            CopyMem(rotoScene->scene[i].keyframes, rotoScene->scene[i].ikeyframes, rotoScene->scene[i].nkeyframes_points << 2);
        }
        rotoScene->scene[i].color             = (LONG*) &data[k + j + ((rotoScene->scene[i].nkeyframes * rotoScene->scene[i].nkeyframes_points) << 2) + 3];
        rotoScene->scene[i].color_pointer     = 0;

        k += j + ((rotoScene->scene[i].nkeyframes*rotoScene->scene[i].nkeyframes_points) << 2) + (rotoScene->scene[i].nvectors*rotoScene->scene[i].nkeyframes<<1) + 3;
    }

    
    rotoScene->palette.ncolors = data[k++];
    rotoScene->palette.hicolor = &data[k];
    rotoScene->palette.locolor = &data[k+rotoScene->palette.ncolors];
    m = -6;
    for (i = 0; i < 30; i++) {
        if ((i&1) == 0) {
            m+=6;
        }
        l[i] = m;
    }
   
    rotoFrame = 0;
}

void renderScene(WORD x, WORD y, RotoscopingScene *rotoScene, UBYTE *chunky, BOOL loop, BOOL play) {
    UBYTE j = 0;
    UWORD i, n;
    WORD inc = cvbinc;
    if (loop == FALSE && rotoFrame + inc > ((rotoFrame/10)*10) + 10) 
        inc = -(inc - 10); 
    for (n = 0; n < inc; n++) {
        if (play)
            rotoFrame++;
        cscene = &rotoScene->scene[j];
        if (cscene->nvectors == 0)
            continue;
        
        if (n == 0) {
            drawVectors(x, y, cscene->nvectors, cscene->vectors, &cscene->keyframes[cscene->keyframe_pointer], chunky, rowchunky);

            if (play == TRUE && rotoFrame < 30) {
                WORD *keyptr = cscene->keyframes + cscene->keyframe_pointer;
                WORD *intptr = cscene->keyframes_int + cscene->keyframe_pointer;
               
                if (rotoFrame + inc > 30)
                    inc = 30 - rotoFrame;
                switch (inc) {
                    case 1:
                        for (i = 0; i < (cscene->nkeyframes_points<<1); i++) {
                            *keyptr++ += *intptr++;
                        }
                        break;
                    case 2:
                        for (i = 0; i < (cscene->nkeyframes_points<<1); i++) {
                            *keyptr++ += (*intptr++)<<1;
                        }
                        break;
                    case 3:
                        for (i = 0; i < (cscene->nkeyframes_points<<1); i++) {
                            *keyptr++ += (*intptr<<1)+*intptr;
                            intptr ++;
                        }
                    case 4:
                        for (i = 0; i < (cscene->nkeyframes_points<<1); i++) {
                            *keyptr++ += (*intptr++)<<2;
                        }
                        break;
                    default:
                        for (i = 0; i < (cscene->nkeyframes_points<<1); i++) {
                            *keyptr++ += (*intptr++)*inc;
                        }
                        break;
                }
            }
        }
        
        if (loop == TRUE && play == TRUE && rotoFrame >= 10 && cscene->nkeyframes > 0) {
            CopyMem(cscene->ikeyframes, cscene->keyframes, cscene->nkeyframes_points << 2);
            rotoFrame %= 10;
        }
        if (loop == FALSE && play == TRUE) {
            if (rotoFrame == 10) 
                cscene->keyframe_pointer += cscene->nkeyframes_points << 1;
            else if (rotoFrame == 20)
                cscene->keyframe_pointer += cscene->nkeyframes_points << 1;
            else if (rotoFrame == 30)
               cscene->keyframe_pointer += cscene->nkeyframes_points << 1;
        }
    }
}


void freeRotoScene(RotoscopingScene *rotoScene) {
    WORD i;

    for (i = 0; i < 4; i++) {
        if (rotoScene->scene[i].nkeyframes > 0) {
            FreeMem(rotoScene->scene[i].ikeyframes, rotoScene->scene[i].nkeyframes_points << 2);
        }
    }
}

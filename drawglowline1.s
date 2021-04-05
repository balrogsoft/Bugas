
    xdef _drawGlowLine
    xdef drawGlowLine

	section	code,code
	
BPLX1	      EQU 320
BPLX1C	      EQU 319
BPLX1SH	      EQU 20480
FP	          EQU 6

CLIP_RIGHT    EQU 20224
; 20416
CLIP_BOTTOM   EQU 12544
;12736

_drawGlowLine
drawGlowLine
    movem.l d6-d7/a4,-(sp)
    move.b  d5,glow
; sx = x0<x1 ? 1 : -1

    cmp.w   d0,d2
    bgt.b   .plus1
    move.w  #-1,sx
    bra.b   .plus1_exit
.plus1
    move.w  #1,sx
.plus1_exit

; sy = y0<y1 ? 1 : -1

    cmp.w   d1,d3
    bgt.b   .plus2
    move.w  #-1,sy
    bra.b   .plus2_exit
.plus2
    move.w  #1,sy
.plus2_exit


; dx = d5
; dy = d6

; dx = abs(x1-x0)

    move.w  d2,d5
    sub.w  d0,d5
    bpl.b   .plus3
    neg.w   d5
.plus3

; dy = abs(y1-y0)

    move.w  d3,d6
    sub.w  d1,d6
    bpl.b   .plus4
    neg.w  d6
.plus4

; err = (dx>dy ? (dx>>1) : -(dy>>1))
    
    cmp.w   d5,d6
    bpl.b   .plus5
    move.w  d5,d7
    asr.w   #1,d7
    move.w  d7,error
    bra.b   .plus5_exit
.plus5
    move.w  d6,d7
    asr.w   #1,d7
    neg.w   d7
    move.w  d7,error
.plus5_exit

    move.w  d2,x1
    move.w  d3,y1
    
    
    cmpi.w   #4,d0
    blt.b   .start
    
    cmpi.w   #315,d0
    bgt.b   .start
    
    cmpi.w   #4,d1
    blt.b   .start
    
    cmpi.w   #175,d1
    bgt.b   .start
    
    move.b  #1,inside
    
.start
    
    cmpi.b  #3,glow
    beq.b   .loop4
    
    cmpi.b  #2,glow
    beq.b   .loop3
    
    cmpi.b  #1,glow
    beq.b   .loop2
    
    
    
.loop1
; chunky[x0+rowchunky[y0]] = col;
; chunky[x0+1+rowchunky[y0]] = col;


    cmp.w   #4,d0
    blt.b   .outside1
    
    cmp.w   #315,d0
    bgt.b   .outside1
    
    cmp.w   #4,d1
    blt.b   .outside1
    
    cmp.w   #175,d1
    bgt.b   .outside1
    
    move.l  a0,a4
    adda.l  (a1,d1.w*4),a4
;    ext.l   d7
;    and.l  #$FFFF,d7
;    adda.l  d7,a4


    adda.l   d0,a4
    
    move.b  d4,(a4)
    
;  if (dx < dy) {
    cmp.w   d5,d6
    ble.b   .nodeltax1
    
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshifty1
    move.w  #7,zeroshift
.nozeroshifty1
    
    clr.w   d2
    move.b  -1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift11
    move.w  zeroshift,d2
.nozeroshift11
; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)

    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  d4,d2
    and.w   #7,d2
    move.w  d2,col7
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,-1(a4)
    
    
 
    
    move.b  1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift12
    move.w  zeroshift,d2
.nozeroshift12

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,1(a4)
    
    
    bra.b   .nodeltay1
.nodeltax1
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshiftx1
    move.w  #7,zeroshift
.nozeroshiftx1
    
    
    clr.w   d2
    move.b  -320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift19
    move.w  zeroshift,d2
.nozeroshift19

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3

    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    
    move.w  d4,d2
    and.w   #7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    move.w  d2,col7
    
    
    add.w   d7,d3
    
    
    move.b  d3,-320(a4)
    
    
    
    move.b  320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift110
    move.w  zeroshift,d2
.nozeroshift110

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    
    move.b  d3,320(a4)
    
    bra.b   .nodeltay1
    
.outside1
    ;cmpi.b  #1,inside
    ;beq.b   .exit

.nodeltay1
  
    
    
    move.w  error,d7
    move.w  d7,d2
    
; if (e2 >-dx) {
    
    move.w  d5,d3
    neg.w   d3
    
    cmp.w   d2,d3
    bge.b   .noequal12
 
; err -= dy;
 
    sub.w   d6,d7
    move.w  d7,error

;  x0 += sx; 

    add.w   sx,d0
.noequal12
    
; if (e2 < dy) {

    cmp.w   d2,d6
    ble.b   .noequal13

; err += dx;
    
    add.w   d5,d7
    move.w  d7,error

; y0 += sy; 
    add.w   sy,d1
    
.noequal13
    
 ; if (x0==x1 && y0==y1) break;   
    
    cmp.w   x1,d0
    bne.b   .noequal11
    
    cmp.w   y1,d1
    bne.b   .noequal11
    
    bra.b   .exit
.noequal11
    
    bra.b   .loop1


    
    
    
    
    
    
    
     
    
    
.loop2
; chunky[x0+rowchunky[y0]] = col;
; chunky[x0+1+rowchunky[y0]] = col;


    cmp.w   #4,d0
    blt.b   .outside2
    
    cmp.w   #315,d0
    bgt.b   .outside2
    
    cmp.w   #4,d1
    blt.b   .outside2
    
    cmp.w   #175,d1
    bgt.b   .outside2
    
    
    move.l  a0,a4
    adda.l   (a1,d1.w*4),a4

;    move.w  (a1,d1.w*2),d7
;    ext.l   d7
;    and.l  #$FFFF,d7
;    adda.l  d7,a4


    adda.l   d0,a4
    
    move.b  d4,(a4)
    
;  if (dx < dy) {
    cmp.w   d5,d6
    ble.b   .nodeltax2
    
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshifty2
    move.w  #7,zeroshift
.nozeroshifty2
    
    clr.w   d2
    move.b  -1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift21
    move.w  zeroshift,d2
.nozeroshift21
; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)

    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  d4,d2
    and.w   #7,d2
    move.w  d2,col7
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,-1(a4)
    
    
 
    
    move.b  1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift22
    move.w  zeroshift,d2
.nozeroshift22

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,1(a4)
    
    
    
    
    move.b  -2(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift23
    move.w  zeroshift,d2
.nozeroshift23

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
   
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$FFF8,d3
    
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,-2(a4)
    
    
    
    
    move.b  2(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift24
    move.w  zeroshift,d2
.nozeroshift24

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$FFF8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
   
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,2(a4)
    
    bra.b   .nodeltay2
.nodeltax2
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshiftx2
    move.w  #7,zeroshift
.nozeroshiftx2
    
    
    clr.w   d2
    move.b  -320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift29
    move.w  zeroshift,d2
.nozeroshift29

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3

    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    
    move.w  d4,d2
    and.w   #7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    move.w  d2,col7
    
    
    add.w   d7,d3
    
    
    move.b  d3,-320(a4)
    
    
    
    move.b  320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift210
    move.w  zeroshift,d2
.nozeroshift210

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    
    move.b  d3,320(a4)
    

    
    
    move.b  -640(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift211
    move.w  zeroshift,d2
.nozeroshift211

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$fff8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,-640(a4)
    
    
    
    
    move.b  640(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift212
    move.w  zeroshift,d2
.nozeroshift212

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$fff8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,640(a4)
    
    bra.b   .nodeltay2
    
.outside2
   ; cmpi.b  #1,inside
   ; beq.b   .exit
    
.nodeltay2
  
    
    
    move.w  error,d7
    move.w  d7,d2
    
; if (e2 >-dx) {
    
    move.w  d5,d3
    neg.w   d3
    
    cmp.w   d2,d3
    bge.b   .noequal22
 
; err -= dy;
 
    sub.w   d6,d7
    move.w  d7,error

;  x0 += sx; 

    add.w   sx,d0
.noequal22
    
; if (e2 < dy) {

    cmp.w   d2,d6
    ble.b   .noequal23

; err += dx;
    
    add.w   d5,d7
    move.w  d7,error

; y0 += sy; 
    add.w   sy,d1
    
.noequal23
    
 ; if (x0==x1 && y0==y1) break;   
    
    cmp.w   x1,d0
    bne.b   .noequal21
    
    cmp.w   y1,d1
    bne.b   .noequal21
    
    bra.b   .exit
.noequal21
    
    bra.b   .loop2


    
    
    
    
    
    
    
    
     
    
    
    
    
    
    
    
    
.loop3
; chunky[x0+rowchunky[y0]] = col;
; chunky[x0+1+rowchunky[y0]] = col;


    cmp.w   #4,d0
    blt.b   .outside3
    
    cmp.w   #315,d0
    bgt.b   .outside3
    
    cmp.w   #4,d1
    blt.b   .outside3
    
    cmp.w   #175,d1
    bgt.b   .outside3
    
    
    move.l  a0,a4
    adda.l   (a1,d1.w*4),a4
;    move.w  (a1,d1.w*2),d7
;    ext.l   d7
;    and.l  #$FFFF,d7
;    adda.l  d7,a4


    adda.l   d0,a4
    
    move.b  d4,(a4)
    
;  if (dx < dy) {
    cmp.w   d5,d6
    ble.b   .nodeltax3
    
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshifty3
    move.w  #7,zeroshift
.nozeroshifty3
    
    clr.w   d2
    move.b  -1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift31
    move.w  zeroshift,d2
.nozeroshift31
; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)

    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  d4,d2
    and.w   #7,d2
    move.w  d2,col7
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,-1(a4)
    
    
 
    
    move.b  1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift32
    move.w  zeroshift,d2
.nozeroshift32

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,1(a4)
    
    
    
    move.b  -2(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift33
    move.w  zeroshift,d2
.nozeroshift33

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
   
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$FFF8,d3
    
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,-2(a4)
    
    
    
    
    move.b  2(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift34
    move.w  zeroshift,d2
.nozeroshift34

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$FFF8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
   
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,2(a4)
    
    
    
;    pcol0 = chunky[x0-3+rowchunky[y0]];

    move.b  -3(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift35
    move.w  zeroshift,d2
.nozeroshift35

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,-3(a4)
    
    
    
    
    
    
    move.b  3(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift36
    move.w  zeroshift,d2
.nozeroshift36

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2

    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,3(a4)
    
    bra.b   .nodeltay3
.nodeltax3
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshiftx3
    move.w  #7,zeroshift
.nozeroshiftx3
    
    
    clr.w   d2
    move.b  -320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift39
    move.w  zeroshift,d2
.nozeroshift39

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3

    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    
    move.w  d4,d2
    and.w   #7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    move.w  d2,col7
    
    
    add.w   d7,d3
    
    
    move.b  d3,-320(a4)
    
    
    
    move.b  320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift310
    move.w  zeroshift,d2
.nozeroshift310

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    
    move.b  d3,320(a4)
    
    
    
    
    move.b  -640(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift311
    move.w  zeroshift,d2
.nozeroshift311

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$fff8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,-640(a4)
    
    
    
    
    move.b  640(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift312
    move.w  zeroshift,d2
.nozeroshift312

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$fff8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,640(a4)
    
    
    
    
    move.b  -960(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift313
    move.w  zeroshift,d2
.nozeroshift313

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
   
    move.b  d3,-960(a4)
    
    
    
    move.b  960(a4),d2
    
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift314
    move.w  zeroshift,d2
.nozeroshift314

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,960(a4)
    
    bra.b   .nodeltay3
    
.outside3
  ; cmpi.b  #1,inside
  ;  beq.b   .exit

.nodeltay3
  
    
    
    move.w  error,d7
    move.w  d7,d2
    
; if (e2 >-dx) {
    
    move.w  d5,d3
    neg.w   d3
    
    cmp.w   d2,d3
    bge.b   .noequal32
 
; err -= dy;
 
    sub.w   d6,d7
    move.w  d7,error

;  x0 += sx; 

    add.w   sx,d0
.noequal32
    
; if (e2 < dy) {

    cmp.w   d2,d6
    ble.b   .noequal33

; err += dx;
    
    add.w   d5,d7
    move.w  d7,error

; y0 += sy; 
    add.w   sy,d1
    
.noequal33
    
 ; if (x0==x1 && y0==y1) break;   
    
    cmp.w   x1,d0
    bne.b   .noequal31
    
    cmp.w   y1,d1
    bne.b   .noequal31
    
    bra.b   .exit
.noequal31
    
    bra.b   .loop3


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
 
    
    
.loop4
; chunky[x0+rowchunky[y0]] = col;
; chunky[x0+1+rowchunky[y0]] = col;


    cmp.w   #4,d0
    blt.b   .outside4
    
    cmp.w   #315,d0
    bgt.b   .outside4
    
    cmp.w   #4,d1
    blt.b   .outside4
    
    cmp.w   #175,d1
    bgt.b   .outside4
    
    move.l  a0,a4
    adda.l   (a1,d1.w*4),a4
;    move.w  (a1,d1.w*2),d7
;    ext.l   d7
;    and.l  #$FFFF,d7
;    adda.l  d7,a4


    adda.l   d0,a4
    
    move.b  d4,(a4)
    
;  if (dx < dy) {
    cmp.w   d5,d6
    ble.b   .nodeltax4
    
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshifty4
    move.w  #7,zeroshift
.nozeroshifty4
    
    clr.w   d2
    move.b  -1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift41
    move.w  zeroshift,d2
.nozeroshift41
; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)

    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  d4,d2
    and.w   #7,d2
    move.w  d2,col7
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,-1(a4)
    
    
 
    
    move.b  1(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift42
    move.w  zeroshift,d2
.nozeroshift42

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    move.b  d3,1(a4)
    
    
    
    move.b  -2(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift43
    move.w  zeroshift,d2
.nozeroshift43

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
   
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$FFF8,d3
    
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,-2(a4)
    
    
    
    
    move.b  2(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift44
    move.w  zeroshift,d2
.nozeroshift44

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$FFF8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
   
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,2(a4)
    
    
;    pcol0 = chunky[x0-3+rowchunky[y0]];

    move.b  -3(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift45
    move.w  zeroshift,d2
.nozeroshift45

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,-3(a4)
    
    
    
    
    
    
    move.b  3(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift46
    move.w  zeroshift,d2
.nozeroshift46

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2

    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,3(a4)
    
    
;    random_v = (random_v * (ULONG)2654435761);
;    return random_v>>16;
    
    move.l  random_v(pc),d2
    add.l   #2654435761,d2
    move.l  d2,d3
    swap    d3
    move.l  d3,random_v
    and.w   #1,d2
    cmpi.w  #0,d2
    bne.w   .nodeltay4
  
;    pcol0 = chunky[x0-4+rowchunky[y0]];

    move.b  -4(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift47
    move.w  zeroshift,d2
.nozeroshift47

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2

    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,-4(a4)
    
    
    
    
    move.l  random_v(pc),d2
    add.l   #2654435761,d2
    move.l  d2,d3
    swap    d3
    move.l  d3,random_v
    and.w   #1,d2
    cmpi.w  #0,d2
    bne.w   .nodeltay4
    
    
    move.b  4(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift48
    move.w  zeroshift,d2
.nozeroshift48

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  d2,col7

    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,4(a4)
    
    
    
    bra.b   .nodeltay4
.nodeltax4
    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshiftx4
    move.w  #7,zeroshift
.nozeroshiftx4
    
    
    clr.w   d2
    move.b  -320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift49
    move.w  zeroshift,d2
.nozeroshift49

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    
    move.w  d4,d3
    lsr.w   #3,d3
    move.w  d3,col3
    
    add.w   d7,d3

    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    
    move.w  d4,d2
    and.w   #7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    move.w  d2,col7
    
    
    add.w   d7,d3
    
    
    move.b  d3,-320(a4)
    
    
    
    move.b  320(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift410
    move.w  zeroshift,d2
.nozeroshift410

; pcol0 = ((((pcol0 >> 3) + (col >> 3))>>1) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  col3,d3
    
    add.w   d7,d3
    
    and.w   #$fffe,d3
    lsl.w   #2,d3
    
; + (((pcol0&7) + (col&7))>>1);
    
    and.w   #7,d2
    move.w  d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #1,d7
    
    add.w   d7,d3
    
    
    move.b  d3,320(a4)
    
    
    move.b  -640(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift411
    move.w  zeroshift,d2
.nozeroshift411

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$fff8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,-640(a4)
    
    
    
    
    move.b  640(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift412
    move.w  zeroshift,d2
.nozeroshift412

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    
    lsl.w   #2,d3
    add.w   d7,d3
        
    move.w  col3,d7
    lsl.w   #1,d7
    add.w   col3,d7
    
    add.w   d7,d3
    
    and.w   #$fff8,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #2,d7
    add.w   d2,d7
        
    move.w  col7,d2
    lsl.w   #1,d2
    add.w   col7,d2
    
    add.w   d2,d7
    lsr.w   #3,d7
    
    add.w   d7,d3
    
    
    move.b  d3,640(a4)
    
    
    move.b  -960(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift413
    move.w  zeroshift,d2
.nozeroshift413

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
   
    move.b  d3,-960(a4)
    
    
    
    move.b  960(a4),d2
    
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift414
    move.w  zeroshift,d2
.nozeroshift414

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,960(a4)
    
    
    move.l  random_v(pc),d2
    add.l   #2654435761,d2
    move.l  d2,d3
    swap    d3
    move.l  d3,random_v
    and.w   #1,d2
    cmpi.w  #0,d2
    bne.w   .nodeltay4
    
    move.b  -1280(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift415
    move.w  zeroshift,d2
.nozeroshift415

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
   
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,-1280(a4)
    
    
    move.l  random_v(pc),d2
    add.l   #2654435761,d2
    move.l  d2,d3
    swap    d3
    move.l  d3,random_v
    and.w   #1,d2
    cmpi.w  #0,d2
    bne.w   .nodeltay4
    
    move.b  1280(a4),d2
    
    ;cmpi.b  #0,d2
    bne.b   .nozeroshift416
    move.w  zeroshift,d2
.nozeroshift416

; pcol0 = ((((pcol0 >> 3) + (pcol0 >> 3) + (pcol0 >> 3) + (col >> 3))>>2) << 3)
    move.w  d2,d7
    lsr.w  #3,d7
    
    move.w  d7,d3
    lsl.w   #1,d3
    add.w   d7,d3
    
    move.w  col3,d7
    
    add.w   d7,d3
    
    and.w   #$fffc,d3
    lsl.w   #1,d3
    
; + (((pcol0&7) + (pcol0&7) + (pcol0&7) + (col&7))>>2);
    
    and.w   #7,d2
    move.w  d2,d7
    
    lsl.w   #1,d7
    add.w   d2,d7
    
    move.w  col7,d2
    
    add.w   d2,d7
    lsr.w   #2,d7
    
    add.w   d7,d3
    
    move.b  d3,1280(a4)
    
    bra.b   .nodeltay4
    
.outside4
    ;cmpi.b  #1,inside
    ;beq.b   .exit
    
.nodeltay4
  
    
    
    move.w  error,d7
    move.w  d7,d2
    
; if (e2 >-dx) {
    
    move.w  d5,d3
    neg.w   d3
    
    cmp.w   d2,d3
    bge.b   .noequal42
 
; err -= dy;
 
    sub.w   d6,d7
    move.w  d7,error

;  x0 += sx; 

    add.w   sx,d0
.noequal42
    
; if (e2 < dy) {

    cmp.w   d2,d6
    ble.b   .noequal43

; err += dx;
    
    add.w   d5,d7
    move.w  d7,error

; y0 += sy; 
    add.w   sy,d1
    
.noequal43
    
 ; if (x0==x1 && y0==y1) break;   
    
    cmp.w   x1,d0
    bne.b   .noequal41
    
    cmp.w   y1,d1
    bne.b   .noequal41
    
    bra.b   .exit
.noequal41
    
    bra.b   .loop4



.exit
    movem.l	(sp)+,d6-d7/a4
    rts
    
random_v    dc.l    $9c33fe02
x1          ds.w    1
y1          ds.w    1
sx          ds.w    1
sy          ds.w    1
int         ds.b    1
glow        ds.b    1
error       ds.w    1
zeroshift   ds.w    1
col3        ds.w    1
col7        ds.w    1
inside      ds.b    1

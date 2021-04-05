
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
    movem.l d0-d7/a4,-(sp)

    move.b  d5,int
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

    
    move.w  #0,zeroshift
    cmpi.b  #63,d4
    bne.b   .nozeroshifty1
    move.w  #7,zeroshift
.nozeroshifty1

    move.w  d2,x1
    move.w  d3,y1
    
    move.w  d5,dx
    move.w  d6,dy
    

     move.w  d4,d2
     lsl.w   #8,d2
     or.w   d4,d2
     move.w d2,d3
     swap   d3
     or.l   d2,d3
     move.l d3,color4
     andi.w #$ff,d4
     moveq.l  #0,d2
     moveq.l #0,d3

 ;   move.w   #1,loop
    
    cmpi.b  #3,int
    beq.b   .loop4
    
    cmpi.b  #2,int
    beq.b   .loop3
    
    cmpi.b  #1,int
    beq.b   .loop2
    
    INCLUDE "drawglowline_int1.s"
    
    INCLUDE "drawglowline_int2.s"
    
    INCLUDE "drawglowline_int3.s"
    
    INCLUDE "drawglowline_int4.s"


    

.exit
    movem.l	(sp)+,d0-d7/a4
    rts    
    
color4      ds.l    1
random_v    dc.l    $9c33fe02
blend       ds.l    1
zeroshift   ds.w    1
x0          ds.w    1
y0          ds.w    1
x1          ds.w    1
y1          ds.w    1
dx          ds.w    1
dy          ds.w    1
sx          ds.w    1
sy          ds.w    1
error       ds.w    1
int         ds.b    1


    xdef _drawVectors
    xdef drawVectors

	section	code,code
	
BPLX1	      EQU 320
BPLX1C	      EQU 319
BPLX1SH	      EQU 20480
FP	          EQU 6

CLIP_RIGHT    EQU 20416
CLIP_BOTTOM   EQU 12992

;d0 - nvectors
;a0 - chunky buffer
;a1 - vectors
;a2 - points
;a6 - debug word values

_drawVectors
drawVectors
    movem.l d3-d7/a4-a6,-(sp)
    
;    move.l  a4,debug
    
    move.l  a3,rowchunky
    asl.w   #FP,d1
    asl.w   #FP,d2
    move.w  d1,x
    move.w  d2,y
    
    subq.w  #1,d0
    move.w  d0,npoly
.loop
    move.l  a0,a4
    move.w  x,d1
    move.w  y,d2
    


; d0 = dx1
; d1 = dx2
; d2 = dx3
	
; d3 = scanleft
; d4 = scanright
	
; d5 = y1
; d6 = y2


    move.l  6(a1),color

    moveq   #0,d6

    lea     coordsfp(pc),a5

    move.w	0(a1),d3     
    move.w	2(a1),d4
    move.w	4(a1),d5     

; Store coordinates with fixed point math

    move.w  0(a2,d3.w*4),0(a5)  ; x1
    move.w  2(a2,d3.w*4),2(a5)  ; y1
    move.w  0(a2,d4.w*4),4(a5)  ; x2
    move.w  2(a2,d4.w*4),6(a5)  ; y2
    move.w  0(a2,d5.w*4),8(a5)  ; x3
    move.w  2(a2,d5.w*4),10(a5) ; y3
    
    add.w   d1,0(a5)
    add.w   d2,2(a5)
    add.w   d1,4(a5)
    add.w   d2,6(a5)
    add.w   d1,8(a5)
    add.w   d2,10(a5)
    
; Pseudo clipping coordinates, put coords in screen border
    move.l  a5,a6
    
;    moveq.w #2,d0
    
    tst.w   (a6)
    bge.b   .nox1clip1
    or.b    #1,d6
    bra.b   .nox1clip2
.nox1clip1
    cmpi.w  #CLIP_RIGHT,(a6)
    blt.b   .nox1clip2
    or.b    #1,d6
.nox1clip2
    
    tst.w   2(a6)
    bge.b   .noy1clip1
    or.b    #1,d6
    bra.w   .noy1clip2
.noy1clip1

    cmpi.w  #CLIP_BOTTOM,2(a6)
    blt.b   .noy1clip2
    or.b    #1,d6
.noy1clip2


    tst.w   4(a6)
    bge.b   .nox2clip1
    or.b    #2,d6
    bra.b   .nox2clip2
.nox2clip1
    cmpi.w  #CLIP_RIGHT,4(a6)
    blt.b   .nox2clip2
    or.b    #2,d6
.nox2clip2
    
    tst.w   6(a6)
    bge.b   .noy2clip1
    or.b    #2,d6
    bra.w   .noy2clip2
.noy2clip1

    cmpi.w  #CLIP_BOTTOM,6(a6)
    blt.b   .noy2clip2
    or.b    #2,d6
.noy2clip2


    tst.w   8(a6)
    bge.b   .nox3clip1
    or.b    #4,d6
    bra.b   .nox3clip2
.nox3clip1
    cmpi.w  #CLIP_RIGHT,8(a6)
    blt.b   .nox3clip2
    or.b    #4,d6
.nox3clip2
    
    tst.w   10(a6)
    bge.b   .noy3clip1
    or.b    #4,d6
    bra.w   .noy3clip2
.noy3clip1

    cmpi.w  #CLIP_BOTTOM,10(a6)
    blt.b   .noy3clip2
    or.b    #4,d6
.noy3clip2



    cmpi.w  #7,d6
    beq.b  .skipbottom 
    
; Sort triangle points before draw it
    move.w  10(a5),d0   ; y3
    cmp.w   6(a5),d0
    bpl.b   .noSwap1
    move.l  4(a5),d0    ; x2
    move.l  8(a5),4(a5) ; x3
    move.l  d0,8(a5)    ; x3
.noSwap1

    move.w  6(a5),d0    ; y2
    cmp.w   2(a5),d0
    bpl.b   .noSwap2
    move.l  (a5),d0     ; x1 y1
    move.l  4(a5),(a5)  ; x2 y2
    move.l  d0,4(a5)    ; x2 y2
.noSwap2

    move.w  10(a5),d0   ; y3
    cmp.w   6(a5),d0
    bpl.b   .noSwap3
    move.l  4(a5),d0    ; x2
    move.l  8(a5),4(a5) ; x3
    move.l  d0,8(a5)    ; x3
.noSwap3

    
    
    
; Store rounded coordinates 

    lea     coords(pc),a6
    
    move.w  0(a5),d0
    move.w  2(a5),d1
    move.w  4(a5),d2
    move.w  6(a5),d3
    move.w  8(a5),d4
    move.w  10(a5),d5
    
    asr.w   #FP,d0
    asr.w   #FP,d1
    asr.w   #FP,d2
    asr.w   #FP,d3
    asr.w   #FP,d4
    asr.w   #FP,d5
    
    move.w  d0,0(a6)
    move.w  d1,2(a6)
    move.w  d2,4(a6)
    move.w  d3,6(a6)
    move.w  d4,8(a6)
    move.w  d5,10(a6)
  
; Calculate triangle vertices increment

    move.w  4(a6),d0
;    move.w  (a6),d4

    sub.w   (a6),d0       ; (x2>>6 - x1>>6) << 6
    asl.w   #FP,d0

    ;move.w  2(a6),d5    ; Compare y1 with y2, if equal jump to avoid a 0 div
    move.w  6(a6),d6

    cmp.w   2(a6),d6
    beq.b   .div0_avoid1

;    move.w  d6,d4       ; (y2>>6 - y1>>6) 
    sub.w   2(a6),d6

    ext.l   d0
    divs.w  d6,d0       ; d0 = dx1 = ((x2>>6 - x1>>6) << 6) / (y2>>6 - y1>>6)
    bra.b   .dx1done
.div0_avoid1
    moveq.w #0,d0
.dx1done
    move.w  d0,dx1
    

    move.w  8(a6),d1    ; x3
;    move.w  (a6),d4     ; x1

    sub.w   (a6),d1       ; (x3 - x1) << 6
    asl.w   #FP,d1

   ; move.w  2(a6),d5    ; y2, Compare y1 with y3, if equal jump to avoid a 0 div
    move.w  10(a6),d6   ; y3

    cmp.w   2(a6),d6
    beq.b   .div0_avoid2


;    move.w  d6,d4       ; (y3 - y1) 
    sub.w   2(a6),d6

    ext.l   d1
    divs.w  d6,d1        ; d1 = dx2 = ((x3 - x1) << 6) / (y3 - y1)
    bra.b   .dx2done
.div0_avoid2
    moveq.w #0,d1
.dx2done
    move.w  d1,dx2


    move.w  8(a6),d2
  ;  move.w  4(a6),d4

    sub.w   4(a6),d2        ; (x3 - x2) << 6
    asl.w   #FP,d2

  ;  move.w  6(a6),d5     ; Compare y2 with y3, if equal jump to avoid a 0 div
    move.w  10(a6),d6

    cmp.w   6(a6),d6
    beq.b   .div0_avoid3

;    move.w  d6,d4        ; (y3 - y2) 
    sub.w   6(a6),d6

    ext.l   d2
    divs.w  d6,d2        ; d2 = dx3 = ((x3 - x2) << 6) / (y3 - y2)
    bra.b   .dx3done
.div0_avoid3
    moveq.w #0,d2
.dx3done
    move.w  d2,dx3


    cmp.w   d1,d0       ; if dx1 < dx2 then jump .dx1dx2
    blt.b   .dx1dx2
    exg     d0,d1
.dx1dx2	  
    
    
    
    
    
    
    
    
    move.w  2(a6),d5

    cmpi.w  #0,d5
    blt.b   .isoutside

    move.l  rowchunky(pc),a3
    adda.l   (a3,d5.w*4),a4
    bra.b   .isinside
.isoutside    
  ;  moveq.l  #0,d6
    move.w  d5,d6
    asl.w   #2,d6
    sub.w   d5,d6
    add.w   d6,d6
    sub.w   d5,d6
    asl.w   #6,d6
    ext.l   d6
    adda.l  d6,a4
.isinside
    
    moveq.l  #0,d6
    move.w  d5,ytmp
    
;.start
    move.w  (a5),d3     ; scanleft = x1 << 6
    move.w  d3,d4       ; scanright = x1 << 6

 ;   move.w  2(a6),d6      ; y1
    move.w  6(a6),d5      ; y2

    sub.w   2(a6),d5
    subq.w  #1,d5
    blt.b   .skiptop
    move.l  color(pc),d2
.top

; d6 scanleft
; d7 scanright
 
;   Vertical clipping
    cmpi.w  #0,ytmp
    blt.b   .skipline1
    cmpi.w  #184,ytmp
    bgt.b   .skipbottom
    
    
    move.w  d3,d6       ; d6 = scanleft
    move.w  d4,d7      ; d7 = scanright
       
;  Horizontal clipping

    cmpi.w  #0,d4
    blt.b   .skipline1
    
    cmpi.w  #0,d3
    bge.b   .noclipping11
    moveq.w #0,d6
.noclipping11

    cmpi.w  #CLIP_RIGHT,d3
    bgt.b   .skipline1
    
    cmpi.w  #CLIP_RIGHT,d4
    ble.b   .noclipping12
    move.w  #CLIP_RIGHT,d7
.noclipping12


    lsr.w   #FP,d6      ; scanleft >> 6
    lsr.w   #FP,d7     ; scanright>>6


    move.l  a4,a3        ; a3 = Start line y pointer
    adda.l   d6,a3       ; chunky [a3+d6] [y + scanleft]

    sub.w   d6,d7      ; d6 = scanright - scanleft
    ;subq.w  #1,d7
    bmi.b   .skipline1


;    move.w  d4,d6
;    and.w   #$3f,d6
;    lsr.w   #5,d6
;    add.w   d6,d7

    ; There aren't enought pixels to draw long values? skip to linetopb
    
    move.w  d7,d6
    and.w   #3,d7
    lsr.w   #2,d6
    subq.w  #1,d6
    blt.b   .linetopb
    
    ; Write long values
.linetopl
    move.l  d2,(a3)+
    dbf.b   d6,.linetopl
    
.linetopb
    move.b  d2,(a3)+
    dbf.b   d7,.linetopb
    
.skipline1
    add.w   d1,d4        ; scanright + rightadd
    add.w   d0,d3        ; scanleft + leftadd
    lea     320(a4),a4
    add.w   #1,ytmp
    dbf.b   d5,.top
.skiptop

    move.w  dx2(pc),d1
    move.w  dx3(pc),d2
    
    cmp.w   d1,d2       ; (dx2 < dx3) ?
    blt.b   .dx2dx31
    
    move.w  d2,d0       ; dx1=dx3
    
    move.w  2(a6),d5    ; y1==y2?
 ;   move.w  6(a6),d6
    
    cmp.w   6(a6),d5
    bne.b   .dx2dx32
    
    move.w	4(a5),d3     ; scanleft = x2 << 6
    move.w  (a5),d4      ; scanright = x1 << 6
    
    bra.b   .dx2dx32
.dx2dx31 

    move.w  dx2(pc),d0
    move.w  dx3(pc),d1
    
    move.w  2(a6),d5    ; y1==y2?
   ; move.w  6(a6),d6 
    
    cmp.w   6(a6),d5
    bne.b   .dx2dx32
    
    move.w  (a5),d3         ; scanleft = x1 << 6
    move.w  4(a5),d4        ; scanright = x2 << 6

.dx2dx32


    move.w  6(a6),ytmp      ; y2
    
    move.w  10(a6),d5     ; y3

    sub.w   6(a6),d5
    subq.w  #1,d5
    blt.b   .skipbottom
    move.l  color,d2
.bottom


    move.w  d3,d6       ; d6 = scanleft
    move.w  d4,d7       ; d7 = scanright
    
;   Vertical clipping
    cmpi.w  #0,ytmp
    blt.b   .skipline2
    cmpi.w  #184,ytmp
    bgt.b   .skipbottom

;  Horizontal clipping

    cmpi.w  #0,d4
    blt.b   .skipline2
    
    cmpi.w  #0,d3
    bge.b   .noclipping21
    moveq.w #0,d6
.noclipping21

    cmpi.w  #CLIP_RIGHT,d3
    bgt.b   .skipline2
    
    cmpi.w  #CLIP_RIGHT,d4
    ble.b   .noclipping22
    move.w  #CLIP_RIGHT,d7
.noclipping22

    lsr.w   #FP,d6      ; scanleft >> 6
    lsr.w   #FP,d7      ; scanright>>6
 

    move.l  a4,a3       ; a3 = Start line y pointer
    add.l   d6,a3       ; chunky [a3+d6] [y + scanleft]
    sub.w   d6,d7       ; d6 = scanright - scanleft
    ;subq.w  #1,d7
    bmi.b   .skipline2


;    move.w  d4,d6
;    and.w   #$3f,d6
;    lsr.w   #5,d6
;    add.w   d6,d7

    move.w  d7,d6

    and.w   #3,d7
    lsr.w   #2,d6
    subq.w  #1,d6
    
    blt.b   .linebottomb
    
    
.linebottoml
    move.l  d2,(a3)+
    dbf.b	d6,.linebottoml

.linebottomb  
    move.b  d2,(a3)+
    dbf.b   d7,.linebottomb

.skipline2
    add.w   d1,d4       ; scanright + rightadd
    add.w   d0,d3       ; scanleft + leftadd
    lea     320(a4),a4
    add.w   #1,ytmp
    dbf.b   d5,.bottom
.skipbottom
    subq.w  #1,npoly
    add.l   #10,a1
    bpl.b   .loop
.exit
    
;   move.w  d4,d7      ; d7 = scanright
    ;lsr.w	#FP,d7     ; scanright>>6
    
;    move.l  debug,a4
;    ext.l   d5
;    ext.l   d6
;    move.l  d5,(a4)
;    move.l  d6,4(a4)
;    move.l  d4,8(a4)
;    move.l  d7,12(a4)
    ;move.w  dx3,4(a4)
;    move.w  d7,6(a6)
;    move.w  d0,8(a6)
;    move.w  d1,10(a6)
;.exit2
    movem.l	(sp)+,d3-d7/a4-a6
    rts
    
ytmp        ds.w    1
x           ds.w    1
y           ds.w    1
rowchunky   ds.l    1
npoly       ds.w    1
dx1         ds.w    1
dx2         ds.w    1
dx3         ds.w    1
color       ds.l    1
coordsfp    ds.w    12
coords      ds.w    12
debug       ds.l    1

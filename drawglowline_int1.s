
.loop1
; chunky[x0+rowchunky[y0]] = col;
; chunky[x0+1+rowchunky[y0]] = col;
    
    move.w  d0,x0
    move.w  d1,y0

    cmp.w   #4,d0
    blt.b   .nodeltax1
    
    cmp.w   #315,d0
    bgt.b   .nodeltax1
    
    cmp.w   #4,d1
    blt.b   .nodeltax1
    
    cmp.w   #184,d1
    bgt.b   .nodeltax1
    
    move.l  a0,a4
    adda.l  (a1,d1.w*4),a4
;    move.w  (a1,d1.w*2),d7
;    ext.l   d7
;    and.l  #$FFFF,d7
;    adda.l  d7,a4


    adda.l   d0,a4
    
    move.b  d4,(a4)
;    bra.b   .nodeltax1
    
    
;  if (dx < dy) {
    move.w  dy,d2
    cmp.w   dx,d2
    ble.b   .nodeltay1
    
; d0 =
; d1 =
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1

    moveq.w  #0,d2

;   ocol = *pcol;
    move.b  -1(a4),d2
    

;    pcol0 = ocol & 0xff;
    move.w  d2,d3
    and.w   #$ff,d3

    
    bne.b   .nozeroshift11
    move.w  zeroshift,d2
.nozeroshift11

;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += ocol & 7;
    move.w  d2,d7
    and.w   #7,d7
    add.w   d5,d7


;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 1;
    lsr.w   #1,d3
    
    move.b  d3,-1(a4)
    
    
    
    moveq.w  #0,d2
    
;   ocol = *pcol;
    move.b  1(a4),d2
    
    
;    pcol0 = (ocol >> 24) & 0xff;
    move.w  d2,d3
   ; lsr.w   #8,d3
    and.w   #$ff,d3

    
    bne.b   .nozeroshift15
    move.w  zeroshift,d2
.nozeroshift15
    
    
;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += (ocol >> 24) & 7;
    move.w  d2,d7
  ;  lsr.w   #8,d7
    and.w   #7,d7
    add.w   d5,d7


;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 1;
    lsr.w   #1,d3
;   ncol = pcol0<<24;
 ;   lsl.w   #8,d3
   
    move.b  d3,1(a4)
    
    
    
    bra.b   .nodeltax1
    
    
    
    
.nodeltay1

    ; d0 =
; d1 =
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1

    moveq.l  #0,d3

;   ocol = *pcol;
    move.b  -320(a4),d2
    
;    pcol0 = ocol & 0xff;
    move.w  d2,d3
    and.w   #$ff,d3
    
    bne.b   .nozeroshift19
    move.w  zeroshift,d2
.nozeroshift19


;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += ocol & 7;
    move.w  d2,d7
    and.w   #7,d7
    add.w   d5,d7


;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 1;
    lsr.w   #1,d3
    
    move.b  d3,-320(a4)
    
    
    
    
;   ocol = *pcol;
    move.b  320(a4),d2
    

;    pcol0 = ocol & 0xff;
    move.w  d2,d3
    and.w   #$ff,d3

    bne.b   .nozeroshift110
    move.w  zeroshift,d2
.nozeroshift110

;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += ocol & 7;
    move.w  d2,d7
    and.w   #7,d7
    add.w   d5,d7


;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 1;
    lsr.w   #1,d3
    
    move.b  d3,320(a4)
    
    
    
    
.nodeltax1

    move.w  x0,d0
    move.w  y0,d1
    
    
    move.w  error,d7
    move.w  d7,d2
    
; if (e2 >-dx) {
    
    move.w  dx,d3
    neg.w   d3
    
    cmp.w   d2,d3
    bge.b   .noequal12
 
; err -= dy;
 
    sub.w   dy,d7
    move.w  d7,error

;  x0 += sx; 

    add.w   sx,d0
.noequal12
    
; if (e2 < dy) {

    cmp.w   dy,d2
    bge.b   .noequal13

; err += dx;
    
    add.w   dx,d7
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

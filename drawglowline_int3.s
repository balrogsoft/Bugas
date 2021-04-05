
.loop3
; chunky[x0+rowchunky[y0]] = col;
; chunky[x0+1+rowchunky[y0]] = col;
    
    move.w  d0,x0
    move.w  d1,y0

    cmp.w   #4,d0
    blt.b   .nodeltax3
    
    cmp.w   #315,d0
    bgt.b   .nodeltax3
    
    cmp.w   #4,d1
    blt.b   .nodeltax3
    
    cmp.w   #184,d1
    bgt.b   .nodeltax3
    
    move.l  a0,a4

    adda.l  (a1,d1.w*4),a4
;    move.w  (a1,d1.w*2),d7
;    ext.l   d7
;    and.l  #$FFFF,d7
;    adda.l  d7,a4


    adda.l   d0,a4
    
    move.b  d4,(a4)

;    bra.b   .nodeltax3
    
    
;  if (dx < dy) {
    move.w  dy,d2
    cmp.w   dx,d2
    ble.b   .nodeltay3
    
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
    move.l  -4(a4),d2
    
;    pcol0 = ocol & 0xff;
    move.w  d2,d0
    and.w   #$ff,d0
    
    move.w  d0,d3
    
    bne.b   .nozeroshift31
    move.w  zeroshift,d0
.nozeroshift31
    
    
;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += ocol & 7;
    move.w  d0,d7
    and.w   #7,d7
    add.w   d5,d7


;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 1;
    lsr.w   #1,d3
    
    move.l  d3,d6
    
    
; d0 = 
; d1 = col3
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1

;    pcol0 = ((ocol>>8) & 0xff)
    move.w  d2,d0
    lsr.w   #8,d0
    and.w   #$ff,d0
    

;    pcol0 *= 5
    move.w  d0,d3
    lsl.w   #2,d3
    add.w   d0,d3
    
    
    bne.b   .nozeroshift32
    move.w  zeroshift,d0
.nozeroshift32

;   pcol0 += 3*col;
    move.w  d4,d1
    lsl.w   #1,d1
    add.w   d4,d1
    add.w   d1,d3


;   pcol0 &= 0x7c0;
    and.w   #$7c0,d3

;   pcol1 = ((ocol>>8) & 7);
    and.w   #7,d0
    
;   pcol1 *= 5;    
    move.w  d0,d7
    lsl.w   #2,d7
    add.w   d0,d7

;   pcol1 += 3*(col & 7);
    move.w  d5,d0
    lsl.w   #1,d0
    add.w   d5,d0
    add.w   d0,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 3;
    lsr.w   #3,d3
    
;   ncol |= pcol0<<8;
    lsl.w   #8,d3
    
    or.l  d3,d6
    
    
    
    
    
; d0 = 
; d1 = col3
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1


;    pcol0 = ((ocol>>16) & 0xff);
    swap  d2
    move.w  d2,d0
    and.w   #$ff,d0
    
    
;    pcol0 *= 3
    move.w  d0,d3
    lsl.w   #1,d3
    add.w   d0,d3
    
    bne.b   .nozeroshift33
    move.w  zeroshift,d0
.nozeroshift33

;   pcol0 += col;
    add.w   d4,d3


;    pcol0 &= 0x3e0;
    and.w   #$3e0,d3

;   pcol1 = ((ocol>>16) & 7)
    and.w   #7,d0
    
;   pcol1 *= 3;    
    move.w  d0,d7
    lsl.w   #1,d7
    add.w   d0,d7

;   pcol1 += (col & 7);
    add.w   d5,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 2;
    lsr.w   #2,d3
    
;   ncol |= pcol0<<16;
    and.w   #$ff,d3
    swap  d3
    
    or.l  d3,d6
    
    
    
; d0 = 
; d1 = col3
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1


    
    move.w  d2,d3
    
;   ncol |= pcol0<<24;
    and.l   #$ff,d3
    lsl.w   #8,d3
    swap  d3

    or.l  d3,d6
    
    
    
    move.l  d6,-4(a4)
    
    
    
    
    
    
    
    
    moveq.l  #0,d3
    
    
;   ocol = *pcol;
    move.l  1(a4),d2
    
    
    
;    pcol0 = (ocol >> 24) & 0xff;
    swap  d2
    move.w  d2,d0
    lsr.w   #8,d0
    and.w   #$ff,d0
    

    move.w  d0,d3

    bne.b   .nozeroshift35
    move.w  zeroshift,d0
.nozeroshift35
;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += (ocol >> 24) & 7;
    move.w  d0,d7
    and.w   #7,d7
    add.w   d5,d7


;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 1;
    lsr.w   #1,d3
;   ncol = pcol0<<24;
    lsl.w   #8,d3
    move.l  d3,d6

    swap    d6
    
; d0 = 
; d1 = col3
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1

;    pcol0 = ((ocol>>16) & 0xff)
    move.w  d2,d0
;    lsr.w   #8,d0
    and.w   #$ff,d0
    
;    pcol0 *= 5
    move.w  d0,d3
    lsl.w   #2,d3
    add.w   d0,d3
    
    bne.b   .nozeroshift36
    move.w  zeroshift,d0
.nozeroshift36

    

;   pcol0 += 3*col;
    move.w  d4,d1
    lsl.w   #1,d1
    add.w   d4,d1
    add.w   d1,d3


;   pcol0 &= 0x7c0;
    and.w   #$7c0,d3

;   pcol1 = ((ocol>>16) & 7);
    and.w   #7,d0
    
;   pcol1 *= 5;    
    move.w  d0,d7
    lsl.w   #2,d7
    add.w   d0,d7

;   pcol1 += 3*(col & 7);
    move.w  d5,d0
    lsl.w   #1,d0
    add.w   d5,d0
    add.w   d0,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 3;
    lsr.w   #3,d3
    
;   ncol |= pcol0<<16;
    ;lsl.w   #8,d3
    swap d3
    or.l  d3,d6
    
    
    
    
    
; d0 = 
; d1 = col3
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1


;    pcol0 = ((ocol>>8) & 0xff);
    swap  d2
    move.w  d2,d0
    lsr.w   #8,d0
    and.w   #$ff,d0


;    pcol0 *= 3
    move.w  d0,d3
    lsl.w   #1,d3
    add.w   d0,d3

    bne.b   .nozeroshift37
    move.w  zeroshift,d0
.nozeroshift37

;   pcol0 += col;
    add.w   d4,d3


;    pcol0 &= 0x3e0;
    and.w   #$3e0,d3

;   pcol1 = ((ocol>>8) & 7)
    and.w   #7,d0
    
;   pcol1 *= 3;    
    move.w  d0,d7
    lsl.w   #1,d7
    add.w   d0,d7

;   pcol1 += (col & 7);
    add.w   d5,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 2;
    lsr.w   #2,d3
    
;   ncol |= pcol0<<8;
    and.w   #$ff,d3
    lsl.w   #8,d3
    
    or.l  d3,d6
    
    
    
; d0 = 
; d1 = col3
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1
 
    move.w  d2,d3
   
;   ncol |= pcol0;
    and.l   #$ff,d3
    
    or.l  d3,d6
    
    
    
    move.l  d6,1(a4)
    
    
    
    bra.b   .nodeltax3
    
    
    
    
.nodeltay3

    ; d0 =
; d1 =
; d2 = *pcol
; d3 = pcol0
; d4 = col
; d5 = col&7
; d6 = ncol
; d7 = pcol1

    moveq.l  #0,d3
    moveq.l  #0,d0

;   ocol = *pcol;
    move.b  -320(a4),d2
    
;    pcol0 = ocol & 0xff;
    move.w  d2,d0
    and.w   #$ff,d0
    move.w  d0,d3
    
    bne.b   .nozeroshift39
    move.w  zeroshift,d0
.nozeroshift39

;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += ocol & 7;
    move.w  d0,d7
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
    move.w  d2,d0
    and.w   #$ff,d0
    move.w  d0,d3
    
    bne.b   .nozeroshift310
    move.w  zeroshift,d0
.nozeroshift310

;   pcol0 += col;
    add.w   d4,d3

;   pcol0 &= 0x1f0;
    and.w   #$1f0,d3

;   pcol1 = col & 7;
    move.w  d4,d5
    and.w   #7,d5

;   pcol1 += ocol & 7;
    move.w  d0,d7
    and.w   #7,d7
    add.w   d5,d7


;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 1;
    lsr.w   #1,d3
    
    move.b  d3,320(a4)
    
  
  
  
    move.b  -640(a4),d2
  
;    pcol0 = ((ocol>>8) & 0xff)
    move.w  d2,d0
  ;  lsr.w   #8,d0
    and.w   #$ff,d0
    
    

;    pcol0 *= 5
    move.w  d0,d3
    lsl.w   #2,d3
    add.w   d0,d3
    
    bne.b   .nozeroshift311
    move.w  zeroshift,d0
.nozeroshift311

;   pcol0 += 3*col;
    move.w  d4,d1
    lsl.w   #1,d1
    add.w   d4,d1
    add.w   d1,d3


;   pcol0 &= 0x7c0;
    and.w   #$7c0,d3

;   pcol1 = ((ocol>>8) & 7);
    and.w   #7,d0
    
;   pcol1 *= 5;    
    move.w  d0,d7
    lsl.w   #2,d7
    add.w   d0,d7

;   pcol1 += 3*(col & 7);
    move.w  d5,d0
    lsl.w   #1,d0
    add.w   d5,d0
    add.w   d0,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 3;
    lsr.w   #3,d3
    
;   ncol |= pcol0<<8;
    ;lsl.w   #8,d3
    
    move.b  d3,-640(a4)
    
    
  
  
    move.b  640(a4),d2
  
;    pcol0 = ((ocol>>8) & 0xff)
    move.w  d2,d0
   ; lsr.w   #8,d0
    and.w   #$ff,d0
    
;    pcol0 *= 5
    move.w  d0,d3
    lsl.w   #2,d3
    add.w   d0,d3
    
    
    bne.b   .nozeroshift312
    move.w  zeroshift,d0
.nozeroshift312

;   pcol0 += 3*col;
    move.w  d4,d1
    lsl.w   #1,d1
    add.w   d4,d1
    add.w   d1,d3


;   pcol0 &= 0x7c0;
    and.w   #$7c0,d3

;   pcol1 = ((ocol>>8) & 7);
    and.w   #7,d0
    
;   pcol1 *= 5;    
    move.w  d0,d7
    lsl.w   #2,d7
    add.w   d0,d7

;   pcol1 += 3*(col & 7);
    move.w  d5,d0
    lsl.w   #1,d0
    add.w   d5,d0
    add.w   d0,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 3;
    lsr.w   #3,d3
    
;   ncol |= pcol0<<8;
  ;  lsl.w   #8,d3
    
    move.b  d3,640(a4)
    
    
    
    
    
    
;    pcol0 = ((ocol>>16) & 0xff);

    move.b  -960(a4),d2
    move.w  d2,d0
    and.w   #$ff,d0
    
;    pcol0 *= 3
    move.w  d0,d3
    lsl.w   #1,d3
    add.w   d0,d3
    
    bne.b   .nozeroshift313
    move.w  zeroshift,d0
.nozeroshift313

    
;   pcol0 += col;
    add.w   d4,d3


;    pcol0 &= 0x3e0;
    and.w   #$3e0,d3

;   pcol1 = ((ocol>>16) & 7)
    and.w   #7,d0
    
;   pcol1 *= 3;    
    move.w  d0,d7
    lsl.w   #1,d7
    add.w   d0,d7

;   pcol1 += (col & 7);
    add.w   d5,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 2;
    lsr.w   #2,d3
    
;   ncol |= pcol0<<16;
    and.w   #$ff,d3
    
    move.b  d3,-960(a4)
    
    
    
    move.b  960(a4),d2
    move.w  d2,d0
    and.w   #$ff,d0
    
;    pcol0 *= 3
    move.w  d0,d3
    lsl.w   #1,d3
    add.w   d0,d3
    
    bne.b   .nozeroshift314
    move.w  zeroshift,d0
.nozeroshift314
    

;   pcol0 += col;
    add.w   d4,d3


;    pcol0 &= 0x3e0;
    and.w   #$3e0,d3

;   pcol1 = ((ocol>>16) & 7)
    and.w   #7,d0
    
;   pcol1 *= 3;    
    move.w  d0,d7
    lsl.w   #1,d7
    add.w   d0,d7

;   pcol1 += (col & 7);
    add.w   d5,d7

;   pcol0 += pcol1;
    add.w   d7,d3
;   pcol0 >>= 2;
    lsr.w   #2,d3
    
;   ncol |= pcol0<<16;
    and.w   #$ff,d3
    
    move.b  d3,960(a4)
    
    
.nodeltax3

    move.w  x0,d0
    move.w  y0,d1
    
    
    move.w  error,d7
    move.w  d7,d2
    
; if (e2 >-dx) {
    
    move.w  dx,d3
    neg.w   d3
    
    cmp.w   d2,d3
    bge.b   .noequal32
 
; err -= dy;
 
    sub.w   dy,d7
    move.w  d7,error

;  x0 += sx; 

    add.w   sx,d0
.noequal32
    
; if (e2 < dy) {

    cmp.w   dy,d2
    bge.b   .noequal33

; err += dx;
    
    add.w   dx,d7
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

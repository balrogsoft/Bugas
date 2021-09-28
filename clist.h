UWORD coplist[] =
{
    FMODE,   0xf,
    BPLCON0, 0x6200,
    BPLCON1, 0,
    BPLCON2, 0x224,
    DIWSTRT, 0x5281,
    DIWSTOP, 0x06c1,

    DDFSTRT, 0x0038, 
    DDFSTOP, 0x00b0,
    
    BPL1MOD, 0,
    BPL2MOD, 0,

    SPR0PTH, 0,
    SPR0PTL, 0,
        
    SPR1PTH, 0,
    SPR1PTL, 0,
        
    SPR2PTH, 0,
    SPR2PTL, 0,
        
    SPR3PTH, 0,
    SPR3PTL, 0,
        
    SPR4PTH, 0,
    SPR4PTL, 0,
        
    SPR5PTH, 0,
    SPR5PTL, 0,
        
    SPR6PTH, 0,
    SPR6PTL, 0,
        
    SPR7PTH, 0,
    SPR7PTL, 0,

    BPL1PTH, 0,
    BPL1PTL, 0,

    BPL2PTH, 0,
    BPL2PTL, 0,

    BPL3PTH, 0,
    BPL3PTL, 0,

    BPL4PTH, 0,
    BPL4PTL, 0,

    BPL5PTH, 0,
    BPL5PTL, 0,

    BPL6PTH, 0,
    BPL6PTL, 0,

    0x0106, 0x0000,
    0x0180, 0x0000,
    0x0182, 0x0fff,
    0x0184, 0x0000,
    0x0186, 0x0000,
    0x0188, 0x0000,
    0x018A, 0x0000,
    0x018C, 0x0000,
    0x018E, 0x0000,
    0x0190, 0x0000,
    0x0192, 0x0000,
    0x0194, 0x0000,
    0x0196, 0x0000,
    0x0198, 0x0000,
    0x019A, 0x0000,
    0x019C, 0x0000,
    0x019E, 0x0000,
    0x01A0, 0x0000,
    0x01A2, 0x0000,
    0x01A4, 0x0000,
    0x01A6, 0x0000,
    0x01A8, 0x0000,
    0x01AA, 0x0000,
    0x01AC, 0x0000,
    0x01AE, 0x0000,
    0x01B0, 0x0000,
    0x01B2, 0x0000,
    0x01B4, 0x0000,
    0x01B6, 0x0000,
    0x01B8, 0x0000,
    0x01BA, 0x0000,
    0x01BC, 0x0000,
    0x01BE, 0x0000,
    0x0106, 0x2000,
    0x0180, 0x0000,
    0x0182, 0x0000,
    0x0184, 0x0000,
    0x0186, 0x0000,
    0x0188, 0x0000,
    0x018A, 0x0000,
    0x018C, 0x0000,
    0x018E, 0x0000,
    0x0190, 0x0000,
    0x0192, 0x0000,
    0x0194, 0x0000,
    0x0196, 0x0000,
    0x0198, 0x0000,
    0x019A, 0x0000,
    0x019C, 0x0000,
    0x019E, 0x0000,
    0x01A0, 0x0000,
    0x01A2, 0x0000,
    0x01A4, 0x0000,
    0x01A6, 0x0000,
    0x01A8, 0x0000,
    0x01AA, 0x0000,
    0x01AC, 0x0000,
    0x01AE, 0x0000,
    0x01B0, 0x0000,
    0x01B2, 0x0000,
    0x01B4, 0x0000,
    0x01B6, 0x0000,
    0x01B8, 0x0000,
    0x01BA, 0x0000,
    0x01BC, 0x0000,
    0x01BE, 0x0000,
    0x0106, 0x0200,
    0x0180, 0x0000,
    0x0182, 0x0000,
    0x0184, 0x0000,
    0x0186, 0x0000,
    0x0188, 0x0000,
    0x018A, 0x0000,
    0x018C, 0x0000,
    0x018E, 0x0000,
    0x0190, 0x0000,
    0x0192, 0x0000,
    0x0194, 0x0000,
    0x0196, 0x0000,
    0x0198, 0x0000,
    0x019A, 0x0000,
    0x019C, 0x0000,
    0x019E, 0x0000,
    0x01A0, 0x0000,
    0x01A2, 0x0000,
    0x01A4, 0x0000,
    0x01A6, 0x0000,
    0x01A8, 0x0000,
    0x01AA, 0x0000,
    0x01AC, 0x0000,
    0x01AE, 0x0000,
    0x01B0, 0x0000,
    0x01B2, 0x0000,
    0x01B4, 0x0000,
    0x01B6, 0x0000,
    0x01B8, 0x0000,
    0x01BA, 0x0000,
    0x01BC, 0x0000,
    0x01BE, 0x0000,
    0x0106, 0x2200,
    0x0180, 0x0000,
    0x0182, 0x0000,
    0x0184, 0x0000,
    0x0186, 0x0000,
    0x0188, 0x0000,
    0x018A, 0x0000,
    0x018C, 0x0000,
    0x018E, 0x0000,
    0x0190, 0x0000,
    0x0192, 0x0000,
    0x0194, 0x0000,
    0x0196, 0x0000,
    0x0198, 0x0000,
    0x019A, 0x0000,
    0x019C, 0x0000,
    0x019E, 0x0000,
    0x01A0, 0x0000,
    0x01A2, 0x0000,
    0x01A4, 0x0000,
    0x01A6, 0x0000,
    0x01A8, 0x0000,
    0x01AA, 0x0000,
    0x01AC, 0x0000,
    0x01AE, 0x0000,
    0x01B0, 0x0000,
    0x01B2, 0x0000,
    0x01B4, 0x0000,
    0x01B6, 0x0000,
    0x01B8, 0x0000,
    0x01BA, 0x0000,
    0x01BC, 0x0000,
    0x01BE, 0x0000,
    BPLCON4, 0x0020,

    0xffff, 0xfffe,
    0xffff, 0xfffe
};



#ifndef _C2P_H
#define	_C2P_H

#ifdef __VBCC__ 
#define REG(x,y) __reg(#x) y

// VBCC c2p functions

extern void c2p1x1_6_c5_030_smcinit(REG(d0,ULONG),REG(d1,ULONG),REG(d2, ULONG),REG(d3, ULONG),REG(d4, ULONG),REG(d5, ULONG),REG(d6, ULONG));
extern void c2p1x1_6_c5_030_init(REG(d0,ULONG),REG(d1,ULONG),REG(d2, ULONG),REG(d3, ULONG),REG(d4, ULONG),REG(d5, ULONG),REG(d6, ULONG));
extern void c2p1x1_6_c5_030(REG(a0, *UBYTE), REG(a1, *UBYTE));

#else

// SAS\C c2p functions

void __asm c2p1x1_6_c5_030_smcinit( register __d0 ULONG chunky_x,
                                    register __d1 ULONG chunky_y,
                                    register __d2 ULONG offset_x,
                                    register __d3 ULONG offset_y,
                                    register __d4 ULONG rowlen,
                                    register __d5 ULONG bplsize,
                                    register __d6 ULONG chunkylen);


void __asm c2p1x1_6_c5_030_init(register __d0 ULONG chunky_x,
                                register __d1 ULONG chunky_y,
                                register __d2 ULONG offset_x,
                                register __d3 ULONG offset_y,
                                register __d4 ULONG rowlen,
                                register __d5 ULONG bplsize,
                                register __d6 ULONG chunkylen);


void __asm c2p1x1_6_c5_030(register __a0 UBYTE *chunky_buffer,
						   register __a1 UBYTE *bitplanes);


/*
void __asm c2p1x1_6_c3_sc_gen_init(register __d0 ULONG chunky_x,
                                register __d1 ULONG chunky_y,
                                register __d2 ULONG offset_x,
                                register __d3 ULONG offset_y,
                                register __d4 ULONG rowlen,
                                register __d5 ULONG bplsize);


void __asm c2p1x1_6_c3_sc_gen(register __a0 UBYTE *chunky_buffer,
						   register __a1 UBYTE *bitplanes);

*/
#endif

#endif

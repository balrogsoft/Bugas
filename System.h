#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <exec/execbase.h>
#include <exec/memory.h>  
#include <hardware/cia.h>
#include <hardware/custom.h>
#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <graphics/gfxbase.h>
#include <graphics/gfxmacros.h>
#include <graphics/view.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>

#include <devices/timer.h>
#include <devices/input.h>

#include <proto/timer.h>

#include <clib/exec_protos.h>

#define GETLONG(var) (*(volatile ULONG*)&var)

#define BPL0 (26 * 2) + 1
#define BPL1 (28 * 2) + 1
#define BPL2 (30 * 2) + 1
#define BPL3 (32 * 2) + 1
#define BPL4 (34 * 2) + 1
#define BPL5 (36 * 2) + 1


#define SPR1 (10 * 2) + 1


#define BITMAPLINEBYTES 40
#define BITMAPLINEBYTESI 40
#define BITMAPLINEBYTESB 38

#define CWIDTH 320 
#define CHEIGHT 180
#define CWIDTH2 160 
#define CHEIGHT2 90

#define WIDTH 320  
#define HEIGHT 180

UWORD *cop1, *cop2;
UWORD GetVBR[] = {0x4e7a, 0x0801, 0x4e73}; // MOVEC.L VBR,D0 RTE

extern struct ExecBase *SysBase;
struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;
struct copinit *oldCopinit;
struct ViewPort *oldViewPort;
struct CIA *cia = (struct CIA *) 0xBFE001;

struct Custom *custom = (struct Custom *)0xdff000;


struct MsgPort *inputMP;
struct IOStdReq *inputReq;
struct Interrupt inputHandler;
BYTE inputDevice = -1;

#ifdef IEEESP
struct Library *MathIeeeSingBasBase;
struct Library *MathIeeeSingTransBase;
#else
struct Library *MathBase;
struct Library *MathTransBase;
#endif

UBYTE NameString[]="InputHandlerBugas";

static ULONG vbcounter = 0;
static ULONG vbinc = 0;
static ULONG cvb = 0;
static ULONG cvbinc = 0;
struct Interrupt *vbint = NULL;

UWORD oldInt, oldDMA;
struct View *oldview;

ULONG vposr;
void *vbr;
UBYTE palflag;

ULONG clist[2];

UBYTE *bplptr[2];

ULONG clearColor = 0;

BYTE dbcount = 0;

#define waitVBlank() \
  do { \
    vposr = GETLONG(custom->vposr); \
  } while ((vposr & 0x1FF00) >= 0x12F00); \
  do { \
    vposr = GETLONG(custom->vposr); \
  } while ((vposr & 0x1FF00) < 0x12F00); \


#define WaitBlitter() while (GETLONG(custom->dmaconr) & DMAF_BLTDONE){}


static LONG __interrupt __saveds VBServer(void) {
    vbcounter++;
    vbinc++;
    return 0;
}

static LONG __interrupt __saveds NullInputHandler(void)
{
    return 0;
}

int installVBInterrupt(void)
{
   if(vbint = AllocMem(sizeof(struct Interrupt), MEMF_PUBLIC|MEMF_CLEAR)) {
      vbint->is_Node.ln_Type = NT_INTERRUPT;
      vbint->is_Node.ln_Pri = -60;
      vbint->is_Node.ln_Name = "VB-Bugas";
      vbint->is_Data = (APTR)&vbcounter;
      vbint->is_Code = (VOID (*)())VBServer;
      AddIntServer(INTB_VERTB, vbint);
   }
   else {
      printf("Not enough memory.\n");
      return 0;
   }
   return 1;
}

void removeVBInterrupt(void)
{
   RemIntServer(INTB_VERTB, vbint);
   FreeMem(vbint, sizeof(struct Interrupt));
}
void installInputHandler(void) {
    if ((inputMP = CreateMsgPort()))
    {
        if ((inputReq = CreateIORequest(inputMP, sizeof(*inputReq))))
        {
            inputDevice = OpenDevice("input.device", 0, (struct IORequest *)inputReq, 0);
            if (inputDevice == 0)
            {
                inputHandler.is_Node.ln_Type = NT_INTERRUPT;
                inputHandler.is_Node.ln_Pri = 127;
                inputHandler.is_Data = 0;
                inputHandler.is_Code = (APTR)NullInputHandler;
                inputHandler.is_Node.ln_Name=NameString;

                inputReq->io_Command = IND_ADDHANDLER;
                inputReq->io_Data = &inputHandler;

                DoIO((struct IORequest *)inputReq);
            }
        }
    }
}

void removeInputHandler(void) {
    if (inputDevice == 0)
    {
        inputReq->io_Data = &inputHandler;
        inputReq->io_Command = IND_REMHANDLER;
        DoIO((struct IORequest *)inputReq);
        CloseDevice((struct IORequest *)inputReq);
    }

    if (inputReq) 
        DeleteIORequest(inputReq);
    if (inputMP) 
        DeleteMsgPort(inputMP);
}

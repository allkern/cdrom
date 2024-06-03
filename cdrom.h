#ifndef CDROM_H
#define CDROM_H

#include <stdint.h>

#include "queue.h"
#include "disc.h"

/*
    Pause (single speed)   021181ch  020eaefh..0216e3ch ;\time equal to
    Pause (double speed)   010bd93h  010477Ah..011B302h ;/about 5 sectors
    Pause (when paused)    0001df2h  0001d25h..0001f22h
    Stop (single speed)    0d38acah  0c3bc41h..0da554dh
    Stop (double speed)    18a6076h  184476bh..192b306h
    Stop (when stopped)    0001d7bh  0001ce8h..0001eefh
*/

#define CD_DELAY_1MS 33869
#define CD_DELAY_PAUSE_SS 2168860
#define CD_DELAY_PAUSE_DS 1097107
#define CD_DELAY_STOP_SS 13863626
#define CD_DELAY_STOP_DS 25845878

/*
    7   Speed       (0=Normal speed, 1=Double speed)
    6   XA-ADPCM    (0=Off, 1=Send XA-ADPCM sectors to SPU Audio Input)
    5   Sector Size (0=800h=DataOnly, 1=924h=WholeSectorExceptSyncBytes)
    4   Ignore Bit  (0=Normal, 1=Ignore Sector Size and Setloc position)
    3   XA-Filter   (0=Off, 1=Process only XA-ADPCM sectors that match Setfilter)
    2   Report      (0=Off, 1=Enable Report-Interrupts for Audio Play)
    1   AutoPause   (0=Off, 1=Auto Pause upon End of Track) ;for Audio Play
    0   CDDA        (0=Off, 1=Allow to Read CD-DA Sectors; ignore missing EDC)
*/

#define MODE_CDDA           0x01
#define MODE_AUTOPAUSE      0x02
#define MODE_REPORT         0x04
#define MODE_XA_FILTER      0x08
#define MODE_IGNORE         0x10
#define MODE_SECTOR_SIZE    0x20
#define MODE_XA_ADPCM       0x40
#define MODE_SPEED          0x80

/*
    Command          Parameters      Response(s)
    00h -            -               INT5(11h,40h)  ;reportedly "Sync" uh?
    01h Getstat      -               INT3(stat)
    02h Setloc     E amm,ass,asect   INT3(stat)
    03h Play       E (track)         INT3(stat), optional INT1(report bytes)
    04h Forward    E -               INT3(stat), optional INT1(report bytes)
    05h Backward   E -               INT3(stat), optional INT1(report bytes)
    06h ReadN      E -               INT3(stat), INT1(stat), datablock
    07h MotorOn    E -               INT3(stat), INT2(stat)
    08h Stop       E -               INT3(stat), INT2(stat)
    09h Pause      E -               INT3(stat), INT2(stat)
    0Ah Init         -               INT3(late-stat), INT2(stat)
    0Bh Mute       E -               INT3(stat)
    0Ch Demute     E -               INT3(stat)
    0Dh Setfilter  E file,channel    INT3(stat)
    0Eh Setmode      mode            INT3(stat)
    0Fh Getparam     -               INT3(stat,mode,null,file,channel)
    10h GetlocL    E -               INT3(amm,ass,asect,mode,file,channel,sm,ci)
    11h GetlocP    E -               INT3(track,index,mm,ss,sect,amm,ass,asect)
    12h SetSession E session         INT3(stat), INT2(stat)
    13h GetTN      E -               INT3(stat,first,last)  ;BCD
    14h GetTD      E track (BCD)     INT3(stat,mm,ss)       ;BCD
    15h SeekL      E -               INT3(stat), INT2(stat)  ;\use prior Setloc
    16h SeekP      E -               INT3(stat), INT2(stat)  ;/to set target
    17h -            -               INT5(11h,40h)  ;reportedly "SetClock" uh?
    18h -            -               INT5(11h,40h)  ;reportedly "GetClock" uh?
    19h Test         sub_function    depends on sub_function (see below)
    1Ah GetID      E -               INT3(stat), INT2/5(stat,flg,typ,atip,"SCEx")
    1Bh ReadS      E?-               INT3(stat), INT1(stat), datablock
    1Ch Reset        -               INT3(stat), Delay
    1Dh GetQ       E adr,point       INT3(stat), INT2(10bytesSubQ,peak_lo) ;\not
    1Eh ReadTOC      -               INT3(late-stat), INT2(stat)           ;/vC0
    1Fh VideoCD      sub,a,b,c,d,e   INT3(stat,a,b,c,d,e)   ;<-- SCPH-5903 only
    1Fh..4Fh -       -               INT5(11h,40h)  ;-Unused/invalid
    50h Secret 1     -               INT5(11h,40h)  ;\
    51h Secret 2     "Licensed by"   INT5(11h,40h)  ;
    52h Secret 3     "Sony"          INT5(11h,40h)  ; Secret Unlock Commands
    53h Secret 4     "Computer"      INT5(11h,40h)  ; (not in version vC0, and,
    54h Secret 5     "Entertainment" INT5(11h,40h)  ; nonfunctional in japan)
    55h Secret 6     "<region>"      INT5(11h,40h)  ;
    56h Secret 7     -               INT5(11h,40h)  ;/
    57h SecretLock   -               INT5(11h,40h)  ;-Secret Lock Command
    58h..5Fh Crash   -               Crashes the HC05 (jumps into a data area)
    6Fh..FFh -       -               INT5(11h,40h)  ;-Unused/invalid
*/

#define CDL_GETSTAT    0x01
#define CDL_SETLOC     0x02
#define CDL_PLAY       0x03
#define CDL_FORWARD    0x04
#define CDL_BACKWARD   0x05
#define CDL_READN      0x06
#define CDL_MOTORON    0x07
#define CDL_STOP       0x08
#define CDL_PAUSE      0x09
#define CDL_INIT       0x0a
#define CDL_MUTE       0x0b
#define CDL_DEMUTE     0x0c
#define CDL_SETFILTER  0x0d
#define CDL_SETMODE    0x0e
#define CDL_GETPARAM   0x0f
#define CDL_GETLOCL    0x10
#define CDL_GETLOCP    0x11
#define CDL_SETSESSION 0x12
#define CDL_GETTN      0x13
#define CDL_GETTD      0x14
#define CDL_SEEKL      0x15
#define CDL_SEEKP      0x16
#define CDL_TEST       0x19
#define CDL_GETID      0x1a
#define CDL_READS      0x1b
#define CDL_RESET      0x1c
#define CDL_GETQ       0x1d
#define CDL_READTOC    0x1e
#define CDL_VIDEOCD    0x1f

/*
    ___These values appear in the FIRST response; with stat.bit0 set___
    10h - Invalid Sub_function (for command 19h), or invalid parameter value
    20h - Wrong number of parameters (most CD commands need an exact number of parameters)
    40h - Invalid command
    80h - Cannot respond yet (eg. required info was not yet read from disk yet)
            (namely, TOC not-yet-read or so)
            (also appears if no disk inserted at all)
    ___These values appear in the SECOND response; with stat.bit2 set___
    04h - Seek failed (when trying to use SeekL on Audio CDs)
    ___These values appear even if no command was sent; with stat.bit2 set___
    08h - Drive door became opened
*/

#define CD_ERR_SEEK_FAILED           0x04
#define CD_ERR_DOOR_OPENED           0x08
#define CD_ERR_INVALID_SUBFUNCTION   0x10
#define CD_ERR_WRONG_PARAMETER_COUNT 0x20
#define CD_ERR_INVALID_COMMAND       0x40
#define CD_ERR_NO_DISC               0x80

/*
    7  Play          Playing CD-DA         ;\only ONE of these bits can be set
    6  Seek          Seeking               ; at a time (ie. Read/Play won't get
    5  Read          Reading data sectors  ;/set until after Seek completion)
    4  ShellOpen     Once shell open (0=Closed, 1=Is/was Open)
    3  IdError       (0=Okay, 1=GetID denied) (also set when Setmode.Bit4=1)
    2  SeekError     (0=Okay, 1=Seek error)     (followed by Error Byte)
    1  Spindle Motor (0=Motor off, or in spin-up phase, 1=Motor on)
    0  Error         Invalid Command/parameters (followed by Error Byte)
*/

#define CD_STAT_PLAY        0x80
#define CD_STAT_SEEK        0x40
#define CD_STAT_READ        0x20
#define CD_STAT_SHELLOPEN   0x10
#define CD_STAT_IDERROR     0x08
#define CD_STAT_SEEKERROR   0x04
#define CD_STAT_SPINDLE     0x02
#define CD_STAT_ERROR       0x01

// #define SET_INT(n) cdrom->ifr = n;

enum {
    CD_STATE_IDLE,
    CD_STATE_TX_RESP1,
    CD_STATE_TX_RESP2,
    CD_STATE_READ,
    CD_STATE_PLAY
};

enum {
    QUERY_TRACK_COUNT,
    QUERY_TRACK_ADDR,
    QUERY_TRACK_TYPE
};

typedef void (*read_sector_func)(void*, uint32_t, void*);
typedef int (*query_sector_func)(void*, uint32_t);
typedef int (*get_track_count_func)(void*);
typedef uint32_t (*get_track_lba_func)(void*, int);

typedef struct {
    void* disc_udata;
    read_sector_func disc_read_sector;
    query_sector_func disc_query_sector;
    get_track_count_func disc_get_track_count;
    get_track_lba_func disc_get_track_lba;

    int index;
    uint8_t ier;
    uint8_t ifr;
    uint8_t vol[4];
    uint8_t mode;
    int data_req;
    queue_t* data;
    queue_t* response;
    queue_t* parameters;
    uint8_t pending_command;
    int busy;
    int xa_playing;
    int state;
    int int1_pending;
    int int2_pending;
    int delay;
    uint32_t lba;
    uint32_t cdda_lba;
    uint32_t xa_lba;
} psx_cdrom_t;

typedef void (*cdrom_write_func)(psx_cdrom_t*, uint8_t);
typedef uint8_t (*cdrom_read_func)(psx_cdrom_t*);

psx_cdrom_t* psx_cdrom_create(void);
void psx_cdrom_init(psx_cdrom_t* cdrom);
uint32_t psx_cdrom_read32(psx_cdrom_t* cdrom, uint32_t addr);
uint32_t psx_cdrom_read16(psx_cdrom_t* cdrom, uint32_t addr);
uint32_t psx_cdrom_read8(psx_cdrom_t* cdrom, uint32_t addr);
void psx_cdrom_write32(psx_cdrom_t* cdrom, uint32_t addr, uint32_t value);
void psx_cdrom_write16(psx_cdrom_t* cdrom, uint32_t addr, uint32_t value);
void psx_cdrom_write8(psx_cdrom_t* cdrom, uint32_t addr, uint32_t value);
void psx_cdrom_update(psx_cdrom_t* cdrom, int cycles);
void psx_cdrom_destroy(psx_cdrom_t* cdrom);

#endif
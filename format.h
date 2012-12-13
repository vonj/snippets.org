/*
** Data structures and discriptions for IOCTL comes from the following
** MS-DOS Functions ISBN 1-55615-128-4
** Ralf Brown's Interupt List
*/

#ifndef FORMAT__H
#define FORMAT__H

#include "extkword.h"          /* For FAR  */

#ifdef  _MSC_VER
 #pragma warning(disable:4103)
#endif

#define MAX_SECTORS 18

#pragma pack(1)

#define IOCTL               0x4400
#define IOCTL_REMOVABLE     0x08
#define IOCTL_REMOTE        0x09
#define IOCTL_BLOCK         0x0D
#define SetDeviceParameters 0x40
#define WriteTrack          0x41
#define FormatVerifyTrack   0x42
#define SetAccessFlag       0x47        /* dos 4.0+ */
#define GetDeviceParameters 0x60
#define ReadTrack           0x61
#define VerifyTrack         0x62
#define GetAccessFlag       0x67        /* dos 4.0+ */
#define DiskDrive           0x0800

#define CHECK_REMOVABLE (IOCTL + IOCTL_REMOVABLE)
#define CHECK_REMOTE    (IOCTL + IOCTL_REMOTE)
#define GENERIC_IO      (IOCTL + IOCTL_BLOCK)
#define GENERIC_GETDEV  (DiskDrive + GetDeviceParameters)
#define GENERIC_SETDEV  (DiskDrive + SetDeviceParameters)
#define GENERIC_READ    (DiskDrive + ReadTrack)
#define GENERIC_WRITE   (DiskDrive + WriteTrack)
#define GENERIC_VERIFY  (DiskDrive + VerifyTrack)
#define GENERIC_FORMAT  (DiskDrive + FormatVerifyTrack)
#define GENERIC_GETACC  (DiskDrive + GetAccessFlag)
#define GENERIC_SETACC  (DiskDrive + SetAccessFlag)

#define SERIAL_NUMBER       0x6900
#define GetDate             0x2A00
#define GetTime             0x2C00
#define SERIAL_READ             0
#define SERIAL_WRITE            1

typedef struct
    {
    char            Formats_[6];
    unsigned char   Device_Type;
    unsigned char   Media_Type;
    unsigned int    Heads;
    unsigned int    Tracks;
    unsigned int    Sectors_Per_Tracks;
    unsigned int    Max_Entries;        /* in sectors */
    unsigned char   Media_Descriptor;
    unsigned char   Cluster_Size;
    unsigned int    FatSize;
    unsigned int    Available;
    } FORMAT_TABLE;

/*
**  Bios Parameter Block:
**      Function 44
**      Sub-function 0D
**      minor code 40 and 60
*/

typedef struct
    {
    unsigned int    Bytes_Sector;
    unsigned char   Cluster_Size;
    unsigned int    Reserved_Sectors;
    unsigned char   Number_FATS;
    unsigned int    Max_Root_Entries;
    unsigned int    Number_Sectors;
    unsigned char   Media_Descriptor;
    unsigned int    Sectors_FAT;
    unsigned int    Sectors_Track;
    unsigned int    Number_Heads;
    unsigned long   Hidden_Sectors;
    unsigned long   Large_Number_Sectors;   /* if Number_Sectors == 0 */
    unsigned char   Reserved[6];
    } BPB;

/*
**  Device Parameter Block:
**      Function 44
**      Sub-function 0D
**      minor codes 40 and 60
*/

typedef struct
    {
    unsigned char   Function;
    unsigned char   Device_Type;
    unsigned int    Device_Attribute;
    unsigned int    Tracks;
    unsigned char   Media_Type;
    BPB             bpb;
    struct TL
        {
        unsigned int    Number_Sectors;
        struct SECID
            {
            unsigned int    Sector_Number;
            unsigned int    Sector_Size;
            } track_layout[MAX_SECTORS];
        } track;
    } DPB;

/*
**  Format Parameter Block:
**      Function 44
**      Sub-function 0D,
**      minor code 42 and 62
*/

typedef struct
    {
    unsigned char   Function;
    unsigned int    Head;
    unsigned int    Track;
    } FPB;

/*
**  Volume Serial Parameter Block:
**      Function 44
**      Sub-function 0D,
**      minor code 46 and 66
*/

typedef struct
    {
    unsigned int    Function;           /* should be 0              */
    unsigned long   SerialNumber;       /* binary                   */
    unsigned char   VolumeLabel[11];    /* or "NO NAME   "          */
    unsigned char   FileSystemType[8];  /* "FAT12   " or "FAT16   " */
    } VPB;

/*
**  Access Parameter Block:
**      Function 44
**      Sub-function 0D
**      minor code 47 and 67
*/

typedef struct
    {
    unsigned char   Function;
    unsigned char   Flag;
    } APB;

/*
**  IO Parameter Block:
**      Function 44
**      Sub-function 0D
**      minor code 41, 61 and 62
*/

typedef struct
    {
    unsigned char       Function;
    unsigned int        Side;
    unsigned int        Track;
    unsigned int        FirstSector;
    unsigned int        NumberSectors;
    unsigned char FAR  *dta;
    } IOPB;


/* boot sector description */

#define BJLEN   3
#define OLEN    8
#define BREV    1
#define NLEN    11
#define FLEN    8
#define BLEN    512 - (FLEN + NLEN + BREV + OLEN + BJLEN + sizeof(BPB))

typedef struct
    {
    unsigned char   boot_start_[BJLEN];
    unsigned char   os_name_[OLEN];
    BPB             bpb;
    unsigned char   rev_[BREV];
    unsigned char   volname_[NLEN];
    unsigned char   fat_[FLEN];
    unsigned char   boot_code_[BLEN];
    } BOOTSECTOR;

#endif /* FORMAT__H */

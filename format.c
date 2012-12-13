/*
**  Writen by:  Wayne Halsdorf
**  Finished:   12-Jul-1994
**
**  This program is released to the PUBLIC DOMAIN.
**
**  A method for formatting floppy disks.
**
**  Testted with Microsoft C 7.0
*/

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#if defined(__TURBOC__)
 #include    <alloc.h>
#elif defined(__ZTC__)
 #if !defined(__SC__) || (__SC__ < 0x700)
  #error FORMAT.C >> _fcalloc() not supported bt ZTC or SC ver. 6.xx
 #endif
#else
 #include    <malloc.h>
#endif
#include    <dos.h>
#include    <direct.h>
#include    <time.h>
#include    <conio.h>
#include    <bios.h>
#include    <ctype.h>
#include    "format.h"


DPB        FAR        *dpb;
FPB        FAR        *fpb;
APB        FAR        *apb;
IOPB       FAR        *iopb;
BOOTSECTOR FAR        *BootSector;

FORMAT_TABLE    D360[] = {
      {"360K", 0, 1, 2, 40,  9, 7, 0xFD, 1, 3, 706},
      {"360K", 0, 1, 2, 40,  9, 7, 0xFD, 2, 2, 354},
      {"360K", 0, 1, 2, 40,  9, 5, 0xFD, 4, 1, 178}
};

FORMAT_TABLE    D1200[] = {
      {"360K", 0, 1, 2, 40,  9,  7, 0xFD, 1, 3,  706},
      {"360K", 0, 1, 2, 40,  9,  7, 0xFD, 2, 2,  354},
      {"360K", 0, 1, 2, 40,  9,  5, 0xFD, 4, 1,  178},
      {"1.2M", 1, 0, 2, 80, 15,  7, 0xF9, 1, 7, 2378},
      {"1.2M", 1, 0, 2, 80, 15,  7, 0xF9, 2, 4, 1192},
      {"1.2M", 1, 0, 2, 80, 15,  7, 0xF9, 4, 2,  597},
      {"1.2M", 1, 0, 2, 80, 15,  5, 0xF9, 8, 1,  299},
      {"1.2M", 1, 0, 2, 80, 15, 14, 0xF9, 1, 7, 2371},
      {"1.2M", 1, 0, 2, 80, 15, 15, 0xF9, 2, 4, 1188},
      {"1.2M", 1, 0, 2, 80, 15, 15, 0xF9, 4, 2,  595},
      {"1.2M", 1, 0, 2, 80, 15, 13, 0xF9, 8, 1,  298}
};

FORMAT_TABLE    D720[] = {
      { "720K", 2, 0, 2, 80,  9,  7, 0xF9, 1, 5, 1422},
      { "720K", 2, 0, 2, 80,  9,  7, 0xF9, 2, 3,  713},
      { "720K", 2, 0, 2, 80,  9,  7, 0xF9, 4, 2,  357},
      { "720K", 2, 0, 2, 80,  9,  5, 0xF9, 8, 1,  179}
};

FORMAT_TABLE    D1440[] = {
      { "720K", 2, 0, 2, 80,  9,  7, 0xF9, 1, 5, 1422},
      { "720K", 2, 0, 2, 80,  9,  7, 0xF9, 2, 3,  713},
      { "720K", 2, 0, 2, 80,  9,  7, 0xF9, 4, 2,  357},
      { "720K", 2, 0, 2, 80,  9,  5, 0xF9, 8, 1,  179},
      {"1.44M", 7, 0, 2, 80, 18,  7, 0xF0, 1, 9, 2854},
      {"1.44M", 7, 0, 2, 80, 18,  7, 0xF0, 2, 5, 1431},
      {"1.44M", 7, 0, 2, 80, 18,  5, 0xF0, 4, 3,  717},
      {"1.44M", 7, 0, 2, 80, 18,  3, 0xF0, 8, 2,  359},
      {"1.44M", 7, 0, 2, 80, 18, 14, 0xF0, 1, 9, 2847},
      {"1.44M", 7, 0, 2, 80, 18, 13, 0xF0, 2, 5, 1428},
      {"1.44M", 7, 0, 2, 80, 18, 13, 0xF0, 4, 3,  715},
      {"1.44M", 7, 0, 2, 80, 18, 11, 0xF0, 8, 2,  358}
};

unsigned int    GetKey(void)
{
      while (!_bios_keybrd(_KEYBRD_READY))
            ;
      return(_bios_keybrd(_KEYBRD_READ) & 0xff);
}

void    GetLine(char FAR *in, unsigned int len)
{
      unsigned int    k, l;
      char FAR *p;

      p = in;
      l = 0;
      *p = '\0';
      do
      {
            k = GetKey();
            if(k == 8 && l)
            {
                  l--;
                  p--;
                  *p = '\0';
                  printf("\b \b");
            }
            else  if(__iscsym(k) && l < len)
            {
                  k = (unsigned int)toupper(k);
                  *p = (char)k;
                  l++;
                  p++;
                  *p = '\0';
                  printf("%c", k);
            }
      } while (k != 0x0d);
}

void    InitVars(void)
{
      dpb = _fcalloc(1, sizeof(DPB));
      fpb = _fcalloc(1, sizeof(FPB));
      apb = _fcalloc(1, sizeof(APB));
      BootSector = _fcalloc(1, sizeof(BOOTSECTOR));
}

void    FreeVars(void)
{
      if(apb)
            _ffree(apb);
      if(dpb)
            _ffree(dpb);
      if(fpb)
            _ffree(fpb);
      if(iopb->dta)
            _ffree(iopb->dta);
      if(iopb)
            _ffree(iopb);
      if(BootSector)
            _ffree(BootSector);
}

void    Error_Message(char *message_)
{
      printf(message_);
      FreeVars();
      exit(1);
}

unsigned int    AvailableDrives(void)
{
      _outp(0x70, 16);
      return(_inp(0x71));
}

void    SelectDrive(unsigned int *dr)
{
      unsigned int    d, a;

      a = 0;
      d = AvailableDrives();
      if(d & 0x0f)            /* drive b */
            a = 2;
      if(d & 0xf0)            /* drive a */
            a |= 1;

      switch(a)
      {
      case 0:
            Error_Message("No floppies found\n");
            break;

      case 1:
            *dr = 1;
            break;

      case 2:
            *dr = 2;
            break;

      case 3:
            printf("Format which drive (A or B) : ");
            do
            {
                  d = GetKey();
                  d = (unsigned int)(toupper(d) - 'A');
            } while (d > 1);
            printf("%c\n", d + 'A');
            *dr = d + 1;
            break;
      }
}

void    GetDrive(char *c_, unsigned int *dr)
{
      *dr = (unsigned int)(toupper(*c_) - 'A' + 1);
      if(*dr < 1)
            Error_Message("Attempted to format unknown device\n");

      if(*dr > 2)
            Error_Message("Attempted to format non-floppy device\n");
}

void    GetFloppyTable(unsigned int drive_number,
                       unsigned int *s,
                       FORMAT_TABLE **ft)
{
      unsigned int    d;

      d = AvailableDrives();
      if(drive_number == 1)
            d = d >> 4;
      else  d = d & 15;

      switch(d)
      {
      case 1:
            *ft = D360;
            *s = sizeof(D360) / sizeof(FORMAT_TABLE);
            break;

      case 2:
            *ft = D1200;
            *s = sizeof(D1200) / sizeof(FORMAT_TABLE);
            break;

      case 3:
            *ft = D720;
            *s = sizeof(D720) / sizeof(FORMAT_TABLE);
            break;

      case 4:
            *ft = D1440;
            *s = sizeof(D1440) / sizeof(FORMAT_TABLE);
            break;

      default:
            Error_Message("Don't know about formats for select drive\n");
            break;
      }
}

void    DriveParameters(unsigned int *format,
                        FORMAT_TABLE *ftable,
                        unsigned int s)
{
      unsigned int    x;

      printf("Available formats for selected drive\n");
      printf("    Format Directory Cluster  Clusters  Storage\n");
      printf("     Type  Available  Size    Available Capacity\n");
      for (x = 0; x < s; x++)
      {
            printf(" %c) %6s    %3u     %4u      %4u   %9lu\n", x + 'A',
                   ftable[x].Formats_,
                   ftable[x].Max_Entries * 16,
                   ftable[x].Cluster_Size * 512,
                   ftable[x].Available,
                   512L * ftable[x].Available * ftable[x].Cluster_Size);
      }
      printf("Input which format: ");

      do
      {
            x = GetKey();
            x = (unsigned int)(toupper((int)x) - 'A');
      } while (x >= s);

      printf("%c\n", x + 'A');
      *format = x;
}

unsigned int DOS_IOCTL(unsigned drive, unsigned func, void _far *data)
{
      union   REGS   ir, or;
      struct  SREGS  sr;

      ir.x.ax = GENERIC_IO;
      ir.x.bx = drive;
      ir.x.cx = func;
      ir.x.dx = _FP_OFF(data);
      sr.ds = _FP_SEG(data);
      _int86x(0x21, &ir, &or, &sr);
      return(or.x.cflag);
}

unsigned int Extended_Error_Code(void)
{
      union   _REGS   ir, or;

      ir.h.ah = 0x59;
      ir.x.bx = 0;
      _int86(0x21, &ir, &or);
      return(or.h.bh);
}

unsigned int SerialNumber(unsigned drive, unsigned func, void _far *data)
{
      union   _REGS   ir, or;
      struct  _SREGS  sr;

      ir.x.ax = SERIAL_NUMBER + func;
      ir.x.bx = drive;
      ir.x.dx = _FP_OFF(data);
      sr.ds = _FP_SEG(data);
      _int86x(0x21, &ir, &or, &sr);
      if(or.x.cflag)
            return(or.x.ax);
      else  return(0);
}

unsigned long GenSerialNumber(void)
{
      union   _REGS   ir, or;
      union {
            unsigned long   d;
            unsigned int    i[2];
      } g;

      ir.x.ax = GetDate;
      _int86(0x21, &ir, &or);
      g.i[0] = or.x.cx;
      g.i[1] = or.x.dx;
      ir.x.ax = GetTime;
      _int86(0x21, &ir, &or);
      g.i[0] += or.x.cx;
      g.i[1] += or.x.dx;
      return(g.d);
}

unsigned int    FlopyStatus(unsigned int drive)
{
      union   _REGS   ir, or;

      ir.h.ah = 1;
      ir.x.dx = drive - 1;
      _int86(0x13, &ir, &or);
      return(or.h.al);
}

void    ResetDisk(unsigned int drive)
{
      union   _REGS   ir, or;

      ir.h.ah = 0;
      ir.x.dx = drive - 1;
      _int86(0x13, &ir, &or);
}

/*
** gain acces to drive (required if disk in drive is unformated)
*/

void    SetAccess(unsigned int drivenumber, APB FAR *apb)
{
      apb->Function = 0;
      DOS_IOCTL(drivenumber, GENERIC_GETACC, apb);
      if(apb->Flag == 0)
      {
            apb->Flag = 1;
            DOS_IOCTL(drivenumber, GENERIC_SETACC, apb);
      }
}

void    InitParameters(unsigned int drive_number,
                       unsigned int format,
                       FORMAT_TABLE *ftable)
{
      unsigned int    t;

      dpb->Function = 0;
      dpb->Device_Type = ftable[format].Device_Type;
      dpb->Tracks = ftable[format].Tracks;
      dpb->Media_Type = ftable[format].Media_Type;
      dpb->bpb.Bytes_Sector = 0x0200;
      dpb->bpb.Cluster_Size = ftable[format].Cluster_Size;
      dpb->bpb.Reserved_Sectors = 1;
      dpb->bpb.Number_FATS = 2;
      dpb->bpb.Max_Root_Entries = ftable[format].Max_Entries * 16;
      dpb->bpb.Number_Sectors   = ftable[format].Tracks *
                                  ftable[format].Heads *
                                  ftable[format].Sectors_Per_Tracks;
      dpb->bpb.Media_Descriptor = ftable[format].Media_Descriptor;
      dpb->bpb.Sectors_FAT = ftable[format].FatSize;
      dpb->bpb.Sectors_Track = ftable[format].Sectors_Per_Tracks;
      dpb->bpb.Number_Heads = ftable[format].Heads;
      dpb->bpb.Hidden_Sectors = 0;
      dpb->bpb.Large_Number_Sectors = 0;
      dpb->track.Number_Sectors = ftable[format].Sectors_Per_Tracks;
      for (t = 0; t < dpb->track.Number_Sectors; t++)
      {
            dpb->track.track_layout[t].Sector_Number = t + 1;
            dpb->track.track_layout[t].Sector_Size = dpb->bpb.Bytes_Sector;
      }

      iopb = _fcalloc(1, sizeof(IOPB));
      iopb->dta = _fcalloc(dpb->bpb.Sectors_Track * dpb->bpb.Bytes_Sector,
                           sizeof(char));
      DOS_IOCTL(drive_number, GENERIC_SETDEV, dpb);
      dpb->Function = 5;
}

#ifdef  _MSC_VER
#pragma warning(disable:4001)
#endif

/*
** format each track, both sides
*/

void    FormatDisk(unsigned int drive_number,
                   unsigned int ftype,
                   FORMAT_TABLE *ftable)
{
      unsigned int    retries, er;

      printf("Insert disk in drive %c: and press enter ",
             drive_number + 'A' - 1);

      while (GetKey() != 0x0d)
            ;

      printf("\nFormatting %s Data Disk, Please Wait.\n",
             ftable[ftype].Formats_);
      fpb->Function = 0;      /* format/Verify track */
      for (fpb->Track = 0; fpb->Track < dpb->Tracks; fpb->Track += 1)
      {
            for (fpb->Head = 0;
                 fpb->Head < ftable[ftype].Heads;
                 fpb->Head += 1)
            {
                  SetAccess(drive_number, apb);
                  DOS_IOCTL(drive_number, GENERIC_SETDEV, dpb);
                  printf("Formating Track %2u Side %u\r",
                         fpb->Track, fpb->Head);
                  retries = 0;
                  while (retries++ < 4 && DOS_IOCTL(drive_number,
                        GENERIC_FORMAT, fpb))
                  {
                        er = FlopyStatus(drive_number);
                        if((er & 0x80) == 0)
                        {
                              er = Extended_Error_Code();
                              if(er == 0x0d)
                                    Error_Message("Invlaid Media\n");

                              if(er == 0x0b)
                              {
                                    printf("Disk write proteced.\n"
                                           "Press enter after correcting "
                                           "or escape to abort\n");
                                    do
                                    {
                                          er = GetKey();
                                          if(er == 0x1b)
                                                Error_Message("Format aborted"
                                                      ". Disk now invalid.\n");
                                    } while (er != 0x0d);
                              }
                        }
                        ResetDisk(drive_number);
                        SetAccess(drive_number, apb);
                        DOS_IOCTL(drive_number, GENERIC_SETDEV, dpb);
                  }
            }
      }
}

void    InitializeDisk(unsigned int drive_number)
{
      struct tm *today;
      VPB     FAR   *vpb;
      time_t   timer;
      unsigned int    er;
      char    *er_;
      static char FAR iname[NLEN + 1];
      static unsigned char FAR boot_start[BJLEN] = {0xeb,0x3C,0x90};
      static unsigned char FAR os_[OLEN] = {"PDDF 1.0"};
      static unsigned char FAR fat_start[3] = {0xfd,0xff,0xff};
      static unsigned char FAR name[NLEN] = {"NO NAME    "};
      static unsigned char FAR fatname[FLEN] = {"FAT12   "};
      static unsigned char FAR boot_code[BLEN];
      static unsigned char boot_code_default[] = {
            0xfa,                   /* 7c3e cli                     */
            0xbc, 0x00, 0x7c,       /* 7c3f mov sp, 7c00h           */
            0xfb,                   /* 7c42 sti                     */
            0xb2, 0x00,             /* 7c43 mov dl,0                */
            0x33, 0xc0,             /* 7c45 xor ax,ax               */
            0xcd, 0x13,             /* 7c47 int 13h                 */
            0x0e,                   /* 7c49 push cs                 */
            0x1f,                   /* 7c4a pop  ds                 */
            0xfc,                   /* 7c4b cld                     */
            0xbe, 0x63, 0x7c,       /* 7c4c mov si, OFFSET message  */
            0xac,                   /* 7c4f lodsb                   */
            0x0a, 0xc0,             /* 7c50 or  al,al               */
            0x74, 0x09,             /* 7c52 je  7c5c                */
            0xb4, 0x0e,             /* 7c54 mov ah,0eh              */
            0xbb, 0x07, 0x00,       /* 7c56 mov bx,7                */
            0xcd, 0x10,             /* 7c59 int 10h                 */
            0xeb, 0xf2,             /* 7c5b jmp SHORT 7c50          */
            0x33, 0xc0,             /* 7c5d xor ax,ax               */
            0xcd, 0x16,             /* 7c5f int 16h                 */
            0xcd, 0x19              /* 7c61 int 19h                 */
                                    /* 7c63 db  'message'           */
	};
	static char boot_text_default[] = {
            "\r\nNon-System disk for DATA USE ONLY!"
            "\r\nProduced by a Public Domain Disk Formater 1.0 ."
            "\r\nSource code freely available."
            "\r\nReplace or remove disk then press any key when ready\r\n"
      };

      printf("\nInitializing Disk\n");
      _fmemcpy(&boot_code[0], boot_code_default, sizeof(boot_code_default));
      _fmemcpy(&boot_code[sizeof(boot_code_default)], boot_text_default,
               sizeof(boot_text_default));

      /*
      ** test to see if BOOT FATS and DIRECTORY records can be placed
      */

      iopb->Function = 0;
      iopb->Side = 0;
      iopb->Track = 0;
      iopb->FirstSector = 0;
      iopb->NumberSectors = dpb->bpb.Sectors_Track;
      er = DOS_IOCTL(drive_number, GENERIC_READ, iopb);
      _fmemset(iopb->dta, 0, dpb->bpb.Sectors_Track * dpb->bpb.Bytes_Sector);
      er = DOS_IOCTL(drive_number, GENERIC_WRITE, iopb);
      er = DOS_IOCTL(drive_number, GENERIC_VERIFY, iopb);
      if(er)
            Error_Message("Track 0 side 0 defective\n");

      iopb->Function = 0;
      iopb->Side = 1;
      iopb->Track = 0;
      iopb->FirstSector = 0;
      iopb->NumberSectors = dpb->bpb.Sectors_Track;
      er = DOS_IOCTL(drive_number, GENERIC_WRITE, iopb);
      er = DOS_IOCTL(drive_number, GENERIC_VERIFY, iopb);
      if(er)
            Error_Message("Track 0 side 1 defective\n");

      /*
      ** place BOOT Record
      */

      iopb->Function = 0;
      iopb->Side = 0;
      iopb->Track = 0;
      iopb->FirstSector = 0;
      iopb->NumberSectors = 1;
      _fmemcpy(BootSector->boot_start_, boot_start, BJLEN);
      _fmemcpy(BootSector->os_name_, os_, OLEN);
      _fmemcpy(&(BootSector->bpb), &(dpb->bpb), sizeof(BPB) + 1);
      BootSector->bpb.Reserved[2] = 0x29;       /* needed for serial number */
      _fmemcpy(BootSector->volname_, name, NLEN);
      _fmemcpy(BootSector->fat_, fatname, FLEN);
      _fmemcpy(BootSector->boot_code_, boot_code, BLEN);
      _fmemcpy(iopb->dta, BootSector, sizeof(BOOTSECTOR));
      er = DOS_IOCTL(drive_number, GENERIC_WRITE, iopb);
      er = DOS_IOCTL(drive_number, GENERIC_VERIFY, iopb);
      if(er)
            Error_Message("Bad Boot Sector\n");

      /*
      ** place first FAT
      */

      _fmemset(iopb->dta, 0, dpb->bpb.Sectors_FAT * dpb->bpb.Bytes_Sector);
      _fmemcpy(iopb->dta, fat_start, sizeof(fat_start));
      iopb->Function = 0;
      iopb->Side = 0;
      iopb->Track = 0;
      iopb->FirstSector = 1;
      iopb->NumberSectors = dpb->bpb.Sectors_FAT;
      er = DOS_IOCTL(drive_number, GENERIC_WRITE, iopb);
      er = DOS_IOCTL(drive_number, GENERIC_VERIFY, iopb);
      if(er)
            Error_Message("Bad File Allocation Table\n");

      /*
      ** place second FAT
      */

      iopb->Function = 0;
      iopb->Side = 0;
      iopb->Track = 0;
      iopb->FirstSector = 1 + dpb->bpb.Sectors_FAT;
      iopb->NumberSectors = dpb->bpb.Sectors_FAT;
      er = DOS_IOCTL(drive_number, GENERIC_WRITE, iopb);
      er = DOS_IOCTL(drive_number, GENERIC_VERIFY, iopb);
      if(er)
            Error_Message("Bad File Allocation Table\n");

      /*
      ** place extend information on BOOT record
      */

      vpb = _fcalloc(1, sizeof(VPB));
      SerialNumber(drive_number, SERIAL_READ, vpb);
      today = localtime( &timer );
      vpb->SerialNumber = GenSerialNumber();
      printf("Internal Volume Name: ");
      GetLine(iname, NLEN);
      if(_fstrlen(iname))
      {
            _fmemset(vpb->VolumeLabel, ' ', NLEN);
            _fmemcpy(vpb->VolumeLabel, iname, _fstrlen(iname));
      }
      er = SerialNumber(drive_number, SERIAL_WRITE, vpb);
      _ffree(vpb);
      if(er)
      {
            er = Extended_Error_Code();
            switch(er)
            {
            case 0x15:
                  er_ = "Drive not Ready. Can't update Boot.\n";
                  break;

            case 0x1F:
                  er_ = "General Failure. Can't update Boot\n";
                  break;

            default:
                  er_ = "Can't update Boot\n";
                  break;
            }
            Error_Message(er_);
      }
}

void    CheckDrive(unsigned int drive_number)
{
      union   _REGS   ir, or;
      struct  _SREGS  sr;

      ir.x.ax = CHECK_REMOVABLE;
      ir.x.bx = drive_number;
      _int86x(0x21, &ir, &or, &sr);
      if(or.x.cflag)
            Error_Message("Can't determine if media is removable.\n");

      if(or.x.ax)
            Error_Message("Selected media not removable, "
                          "my not be a floppy.\n");

      ir.x.ax = CHECK_REMOTE;
      _int86x(0x21, &ir, &or, &sr);
      if(or.x.cflag)
            Error_Message("Can't determine if media is remote or shared.\n");

      if((or.x.dx & 0x8000) != 0)
            Error_Message("Can't format SUBSTITUTE drive.\n");

      if((or.x.dx & 0x1000) != 0)
            Error_Message("Can't format REMOTE drive.\n");
}

/*
** get real dos version
*/

void    CheckDosVersion(void)
{
      if(_osmajor < 4)
            Error_Message("Must be DOS 4.0 or higher.\n");
}

int     main(int argc, char *argv[])
{
      unsigned int    format, drive_number, size;
      FORMAT_TABLE    *ftable;

      CheckDosVersion();
      InitVars();

      if(argc == 1)
            SelectDrive(&drive_number);
      else  GetDrive(argv[1], &drive_number);

      CheckDrive(drive_number);
      GetFloppyTable(drive_number, &size, &ftable);
      DriveParameters(&format, ftable, size);
      InitParameters(drive_number, format, ftable);
      FormatDisk(drive_number, format, ftable);
      InitializeDisk(drive_number);
      FreeVars();
      return(0);
}

/*  FILE: vt_serial_io.c --                                     */
/*                                                              */
/*  Low level serial port communication routines                */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright(C) 1990,91,92,93 Virtual Technologies     --  */
/*  --  Copyright(C) 1990,91,92 Larry Edwards               --  */
/*  --                                                      --  */
/*  --    Authors: William Chapin and Larry Edwards         --  */
/*  ==========================================================  */

#include <gadget/gadgetConfig.h>

#ifdef HAVE_SYS_STDSYMS_H
#   include <sys/stdsyms.h>
#endif

#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#ifdef HAVE_SYS_Z8530_H
#   include <sys/z8530.h>
#else
    // These are taken from sys/z8530.h on IRIX.
#   define LDISC1   1
#   define SIOC     ('z' << 8)
#   define SIOC_EXTCLK  (SIOC | 1)
#   define SIOC_RS422   (SIOC | 2)
#   define EXTCLK_OFF   0xff
#   define RS422_OFF    0x0
#endif

#include <termios.h>
#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>

#include <gadget/Devices/VirtualTechnologies/CyberGlove.h>
#include <gadget/Devices/VirtualTechnologies/vt_globals.h>
#include <gadget/Devices/VirtualTechnologies/vt_error.h>

/* UNIX allows a max of 20 file descriptors ... really should use */
/* getdtablesize() to determine this but 20 should be fine.       */
#define MAXNUMFILEDESCR 20

#define MAX_SERIAL_PORT   8

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*  Global variables local to this module                      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


static struct termios termiotemplate = { 0, 0, CS8|CREAD|CLOCAL, 0, LDISC1, 0 };
static struct termios old_termio[MAXNUMFILEDESCR];

static char open_port_names[MAXNUMFILEDESCR][15];
static int num_open_ports = 0;
static char failed_routine_name[256];


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*  routines to correct opoosite endian problems               */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  SWAP_BYTES_WORD --                                          */
/*                                                              */
/*  Swap bytes in a 32 bit word. Used to correct for endian     */
/*  differences.                                                */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static void
swap_bytes_word(void *word)
{
  unsigned long swappedword, temp = *((unsigned long *) word);

  /* NOTE: on the IRIS the way we do it here is actually faster than */
  /* swapping individual bytes as we do for half words */

  swappedword  =  (temp << 24);
  swappedword |= ((temp <<  8) & 0x00FF0000);
  swappedword |= ((temp >>  8) & 0x0000FF00);
  swappedword |= ((temp >> 24) & 0x000000FF);

  *((unsigned long *) word) = swappedword;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  SWAP_BYTES_HALF_WORD --                                     */
/*                                                              */
/*  Swap bytes in a 16 bit half word. Used to correct for       */
/*  endian differences.                                         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

static void
swap_bytes_half_word(void *halfword)
{
  unsigned char temp, *byte = (unsigned char*)halfword;

  temp = byte[0];
  byte[0] = byte[1];
  byte[1] = temp;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*  serial port initialization and termination routines        */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


static int
baud_from_int(int baudrate)
{

  switch(baudrate)
  {
  case 300:
    return B300;
  case 600:
    return B600;
  case 1200:
    return B1200;
  case 2400:
    return B2400;
  case 4800:
    return B4800;
  case 9600:
    return B9600;
  case 19200:
    return B19200;
  case 38400:
    return B38400;
  default:
    sprintf(failed_routine_name,"baud_from_int(%d)", baudrate);
    vt_set_error(failed_routine_name,IO_ERROR1);
    vt_fatal_error("");
  }
  return 0; // never happens.
}

int
CyberGloveBasic::vt_serial_open(char *devname, int baudrate)
{
  struct termios temp_termio;
  int portfd;
  //Boolean first_open = FALSE;

  if (num_open_ports+1 > MAX_SERIAL_PORT)
  {
    vt_set_error("vt_serial_open",IO_ERROR2);
    vt_fatal_error("");
  }
  /* make sure we're not already using this serial port */
  for (portfd = 0; portfd < MAXNUMFILEDESCR; portfd++)
    if (strcmp(devname,open_port_names[portfd]) == 0)
    {
      sprintf(failed_routine_name,"vt_serial_open(%s,%d)",devname,baudrate);
      vt_set_error(failed_routine_name,IO_ERROR3);
      vt_fatal_error("");
    }

  if ((portfd = ::open(devname, O_RDWR)) == -1)
  {
    sprintf(failed_routine_name,"open(\"%s\",O_RDWR)",devname);
    vt_fatal_unix_error("vt_serial_open",failed_routine_name);
  }
  /* The 8-pin DIN ports can be set to RS232 or RS422 in software, we want */
  /* RS232 so we explicitly turn off RS422 here. */
  ioctl(portfd,SIOC_RS422,RS422_OFF);
  ioctl(portfd,SIOC_EXTCLK,EXTCLK_OFF);

//  if (first_open)
//    atexit((void (*)())vt_serial_close_ports); /* make sure we close allports on exit */

  num_open_ports++;
  strcpy(open_port_names[portfd],devname);

  /* finish setting up new tty state */
  /* The following state sets a read timeout of 0.10 secs */

  temp_termio = termiotemplate;
  cfsetospeed(&temp_termio, baud_from_int(baudrate));
  cfsetispeed(&temp_termio, baud_from_int(baudrate));
  temp_termio.c_cc[VMIN] = 0;   /* incoming char buffer size */
  temp_termio.c_cc[VTIME] = 1;  /* timeout (in tenths of a second) */

  /* save old tty state */
  if (tcgetattr(portfd, &old_termio[portfd]) == -1)
  {
    sprintf(failed_routine_name, "tcgetattr(%d, %#x)", portfd,
            &old_termio[portfd]);
    vt_fatal_unix_error("vt_serial_open", failed_routine_name);
  }

  /* enact new tty state: */
  if (tcsetattr(portfd, TCSANOW, &temp_termio) == -1)
  {
    sprintf(failed_routine_name, "tcsetattr(%d, TCSANOW, %#x)", portfd,
            &temp_termio);
    vt_fatal_unix_error("vt_serial_open", failed_routine_name);
  }

  return (portfd);
}

int
CyberGloveBasic::vt_serial_close(int portfd)
{
  int returnval = Ok;

  num_open_ports--;
  strcpy(open_port_names[portfd],"");

  if (tcsetattr(portfd, TCSANOW, &old_termio[portfd]) == -1)
  {
    sprintf(failed_routine_name, "tcsetattr(%d, TCSANOW, %#x)", portfd,
            &old_termio[portfd]);
    returnval = vt_print_unix_error("vt_serial_close", failed_routine_name);
  }

  if (::close(portfd) == -1)
  {
    sprintf(failed_routine_name,"close(%d)",portfd);
    returnval = vt_print_unix_error("vt_serial_close",failed_routine_name);
  }

  return (returnval);
}


void
CyberGloveBasic::vt_serial_close_ports(void)
{
  int i;

  for (i=0; i < MAXNUMFILEDESCR; i++)
    if (strlen(open_port_names[i]) != 0)
      if (vt_serial_close(i) < Ok)
    vt_print_error("vt_serial_close_ports");
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*  routines to clear or flush a serial port input buffer      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


int
CyberGloveBasic::vt_serial_flush_in(int portfd)
{
  /* flush the input buffer immediately without waiting for more input */
  if (tcflush(portfd, TCIFLUSH) == -1)
  {
    sprintf(failed_routine_name, "tcflush(%d, TCIFLUSH)", portfd);
    return (vt_print_unix_error("vt_serial_flush_in", failed_routine_name));
  }

  return (Ok);
}


int
CyberGloveBasic::vt_serial_clear_to_terminator(int portfd, int terminator)
{
  int returnval;
  char buffer[1];

  /* We stop when we hit a terminator or we timeout or there is a read error */
  while (((returnval = read(portfd,buffer,1)) > 0) && (buffer[0] != terminator));

  if (returnval == -1)
    return (vt_print_unix_error("vt_serial_read_string","read"));

  return (Ok);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*  routines to read and write from and to a port              */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


static jmp_buf env;

int
timed_read(int filedescr, void *buffer, unsigned int numbytes)
{
  static struct itimerval value = {{0,0},{0,0}};
  static int tot_bytes_read;
  int num_bytes_read;

  tot_bytes_read = 0;

  value.it_value.tv_usec = 100000*numbytes; /* how long timer will run */
  setitimer(ITIMER_REAL,&value,NULL); /* start timer ticking */

  if (setjmp(env) == 0)
  {
    value.it_value.tv_usec = 0; /* value to turn off timer */
    do
    {
      if ((num_bytes_read = read(filedescr,&((char *) buffer)[tot_bytes_read],
                 numbytes-tot_bytes_read)) == -1)
      {
    setitimer(ITIMER_REAL,&value,NULL); /* turn off timer */
    return (-1);
      }
      tot_bytes_read += num_bytes_read;
    } while (tot_bytes_read < numbytes);
    setitimer(ITIMER_REAL,&value,NULL); /* turn off timer */
    return (tot_bytes_read);
  }

  return (tot_bytes_read);
}

int
CyberGloveBasic::vt_serial_read_byte(int portfd)
{
  unsigned char charbuf;
  int num_bytes_read;

  if ((num_bytes_read = read(portfd,&charbuf,1)) == -1)
    return (vt_print_unix_error("vt_serial_read_byte","read"));
  else if (num_bytes_read != 1)
    return (vt_set_error("vt_serial_read_byte",IO_ERROR5));
  else
    return ((int) charbuf);
}

int
CyberGloveBasic::vt_serial_read_ushort(int portfd)
{
  unsigned short *ushortbuf;
  char buffer[2];
  int num_bytes_read;
#ifndef IS_LITTLE_ENDIAN
  enum {BYTE1,BYTE0};
#else
  enum {BYTE0,BYTE1};
#endif
  if ((num_bytes_read = read(portfd,&buffer[BYTE0],1)) == -1)
    return (vt_print_unix_error("vt_serial_read_ushort","read"));

  if ((num_bytes_read += read(portfd,&buffer[BYTE1],1)) == -1)
    return (vt_print_unix_error("vt_serial_read_ushort","read"));

  if (num_bytes_read != 2)
    return (vt_set_error("vt_serial_read_ushort",IO_ERROR5));

  ushortbuf = (unsigned short *) buffer;

  return ((int) *ushortbuf);
}

int
CyberGloveBasic::vt_serial_read_bytes(int portfd, unsigned char *buffer, int numbytes)
{
  int num_bytes_read,returnval;

  for (num_bytes_read = 0; num_bytes_read < numbytes; num_bytes_read++)
    if ((returnval = read(portfd,&buffer[num_bytes_read],1)) < 1)
      break;

  if (returnval == -1)
  {
    vt_print_unix_error("vt_serial_read_bytes","read");
    return(0);
  }
  else if (num_bytes_read != numbytes)
  {
    vt_set_error("vt_serial_read_bytes",IO_ERROR5);
    return (-num_bytes_read);
  }
  else
    return (Ok);
}

int
CyberGloveBasic::vt_serial_read_shorts(int portfd, void *buffer, int num_shorts)
{
  int num_bytes_read,returnval, numbytes = num_shorts*sizeof(short);

  for (num_bytes_read = 0; num_bytes_read < numbytes; num_bytes_read++)
    if ((returnval = read(portfd,&((char *) buffer)[num_bytes_read],1)) < 1)
      break;

  if (returnval == -1)
  {
    vt_print_unix_error("vt_serial_read_shorts","read");
    return(0);
  }
  else if (num_bytes_read != numbytes)
  {
    vt_set_error("vt_serial_read_shorts",IO_ERROR5);
    return (-num_bytes_read/2);
  }

#ifndef IS_LITTLE_ENDIAN
  {
    int i;
    short *shorts = (short *)buffer;

    for (i=0; i < num_shorts; i++)
      swap_bytes_half_word(&shorts[i]);
  }
#endif

  return (Ok);
}

int
CyberGloveBasic::vt_serial_read_longs(int portfd, void *buffer, int num_longs)
{
  int num_bytes_read,returnval, numbytes = num_longs*sizeof(long);

  for (num_bytes_read = 0; num_bytes_read < numbytes; num_bytes_read++)
    if ((returnval = read(portfd,&((char *)buffer)[num_bytes_read],1)) < 1)
      break;

  if (returnval == -1)
  {
    vt_print_unix_error("vt_serial_read_longs","read");
    return(0);
  }
  else if (num_bytes_read != numbytes)
  {
    vt_set_error("vt_serial_read_longs",IO_ERROR5);
    return (-num_bytes_read/4);
  }

#ifndef IS_LITTLE_ENDIAN
  {
    int i;
    for (i=0; i < num_longs; i++)
      swap_bytes_word(&((long *) buffer)[i]);
  }
#endif
  return (Ok);
}

int
CyberGloveBasic::vt_serial_read_string(int portfd, char *buffer, int length)
{
  int num_bytes_read,returnval;

  /* NOTE: buffer must be at least length+1 bytes long */
  /* We stop when we hit a string terminator or we've read */
  /* length chars or we timeout or there is a read error */

  for (num_bytes_read = 0; num_bytes_read < length; num_bytes_read++)
    if (((returnval = read(portfd,&buffer[num_bytes_read],1)) < 1) ||
    (buffer[num_bytes_read] == 0))
      break;

  if (returnval == -1)
  {
    vt_print_unix_error("vt_serial_read_string","read");
    return(0);
  }
  else if (returnval == 0)
  {
    buffer[num_bytes_read] = 0; /* NULL terminate it just to be anal */
    vt_set_error("vt_serial_read_string",IO_ERROR5);
    return (-num_bytes_read);
  }
  else if (num_bytes_read == length)
    buffer[length] = 0;     /* NULL terminate the sucker */
  /* if none of the above ifs are true then we stopped because we hit a */
  /* string terminator so we don't have to terminate the string         */

  return (Ok);
}

char *
CyberGloveBasic::vt_serial_read_line(int portfd, char *buffer, int length)
{
  int ch, index = 0;

  /* NOTE: buffer must be at least length+1 bytes long */
  /* we stop if we have read length bytes or there is nothing to read or if */
  /* the character read is a NULL, carriage return or newline ... whew */
  /* that was a mouthfull */

  while ((index < length) && ((ch = vt_serial_read_byte(portfd)) > 0))
  {
    buffer[index++] = ch;
    if ((ch == '\r') || (ch == '\n'))
      break;
  }

  buffer[index] = 0;        /* you're terminated f*cker! */

  if (ch == -1)
    vt_print_error("vt_serial_read_line");

  return (buffer);
}

int
CyberGloveBasic::vt_serial_write_byte(int portfd, char ch)
{
  int num_bytes_written;

  if ((num_bytes_written = write(portfd,&ch,1)) == -1)
  {
    vt_print_unix_error("vt_serial_write_byte","write");
    return(0);
  }
  else if (num_bytes_written == 0)
  {
    vt_set_error("vt_serial_write_byte",IO_ERROR4);
    return(0);
  }

  return(Ok);
}


int
CyberGloveBasic::vt_serial_write_bytes(int portfd, unsigned char buffer[], int numbytes)
{
  int num_bytes_written;

  if ((num_bytes_written = write(portfd,buffer,numbytes)) == -1)
  {
    vt_print_unix_error("vt_serial_write_bytes","write");
    return(0);
  }
  else if (num_bytes_written != numbytes)
  {
    vt_set_error("vt_serial_write_bytes",IO_ERROR4);
    return(-num_bytes_written);
  }

  return(Ok);
}


int
CyberGloveBasic::vt_serial_write_shorts(int portfd, void *buffer, int num_shorts)
{
  int num_bytes_written, numbytes = num_shorts*sizeof(short);
#ifndef IS_LITTLE_ENDIAN
  int i;
  short *shorts = (short *)buffer;

  for (i=0; i < num_shorts; i++)
    swap_bytes_half_word(&shorts[i]);
#endif

  if ((num_bytes_written = write(portfd,buffer,numbytes)) == -1)
  {
    vt_print_unix_error("vt_serial_write_shorts","write");
    return(0);
  }
  else if (num_bytes_written != numbytes)
  {
    vt_set_error("vt_serial_write_shorts",IO_ERROR4);
    return(-num_bytes_written/2);
  }

  return(Ok);
}


int
CyberGloveBasic::vt_serial_write_string(int portfd, char string[])
{
  int numbytes,num_bytes_written;

  numbytes = strlen(string);
  if ((num_bytes_written = write(portfd,string,numbytes)) == -1)
  {
    vt_print_unix_error("vt_serial_write_string","write");
    return(0);
  }
  else if (num_bytes_written != numbytes)
  {
    vt_set_error("vt_serial_write_string",IO_ERROR4);
    return(-num_bytes_written);
  }

  return(Ok);
}

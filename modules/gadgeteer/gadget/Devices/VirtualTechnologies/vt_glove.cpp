/*  FILE: vt_glove.c  --                                        */
/*                                                              */
/*  CyberGlove initialization and control routines              */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1990,91,92,93 Virtual Technologies    --  */
/*  --                                                      --  */
/*  --    Original author: William L. Chapin                --  */
/*  --    Heavily modified by: Larry Edwards                --  */
/*  ==========================================================  */


#include <vrj/vrjConfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <varargs.h>
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

#include <gad/Devices/VirtualTechnologies/CyberGlove.h>
#include <gad/Devices/VirtualTechnologies/vt_types.h>
#include <gad/Devices/VirtualTechnologies/vt_globals.h>
#include <gad/Devices/VirtualTechnologies/vt_glove-types.h>
#include <gad/Devices/VirtualTechnologies/vt_error.h>

#define delay(t) (usleep(t*100)) /* t is in milliseconds */


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  Private Global Constants                                   */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/* some handy delays */
static const int STATUS_WAIT = 1000, REBOOT_WAIT = 2500;


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  Forward Declarations for Private Functions                 */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

void abduct_18(void);


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  Initialization                                             */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


CbGlove
CyberGloveBasic::allocate_CbGlove(void)
{
  CbGlove newglove;
  CbGlovePrivateStruct *private_data;
  int finger, joint;

  if ((newglove = (CbGlove) calloc(1,sizeof(CbGloveStruct))) == NULL)
    vt_fatal_unix_error("allocate_CbGlove","calloc");

  if ((newglove->private_data =
       (CbGlovePrivateStruct *) calloc(1,sizeof(CbGlovePrivateStruct))) == NULL)
    vt_fatal_unix_error("allocate_CbGlove","calloc");

  newglove->portfd = -1;
  newglove->timestamp = 0L;

  private_data = newglove->private_data;

  private_data->param_flags.word = CG_STARTUP_FLAGS; /*  Powerup defaults   */

  for (finger = THUMB; finger <= FINGERS; finger++)
    for (joint = MCP; joint < JOINTS; joint++)
    {
      private_data->mapping[finger][joint].offset = 50;
      private_data->mapping[finger][joint].gain =  0.4;
    }

//  private_data->abduct_fingers = abduct_18;

  memset(private_data->ascii_record,NULL,CG_RECSIZE);

  return (newglove);
}

int
CyberGloveBasic::glove_boot(CbGlove glove)
{
  CbGlovePrivate private_data = glove->private_data;

  /* reboot the CbGlove Interface Unit */
  if (reboot_CGIU(glove,NULL) < Ok)
    return(vt_print_error("glove_boot"));

  /* check to see if CbGlove is connected to the interface unit and valid */
  if (glove_ok_query(glove,NULL) < Ok)
    return(vt_print_error("glove_boot"));

  if (!(private_data->param_flags.word & CG_GLOVE_VALID_FLAG))
    return(vt_set_error("glove_boot",CG_ERROR2));

  if (!(private_data->param_flags.word & CG_GLOVE_IN_FLAG))
    return (vt_set_error("glove_boot",CG_ERROR3));

  /* read the parameter flags */
  if (read_param_flags(glove,NULL) < Ok)
    return (vt_print_error("glove_boot"));

  /* find out which sensors we can enable */
  if (get_avail_sensor_mask(glove,NULL) < Ok)
    return (vt_print_error("glove_boot"));
  private_data->sensors_in_use.word = private_data->available_sensors.word;

  /* find out how many sensors we got */
  if (get_max_num_sensors(glove,NULL) < Ok)
    return (vt_print_error("glove_boot"));
  private_data->num_values_returned = private_data->num_enabled_sensors =
    private_data->num_sensors_available;

/*
  switch (private_data->num_sensors_available)
  {
  case 18:
    private_data->abduct_fingers = abduct_18; break;
  default:
    private_data->abduct_fingers = abduct_18; break;
  }
*/

  return (Ok);
}

int
CyberGloveBasic::vt_open_glove_port(char *dev_name, int baudrate)
{
  if ((current_glove->portfd = vt_serial_open(dev_name,baudrate)) < Ok)
    return (vt_print_error("vt_open_glove_port"));

  if (glove_boot(current_glove) < Ok)
  {
    vt_serial_close(current_glove->portfd);
    current_glove->portfd = -1;
    return (vt_print_error("vt_open_glove_port"));
  }

  return (Ok);
}

void
CyberGloveBasic::vt_close_glove_port(CbGlove glove)
{
  vt_serial_close(glove->portfd);
  glove->portfd = -1;
}

CbGlove
CyberGloveBasic::vt_create_CbGlove(Boolean connect, ...)
{
  char *dev_name;
  int baudrate;
  va_list address_arg1 = (va_list) &connect;

  current_glove = allocate_CbGlove();
  current_glove_private = current_glove->private_data;

  if (connect)
  {
    dev_name = va_arg(address_arg1, char *);
    baudrate = va_arg(address_arg1,int);
    if (vt_open_glove_port(dev_name,baudrate) < Ok)
      vt_print_error("vt_create_CbGlove");
  }

  return (current_glove);
}

void
CyberGloveBasic::vt_destroy_CbGlove(CbGlove glove)
{
  if (glove->portfd != -1)
    vt_serial_close(glove->portfd);
  free(glove->private_data);
  free(glove);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
/*  VT_UNPROCESS_GLOVE_ANGLES --                                */
/*                                                              */
/*  set the raw glove values from the joint angles ... only     */
/*  used to initialize raw values so that when glove is off     */
/*  the joint_angles will retain reasonable values.             */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

void
CyberGloveBasic::vt_unprocess_glove_angles(void)
{
  int finger, joint;
  glove_sensor_map_struct (*mapping)[MAX_GROUP_VALUES];
  volatile float (*angle)[MAX_GROUP_VALUES];
  unsigned char (*raw_value)[MAX_GROUP_VALUES];

  mapping = current_glove_private->mapping;
  angle = current_glove->joint_angle;
  raw_value = current_glove->raw_value;

  /* NOTE: we negate gain here, so that sense of rotations is appropriate */
  /* for the digit coordinate systems                                     */
  for (finger = THUMB; finger < FINGERS+1; finger++)
    for (joint = MCP; joint < JOINTS; joint++)
      raw_value[finger][joint] = mapping[finger][joint].offset -
                            mapping[finger][joint].gain *
               angle[finger][joint];

  /* if we have a left handed glove negate abduction angles */
  if (!(current_glove_private->param_flags.word & CG_RIGHT_HAND_FLAG))
  {
    for (finger = THUMB; finger < FINGERS; finger++)
      raw_value[finger][ABDUCT] = -raw_value[finger][ABDUCT];

    raw_value[CARPUS][WRIST_YAW] = -raw_value[CARPUS][WRIST_YAW];
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  routines to check the glove's responses                    */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


int
CyberGloveBasic::query_reply_head_check(int portfd, char *query_string)
{
  unsigned char reply[2];

  if (vt_serial_read_bytes(portfd,reply,2) < Ok)
    return (vt_print_error("query_reply_head_check"));

  if (strncmp(query_string,(const char *)reply,2) != 0)
    return (vt_set_error("query_reply_head_check",CG_ERROR5));

  return (Ok);
}

int
CyberGloveBasic::reply_tail_check(CbGlove glove)
{
  int i, lastbytes[5], portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  Boolean sample_error = FALSE, glove_error = FALSE;

  if ((lastbytes[0] = vt_serial_read_byte(portfd)) == -1)
    return (vt_print_error("reply_tail_check"));

  if (lastbytes[0] == CG_TERMINATOR)
    return (Ok);

  for (i=1; i < 5; i++)
  {
    if ((lastbytes[i] = vt_serial_read_byte(portfd)) == -1)
      return (vt_print_error("reply_tail_check"));

    if (i & 1)       /* i.e. if i is odd */
    {
      switch (lastbytes[i])
      {
      case 'e':   break;      /* we dropped a byte somewhere */
      case 's':
   sample_error = TRUE;
   break;
      case 'g':
   glove_error = TRUE;
   break;
      case CG_TERMINATOR:  /* should only find terminator on even i */
   return (vt_set_error("reply_tail_check",CG_ERROR5));
      default:
   vt_serial_clear_to_terminator(portfd,CG_TERMINATOR);
   return (vt_set_error("reply_tail_check",CG_ERROR5));
      }
    }
    else
      switch (lastbytes[i])
      {
      case 'e':   break;
      case 's':break;      /* we dropped a byte somewhere */
      case 'g':break;      /* we dropped a byte somewhere */
      case CG_TERMINATOR:
   if (sample_error && glove_error)
   {
     private_data->param_flags.word &= ~CG_GLOVE_IN_FLAG;
     return (vt_set_error("reply_tail_check",CG_ERROR9));
   }
   else if (glove_error)
   {
     private_data->param_flags.word &= ~CG_GLOVE_IN_FLAG;
     return (vt_set_error("reply_tail_check",CG_ERROR3));
   }
   else
     return (vt_set_error("reply_tail_check",CG_ERROR10));
      default:
   vt_serial_clear_to_terminator(portfd,CG_TERMINATOR);
   return (vt_set_error("reply_tail_check",CG_ERROR5));
      }
  }

  printf("\nYO BABY!\n"); fflush(stdout);
  return Ok;
}

int
CyberGloveBasic::command_reply_check(CbGlove glove, char command_char)
{
  unsigned char reply[4] = "FFF";

  if (vt_serial_read_bytes(glove->portfd,reply,2) < Ok)
    return (vt_print_error("command_reply_check"));

  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("command_reply_check"));

  return (Ok);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  routines to read data returned by the glove                */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


int
CyberGloveBasic::read_and_decode_timestamp(int portfd, unsigned long *timestamp)
{
  word_byte_union newstamp;
  int bitflipflags;

  if ((bitflipflags = vt_serial_read_byte(portfd)) == -1)
    return (vt_print_error("read_and_decode_timestamp"));

  if (bitflipflags == 0)
    return (vt_set_error("read_and_decode_timestamp",CG_ERROR8));

  if (vt_serial_read_longs(portfd,&newstamp.word,1) < Ok)
    return (vt_print_error("read_and_decode_timestamp"));

  if ((newstamp.byte[LONG_BYTE_0] == 0) || (newstamp.byte[LONG_BYTE_1] == 0) ||
      (newstamp.byte[LONG_BYTE_2] == 0) || (newstamp.byte[LONG_BYTE_3] == 0))
    return (vt_set_error("read_and_decode_timestamp",CG_ERROR8));

  newstamp.byte[LONG_BYTE_0] = (bitflipflags & 0x01) ^
                                newstamp.byte[LONG_BYTE_0];
  bitflipflags = ((unsigned char) bitflipflags) >> 1;
  newstamp.byte[LONG_BYTE_1] = (bitflipflags & 0x01) ^
                                newstamp.byte[LONG_BYTE_1];
  bitflipflags = ((unsigned char) bitflipflags) >> 1;
  newstamp.byte[LONG_BYTE_2] = (bitflipflags & 0x01) ^
                                newstamp.byte[LONG_BYTE_2];
  bitflipflags = ((unsigned char) bitflipflags) >> 1;
  newstamp.byte[LONG_BYTE_3] = (bitflipflags & 0x01) ^
                                newstamp.byte[LONG_BYTE_3];

  *timestamp = newstamp.word;

  return(Ok);
}

int
CyberGloveBasic::vt_read_glove_data(void)
{
  int portfd = current_glove->portfd;
  unsigned long sensor_mask, stencil = 0x1;
  unsigned char value[MAX_SENSOR_VALUES];
  int8_t status_byte;
  int reply_head,i,joint,finger,num_values, count = 0;
  char message[256];

  sensor_mask = current_glove_private->sensors_in_use.word;
  num_values = current_glove_private->num_values_returned;
  if (num_values == 0)
    return (Ok);

  if ((reply_head = vt_serial_read_byte(current_glove->portfd)) == -1)
    return (vt_print_error("vt_read_glove_data"));

  if ((reply_head != CG_REQUEST_DATA_CH) && (reply_head != CG_SEND_STREAM_CH))
  {
    vt_serial_clear_to_terminator(portfd,CG_TERMINATOR);
    sprintf(message,"vt_read_glove_data: head byte = %d:",reply_head);
    return (vt_set_error(message,CG_ERROR11));
  }

  if (vt_serial_read_bytes(portfd,value,num_values) < Ok)
    return (vt_print_error("vt_read_glove_data"));

  /* sensor values should never be zero ... check them */
  for (i=0; i < num_values; i++)
    if (value[i] == 0)
      return (vt_set_error("vt_read_glove_data",CG_ERROR8));

  sensor_mask = current_glove_private->sensors_in_use.word;
  for (finger=0; finger < MAX_SENSOR_GROUPS; finger++)
    for (joint=0; joint < MAX_GROUP_VALUES; joint++)
    {
      if (sensor_mask & stencil)
   current_glove->raw_value[finger][joint] = value[count++];
      stencil <<= 1;
    }

  if (current_glove_private->param_flags.word & CG_TIME_STAMP_FLAG)
    if (read_and_decode_timestamp(portfd,&current_glove->timestamp) < Ok)
      return (vt_print_error("vt_read_glove_data"));

  if (current_glove_private->param_flags.word & CG_STATUS_BYTE_FLAG)
  {
    if ((status_byte = vt_serial_read_byte(portfd)) == -1)
      return (vt_print_error("vt_read_glove_data"));
    else
      current_glove_private->param_flags.byte[LONG_BYTE_0] = status_byte;
  }

  if (reply_tail_check(current_glove) < Ok)
    return (vt_print_error("vt_read_glove_data"));

  return (Ok);
}

int
CyberGloveBasic::vt_read_processed_glove_data(void)
{
  if (vt_read_glove_data() < Ok)
    return (vt_print_error("vt_read_processed_glove_data"));

  vt_process_glove_data();

  return (Ok);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  routines to send glove commands and read glove data        */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SET_BAUD_RATE --                                            */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   unsigned char baudbyte                                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::set_baud_rate(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  unsigned char baudbyte, dontcaremask = 0x37;
  va_list temp = (va_list)address_arg1;

  baudbyte = va_arg(temp, int);
  baudbyte &= dontcaremask;

  /* send the set baud rate command */
  if ((vt_serial_write_byte(portfd,CG_BAUD_RATE_CH) < Ok) ||
      (vt_serial_write_byte(portfd,baudbyte) < Ok))
    return(vt_print_error("set_baud_rate"));

  /* check the response */
  if (command_reply_check(glove,CG_BAUD_RATE_CH) < Ok)
    return(vt_print_error("set_baud_rate"));

  private_data->baud_divide_byte = baudbyte;

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ENABLE_TIMESTAMP --                                         */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   Boolean enabled                                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::enable_timestamp(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  int enabled;
  va_list temp = (va_list)address_arg1;

  enabled = va_arg(temp, int);

  /* send the timestamp enable/disable command */
  if ((vt_serial_write_byte(portfd,CG_TIME_STAMP_CH) < Ok) ||
      (vt_serial_write_byte(portfd,enabled) < Ok))
    return(vt_print_error("enable_timestamp"));

  /* check the response */
  if (command_reply_check(glove,CG_TIME_STAMP_CH) < Ok)
    return(vt_print_error("enable_timestamp"));

  private_data->param_flags.word = (enabled) ?
                            private_data->param_flags.word | CG_TIME_STAMP_FLAG :
                            private_data->param_flags.word & ~CG_TIME_STAMP_FLAG;

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ENABLE_FILTER --                                            */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   Boolean enabled                                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::enable_filter(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  Boolean enabled;
  va_list temp = (va_list)address_arg1;

  enabled = va_arg(temp, int);

  /* send the filter enable/disable command */
  if ((vt_serial_write_byte(portfd,CG_FILTER_CH) < Ok) ||
      (vt_serial_write_byte(portfd,enabled) < Ok))
    return(vt_print_error("enable_filter"));

  /* check the response */
  if (command_reply_check(glove,CG_FILTER_CH) < Ok)
    return(vt_print_error("enable_filter"));

  private_data->param_flags.word = (enabled ?
             private_data->param_flags.word | CG_FILTER_FLAG :
             private_data->param_flags.word & ~CG_FILTER_FLAG);
  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* REQUEST_SINGLE_SAMPLE --                                    */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::request_single_sample(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;

  /* send the request sensor value command */
  if (vt_serial_write_byte(portfd,CG_REQUEST_DATA_CH) < Ok)
    return(vt_print_error("request_single_sample"));

  /* we don't wait for the response so we can do other things while */
  /* CGIU is sending data */

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* REQUEST_SAMPLE_STREAM --                                    */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::request_sample_stream(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;

  /* send the request sensor value stream command */
  if (vt_serial_write_byte(portfd,'S') < Ok)
    return(vt_print_error("request_sample_stream"));

  /* we don't wait for the response so we can do other things while */
  /* CGIU is sending data */

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* ENABLE_SWITCH_CONTROLS_LIGHT --                             */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   Boolean enabled                                           */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::enable_switch_controls_light(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  Boolean enabled;
  va_list temp = (va_list)address_arg1;

  enabled = va_arg(temp, int);

  /* send the switch controls light command */
  if ((vt_serial_write_byte(portfd,CG_LIGHTCNTL_CH) < Ok) ||
      (vt_serial_write_byte(portfd,enabled) < Ok))
    return(vt_print_error("enable_switch_controls_light"));

  /* check the response */
  if (command_reply_check(glove,CG_LIGHTCNTL_CH) < Ok)
    return(vt_print_error("enable_switch_controls_light"));

  private_data->param_flags.word = (enabled ?
              private_data->param_flags.word | CG_SWITCH_FLAG :
              private_data->param_flags.word & ~CG_SWITCH_FLAG);
  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SET_SENSOR_MASK --                                          */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   unsigned long mask                                        */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::set_sensor_mask(CbGlove glove, void *address_arg1)
{
  int num_enabled_sensors = 0, portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  unsigned long mask;
  unsigned char *byte;
  va_list temp = (va_list)address_arg1;

  /* send the set software sensor mask command */
  if (vt_serial_write_byte(portfd,CG_SENSOR_MASK_CH) < Ok)
    return(vt_print_error("set_sensor_mask"));

  mask = va_arg(temp, unsigned long);
  mask = mask & private_data->available_sensors.word;

  byte = (unsigned char *) &mask;
  /* send the mask */
  if ((vt_serial_write_byte(portfd,byte[LONG_BYTE_0]) < Ok) ||
      (vt_serial_write_byte(portfd,byte[LONG_BYTE_1]) < Ok) ||
      (vt_serial_write_byte(portfd,byte[LONG_BYTE_2]) < Ok))
    return(vt_print_error("set_sensor_mask"));

  /* check the response */
  if (command_reply_check(glove,CG_SENSOR_MASK_CH) < Ok)
    return(vt_print_error("set_sensor_mask"));

  private_data->sensors_in_use.word = mask;

  while (mask != 0)
  {
    if (mask & 0x1)
      num_enabled_sensors++;
    mask >>= 1;
  }
  private_data->num_enabled_sensors = num_enabled_sensors;
  /* probably should check to make sure num_sensor_returned */
  /* is <= num_enabled_sensors - LJE                        */

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SET_NUM_VALUES_RETURNED --                                  */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   int num_values                                            */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::set_num_values_returned(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  int num_values;
  va_list temp = (va_list)address_arg1;

  /* send the set software sensor mask command */
  if (vt_serial_write_byte(portfd,CG_NUM_VALUES_CH) < Ok)
    return(vt_print_error("set_num_values_returned"));

  num_values = va_arg(temp, int);
  if (num_values > private_data->num_enabled_sensors)
    num_values = private_data->num_enabled_sensors;

  /* send the number of values desired */
  if (vt_serial_write_byte(portfd,(unsigned char) num_values) < Ok)
    return(vt_print_error("set_num_values_returned"));

  /* check the response */
  if (command_reply_check(glove,CG_NUM_VALUES_CH) < Ok)
    return(vt_print_error("set_num_values_returned"));

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* SET_PARAM_FLAGS --                                          */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   unsigned long param_flags                                 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::set_param_flags(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  word_byte_union param_flags;
  va_list temp = (va_list)address_arg1;

  /* send the set software sensor mask command */
  if (vt_serial_write_byte(portfd,CG_PARAMETER_CH) < Ok)
    return(vt_print_error("set_param_flags"));

  param_flags.word = va_arg(temp, unsigned long);

  /* send the flags */
  if ((vt_serial_write_byte(portfd,param_flags.byte[LONG_BYTE_0]) < Ok) ||
      (vt_serial_write_byte(portfd,param_flags.byte[LONG_BYTE_1]) < Ok) ||
      (vt_serial_write_byte(portfd,param_flags.byte[LONG_BYTE_2]) < Ok))
    return(vt_print_error("set_param_flags"));

  /* check the response */
  if (command_reply_check(glove,CG_PARAMETER_CH) < Ok)
    return(vt_print_error("set_param_flags"));

  private_data->param_flags.word = param_flags.word;

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* REBOOT_CGIU --                                              */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::reboot_CGIU(CbGlove glove, void *address_arg1)
{
  int lastbyte, portfd = glove->portfd;
  unsigned char buffer[2];

  /* send the request sensor value command */
  if (vt_serial_write_byte(portfd,CG_REBOOT_CH) < Ok)
    return (vt_print_error("reboot_CGIU"));

  delay(REBOOT_WAIT);                      /*  wait for system to boot */
  /* check the response */
  if (vt_serial_read_byte(glove->portfd) != CG_REBOOT_CH)
    return (vt_set_error("reboot_CGIU",CG_ERROR1));
  if (vt_serial_read_bytes(glove->portfd,buffer,2) < Ok)
    return(vt_print_error("reboot_CGIU"));

  if (strncmp((const char *)buffer,"\r\n",2) != 0)
    return (vt_set_error("reboot_CGIU",CG_ERROR5));

  if ((lastbyte = vt_serial_read_byte(glove->portfd)) == -1)
    return(vt_print_error("reboot_CGIU"));

  if (lastbyte != CG_TERMINATOR)
    return (vt_set_error("reboot_CGIU",CG_ERROR5));

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* VT_SEND_GLOVE_QUERY --                                      */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   int id  (glove command id, see enums in vt_glove-types.h) */
/*   any other args required by specific command, see above    */
/*   routines                                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::vt_send_glove_command(int id, ...)
{
  va_list address_arg1 = (va_list) &id;
  static int (CyberGloveBasic::*command_function[])(CbGlove, void *) =
  {
    &CyberGloveBasic::set_baud_rate, NULL, NULL, &CyberGloveBasic::enable_timestamp,
    &CyberGloveBasic::enable_filter, &CyberGloveBasic::request_single_sample,
    &CyberGloveBasic::request_sample_stream, NULL,
    &CyberGloveBasic::enable_switch_controls_light, NULL,
    &CyberGloveBasic::set_sensor_mask, &CyberGloveBasic::set_num_values_returned,
    &CyberGloveBasic::set_param_flags, &CyberGloveBasic::reboot_CGIU, NULL, NULL, NULL,
    NULL, NULL
  };

  if (current_glove->portfd == -1)
    return (Ok);

  va_arg(address_arg1,int);      /* increment to first variable arg */

  if ((this->*command_function[id])(current_glove,address_arg1) < Ok)
    return (vt_print_error("vt_send_glove_command"));

  return (Ok);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  routines to query the glove                                */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GLOVE_OK_QUERY --                                           */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   Boolean *status_return                                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::glove_ok_query(CbGlove glove, void *address_arg1)
{
  int status, portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  char *query_string = CG_GLOVE_QUERY;
  unsigned long flags;
  Boolean *status_return = NULL;
  va_list temp = (va_list)address_arg1;

  if (temp)
    status_return = va_arg(temp,Boolean *);

  if (status_return)
    *status_return = 0;

  query_string[CG_CMD_BYTE] = CG_GLOVE_OK_QUERY_CH;
  /* send the command */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("glove_ok_query"));

  /* check for a bad header returned */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("glove_ok_query"));

  /* read the data returned */
  if ((status = vt_serial_read_byte(portfd)) == -1)
    return (vt_print_error("glove_ok_query"));

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("glove_ok_query"));

  if (status_return)
    *status_return = status;

  flags = private_data->param_flags.word;
  flags = (status & CG_VALID_INIT) ? (flags | CG_GLOVE_VALID_FLAG) :
                                     (flags & ~CG_GLOVE_VALID_FLAG);
  flags = (status & CG_GLOVE_CONNECTED) ? (flags | CG_GLOVE_IN_FLAG) :
                                          (flags & ~CG_GLOVE_IN_FLAG);
  private_data->param_flags.word = flags;

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GET_GLOVE_INFO -                                            */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   char **info_return                                        */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::get_glove_info(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  char *query_string = CG_GLOVE_QUERY;
  static char buffer[2048];
  va_list temp = (va_list)address_arg1;
  char **info_return = NULL;

  if (temp)
    info_return = va_arg(temp, char **);
  /* if they didn't pass us a pointer we might as well return */
  /* since we don't save the info in the glove structure */
  if (!info_return)
    return (Ok);
  *info_return = buffer;
  buffer[0] = 0;

  query_string[CG_CMD_BYTE] = CG_INFO_QUERY_CH;
  /* request the glove info */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("get_glove_info"));

  /* check for a bad header returned */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("get_glove_info"));

  /* wait for the info to be ready */
  delay(STATUS_WAIT);

  /* now read the info */
  if (vt_serial_read_bytes(portfd,(unsigned char *)buffer,2048) < Ok)
  {
    buffer[0] = 0;
    return (vt_print_error("get_glove_info"));
  }

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
  {
    buffer[0] = 0;
    return (vt_print_error("get_glove_info"));
  }

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GET_AVAIL_SENSOR_MASK --                                    */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   unsigned long *mask_return                                */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::get_avail_sensor_mask(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  char *query_string = CG_GLOVE_QUERY;
  unsigned char mask[3];
  va_list temp = (va_list)address_arg1;
  unsigned long *mask_return;

  if (temp)
    mask_return = va_arg(temp,unsigned long *);

  if (mask_return)
    *mask_return = 0;

  query_string[CG_CMD_BYTE] = CG_HARD_MASK_QUERY_CH;
  /* request the hardware sensor mask */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("get_avail_sensor_mask"));

  /* check for a bad header returned */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("get_avail_sensor_mask"));

  /* if we got the appropriate responses read the mask */
  if (vt_serial_read_bytes(portfd,mask,3) < Ok)
    return (vt_print_error("get_avail_sensor_mask"));

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("get_avail_sensor_mask"));

  private_data->available_sensors.byte[LONG_BYTE_0] = mask[0];
  private_data->available_sensors.byte[LONG_BYTE_1] = mask[1];
  private_data->available_sensors.byte[LONG_BYTE_2] = mask[2];

  if (mask_return)
    *mask_return = private_data->available_sensors.word;

  return (Ok);
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GET_SENSOR_MASK --                                          */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   unsigned long *mask_return                                */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::get_sensor_mask(CbGlove glove, void *address_arg1)
{
  int num_enabled_sensors = 0, portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  char *query_string = CG_GLOVE_QUERY;
  word_byte_union mask;
  va_list temp = (va_list)address_arg1;
  unsigned long *mask_return = NULL;

  if (temp)
    mask_return = va_arg(temp,unsigned long *);

  if (mask_return)
    *mask_return = 0;

  query_string[CG_CMD_BYTE] = CG_SENSOR_MASK_CH;
  /* request the hardware sensor mask */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("get_sensor_mask"));

  /* check the response */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("get_sensor_mask"));

  /* if we got the appropriate responses read the mask */
  if (vt_serial_read_bytes(portfd,mask.byte,3) < Ok)
    return (vt_print_error("get_sensor_mask"));

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("get_sensor_mask"));

  private_data->sensors_in_use.byte[LONG_BYTE_0] = mask.byte[0];
  private_data->sensors_in_use.byte[LONG_BYTE_1] = mask.byte[1];
  private_data->sensors_in_use.byte[LONG_BYTE_2] = mask.byte[2];

  if (mask_return)
    *mask_return = private_data->sensors_in_use.word;

  /* just to make sure our idea of how many values come back is right... */
  while (mask.word != 0)
  {
    if (mask.word & 0x1)
      num_enabled_sensors++;
    mask.word >>= 1;
  }
  private_data->num_enabled_sensors = num_enabled_sensors;
  /* LJE check to make sure num_sensor_returned is <= num_enabled_sensors */

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GET_NUM_VALUES_RETURNED --                                  */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   int *num_values_return                                    */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::get_num_values_returned(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  char *query_string = CG_GLOVE_QUERY;
  int num_values;
  va_list temp = (va_list)address_arg1;
  int *num_values_return = NULL;

  if (temp)
    num_values_return = va_arg(temp, int *);

  if (num_values_return)
    *num_values_return = 0;

  query_string[CG_CMD_BYTE] = CG_NUM_VALUES_CH;
  /* query how many sensor values are returned */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("get_num_values_returned"));

  /* check the response */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("get_num_values_returned"));

  if ((num_values = vt_serial_read_byte(portfd)) == -1)
    return (vt_print_error("get_num_values_returned"));

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("get_num_values_returned"));

  private_data->num_values_returned = num_values;

  if (num_values_return)
    *num_values_return = num_values;

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* READ_PARAM_FLAGS --                                         */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   unsigned long *param_return                               */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::read_param_flags(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  char *query_string = CG_GLOVE_QUERY;
  unsigned char param[3];
  va_list temp = (va_list)address_arg1;
  unsigned long *param_return = NULL;

  if (temp)
    param_return = va_arg(temp,unsigned long *);

  if (param_return)
    *param_return = 0;

  query_string[CG_CMD_BYTE] = CG_PARAMETER_CH;
  /* request the parameter flags */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("read_param_flags"));

  /* check the response */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("read_param_flags"));

  /* if we got the appropriate responses read the parameter flags */
  if (vt_serial_read_bytes(portfd,param,3) < Ok)
    return (vt_print_error("read_param_flags"));

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("read_param_flags"));

  private_data->param_flags.byte[LONG_BYTE_0] = param[0];
  private_data->param_flags.byte[LONG_BYTE_1] = param[1];
  private_data->param_flags.byte[LONG_BYTE_2] = param[2];
  if (param_return)
    *param_return = private_data->param_flags.word;

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* RIGHT_HAND_GLOVE_QUERY --                                   */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   Boolean *righthand_return                                 */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::right_hand_glove_query(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  char *query_string = CG_GLOVE_QUERY;
  unsigned long flags;
  int is_righthand;
  va_list temp = (va_list)address_arg1;
  Boolean *righthand_return = NULL;

  if (temp)
    righthand_return = va_arg(temp,Boolean *);

  if (righthand_return)
    *righthand_return = 0;

  query_string[CG_CMD_BYTE] = CG_RIGHT_HAND_QUERY_CH;
  /* send the command */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("right_hand_glove_query"));

  /* check for a bad header returned */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("right_hand_glove_query"));

  /* read the data returned */
  if ((is_righthand = vt_serial_read_byte(portfd)) == -1)
    return (vt_print_error("right_hand_glove_query"));

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("right_hand_glove_query"));

  if (righthand_return)
    *righthand_return = is_righthand;

  flags = private_data->param_flags.word;
  flags = (is_righthand) ? (flags | CG_RIGHT_HAND_FLAG) :
                           (flags & ~CG_RIGHT_HAND_FLAG);
  private_data->param_flags.word = flags;

  return (Ok);
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* GET_MAX_NUM_SENSORS --                                      */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   CbGlove glove                                          */
/*   int *num_sensors_return                                   */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::get_max_num_sensors(CbGlove glove, void *address_arg1)
{
  int portfd = glove->portfd;
  CbGlovePrivate private_data = glove->private_data;
  char *query_string = CG_GLOVE_QUERY;
  int num_sensors;
  va_list temp = (va_list)address_arg1;
  int *num_sensors_return = NULL;

  if (temp)
    num_sensors_return = va_arg(temp,int *);

  if (num_sensors_return)
    *num_sensors_return = 0;

  query_string[CG_CMD_BYTE] = CG_MAX_SENSORS_QUERY_CH;
  /* send the command */
  if (vt_serial_write_bytes(portfd,(unsigned char*)query_string,2) < Ok)
    return (vt_print_error("get_max_num_sensors"));

  /* check for a bad header returned */
  if (query_reply_head_check(portfd,query_string) < Ok)
    return (vt_print_error("get_max_num_sensors"));

  /* read the data returned */
  if ((num_sensors = vt_serial_read_byte(portfd)) == -1)
    return (vt_print_error("get_max_num_sensors"));

  /* check for a bad tail byte returned */
  if (reply_tail_check(glove) < Ok)
    return (vt_print_error("get_max_num_sensors"));

  if (num_sensors_return)
    *num_sensors_return = num_sensors;

  private_data->num_sensors_available = num_sensors;
  return Ok;
}


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/* VT_SEND_GLOVE_QUERY --                                      */
/*                                                             */
/*                                                             */
/* Args:                                                       */
/*   int id  (query command id see enums in vt_glove-types.h)  */
/*   any other args required by specific command, see above    */
/*   routines                                                  */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int
CyberGloveBasic::vt_send_glove_query(int id, ...)
{
  va_list address_arg1 = (va_list) &id;

  static int (CyberGloveBasic::*query_function[])(CbGlove, void *) =
  {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    &CyberGloveBasic::get_sensor_mask, &CyberGloveBasic::get_num_values_returned,
    &CyberGloveBasic::read_param_flags, NULL, NULL, NULL, NULL, NULL,
    NULL, &CyberGloveBasic::glove_ok_query, &CyberGloveBasic::get_glove_info,
    &CyberGloveBasic::get_avail_sensor_mask, &CyberGloveBasic::right_hand_glove_query,
    &CyberGloveBasic::get_max_num_sensors
  };

  if (current_glove->portfd == -1)
    return (Ok);

  va_arg(address_arg1,int);      /* increment to first variable arg */

  if ((this->*query_function[id])(current_glove,address_arg1) < Ok)
    return (vt_print_error("vt_send_glove_query"));

  return (Ok);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/*                                                             */
/*  routines to process the raw glove values                   */
/*                                                             */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/


void
CyberGloveBasic::vt_process_glove_data(void)
{
  int finger, joint;
  glove_sensor_map_struct (*mapping)[MAX_GROUP_VALUES];
  volatile float (*angle)[MAX_GROUP_VALUES];
  unsigned char (*raw_value)[MAX_GROUP_VALUES];

  mapping = current_glove_private->mapping;
  angle = current_glove->joint_angle;
  raw_value = current_glove->raw_value;

  /* NOTE: we negate gain here, so that sense of rotations is appropriate */
  /* for the digit coordinate systems                                     */
  for (finger = THUMB; finger < FINGERS+1; finger++)
    for (joint = MCP; joint < JOINTS; joint++)
      angle[finger][joint] = (raw_value[finger][joint] -
               mapping[finger][joint].offset) *
            -mapping[finger][joint].gain;

  /*  kludge limiter for back of thumb rotation sensor  */
  if (angle[THUMB][MCP] > 0.5)
    angle[THUMB][MCP] = 0.5;

  //  current_glove_private->abduct_fingers();
  abduct_18();

  /* if we have an 18 sensor glove we fake the distal flex */
  if (current_glove_private->num_sensors_available <= 18)
    for (finger = INDEX; finger < FINGERS; finger++)
    {
      angle[finger][DIP] = -(angle[finger][PIP] * angle[finger][PIP] * 0.32 +
              angle[finger][PIP] * 0.17);
    }

  /* if we have a left handed glove negate abduction angles */
  if (!(current_glove_private->param_flags.word & CG_RIGHT_HAND_FLAG))
  {
    for (finger = THUMB; finger < FINGERS; finger++)
      angle[finger][ABDUCT] = -angle[finger][ABDUCT];

    angle[CARPUS][WRIST_YAW] = -angle[CARPUS][WRIST_YAW];
  }
}

void
CyberGloveBasic::abduct_18(void)
{
  /*  Abduct function for gloves w/MDL,RNG,PNK sensors only              */
  /*  This algorithm allows the Index to be set directly with the        */
  /*    index gain slider.  The amount of middle finger bias is          */
  /*    set with the middle gain slider.  Offsets should be taken        */
  /*    with the fingers inline with each respective metacarpal.         */
  /*    Each finger gain should be set, while holding all others         */
  /*    in their offset position.                                        */
  glove_sensor_map_struct (*mapping)[MAX_GROUP_VALUES];
  float index_abduct,middle_abduct;
  volatile float (*angle)[MAX_GROUP_VALUES];
  unsigned char (*raw_value)[MAX_GROUP_VALUES];

  mapping = current_glove_private->mapping;
  angle = current_glove->joint_angle;
  raw_value = current_glove->raw_value;

  index_abduct = (raw_value[MIDDLE][ABDUCT] - mapping[MIDDLE][ABDUCT].offset) *
              mapping[INDEX][ABDUCT].gain;
  middle_abduct = -(angle[RING][ABDUCT] + index_abduct) *
                        mapping[MIDDLE][ABDUCT].gain;

  angle[INDEX][ABDUCT] = index_abduct + middle_abduct;
  angle[RING][ABDUCT] += middle_abduct;
  angle[PINKY][ABDUCT] += angle[RING][ABDUCT];
  angle[MIDDLE][ABDUCT] = middle_abduct;
}

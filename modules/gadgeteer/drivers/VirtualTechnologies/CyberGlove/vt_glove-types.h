/*  FILE: vt_glove-types.h  --                                  */
/*                                                              */
/*  Type declarations and constant definitions for vt_glove.c:  */
/*  CyberGlove communications                                   */
/*                                                              */
/*  ==========================================================  */
/*  --  Copyright (C) 1990,91,92,93 Virtual Technologies    --  */
/*  --                                                      --  */
/*  --    Author:  William L. Chapin                        --  */
/*  ==========================================================  */

#ifndef VT_GLOVE_TYPES_H
#define VT_GLOVE_TYPES_H

#include <vrj/vrjConfig.h>

/* some convenience macros */
#define CG_ASCII_COMM(cmd) tolower((cmd)) /* make ascii version of a command */

/*  Sensor masks  */
enum
{
  CG_VPL_MASK = 0x00033336L,    /* To emulate a DataGlove (ugh!)           */
  CG_MASK14 = 0x006333B7L,  /* To emulate an old 14 sensor CbGlove  */
  CG_MASK16 = 0x006B3B3FL,  /* To emulate an old 16 sensor CbGlove  */
  CG_MASK18 = 0x007BBB3FL,
  CG_MASK22 = 0x007FFF7FL,
  CG_MASK24 = 0x00FFFFFFL
};

/*  Status byte masks  */
enum
{
  CG_GLOVE_IN_FLAG = 0x1, CG_SWITCH_FLAG = 0x2,
  CG_LIGHT_FLAG = 0x4, CG_ALWAYS_ON_FLAG = 0x80
};

/*  Parameter flag masks  */
enum
{
  CG_BIN_SYNC_FLAG = 0x100, CG_ASCII_SYNC_FLAG = 0x200,
  CG_STATUS_BYTE_FLAG = 0x400, CG_LIGHT_CNTRL_FLAG = 0x800,
  CG_FILTER_FLAG = 0x1000, CG_TIME_STAMP_FLAG = 0x2000,
  CG_RIGHT_HAND_FLAG = 0x4000, CG_GLOVE_VALID_FLAG = 0x8000
};

/* Parameter flags at startup */
enum
{
  CG_STARTUP_FLAGS = CG_FILTER_FLAG | CG_RIGHT_HAND_FLAG | CG_LIGHT_CNTRL_FLAG |
                     CG_ALWAYS_ON_FLAG
};

/* Masks for byte returned by Glove valid query */
enum
{
  CG_VALID_INIT = 0x01, CG_GLOVE_CONNECTED = 0x02
};

/*  Max number of char.s in ascii record      */
enum { CG_RECSIZE = 128 };
/*  CbGlove transmission terminator ... should always be present */
enum {  CG_TERMINATOR = 0x00};

/*  Control Commands: no query option  */
enum
{
  CG_ABORT_CH = 0x03,       /*  Aborts command in progress  (^C )        */
  CG_REINIT_CH = 0x09,      /*  Re-Initializes Glove Info   (^I or Tab)  */
  CG_REBOOT_CH = 0x12       /*  Re-Boots CGIU firmware      (^R )        */
};

/*  Sensor Data Requests: no query option  */
enum
{
  CG_REQUEST_DATA_CH = 'G', /*  Requests single binary record as masked  */
  CG_SEND_STREAM_CH = 'S'   /*  Requests a binary record stream          */
};

/* ? prefix makes set/query commands queries */
#define CG_GLOVE_QUERY "? "

/*  Parameter Set/Query Commands  */
enum
{
  CG_CMD_BYTE = 1,
  CG_BAUD_RATE_CH = 'B',    /*  Sets CG communication rate   (1 word )   */
  CG_HARD_SENSOR_MAP_CH = 'C',  /*  Sets CG hardware sensor maps (see doc)   */
  CG_SENSOR_MASK_CH = 'M',  /*  Sets CG sensor sample mask   (3 bytes)   */
  CG_NUM_VALUES_CH = 'N',   /*  Sets CG # sensors to sample  (2 words)   */
  CG_PARAMETER_CH = 'P',    /*  Sets CG parameter bit flags  (3 bytes)   */
  CG_PERIOD_CH = 'T'        /*  Sets CG stream sample period (2 words)   */
};

/*  Boolean Parameter Flag Set/Query Commands ( 0=OFF, 1=ON )  */
enum
{
  CG_TIME_STAMP_CH = 'D',   /*  Sets time stamping on each record        */
  CG_FILTER_CH = 'F',       /*  Sets quantization filter (default=ON)    */
  CG_LIGHTCNTL_CH = 'J',    /*  Turns on/off switch control of light     */
  CG_LIGHT_CH = 'L',        /*  Turns on/off light                       */
  CG_STATUS_CH = 'U',       /*  Turns on/off glove status byte in record */
  CG_SWITCH_CH = 'W',       /*  Sets switch state (on/off)               */
  CG_EXTSYNC_ASCII_CH = 'y',    /*  Enables external sync (ascii mode)       */
  CG_EXTSYNC_BIN_CH = 'Y'   /*  Enables external sync (binary mode)      */
};

/*  Query Only commands  */
enum
{
  CG_INFO_QUERY_CH = 'i',   /* Requests glove info (string)            */
  CG_GLOVE_OK_QUERY_CH = 'G',   /* Queries glove validity (1 byte)         */
  CG_HARD_MASK_QUERY_CH = 'K',  /* Requests glove sensor mask (3 bytes)    */
  CG_RIGHT_HAND_QUERY_CH = 'R', /* Queries glove right handedness (1 byte) */
  CG_MAX_SENSORS_QUERY_CH = 'S', /* Queries max available sensors (1 byte)  */
  CG_PROM_VERSION_QUERY_CH = 'v' /* Requests CG PROM version nums  (string) */
};

enum glove_command_ids
{
  CG_BAUD_RATE,CG_ABORT,CG_HARD_SENSOR_MAP,CG_TIME_STAMP,CG_FILTER,
  CG_REQUEST_DATA,CG_SEND_STREAM,CG_REINIT,CG_LIGHTCNTL,CG_LIGHT,CG_SENSOR_MASK,
  CG_NUM_VALUES,CG_PARAMETER,CG_REBOOT,CG_PERIOD,CG_STATUS,CG_SWITCH,
  CG_EXTSYNC_BIN,CG_EXTSYNC_ASCII,

  CG_GLOVE_OK_QUERY,CG_INFO_QUERY,CG_HARD_MASK_QUERY,CG_RIGHT_HAND_QUERY,
  CG_MAX_SENSORS_QUERY,CG_PROM_VERSION_QUERY
};

enum { MAX_SENSOR_GROUPS = 6, MAX_GROUP_VALUES = 4 };
enum { MAX_SENSOR_VALUES = MAX_SENSOR_GROUPS*MAX_GROUP_VALUES };
enum { THUMB, INDEX, MIDDLE, RING, PINKY, CARPUS, FINGERS = 5 };
enum { MCP, PIP, DIP, ABDUCT, JOINTS,  FINGER_TIP = 3,
     CARPAL0 = 0, WRIST_PITCH, WRIST_YAW, CARPALS };

#ifndef IS_LITTLE_ENDIAN    /* i.e its big endian */
enum long_byte_order { LONG_BYTE_3, LONG_BYTE_2, LONG_BYTE_1, LONG_BYTE_0 };
#else
enum long_byte_order { LONG_BYTE_0, LONG_BYTE_1, LONG_BYTE_2, LONG_BYTE_3 };
#endif

typedef union
{
  unsigned char byte[4];
  unsigned long word;
} word_byte_union;

typedef struct
{
  /* the actual angle = (raw_value - offset) * gain  */
  unsigned char offset;     /* sensor offset */
  float gain;           /* sensor scale factors */
} glove_sensor_map_struct;

typedef struct
{
  word_byte_union available_sensors, sensors_in_use, param_flags;
  int num_values_returned,num_enabled_sensors,num_sensors_available;
  char ascii_record[CG_RECSIZE];
  glove_sensor_map_struct mapping[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
  unsigned char baud_divide_byte;

  void (*abduct_fingers)(void);
} CbGlovePrivateStruct, *CbGlovePrivate;


typedef struct
{
  int portfd;           /* serial port file descriptor */
  Boolean port_open;        /* whether the port is open or not */
  unsigned long timestamp;
  volatile float joint_angle[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
  /* raw 8 bit values: */
  unsigned char raw_value[MAX_SENSOR_GROUPS][MAX_GROUP_VALUES];
  CbGlovePrivateStruct  *private_data;
} CbGloveStruct, *CbGlove;

#endif              /* VT_GLOVE_TYPES_H */

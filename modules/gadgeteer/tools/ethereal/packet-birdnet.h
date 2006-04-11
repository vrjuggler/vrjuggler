#ifndef _PACKET_BIRDNET_H_
#define _PACKET_BIRDNET_H_

struct HEADER {
    unsigned short sequence;     /* 16-bit sequence number */
    unsigned short milliseconds; /* Interval time between packets */
    unsigned char  time[4];      /* Seconds since 00:00:00 Jan. 1, 1970 (GMT) */
    unsigned char  type;         /* Command/response code */
    unsigned char  xtype;        /* Extension to command/response code */
    unsigned char  protocol;     /* Protocol for this packet (BIRDNET is 3) */
    unsigned char  error_code;   /* Error code for an event at the server
                                    concerning the previous client packet */
    unsigned short error_code_extension; /* Additional error code info */
    unsigned short number_bytes; /* Size of the following data field */
};

struct SYSTEM_STATUS {
    unsigned char all;                /* General system status */
    unsigned char error;              /* Reserved */
    unsigned char flockNumber;        /* Number of devices on FBB bus */
    unsigned char serverNumber;       /* Number of servers in system */
    unsigned char transmitterNumber;  /* Address of current active xmtr */
    unsigned char measurementRate[6]; /* Measurement rate * 1000 (text) */
    unsigned char chassisNumber;      /* Chassis ID number */
    unsigned char chassisDevices;     /* Number of devices in chassis */
    unsigned char firstAddress;       /* FBB address of first device */
    unsigned char softwareRevision[2]; /* Software rev. of server app */
};

struct SINGLE_BIRD_STATUS {
    unsigned char status;          /* Device status */
    unsigned char id;              /* Device type */
    unsigned char softwareRev[2];  /* Software revision of firmware */
    unsigned char errorCode;       /* Reserved for future use */
    unsigned char setup;           /* Device setup parameters */
    unsigned char dataFormat;      /* Data Format Code and Data Size */
    unsigned char reportRate;      /* Rate at which data is generated relative
                                      to the sample rate */
    unsigned char scaling[2];      /* Full scale output */
    unsigned char hemisphere;      /* Bird's hemisphere of operation */
    unsigned char FBBaddress;      /* FBB address */
    unsigned char transmitterType; /* FBB address */
    unsigned char spare1;          /* Reserved */
    unsigned char spare2[2];       /* Reserved */
};

struct FILTER_TABLE {
    unsigned short entry[7];
};

struct XYZREF_TABLE {
    unsigned short angle[3];
};

struct BIRD_STATUS {
    struct SINGLE_BIRD_STATUS  status;    /* See SINGLE_BIRD_STATUS */
    struct FILTER_TABLE        alphaMin;  /* Page 61 of I&O Guide */
    struct FILTER_TABLE        alphaMax;  /* Pages 65-6 of I&O Guide */
    struct FILTER_TABLE        Vm;        /* Pages 66-7 of I&O Guide */
    struct XYZREF_TABLE        xyzReferenceFrame;
    struct XYZREF_TABLE        xyzAngleAlign;
};

#endif

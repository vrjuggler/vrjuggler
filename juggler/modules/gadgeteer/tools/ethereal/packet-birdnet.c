/* packet-birdnet.c
 * Routines for BIRDNET protocol dissection
 * Copyright 2001, 2003 Patrick Hartling <patrick@vrac.iastate.edu>
 *
 * packet-birdnet.c,v 1.3 2003/06/13 14:23:38 patrick Exp
 *
 * Ethereal - Network traffic analyzer
 * By Gerald Combs <gerald@unicom.net>
 * Copyright 1998 Gerald Combs
 *
 * Copied from README.developer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif

#ifdef NEED_SNPRINTF_H
# include "snprintf.h"
#endif

#include <epan/packet.h>
#include "packet-birdnet.h"

/* Initialize the protocol and registered fields */
static int proto_birdnet = -1;
static int hf_birdnet_seq = -1;
static int hf_birdnet_int = -1;
/* static int hf_birdnet_time = -1; */
static int hf_birdnet_time[4] = { -1, -1, -1, -1 };
static int hf_birdnet_type = -1;
static int hf_birdnet_xtype = -1;
static int hf_birdnet_proto = -1;
static int hf_birdnet_error_code = -1;
static int hf_birdnet_error_code_ext = -1;
static int hf_birdnet_num_bytes = -1;

static int hf_birdnet_data_sys_all                 = -1;
static int hf_birdnet_data_sys_all_running         = -1;
static int hf_birdnet_data_sys_all_error           = -1;
static int hf_birdnet_data_sys_all_fbb_error       = -1;
static int hf_birdnet_data_sys_all_local_error     = -1;
static int hf_birdnet_data_sys_all_local_power     = -1;
static int hf_birdnet_data_sys_all_master          = -1;
static int hf_birdnet_data_sys_all_crtsync_type    = -1;
static int hf_birdnet_data_sys_all_crtsync         = -1;
static int hf_birdnet_data_sys_error               = -1;
static int hf_birdnet_data_sys_flock_num           = -1;
static int hf_birdnet_data_sys_server_num          = -1;
static int hf_birdnet_data_sys_transmitter_num     = -1;
static int hf_birdnet_data_sys_measurement_rate[6] = { -1, -1, -1, -1, -1, -1 };
static int hf_birdnet_data_sys_chassis_num         = -1;
static int hf_birdnet_data_sys_chassis_devs        = -1;
static int hf_birdnet_data_sys_first_addr          = -1;
static int hf_birdnet_data_sys_software_rev[2]     = { -1, -1 };
static int hf_birdnet_data_sys_fbb_device          = -1;
static int hf_birdnet_data_sys_fbb_device_accessible = -1;
static int hf_birdnet_data_sys_fbb_device_running    = -1;
static int hf_birdnet_data_sys_fbb_device_sensor     = -1;
static int hf_birdnet_data_sys_fbb_device_erc        = -1;
static int hf_birdnet_data_sys_fbb_device_ert3       = -1;
static int hf_birdnet_data_sys_fbb_device_ert2       = -1;
static int hf_birdnet_data_sys_fbb_device_ert1       = -1;
static int hf_birdnet_data_sys_fbb_device_ert0       = -1;

static int hf_birdnet_data_bird_status                     = -1;
static int hf_birdnet_data_bird_status_error               = -1;
static int hf_birdnet_data_bird_status_running             = -1;
static int hf_birdnet_data_bird_status_reserved1           = -1;
static int hf_birdnet_data_bird_status_reserved2           = -1;
static int hf_birdnet_data_bird_status_buttons_present     = -1;
static int hf_birdnet_data_bird_status_sensor_present      = -1;
static int hf_birdnet_data_bird_status_transmitter_present = -1;
static int hf_birdnet_data_bird_status_transmitter_running = -1;
static int hf_birdnet_data_bird_id                         = -1;
static int hf_birdnet_data_bird_software_rev[2]            = { -1, -1 };
static int hf_birdnet_data_bird_error_code                 = -1;
static int hf_birdnet_data_bird_setup                      = -1;
static int hf_birdnet_data_bird_setup_sudden               = -1;
static int hf_birdnet_data_bird_setup_xyz_ref              = -1;
static int hf_birdnet_data_bird_setup_button_data          = -1;
static int hf_birdnet_data_bird_setup_ac_narrow            = -1;
static int hf_birdnet_data_bird_setup_ac_wide              = -1;
static int hf_birdnet_data_bird_setup_dc                   = -1;
static int hf_birdnet_data_bird_data_format                = -1;
static int hf_birdnet_data_bird_report_rate                = -1;
static int hf_birdnet_data_bird_scaling                    = -1;
/* static int hf_birdnet_data_bird_scaling_sys_code           = -1; */
/* static int hf_birdnet_data_bird_scaling_fs_meas            = -1; */
static int hf_birdnet_data_bird_hemisphere                 = -1;
static int hf_birdnet_data_bird_fbb_address                = -1;
static int hf_birdnet_data_bird_transmitter_type           = -1;
static int hf_birdnet_data_bird_transmitter_type_ert       = -1;
static int hf_birdnet_data_bird_transmitter_type_srt       = -1;
static int hf_birdnet_data_bird_transmitter_type_pcbird    = -1;
static int hf_birdnet_data_bird_transmitter_type_active    = -1;
static int hf_birdnet_data_bird_transmitter_type_selected  = -1;
static int hf_birdnet_data_bird_transmitter_type_number    = -1;
static int hf_birdnet_data_bird_spare1                     = -1;
static int hf_birdnet_data_bird_spare2                     = -1;

/* Initialize the subtree pointers */
static gint ett_birdnet_header                = -1;
static gint ett_birdnet_data                  = -1;
static gint ett_birdnet_data_sys_status       = -1;
static gint ett_birdnet_data_sys_xmtr_info    = -1;
static gint ett_birdnet_data_sys_flock        = -1;
static gint ett_birdnet_data_bird             = -1;
static gint ett_birdnet_data_bird_status      = -1;
static gint ett_birdnet_data_bird_setup       = -1;
static gint ett_birdnet_data_bird_data_format = -1;
static gint ett_birdnet_data_bird_scaling     = -1;
static gint ett_birdnet_data_bird_xmtr_type   = -1;

static const guint32 UDP_PORT_BIRDNET = 5000;
static const guint32 TCP_PORT_BIRDNET = 6000;

/*
static const unsigned char MSG_WAKE_UP        = 10;
static const unsigned char RSP_WAKE_UP        = 20;
static const unsigned char MSG_SHUT_DOWN      = 11;
static const unsigned char RSP_SHUT_DOWN      = 21;
static const unsigned char MSG_GET_STATUS     = 101;
static const unsigned char MSG_SEND_SETUP     = 102;
static const unsigned char MSG_SINGLE_SHOT    = 103;
static const unsigned char MSG_RUN_CONTINUOUS = 104;
static const unsigned char MSG_STOP_DATA      = 105;
static const unsigned char RSP_GET_STATUS     = 201;
static const unsigned char RSP_SEND_SETUP     = 202;
static const unsigned char RSP_RUN_CONTINUOUS = 204;
static const unsigned char RSP_STOP_DATA      = 205;
static const unsigned char DATA_PACKET_MULTI  = 210;
static const unsigned char RSP_ILLEGAL        = 40;
static const unsigned char RSP_UNKNOWN        = 50;
static const unsigned char MSG_SYNC_SEQUENCE  = 30;
static const unsigned char RSP_SYNC_SEQUENCE  = 31;
*/
#define MSG_WAKE_UP 10
#define RSP_WAKE_UP 20
#define MSG_SHUT_DOWN 11
#define RSP_SHUT_DOWN 21
#define MSG_GET_STATUS 101
#define MSG_SEND_SETUP 102
#define MSG_SINGLE_SHOT 103
#define MSG_RUN_CONTINUOUS 104
#define MSG_STOP_DATA 105
#define RSP_GET_STATUS 201
#define RSP_SEND_SETUP 202
#define RSP_RUN_CONTINUOUS 204
#define RSP_STOP_DATA 205
#define DATA_PACKET_MULTI 210
#define RSP_ILLEGAL 40
#define RSP_UNKNOWN 50
#define MSG_SYNC_SEQUENCE 30
#define RSP_SYNC_SEQUENCE 31

static const unsigned char BN_SEQUENCE       = 0;
static const unsigned char BN_INTERVAL       = 2;
static const unsigned char BN_TIME           = 4;
static const unsigned char BN_TYPE           = 8;
static const unsigned char BN_XTYPE          = 9;
static const unsigned char BN_PROTOCOL       = 10;
static const unsigned char BN_ERROR_CODE     = 11;
static const unsigned char BN_ERROR_CODE_EXT = 12;
static const unsigned char BN_NUM_BYTES      = 14;

static const unsigned char BN_DATA_SYS_ALL             = 0;
static const unsigned char BN_DATA_SYS_ERROR           = 1;
static const unsigned char BN_DATA_SYS_FLOCK_NUM       = 2;
static const unsigned char BN_DATA_SYS_SERVER_NUM      = 3;
static const unsigned char BN_DATA_SYS_TRANSMITTER_NUM = 4;
static const unsigned char BN_DATA_SYS_MEAS_RATE       = 5;
static const unsigned char BN_DATA_SYS_CHASSIS_NUM     = 11;
static const unsigned char BN_DATA_SYS_CHASSIS_DEVS    = 12;
static const unsigned char BN_DATA_SYS_FIRST_ADDR      = 13;
static const unsigned char BN_DATA_SYS_SOFTWARE_REV    = 14;

static const unsigned char BN_DATA_BIRD_STATUS       = 0;
static const unsigned char BN_DATA_BIRD_ID           = 1;
static const unsigned char BN_DATA_BIRD_SOFTWARE_REV = 2;
static const unsigned char BN_DATA_BIRD_ERROR_CODE   = 4;
static const unsigned char BN_DATA_BIRD_SETUP        = 5;
static const unsigned char BN_DATA_BIRD_DATA_FORMAT  = 6;
static const unsigned char BN_DATA_BIRD_REPORT_RATE  = 7;
static const unsigned char BN_DATA_BIRD_SCALING      = 8;
static const unsigned char BN_DATA_BIRD_HEMISPHERE   = 10;
static const unsigned char BN_DATA_BIRD_FBB_ADDRESS  = 11;
static const unsigned char BN_DATA_BIRD_XMTR_TYPE    = 12;
static const unsigned char BN_DATA_BIRD_SPARE1       = 13;
static const unsigned char BN_DATA_BIRD_SPARE2       = 14;

/*
static const unsigned char BN_SYSTEM_RUNNING      = 0x80;
static const unsigned char BN_SYSTEM_ERROR        = 0x40;
static const unsigned char BN_SYSTEM_FBB_ERROR    = 0x20;
static const unsigned char BN_SYSTEM_LOCAL_ERROR  = 0x10;
static const unsigned char BN_SYSTEM_LOCAL_POWER  = 0x08;
static const unsigned char BN_SYSTEM_MASTER       = 0x04;
static const unsigned char BN_SYSTEM_CRTSYNC_TYPE = 0x02;
static const unsigned char BN_SYSTEM_CRTSYNC      = 0x01;
*/

#define BN_SYSTEM_RUNNING      0x80
#define BN_SYSTEM_ERROR        0x40
#define BN_SYSTEM_FBB_ERROR    0x20
#define BN_SYSTEM_LOCAL_ERROR  0x10
#define BN_SYSTEM_LOCAL_POWER  0x08
#define BN_SYSTEM_MASTER       0x04
#define BN_SYSTEM_CRTSYNC_TYPE 0x02
#define BN_SYSTEM_CRTSYNC      0x01

/*
static const unsigned char FBB_ACCESS  = 0x80;
static const unsigned char FBB_RUNNING = 0x40;
static const unsigned char FBB_SENSOR  = 0x20;
static const unsigned char FBB_ERC     = 0x10;
static const unsigned char FBB_ERT3    = 0x08;
static const unsigned char FBB_ERT2    = 0x04;
static const unsigned char FBB_ERT1    = 0x02;
static const unsigned char FBB_ERT0    = 0x01;
*/

#define FBB_ACCESS  0x80
#define FBB_RUNNING 0x40
#define FBB_SENSOR  0x20
#define FBB_ERC     0x10
#define FBB_ERT3    0x08
#define FBB_ERT2    0x04
#define FBB_ERT1    0x02
#define FBB_ERT0    0x01

/*
static const unsigned char BN_BIRD_ERROR               = 0x80;
static const unsigned char BN_BIRD_RUNNING             = 0x40;
static const unsigned char BN_BIRD_RESERVED1           = 0x20;
static const unsigned char BN_BIRD_RESERVED2           = 0x10;
static const unsigned char BN_BIRD_BUTTONS_PRESENT     = 0x08;
static const unsigned char BN_BIRD_SENSOR_PRESENT      = 0x04;
static const unsigned char BN_BIRD_TRANSMITTER_PRESENT = 0x02;
static const unsigned char BN_BIRD_TRANSMITTER_RUNNING = 0x01;
*/

#define BN_BIRD_ERROR                0x80
#define BN_BIRD_RUNNING              0x40
#define BN_BIRD_RESERVED1            0x20
#define BN_BIRD_RESERVED2            0x10
#define BN_BIRD_BUTTONS_PRESENT      0x08
#define BN_BIRD_SENSOR_PRESENT       0x04
#define BN_BIRD_TRANSMITTER_PRESENT  0x02
#define BN_BIRD_TRANSMITTER_RUNNING  0x01

#define BN_BIRD_SETUP_SUDDEN      0x20
#define BN_BIRD_SETUP_XYZ_REF     0x10
#define BN_BIRD_SETUP_BUTTON_DATA 0x08
#define BN_BIRD_SETUP_AC_NARROW   0x04
#define BN_BIRD_SETUP_AC_WIDE     0x02
#define BN_BIRD_SETUP_DC          0x01

#define BN_BIRD_XMTR_TYPE_ERT      0x80
#define BN_BIRD_XMTR_TYPE_SRT      0x40
#define BN_BIRD_XMTR_TYPE_PCBIRD   0x20
#define BN_BIRD_XMTR_TYPE_ACTIVE   0x10
#define BN_BIRD_XMTR_TYPE_SELECTED 0x0c
#define BN_BIRD_XMTR_TYPE_NUMBER   0x03


static char*
getTypeString (const unsigned char type) {
    static char ret_str[40];

    memset(ret_str, '\0', sizeof(ret_str));

    switch (type) {
      case MSG_WAKE_UP:
        strcpy(ret_str, "Server wakeup request");
        break;
      case RSP_WAKE_UP:
        strcpy(ret_str, "Server wakeup resposne");
        break;
      case MSG_SHUT_DOWN:
        strcpy(ret_str, "Server shutdown request");
        break;
      case RSP_SHUT_DOWN:
        strcpy(ret_str, "Shutdown request response");
        break;
      case MSG_GET_STATUS:
        strcpy(ret_str, "Device status reqeust");
        break;
      case RSP_GET_STATUS:
        strcpy(ret_str, "Device status response");
        break;
      case MSG_SEND_SETUP:
        strcpy(ret_str, "Device configuration request");
        break;
      case RSP_SEND_SETUP:
        strcpy(ret_str, "Device configuration response");
        break;
      case MSG_SINGLE_SHOT:
        strcpy(ret_str, "Single-shot request");
        break;
      case MSG_RUN_CONTINUOUS:
        strcpy(ret_str, "Continuous data request");
        break;
      case RSP_RUN_CONTINUOUS:
        strcpy(ret_str, "Continuous data response");
        break;
      case MSG_STOP_DATA:
        strcpy(ret_str, "Data stop request");
        break;
      case RSP_STOP_DATA:
        strcpy(ret_str, "Data stop response");
        break;
      case DATA_PACKET_MULTI:
        strcpy(ret_str, "Data packet multi");
        break;
      case RSP_ILLEGAL:
        strcpy(ret_str, "Response to illegal command");
        break;
      case RSP_UNKNOWN:
        strcpy(ret_str, "Response to unknown command");
        break;
      case MSG_SYNC_SEQUENCE:
        strcpy(ret_str, "Sync sequence request");
        break;
      case RSP_SYNC_SEQUENCE:
        strcpy(ret_str, "Sync sequence response");
        break;
      default:
        strcpy(ret_str, "Unknown packet type!");
        break;
    }

    return ret_str;
}

static char*
getErrorCode (const unsigned char error_code) {
    static char code_str[80];

    memset(code_str, '\0', sizeof(code_str));

    switch (error_code) {
      case 1:
        strcpy(code_str, "Single packet missing");
        break;
      case 2:
        strcpy(code_str, "Two or more packets missing");
        break;
      case 3:
        strcpy(code_str, "Many, many packets missing");
        break;
      case 4:
        strcpy(code_str, "Packet sequence number repeated");
        break;
      case 6:
        strcpy(code_str, "Unexpected packet type received");
        break;
      case 7:
        strcpy(code_str, "Packet contents could not be determined");
        break;
      case 8:
        strcpy(code_str, "Status requested for non-existant FBB device");
        break;
      case 9:
        strcpy(code_str, "Illegal setup data sent");
        break;
      case 100:
        strcpy(code_str, "System not ready");
        break;
    }

    return code_str;
}

static double
convertMeasurementRateToDouble (const unsigned char rate[6]) {
    int i;
    double data_rate;
    char data_rate_a[7];

    /*
     * Copy the bytes in status_info->measurementRate into data_rate_a.  I
     * know this is probably being overly cautious, but getting the byte
     * ordering right is important.
     */
    for ( i = 0; i < 6; i++ ) {
        data_rate_a[i] = rate[i];
    }

    /* Terminate the measurement rate string. */
    data_rate_a[6] = '\0';

    /*
     * This conversion is described in the measurementRate row of the
     * General System Status table on page 121 of the MotionStar Operation and
     * Installation Guide.
     */
    data_rate = atof(data_rate_a) / 1000.0;

    return data_rate;
}

static void
dissect_birdnet_data_sys_status (tvbuff_t* tvb, struct SYSTEM_STATUS* status,
                                 double* rate)
{
    int i;

    status->all          = tvb_get_guint8(tvb, BN_DATA_SYS_ALL);
    status->error        = tvb_get_guint8(tvb, BN_DATA_SYS_ERROR);
    status->flockNumber  = tvb_get_guint8(tvb, BN_DATA_SYS_FLOCK_NUM);
    status->serverNumber = tvb_get_guint8(tvb, BN_DATA_SYS_SERVER_NUM);

    status->transmitterNumber =
        tvb_get_guint8(tvb, BN_DATA_SYS_TRANSMITTER_NUM);

    for ( i = 0; i < 6; i++ ) {
        status->measurementRate[i] =
            tvb_get_guint8(tvb, BN_DATA_SYS_MEAS_RATE + i);
    }

    status->chassisNumber  = tvb_get_guint8(tvb, BN_DATA_SYS_CHASSIS_NUM);
    status->chassisDevices = tvb_get_guint8(tvb, BN_DATA_SYS_CHASSIS_DEVS);
    status->firstAddress   = tvb_get_guint8(tvb, BN_DATA_SYS_FIRST_ADDR);

    status->softwareRevision[0] = tvb_get_guint8(tvb, BN_DATA_SYS_SOFTWARE_REV);
    status->softwareRevision[1] = tvb_get_guint8(tvb, BN_DATA_SYS_SOFTWARE_REV + 1);

    *rate = convertMeasurementRateToDouble(status->measurementRate);
}

static void
populate_tree_data_sys_status (tvbuff_t* tvb, proto_tree* tree,
                               const struct SYSTEM_STATUS* status,
                               const double rate)
{
    unsigned char erc_num, xmtr_num;
    proto_item* tf;
    proto_tree *status_tree, *xmtr_tree;

    tf = proto_tree_add_uint(tree, hf_birdnet_data_sys_all, tvb,
                             BN_DATA_SYS_ALL, sizeof(status->all),
                             status->all);

    status_tree = proto_item_add_subtree(tf, ett_birdnet_data_sys_status);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_running, tvb,
                           BN_DATA_SYS_ALL, 1, status->all);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_error, tvb,
                           BN_DATA_SYS_ALL, 1, status->all);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_fbb_error, tvb,
                           BN_DATA_SYS_ALL, 1, status->all);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_local_error,
                           tvb, BN_DATA_SYS_ALL, 1, status->all);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_local_power,
                           tvb, BN_DATA_SYS_ALL, 1, status->all);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_master, tvb,
                           BN_DATA_SYS_ALL, 1, status->all);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_crtsync_type,
                           tvb, BN_DATA_SYS_ALL, 1, status->all);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_sys_all_crtsync, tvb,
                           BN_DATA_SYS_ALL, 1, status->all);

    proto_tree_add_uint(tree, hf_birdnet_data_sys_error, tvb,
                        BN_DATA_SYS_ERROR, sizeof(status->error),
                        status->error);
    proto_tree_add_uint(tree, hf_birdnet_data_sys_flock_num, tvb,
                        BN_DATA_SYS_FLOCK_NUM, sizeof(status->flockNumber),
                        status->flockNumber);
    proto_tree_add_uint(tree, hf_birdnet_data_sys_server_num, tvb,
                        BN_DATA_SYS_SERVER_NUM, sizeof(status->serverNumber),
                        status->serverNumber);

    tf = proto_tree_add_uint(tree, hf_birdnet_data_sys_transmitter_num, tvb,
                             BN_DATA_SYS_TRANSMITTER_NUM,
                             sizeof(status->transmitterNumber),
                             status->transmitterNumber);

    erc_num  = status->transmitterNumber >> 4;
    xmtr_num = status->transmitterNumber & 0x03;

    xmtr_tree = proto_item_add_subtree(tf, ett_birdnet_data_sys_xmtr_info);

    proto_tree_add_uint_format(xmtr_tree, hf_birdnet_data_sys_transmitter_num,
                               tvb, BN_DATA_SYS_TRANSMITTER_NUM,
                               sizeof(status->transmitterNumber),
                               status->transmitterNumber,
                               "FBB Address of ERC: %u",
                               (unsigned int) erc_num);
    proto_tree_add_uint_format(xmtr_tree, hf_birdnet_data_sys_transmitter_num,
                               tvb, BN_DATA_SYS_TRANSMITTER_NUM,
                               sizeof(status->transmitterNumber),
                               status->transmitterNumber,
                               "Transmitter number within ERC: %u",
                               (unsigned int) xmtr_num);

    proto_tree_add_uint_format(tree, hf_birdnet_data_sys_measurement_rate[0],
                               tvb, BN_DATA_SYS_MEAS_RATE, sizeof(guint8),
                               hf_birdnet_data_sys_measurement_rate[0],
                               "Measurement rate: %f", rate);

    proto_tree_add_uint(tree, hf_birdnet_data_sys_chassis_num, tvb,
                        BN_DATA_SYS_CHASSIS_NUM, sizeof(status->chassisNumber),
                        status->chassisNumber);
    proto_tree_add_uint(tree, hf_birdnet_data_sys_chassis_devs, tvb,
                        BN_DATA_SYS_CHASSIS_DEVS,
                        sizeof(status->chassisDevices),
                        status->chassisDevices);
    proto_tree_add_uint(tree, hf_birdnet_data_sys_first_addr, tvb,
                        BN_DATA_SYS_FIRST_ADDR, sizeof(status->firstAddress),
                        status->firstAddress);

    proto_tree_add_uint_format(tree, hf_birdnet_data_sys_software_rev[0], tvb,
                               BN_DATA_SYS_SOFTWARE_REV, sizeof(guint8),
                               status->softwareRevision[0],
                               "Server software revision: %d.%d",
                               (unsigned int) status->softwareRevision[0],
                               (unsigned int) status->softwareRevision[1]);
}

static void
process_fbb_device (tvbuff_t* tvb, proto_tree* tree, const unsigned int id,
                    const guint offset, const unsigned short num_bytes)
{
    proto_item* tf;
    proto_tree* flock_tree;
    guint8 fbb_device;

    fbb_device = tvb_get_guint8(tvb, offset);

    tf = proto_tree_add_text(tree, tvb, offset, num_bytes, "Flock #%u: 0x%02x",
                             id, (unsigned int) fbb_device);

    flock_tree = proto_item_add_subtree(tf, ett_birdnet_data_sys_flock);

    proto_tree_add_boolean(flock_tree,
                           hf_birdnet_data_sys_fbb_device_accessible, tvb,
                           offset, 1, fbb_device);
    proto_tree_add_boolean(flock_tree, hf_birdnet_data_sys_fbb_device_running,
                           tvb, offset, 1, fbb_device);
    proto_tree_add_boolean(flock_tree, hf_birdnet_data_sys_fbb_device_sensor,
                           tvb, offset, 1, fbb_device);
    proto_tree_add_boolean(flock_tree, hf_birdnet_data_sys_fbb_device_erc,
                           tvb, offset, 1, fbb_device);

    if ( fbb_device & FBB_ERC ) {
        proto_tree_add_boolean(flock_tree, hf_birdnet_data_sys_fbb_device_ert3,
                               tvb, offset, 1, fbb_device);
        proto_tree_add_boolean(flock_tree, hf_birdnet_data_sys_fbb_device_ert2,
                               tvb, offset, 1, fbb_device);
        proto_tree_add_boolean(flock_tree, hf_birdnet_data_sys_fbb_device_ert1,
                               tvb, offset, 1, fbb_device);
        proto_tree_add_boolean(flock_tree, hf_birdnet_data_sys_fbb_device_ert0,
                               tvb, offset, 1, fbb_device);
    }
}

static void
dissect_birdnet_data_bird_status (tvbuff_t* tvb,
                                  struct BIRD_STATUS* status)
{
    status->status.status         = tvb_get_guint8(tvb, BN_DATA_BIRD_STATUS);
    status->status.id             = tvb_get_guint8(tvb, BN_DATA_BIRD_ID);
    status->status.softwareRev[0] = tvb_get_guint8(tvb, BN_DATA_BIRD_SOFTWARE_REV);
    status->status.softwareRev[1] = tvb_get_guint8(tvb, BN_DATA_BIRD_SOFTWARE_REV + 1);
    status->status.errorCode      = tvb_get_guint8(tvb, BN_DATA_BIRD_ERROR_CODE);
    status->status.setup          = tvb_get_guint8(tvb, BN_DATA_BIRD_SETUP);
    status->status.dataFormat     = tvb_get_guint8(tvb, BN_DATA_BIRD_DATA_FORMAT);
    status->status.reportRate     = tvb_get_guint8(tvb, BN_DATA_BIRD_REPORT_RATE);
    status->status.scaling[0]     = tvb_get_guint8(tvb, BN_DATA_BIRD_SCALING);
    status->status.scaling[1]     = tvb_get_guint8(tvb, BN_DATA_BIRD_SCALING + 1);
    status->status.hemisphere     = tvb_get_guint8(tvb, BN_DATA_BIRD_HEMISPHERE);
    status->status.FBBaddress     = tvb_get_guint8(tvb, BN_DATA_BIRD_FBB_ADDRESS);
    status->status.spare1         = tvb_get_guint8(tvb, BN_DATA_BIRD_SPARE1);
    status->status.spare2[0]      = tvb_get_guint8(tvb, BN_DATA_BIRD_SPARE2);
    status->status.spare2[1]      = tvb_get_guint8(tvb, BN_DATA_BIRD_SPARE2 + 1);
}

static char*
get_id_string (const unsigned char id) {
    static char ret_str[11];

    switch (id) {
      case 1:
        strcpy(ret_str, "6DFOB     ");
        break;
      case 2:
        strcpy(ret_str, "6DERC     ");
        break;
      case 3:
        strcpy(ret_str, "6DBOF     ");
        break;
      case 4:
        strcpy(ret_str, "PCBIRD    ");
        break;
      case 5:
        strcpy(ret_str, "SPACEPAD  ");
        break;
      case 6:
        strcpy(ret_str, "MOTIONSTAR");
        break;
      case 7:
        strcpy(ret_str, "WIRELESS  ");
        break;
      case 255:
        strcpy(ret_str, "UNKNOWN");
        break;
    }

    return ret_str;
}

static char*
getFormatCode (const unsigned char code) {
    static char code_str[20];

    memset(code_str, '\0', sizeof(code_str));

    switch (code) {
      case 0:
        strcpy(code_str, "No data");
        break;
      case 1:
        strcpy(code_str, "POSITION");
        break;
      case 2:
        strcpy(code_str, "ANGLES");
        break;
      case 3:
        strcpy(code_str, "MATRIX");
        break;
      case 4:
        strcpy(code_str, "POSITION/ANGLES");
        break;
      case 5:
        strcpy(code_str, "POSITION/MATRIX");
        break;
      case 6:
        strcpy(code_str, "<invalid>");
        break;
      case 7:
        strcpy(code_str, "QUATERNION");
        break;
      case 8:
        strcpy(code_str, "POSITION/QUATERNION");
        break;
      default:
        strcpy(code_str, "<invalid>");
        break;
    }

    return code_str;
}

static char*
getHemisphereString (const unsigned char hemi) {
    static char hemi_str[7];

    memset(hemi_str, '\0', sizeof(hemi_str));

    switch (hemi) {
      case 0:
        strcpy(hemi_str, "FRONT");
        break;
      case 1:
        strcpy(hemi_str, "REAR");
        break;
      case 2:
        strcpy(hemi_str, "UPPER");
        break;
      case 3:
        strcpy(hemi_str, "LOWER");
        break;
      case 4:
        strcpy(hemi_str, "LEFT");
        break;
      case 5:
        strcpy(hemi_str, "RIGHT");
        break;
    }

    return hemi_str;
}

static void
populate_tree_data_bird_status (tvbuff_t* tvb, proto_tree* tree,
                                const struct BIRD_STATUS* status)
{
    proto_item* tf;
    proto_tree *status_tree, *setup_tree, *format_tree, *scaling_tree;
    proto_tree* xmtr_tree;
    unsigned char data_size, data_code;
    guint16 sys_code;

    tf = proto_tree_add_uint(tree, hf_birdnet_data_bird_status, tvb,
                             BN_DATA_BIRD_STATUS, sizeof(status->status),
                             status->status.status);

    status_tree = proto_item_add_subtree(tf, ett_birdnet_data_bird_status);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_bird_status_error, tvb,
                           BN_DATA_SYS_ALL, 1, status->status.status);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_bird_status_running,
                           tvb, BN_DATA_SYS_ALL, 1, status->status.status);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_bird_status_reserved1,
                           tvb, BN_DATA_SYS_ALL, 1, status->status.status);
    proto_tree_add_boolean(status_tree, hf_birdnet_data_bird_status_reserved2,
                           tvb, BN_DATA_SYS_ALL, 1, status->status.status);
    proto_tree_add_boolean(status_tree,
                           hf_birdnet_data_bird_status_buttons_present, tvb,
                           BN_DATA_SYS_ALL, 1, status->status.status);
    proto_tree_add_boolean(status_tree,
                           hf_birdnet_data_bird_status_sensor_present, tvb,
                           BN_DATA_SYS_ALL, 1, status->status.status);
    proto_tree_add_boolean(status_tree,
                           hf_birdnet_data_bird_status_transmitter_present,
                           tvb, BN_DATA_SYS_ALL, 1, status->status.status);
    proto_tree_add_boolean(status_tree,
                           hf_birdnet_data_bird_status_transmitter_running,
                           tvb, BN_DATA_SYS_ALL, 1, status->status.status);

    proto_tree_add_uint_format(tree, hf_birdnet_data_bird_id, tvb,
                               BN_DATA_BIRD_ID, sizeof(status->status.id),
                               status->status.id, "Device type: %u (%s)",
                               (unsigned int) status->status.id,
                               get_id_string(status->status.id));
    proto_tree_add_uint_format(tree, hf_birdnet_data_bird_software_rev[0], tvb,
                               BN_DATA_BIRD_SOFTWARE_REV,
                               sizeof(status->status.softwareRev[0]),
                               status->status.softwareRev[0],
                               "Software revision: %u.%u",
                               (unsigned int) status->status.softwareRev[0],
                               (unsigned int) status->status.softwareRev[1]);
    proto_tree_add_uint(tree, hf_birdnet_data_bird_error_code, tvb,
                        BN_DATA_BIRD_ERROR_CODE,
                        sizeof(status->status.errorCode),
                        status->status.errorCode);

    tf = proto_tree_add_uint(tree, hf_birdnet_data_bird_setup, tvb,
                             BN_DATA_BIRD_SETUP, sizeof(status->status.setup),
                             status->status.setup);
    setup_tree = proto_item_add_subtree(tf, ett_birdnet_data_bird_setup);

    proto_tree_add_boolean(setup_tree, hf_birdnet_data_bird_setup_sudden,
                           tvb, BN_DATA_BIRD_SETUP, 1, status->status.setup);
    proto_tree_add_boolean(setup_tree, hf_birdnet_data_bird_setup_xyz_ref,
                           tvb, BN_DATA_BIRD_SETUP, 1, status->status.setup);
    proto_tree_add_boolean(setup_tree, hf_birdnet_data_bird_setup_button_data,
                           tvb, BN_DATA_BIRD_SETUP, 1, status->status.setup);
    proto_tree_add_boolean(setup_tree, hf_birdnet_data_bird_setup_ac_narrow,
                           tvb, BN_DATA_BIRD_SETUP, 1, status->status.setup);
    proto_tree_add_boolean(setup_tree, hf_birdnet_data_bird_setup_ac_wide,
                           tvb, BN_DATA_BIRD_SETUP, 1, status->status.setup);
    proto_tree_add_boolean(setup_tree, hf_birdnet_data_bird_setup_dc, tvb,
                           BN_DATA_BIRD_SETUP, 1, status->status.setup);

    tf = proto_tree_add_uint(tree, hf_birdnet_data_bird_data_format, tvb,
                             BN_DATA_BIRD_DATA_FORMAT,
                             sizeof(status->status.dataFormat),
                             status->status.dataFormat);
    format_tree = proto_item_add_subtree(tf, ett_birdnet_data_bird_data_format);

    data_size = status->status.dataFormat >> 4;
    data_code = status->status.dataFormat & 0x0f;

    proto_tree_add_uint_format(format_tree, hf_birdnet_data_bird_data_format,
                               tvb, BN_DATA_BIRD_DATA_FORMAT,
                               sizeof(status->status.dataFormat),
                               status->status.dataFormat,
                               "Data size: %u words",
                               (unsigned int) data_size);
    proto_tree_add_uint_format(format_tree, hf_birdnet_data_bird_data_format,
                               tvb, BN_DATA_BIRD_DATA_FORMAT,
                               sizeof(status->status.dataFormat),
                               status->status.dataFormat, "Data code: %u (%s)",
                               (unsigned int) data_code,
                               getFormatCode(data_code));
    proto_tree_add_uint(tree, hf_birdnet_data_bird_report_rate, tvb,
                        BN_DATA_BIRD_REPORT_RATE,
                        sizeof(status->status.reportRate),
                        status->status.dataFormat);

    tf = proto_tree_add_uint(tree, hf_birdnet_data_bird_scaling, tvb,
                             BN_DATA_BIRD_SCALING, sizeof(guint16),
                             tvb_get_ntohs(tvb, BN_DATA_BIRD_SCALING));
    scaling_tree = proto_item_add_subtree(tf, ett_birdnet_data_bird_scaling);

    /* XXX: Fix me!! */
/*
    sys_code = tvb_get_ntohs(tvb, BN_DATA_BIRD_SCALING);
    proto_tree_add_uint(scaling_tree, hf_birdnet_data_bird_scaling_sys_code,
                        tvb, BN_DATA_BIRD_SCALING, sizeof(guint16),
                        sys_code >> 12);
    proto_tree_add_uint(scaling_tree, hf_birdnet_data_bird_scaling_fs_meas,
                        tvb, BN_DATA_BIRD_SCALING, sizeof(guint16),
                        tvb_get_ntohs(tvb, BN_DATA_BIRD_SCALING));
*/

    proto_tree_add_uint_format(tree, hf_birdnet_data_bird_hemisphere, tvb,
                               BN_DATA_BIRD_FBB_ADDRESS,
                               sizeof(status->status.hemisphere),
                               status->status.hemisphere,
                               "Hemisphere: %u (%s)",
                               status->status.hemisphere,
                               getHemisphereString(status->status.hemisphere));
    proto_tree_add_uint(tree, hf_birdnet_data_bird_fbb_address, tvb,
                        BN_DATA_BIRD_FBB_ADDRESS,
                        sizeof(status->status.FBBaddress),
                        status->status.FBBaddress);

    tf = proto_tree_add_uint(tree, hf_birdnet_data_bird_transmitter_type, tvb,
                             BN_DATA_BIRD_XMTR_TYPE,
                             sizeof(status->status.transmitterType),
                             status->status.transmitterType);
    xmtr_tree = proto_item_add_subtree(tf, ett_birdnet_data_bird_xmtr_type);

    proto_tree_add_boolean(xmtr_tree,
                           hf_birdnet_data_bird_transmitter_type_ert, tvb,
                           BN_DATA_BIRD_XMTR_TYPE, 1,
                           status->status.transmitterType);
    proto_tree_add_boolean(xmtr_tree,
                           hf_birdnet_data_bird_transmitter_type_srt, tvb,
                           BN_DATA_BIRD_XMTR_TYPE, 1,
                           status->status.transmitterType);
    proto_tree_add_boolean(xmtr_tree,
                           hf_birdnet_data_bird_transmitter_type_pcbird, tvb,
                           BN_DATA_BIRD_XMTR_TYPE, 1,
                           status->status.transmitterType);
    proto_tree_add_boolean(xmtr_tree,
                           hf_birdnet_data_bird_transmitter_type_active, tvb,
                           BN_DATA_BIRD_XMTR_TYPE, 1,
                           status->status.transmitterType);
    proto_tree_add_uint(xmtr_tree,
                        hf_birdnet_data_bird_transmitter_type_selected, tvb,
                        BN_DATA_BIRD_XMTR_TYPE, 1,
                        status->status.transmitterType);
    proto_tree_add_uint(xmtr_tree,
                        hf_birdnet_data_bird_transmitter_type_number, tvb,
                        BN_DATA_BIRD_XMTR_TYPE, 1,
                        status->status.transmitterType);

    proto_tree_add_uint(tree, hf_birdnet_data_bird_spare1, tvb,
                        BN_DATA_BIRD_SPARE1, sizeof(status->status.spare1),
                        status->status.spare1);
}

static guint16
populate_tree_data_birds (tvbuff_t* tvb, proto_tree* tree,
                          const guint16 num_bytes)
{
    guint16 bird, bird_count;
    proto_item* tf;
    proto_tree* bird_tree;
    struct BIRD_STATUS bird_status;
    gint offset;

    bird_count = num_bytes / sizeof(struct BIRD_STATUS);
    offset     = 0;

    if ( bird_count == 1 ) {
        memset(&bird_status, 0, sizeof(bird_status));
        dissect_birdnet_data_bird_status(tvb, &bird_status);
        populate_tree_data_bird_status(tvb, tree, &bird_status);
    }
    else {
        for ( bird = 0; bird < bird_count; bird++ ) {
            tf = proto_tree_add_text(tree, tvb, offset, num_bytes,
                                     "Bird #%u", bird + 1);
            bird_tree = proto_item_add_subtree(tf, ett_birdnet_data_bird);

            memset(&bird_status, 0, sizeof(bird_status));
            dissect_birdnet_data_bird_status(tvb, &bird_status);
            populate_tree_data_bird_status(tvb, bird_tree, &bird_status);

            offset += sizeof(bird_status);
        }
    }

    return bird_count;
}

static guint16
dissect_birdnet_data (tvbuff_t* tvb, proto_tree* tree,
                      const unsigned char type, const unsigned char xtype,
                      guint16 num_bytes)
{
    struct SYSTEM_STATUS sys_status;
    double m_rate;
    guint8 flock;
    gint offset;

    offset = 0;

    switch (type) {
      case RSP_GET_STATUS:
        switch (xtype) {
          /* Server configuration sent from server. */
          case 0:
            dissect_birdnet_data_sys_status(tvb, &sys_status, &m_rate);
            populate_tree_data_sys_status(tvb, tree, &sys_status, m_rate);

            num_bytes -= sizeof(struct SYSTEM_STATUS);
            offset     = sizeof(struct SYSTEM_STATUS);

            for ( flock = 0; flock < sys_status.flockNumber; flock++ ) {
                process_fbb_device(tvb, tree, flock + 1, offset, num_bytes);
                offset++;
                num_bytes--;
            }

            break;
          default:
            num_bytes -= populate_tree_data_birds(tvb, tree, num_bytes);
            break;
        }

        break;
      case MSG_SEND_SETUP:
        switch (xtype) {
          /* Server configuration to be sent to the server. */
          case 0:
            dissect_birdnet_data_sys_status(tvb, &sys_status, &m_rate);
            populate_tree_data_sys_status(tvb, tree, &sys_status, m_rate);
            num_bytes -= sizeof(struct SYSTEM_STATUS);
            break;
          default:
            num_bytes -= populate_tree_data_birds(tvb, tree, num_bytes);
            break;
        }

        break;
    }

    return num_bytes;
}

/* Code to actually dissect the packets */
void
dissect_birdnet(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
/* Set up structures needed to add the protocol subtree and manage it */
    struct HEADER header;
    char* type_str;

    /* Extract the BIRDNET header from the packet. */
    header.sequence             = tvb_get_ntohs(tvb, BN_SEQUENCE);
    header.milliseconds         = tvb_get_ntohs(tvb, BN_INTERVAL);
    header.time[0]              = tvb_get_ntohs(tvb, BN_TIME);
    header.time[1]              = tvb_get_guint8(tvb, BN_TIME + 1);
    header.time[2]              = tvb_get_guint8(tvb, BN_TIME + 2);
    header.time[3]              = tvb_get_guint8(tvb, BN_TIME + 3);
    header.type                 = tvb_get_guint8(tvb, BN_TYPE);
    header.xtype                = tvb_get_guint8(tvb, BN_XTYPE);
    header.protocol             = tvb_get_guint8(tvb, BN_PROTOCOL);
    header.error_code           = tvb_get_guint8(tvb, BN_ERROR_CODE);
    header.error_code_extension = tvb_get_ntohs(tvb, BN_ERROR_CODE_EXT);
    header.number_bytes         = tvb_get_ntohs(tvb, BN_NUM_BYTES);

/* Check if protocol decoding is enabled else decode as data and return */
/*    CHECK_DISPLAY_AS_DATA(proto_birdnet, tvb, pinfo, tree); */

                    /* load the display labels     */
/*    pinfo->current_proto = "BIRDNET"; */

/* Make entries in Protocol column and Info column on summary display */
    if (check_col(pinfo->cinfo, COL_PROTOCOL)) {
        col_set_str(pinfo->cinfo, COL_PROTOCOL, "BN");
    }

/* This field shows up as the "Info" column in the display; you should make
   it, if possible, summarize what's in the packet, so that a user looking
   at the list of packets can tell what type of packet it is. See section 1.5
   for more information.

   "col_add_fstr()" can be used instead of "col_add_str()"; it takes
   "printf()"-like arguments. */

    type_str = getTypeString(header.type);

    if (check_col(pinfo->cinfo, COL_INFO)) {
        col_set_str(pinfo->cinfo, COL_INFO, type_str);
    }

/* In the interest of speed, if "tree" is NULL, don't do any work not
   necessary to generate protocol tree items. */
    if ( tree != NULL ) {
        proto_item *ti;
        proto_tree *birdnet_tree;
        proto_item* data_item;

/* NOTE: The offset and length values in the call to
   "proto_tree_add_item()" define what data bytes to highlight in the hex
   display window when the line in the protocol tree display
   corresponding to that item is selected.

   tvb_length(tvb) is a handy way to highlight all data from the offset to
   the end of the packet. */

/* create display subtree for the protocol */
        ti = proto_tree_add_item(tree, proto_birdnet, tvb, 0, tvb_length(tvb),
                                 FALSE);

        birdnet_tree = proto_item_add_subtree(ti, ett_birdnet_header);

/* add an item to the subtree, see section 1.6 for more information */
        proto_tree_add_uint(birdnet_tree, hf_birdnet_seq, tvb,
                            BN_SEQUENCE, sizeof(header.sequence),
                            header.sequence);
        proto_tree_add_uint_format(birdnet_tree, hf_birdnet_int, tvb,
                                   BN_INTERVAL, sizeof(header.milliseconds),
                                   header.milliseconds,
                                   "Packet interval: %hu (ms)",
                                   header.milliseconds);
        proto_tree_add_uint_format(birdnet_tree, hf_birdnet_type, tvb,
                                   BN_TYPE, sizeof(header.type), header.type,
                                   "Type: %u (%s)", (unsigned int) header.type,
                                   type_str);

        proto_tree_add_uint(birdnet_tree, hf_birdnet_xtype, tvb, BN_XTYPE,
                            sizeof(header.xtype), header.xtype);
        proto_tree_add_uint(birdnet_tree, hf_birdnet_proto, tvb, BN_PROTOCOL,
                            sizeof(header.protocol), header.protocol);

        proto_tree_add_uint_format(birdnet_tree, hf_birdnet_error_code, tvb,
                                   BN_ERROR_CODE, sizeof(header.error_code),
                                   header.error_code, "Error code: %u (%s)",
                                   (unsigned int) header.error_code,
                                   (header.error_code == 0 ?
                                       "None" :
                                       getErrorCode(header.error_code)));

        proto_tree_add_uint(birdnet_tree, hf_birdnet_error_code_ext, tvb,
                            BN_ERROR_CODE_EXT,
                            sizeof(header.error_code_extension),
                            header.error_code_extension);

        data_item = proto_tree_add_uint_format(birdnet_tree,
                                               hf_birdnet_num_bytes, tvb,
                                               BN_NUM_BYTES,
                                               sizeof(header.number_bytes),
                                               header.number_bytes,
                                               "Data block: %hu bytes",
                                               header.number_bytes);

        if ( header.number_bytes > 0 ) {
            tvbuff_t* data_tvb;
            proto_tree* data_tree;

            data_tvb  = tvb_new_subset(tvb, sizeof(struct HEADER), -1, -1);
            data_tree = proto_item_add_subtree(data_item, ett_birdnet_data);

            dissect_birdnet_data(data_tvb, data_tree, header.type,
                                 header.xtype, header.number_bytes);
        }
    }

/* If this protocol has a sub-dissector call it here, see section 1.8 */
}


/* Register the protocol with Ethereal */

/* this format is require because a script is used to build the C function
   that calls all the protocol registration.
*/

static const true_false_string flags_set_truth = { "Set", "Not set" };
static const true_false_string flags_set_yn    = { "Yes", "No" };

void
proto_register_birdnet(void)
{
/* Setup list of header fields  See Section 1.6.1 for details*/
    static hf_register_info hf[] = {
        { &hf_birdnet_seq,
          { "Sequence number", "birdnet.sequence", FT_UINT16, BASE_DEC, NULL,
            0x0, "", HFILL
          }
        },
        { &hf_birdnet_int,
          { "Interval time between packets", "birdnet.interval", FT_UINT16,
            BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_time[0],
          { "Time", "birdnet.time.0", FT_UINT8, BASE_DEC, NULL, 0x0, "", HFILL }
        },
        { &hf_birdnet_time[1],
          { "Time", "birdnet.time.1", FT_UINT8, BASE_DEC, NULL, 0x0, "",
            HFILL
          }
        },
        { &hf_birdnet_time[2],
          { "Time", "birdnet.time.2", FT_UINT8, BASE_DEC, NULL, 0x0, "",
            HFILL
          }
        },
        { &hf_birdnet_time[3],
          { "Time", "birdnet.time.3", FT_UINT8, BASE_DEC, NULL, 0x0, "",
            HFILL
          }
        },
        { &hf_birdnet_type,
          { "Command/response code", "birdnet.type", FT_UINT8, BASE_DEC, NULL,
            0x0, "", HFILL
          }
        },
        { &hf_birdnet_xtype,
          { "Extension to command/response code", "birdnet.xtype", FT_UINT8,
            BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_proto,
          { "Protocol", "birdnet.protocol", FT_UINT8, BASE_DEC, NULL, 0x0, "",
            HFILL
          }
        },
        { &hf_birdnet_error_code,
          { "Error code", "birdnet.error_code", FT_UINT8, BASE_DEC, NULL, 0x0,
            "", HFILL
          }
        },
        { &hf_birdnet_error_code_ext,
          { "Error code extension", "birdnet.error_code_ext", FT_UINT16,
            BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_num_bytes,
          { "Data field size", "birdnet.number_bytes", FT_UINT16, BASE_DEC,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_all,
          { "General system status", "birdnet.data.system.all", FT_UINT8,
            BASE_HEX, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_all_running,
          { "System running", "birdnet.data.system.all.running", FT_BOOLEAN,
            8, TFS(&flags_set_truth), BN_SYSTEM_RUNNING, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_all_error,
          { "System error", "birdnet.data.system.all.error", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_SYSTEM_ERROR, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_all_fbb_error,
          { "FBB bus error", "birdnet.data.system.all.fbb_error", FT_BOOLEAN,
            8, TFS(&flags_set_truth), BN_SYSTEM_FBB_ERROR, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_all_local_error,
          { "Local chassis error", "birdnet.data.system.all.local_error",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_SYSTEM_LOCAL_ERROR, "",
            HFILL
          }
        },
        { &hf_birdnet_data_sys_all_local_power,
          { "Local chassis power error", "birdnet.data.system.all.local_power",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_SYSTEM_LOCAL_POWER, "",
            HFILL
          }
        },
        { &hf_birdnet_data_sys_all_master,
          { "Master", "birdnet.data.system.all.master", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_SYSTEM_MASTER, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_all_crtsync_type,
          { "CRT sync type defined", "birdnet.data.system.all.crtsync_type",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_SYSTEM_CRTSYNC_TYPE, "",
            HFILL
          }
        },
        { &hf_birdnet_data_sys_all_crtsync,
          { "CRT sync mode enabled", "birdnet.data.system.all.crtsync",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_SYSTEM_CRTSYNC, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_error,
          { "Error code", "birdnet.data.system.error", FT_UINT8, BASE_DEC,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_flock_num,
          { "Number of devices on FBB bus", "birdnet.data.system.flock_number",
            FT_UINT8, BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_server_num,
          { "Number of servers in the system",
            "birdnet.data.system.server_number", FT_UINT8, BASE_DEC, NULL,
            0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_transmitter_num,
          { "Transmitter address", "birdnet.data.system.transmitter_number",
            FT_UINT8, BASE_HEX, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_measurement_rate[0],
          { "Measurement rate", "birdnet.data.system.measurement_rate.0",
            FT_UINT8, BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_measurement_rate[1],
          { "BIRDNET", "birdnet.data.system.measurement_rate.1", FT_UINT8,
            BASE_DEC, NULL, 0x0, "Measurement rate", HFILL
          }
        },
        { &hf_birdnet_data_sys_measurement_rate[2],
          { "BIRDNET", "birdnet.data.system.measurement_rate.2", FT_UINT8,
            BASE_DEC, NULL, 0x0, "Measurement rate", HFILL
          }
        },
        { &hf_birdnet_data_sys_measurement_rate[3],
          { "BIRDNET", "birdnet.data.system.measurement_rate.3", FT_UINT8,
            BASE_DEC, NULL, 0x0, "Measurement rate", HFILL
          }
        },
        { &hf_birdnet_data_sys_measurement_rate[4],
          { "BIRDNET", "birdnet.data.system.measurement_rate.4", FT_UINT8,
            BASE_DEC, NULL, 0x0, "Measurement rate", HFILL
          }
        },
        { &hf_birdnet_data_sys_measurement_rate[5],
          { "BIRDNET", "birdnet.data.system.measurement_rate.5", FT_UINT8,
            BASE_DEC, NULL, 0x0, "Measurement rate", HFILL
          }
        },
        { &hf_birdnet_data_sys_chassis_num,
          { "Chassis ID number", "birdnet.data.system.chassis_number",
            FT_UINT8, BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_chassis_devs,
          { "Number of devices in chassis",
            "birdnet.data.system.chassis_devices", FT_UINT8, BASE_DEC, NULL,
            0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_first_addr,
          { "FBB address of first device", "birdnet.data.system.first_addr",
            FT_UINT8, BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_software_rev[0],
          { "Server software revision", "birdnet.data.system.software_rev[0]",
            FT_UINT8, BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_software_rev[1],
          { "BIRDNET", "birdnet.data.system.software_rev[1]", FT_UINT8,
            BASE_DEC, NULL, 0x0, "Software revision of server app", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device,
          { "FBB device", "birdnet.data.system.fbb_device",
            FT_BOOLEAN, BASE_HEX, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_accessible,
          { "Accessible", "birdnet.data.system.fbb_device.accessible",
            FT_BOOLEAN, 8, TFS(&flags_set_yn), FBB_ACCESS, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_running,
          { "Running", "birdnet.data.system.fbb_device.running", FT_BOOLEAN,
            8, TFS(&flags_set_yn), FBB_RUNNING, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_sensor,
          { "Has sensor", "birdnet.data.system.fbb_device.sensor", FT_BOOLEAN,
            8, TFS(&flags_set_yn), FBB_SENSOR, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_erc,
          { "Is ERC", "birdnet.data.system.fbb_device.erc", FT_BOOLEAN, 8,
            TFS(&flags_set_yn), FBB_ERC, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_ert3,
          { "ERT #3 present", "birdnet.data.system.fbb_device.ert3",
            FT_BOOLEAN, 8, TFS(&flags_set_yn), FBB_ERT3, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_ert2,
          { "ERT #2 present", "birdnet.data.system.fbb_device.ert2",
            FT_BOOLEAN, 8, TFS(&flags_set_yn), FBB_ERT2, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_ert1,
          { "ERT #1 present", "birdnet.data.system.fbb_device.ert1",
            FT_BOOLEAN, 8, TFS(&flags_set_yn), FBB_ERT1, "", HFILL
          }
        },
        { &hf_birdnet_data_sys_fbb_device_ert0,
          { "ERT #0 present", "birdnet.data.system.fbb_device.ert0",
            FT_BOOLEAN, 8, TFS(&flags_set_yn), FBB_ERT0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_status,
          { "Bird status", "birdnet.data.bird.status", FT_UINT8, BASE_HEX,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_status_error,
          { "Flock card has error", "birdnet.data.bird.status.error",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_BIRD_ERROR, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_status_running,
          { "Flock initializes and configures",
            "birdnet.data.bird.status.running", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_RUNNING, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_status_reserved1,
          { "Reserved", "birdnet.data.bird.status.reserved1", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_RESERVED1, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_status_reserved2,
          { "Reserved", "birdnet.data.bird.status.reserved2", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_RESERVED2, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_status_buttons_present,
          { "Device has buttons", "birdnet.data.bird.status.buttons_present",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_BIRD_BUTTONS_PRESENT, "",
            HFILL
          }
        },
        { &hf_birdnet_data_bird_status_sensor_present,
          { "Device has a sensor", "birdnet.data.bird.status.sensor_present",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_BIRD_SENSOR_PRESENT, "",
            HFILL
          }
        },
        { &hf_birdnet_data_bird_status_transmitter_present,
          { "Device has a transmitter",
            "birdnet.data.bird.status.transmitter_present", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_TRANSMITTER_PRESENT, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_status_transmitter_running,
          { "Device has a running transmitter",
            "birdnet.data.bird.status.transmitter_running", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_TRANSMITTER_RUNNING, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_id,
          { "Device type", "birdnet.data.bird.id", FT_UINT8, BASE_DEC, NULL,
            0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_software_rev[0],
          { "Software revision", "birdnet.data.bird.software_rev",
            FT_UINT8, BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_error_code,
          { "Error code", "birdnet.data.bird.error_code", FT_UINT8, BASE_DEC,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_setup,
          { "Device setup", "birdnet.data.bird.setup", FT_UINT8, BASE_HEX,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_setup_sudden,
          { "Ignore sudden output change", "birdnet.data.bird.setup.sudden",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_BIRD_SETUP_SUDDEN, "",
            HFILL
          }
        },
        { &hf_birdnet_data_bird_setup_xyz_ref,
          { "Position derived from XYZ reference frame",
            "birdnet.data.bird.setup.xyz_ref", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_SETUP_XYZ_REF, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_setup_button_data,
          { "Button data is appended",
            "birdnet.data.bird.setup.button_data", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_SETUP_BUTTON_DATA, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_setup_ac_narrow,
          { "AC narrow filter used",
            "birdnet.data.bird.setup.ac_narrow", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_SETUP_AC_NARROW, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_setup_ac_wide,
          { "AC wide filter used",
            "birdnet.data.bird.setup.ac_wide", FT_BOOLEAN, 8,
            TFS(&flags_set_truth), BN_BIRD_SETUP_AC_WIDE, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_setup_dc,
          { "DC filter used",
            "birdnet.data.bird.setup.dc", FT_BOOLEAN, 8, TFS(&flags_set_truth),
            BN_BIRD_SETUP_DC, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_data_format,
          { "Data format", "birdnet.data.bird.data_format", FT_UINT8, BASE_HEX,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_report_rate,
          { "Report rate", "birdnet.data.bird.report_rate", FT_UINT8, BASE_DEC,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_scaling,
          { "Scaling", "birdnet.data.bird.scaling", FT_UINT16, BASE_HEX,
            NULL, 0x0, "", HFILL
          }
        },
/*
        { &hf_birdnet_data_bird_scaling_sys_code,
          { "System code", "birdnet.data.bird.scaling.sys_code", 16, BASE_DEC,
            NULL, 0xf000, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_scaling_fs_meas,
          { "Full-scale measurement", "birdnet.data.bird.scaling.fs_meas",
            16, BASE_DEC, NULL, 0x0fff, "", HFILL
          }
        },
*/
        { &hf_birdnet_data_bird_hemisphere,
          { "Hemisphere", "birdnet.data.bird.hemisphere", FT_UINT8, BASE_DEC,
            NULL, 0x0fff, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_fbb_address,
          { "FBB address", "birdnet.data.bird.fbb_address", FT_UINT8, BASE_DEC,
            NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_transmitter_type,
          { "Transmitter type", "birdnet.data.bird.xmtr_type", FT_UINT8,
            BASE_HEX, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_transmitter_type_ert,
          { "ERT present", "birdnet.data.bird.xmtr_type.ert", FT_BOOLEAN,
            8, TFS(&flags_set_truth), BN_BIRD_XMTR_TYPE_ERT, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_transmitter_type_srt,
          { "SRT present", "birdnet.data.bird.xmtr_type.srt", FT_BOOLEAN,
            8, TFS(&flags_set_truth), BN_BIRD_XMTR_TYPE_SRT, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_transmitter_type_pcbird,
          { "pcBIRD present", "birdnet.data.bird.xmtr_type.pcbird", FT_BOOLEAN,
            8, TFS(&flags_set_truth), BN_BIRD_XMTR_TYPE_PCBIRD, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_transmitter_type_active,
          { "Transmitter running", "birdnet.data.bird.xmtr_type.active",
            FT_BOOLEAN, 8, TFS(&flags_set_truth), BN_BIRD_XMTR_TYPE_ACTIVE, "",
            HFILL
          }
        },
        { &hf_birdnet_data_bird_transmitter_type_selected,
          { "Selected transmitter", "birdnet.data.bird.xmtr_type.selected",
            FT_UINT8, BASE_DEC, NULL, BN_BIRD_XMTR_TYPE_SELECTED, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_transmitter_type_number,
          { "Number of transmitters conntected to ERC",
            "birdnet.data.bird.xmtr_type.number",
            FT_UINT8, BASE_DEC, NULL, BN_BIRD_XMTR_TYPE_NUMBER, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_spare1,
          { "<Reserved>", "birdnet.data.bird.spare1", FT_UINT8,
            BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
        { &hf_birdnet_data_bird_spare2,
          { "<Reserved>", "birdnet.data.bird.spare2", FT_UINT8,
            BASE_DEC, NULL, 0x0, "", HFILL
          }
        },
    };

/* Setup protocol subtree array */
    static gint *ett[] = {
        &ett_birdnet_header,
        &ett_birdnet_data,
        &ett_birdnet_data_sys_status,
        &ett_birdnet_data_sys_xmtr_info,
        &ett_birdnet_data_sys_flock,
        &ett_birdnet_data_bird,
        &ett_birdnet_data_bird_status,
        &ett_birdnet_data_bird_setup,
        &ett_birdnet_data_bird_data_format,
        &ett_birdnet_data_bird_scaling,
        &ett_birdnet_data_bird_xmtr_type
    };

/* Register the protocol name and description */
    proto_birdnet = proto_register_protocol("Ascension BIRDNET", "BIRDNET",
                                            "birdnet");

/* Required function calls to register the header fields and subtrees used */
    proto_register_field_array(proto_birdnet, hf, array_length(hf));
    proto_register_subtree_array(ett, array_length(ett));
}


/* If this dissector uses sub-dissector registration add a registration routine.
   This format is required because a script is used to find these routines and
   create the code that calls these routines.
*/
void proto_reg_handoff_birdnet(void)
{
    dissector_handle_t birdnet_handle;

    birdnet_handle = create_dissector_handle(dissect_birdnet, proto_birdnet);
    dissector_add("udp.port", UDP_PORT_BIRDNET, birdnet_handle);
    dissector_add("tcp.port", TCP_PORT_BIRDNET, birdnet_handle);
}

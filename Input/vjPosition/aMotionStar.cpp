/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <strings.h>    /* Needed for bzero(3) on some platforms */
#include <math.h>
#include <unistd.h>     /* Needed for close(2) */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>  /* For inet_addr(3) */
#include <errno.h>

#include <Input/vjPosition/aMotionStar.h>

#define RAD2DEG(x) ((x) * 180.0 / M_PI)


namespace FLOCK {

const unsigned char ERROR               = 0x80;
const unsigned char RUNNING             = 0x40;
//const unsigned char RESERVED1           = 0x20;
//const unsigned char RESERVED2           = 0x10;
const unsigned char BUTTONS_PRESENT     = 0x08;
const unsigned char SENSOR_PRESENT      = 0x04;
const unsigned char TRANSMITTER_PRESENT = 0x02;
const unsigned char TRANSMITTER_RUNNING = 0x01;

const unsigned char SUDDEN_OUTPUT_CHANGE = 0x20;
const unsigned char XYZ_REFERENCE        = 0x10;
const unsigned char APPEND_BUTTON_DATA   = 0x08;
const unsigned char AC_NARROW_FILTER     = 0x04;
const unsigned char AC_WIDE_FILTER       = 0x02;
const unsigned char DC_FILTER            = 0x01;

// ----------------------------------------------------------------------------
// Convert the given Flock data format into a human-readable string that
// names the format.
// ----------------------------------------------------------------------------
std::string
getFormatName (const data_format format) {
    std::string name;

    switch (format) {
      case NO_BIRD_DATA:
      case INVALID:
        name = "NONE";
        break;
      case POSITION:
        name = "POSITION";
        break;
      case ANGLES:
        name = "ANGLES";
        break;
      case MATRIX:
        name = "MATRIX";
        break;
      case POSITION_ANGLES:
        name = "POSITION/ANGLES";
        break;
      case POSITION_MATRIX:
        name = "POSITION/MATRIX";
        break;
      case QUATERNION:
        name = "QUATERNION";
        break;
      case POSITION_QUATERNION:
        name = "POSITION/QUATERNION";
        break;
      case FEEDTHROUGH_DATA:
        name = "FEEDTHROUGH DATA";
        break;
    }

    return name;
}

// ----------------------------------------------------------------------------
// Convert the given Flock hemisphere ID into a human-readable string that
// names the hemisphere.
// ----------------------------------------------------------------------------
std::string
getHemisphereName (const hemisphere hemi) {
    std::string name;

    switch (hemi) {
      case FRONT_HEMISPHERE:
        name = "Front";
        break;
      case REAR_HEMISPHERE:
        name = "Rear";
        break;
      case UPPER_HEMISPHERE:
        name = "Upper";
        break;
      case LOWER_HEMISPHERE:
        name = "Lower";
        break;
      case LEFT_HEMISPHERE:
        name = "Left";
        break;
      case RIGHT_HEMISPHERE:
        name = "Right";
        break;
    }

    return name;
}

};

namespace FBB {

const unsigned char ACCESS  = 0x80;
const unsigned char RUNNING = 0x40;
const unsigned char SENSOR  = 0x20;
const unsigned char ERC     = 0x10;
const unsigned char ERT3    = 0x08;
const unsigned char ERT2    = 0x04;
const unsigned char ERT1    = 0x02;
const unsigned char ERT0    = 0x01;

};

namespace BIRDNET {

const unsigned char MSG_WAKE_UP        = 10;
const unsigned char RSP_WAKE_UP        = 20;
const unsigned char MSG_SHUT_DOWN      = 11;
const unsigned char RSP_SHUT_DOWN      = 21;
const unsigned char MSG_GET_STATUS     = 101;
const unsigned char MSG_SEND_SETUP     = 102;
const unsigned char MSG_SINGLE_SHOT    = 103;
const unsigned char MSG_RUN_CONTINUOUS = 104;
const unsigned char MSG_STOP_DATA      = 105;
const unsigned char RSP_GET_STATUS     = 201;
const unsigned char RSP_SEND_SETUP     = 202;
const unsigned char RSP_RUN_CONTINUOUS = 204;
const unsigned char RSP_STOP_DATA      = 205;
const unsigned char DATA_PACKET_MULTI  = 210;
const unsigned char RSP_ILLEGAL        = 40;
const unsigned char RSP_UNKNOWN        = 50;
const unsigned char MSG_SYNC_SEQUENCE  = 30;
const unsigned char RSP_SYNC_SEQUENCE  = 31;

const unsigned char BN_PROTOCOL = 3;

};

extern int errno;


// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static std::ostream&
operator<< (std::ostream& out, const BIRDNET::SINGLE_BIRD_STATUS& device) {
    out << " FBB address: " << (unsigned int) device.FBBaddress << "\n";
//    out << " Accessible: " << (m_birds[i]->accessible ? "YES" : "NO") << "\n";
//    out << " Running: " << (m_birds[i]->running ? "YES" : "NO") << "\n";
    out << " Data format: " << (unsigned int) device.dataFormat << "\n";
    out << " Hemisphere: " << (unsigned int) device.hemisphere << "\n";
    out << " Report rate: " << (unsigned int) device.reportRate << "\n";
    out << " Sudden change filtering: "
        << (device.setup & FLOCK::SUDDEN_OUTPUT_CHANGE ? "ON" : "OFF") << "\n";
    out << " Button data: "
        << (device.setup & FLOCK::APPEND_BUTTON_DATA ? "ON" : "OFF") << "\n";
    out << " AC narrow filter: "
        << (device.setup & FLOCK::AC_NARROW_FILTER ? "ON" : "OFF") << "\n";
    out << " AC wide filter: "
        << (device.setup & FLOCK::AC_WIDE_FILTER ? "ON" : "OFF") << "\n";
    out << " DC filter: "
        << (device.setup & FLOCK::DC_FILTER ? "ON" : "OFF") << "\n";

    return out;
}


// ============================================================================
// Public methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Constructor.  This initializes member variables and determines the
// endianness of the host machine.
// ----------------------------------------------------------------------------
aMotionStar::aMotionStar(const char* address, const unsigned short port,
                         const BIRDNET::protocol proto, const bool master,
                         const FLOCK::hemisphere hemisphere,
                         const FLOCK::data_format bird_format,
                         const BIRDNET::run_mode run_mode,
                         const unsigned char report_rate,
                         const double measurement_rate,
                         const unsigned int birds_requested)
    : m_active(false), m_socket(-1), m_port(port), m_proto(proto),
      m_master(master), m_seq_num(0), m_cur_mrate(0.0),
      m_measurement_rate(measurement_rate), m_run_mode(run_mode),
      m_hemisphere(hemisphere), m_bird_format(bird_format),
      m_report_rate(report_rate), m_birds_requested(birds_requested),
      m_birds_active(0), m_unit_conv(1.0)
{
    union {
        char	c[sizeof(short)];
        short	value;
    } endian;

    // Initialize member variables using passed parameters.
    m_address = address;

    // These are hard-coded because they are always going to be the same
    // values.
    m_xmtr_rot_scale = 180.0;
    m_xmtr_divisor   = 32767.0;

    // Set this to -1.0 so that we can test its value again later to ensure
    // that it was set before trying to do any sampling.
    m_xmtr_pos_scale = -1.0;

    // Determine the endianness of the host platform.  A value of true for
    // m_big_endian means that the host use big endian byte order.  false of
    // course means that it is little endian.  The way this works is that we
    // access the first byte of endian.value directly.  If it is 1, the host
    // treats that as the high-order byte.  Otherwise, it is the low-order
    // byte.
    endian.value = 256;
    m_big_endian = (endian.c[0] ? true : false);
}

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
aMotionStar::~aMotionStar () {
    unsigned int i;

    if ( isActive() ) {
        stop();
    }

    for ( i = 0; i < m_erc_vec.size(); i++ ) {
        delete m_erc_vec[i];
    }

    for ( i = 0; i < m_birds.size(); i++ ) {
        delete m_birds[i];
    }
}

// ----------------------------------------------------------------------------
// Initializes the driver, setting the status for each bird.
// ----------------------------------------------------------------------------
int
aMotionStar::start () {
    int retval;

    retval = 0;

    // Ensure that an address string has been defind for the server before
    // trying to make the connection to the server.
    if ( m_address.size() > 0 ) {
        fprintf(stderr, "\n[aMotionStar] Connecting to %s ...\n",
                m_address.c_str());

        // Create the socket based on the protocol in use.
        switch (m_proto) {
          case BIRDNET::UDP:
            m_socket = socket(PF_INET, SOCK_DGRAM, 0);
            break;
          case BIRDNET::TCP:
            m_socket = socket(PF_INET, SOCK_STREAM, 0);
            break;
        }

        // If the socket could not be created, we are in trouble.
        if ( m_socket < 0 ) {
            fprintf(stderr, "[aMotionStar] Could not create %s socket: %s\n",
                    ((m_proto == BIRDNET::UDP) ? "datagram" : "stream"),
                    strerror(errno));
            retval = -2;
        }
        // Otherwise, keep on truckin'.
        else {
            struct sockaddr_in server_addr;
            struct hostent* host_entry;
            int status;

            fprintf(stderr, "[aMotionStar] %s socket created\n",
                    ((m_proto == BIRDNET::UDP) ? "Datagram" : "Stream"));

            // Fill in the structure with the address of the server to which
            // we will connect.
            bzero((char*) &server_addr, sizeof(server_addr));
            server_addr.sin_family = PF_INET;
            server_addr.sin_port   = htons(m_port);

            // Try to look up address by name.  This will work for an IP
            // address too, but we fall back on inet_addr(3) below just to be
            // safe.
            host_entry = gethostbyname(m_address.c_str());

            // If host_entry is non-NULL, we found an IP address for the
            // hostname in address.  Move that address into the server struct.
            if ( host_entry != NULL ) {
                memmove((void*) &server_addr.sin_addr.s_addr,
                        (void*) host_entry->h_addr,
                        sizeof(server_addr.sin_addr.s_addr));
            }
            // Otherwise, assume that the value in address is already an IP
            // address and use inet_addr(3) on it.
            else {
                server_addr.sin_addr.s_addr = inet_addr(m_address.c_str());
            }

            fprintf(stderr, "[aMotionStar] Got inet address of server\n");

            // Connect to the server.  If this is a datagram socket, it will
            // set the default recipient of all future packets, so this is a
            // valid, safe operation regardless of the protocol being used.
            status = connect(m_socket, (struct sockaddr*) &server_addr,
                             sizeof(server_addr));

            if ( status == -1 ) {
                perror("[aMotionStar] Cannot connect to server");
                retval = -1;
            }
            else {
                fprintf(stderr, "[aMotionStar] Connected to server\n");

                // Now that we are connected, send a wake-up call to the
                // server.
                if ( sendWakeUp() != 0 ) {
                    fprintf(stderr,
                            "[aMotionStar] Could not wake up server\n");
                    retval = -1;
                }
                else {
                    BIRDNET::SYSTEM_STATUS* sys_status;

                    fprintf(stderr,
                            "[aMotionStar] The Sleeper has awakened!\n");

                    // Get the general system status and save it for later.
                    sys_status = getSystemStatus();

                    if ( sys_status == NULL ) {
                        fprintf(stderr, "[aMotionStar] WARNING: Failed to "
                                "read system status\n");
                    }
                    // We only try to set the system status if we got a valid
                    // copy of the current system status.
                    else {
                        fprintf(stderr, "[aMotionStar] Got system status\n");

                        // As long as a positive, non-zero measurement rate is
                        // given, send it to the MotionStar chassis as a system
                        // configuration parameter.
                        if ( m_measurement_rate > 0.0 ) {
                            std::string str_rate;
                            int status;

                            // Bounds checking on the measumrent rate.
                            if ( m_measurement_rate > 144.0 ) {
                                m_measurement_rate = 144.0;
                            }
                            else if ( m_measurement_rate < 20.0 ) {
                                m_measurement_rate = 20.0;
                            }

                            convertMeasurementRate(m_measurement_rate,
                                                   str_rate);
                            status = setSystemStatus(sys_status,
                                                     sys_status->transmitterNumber,
                                                     str_rate.c_str());

                            if ( status != 0 ) {
                                fprintf(stderr, "[aMotionStar] WARNING: Failed "
                                        "to set system status\n");
                            }
                            else {
                                // I use std::cerr here so that I don't have
                                // to deal with fprintf(3)'s float vs. double
                                // formatting.
                                std::cerr << "[aMotionStar] Set measurement "
                                          << "rate to " << m_measurement_rate
                                          << std::endl;
                            }
                        }
                    }

                    // Configure each of the birds.
                    configureBirds();

                    // If the desired run mode is BIRDNET::CONTINUOUS, we need
                    // to request that the data start coming from the server.
                    // If it is BIRDNET::SINGLE_SHOT, there is nothing to do.
                    if ( m_run_mode == BIRDNET::CONTINUOUS ) {
                        if ( setContinuous() != 0 ) {
                            fprintf(stderr,
                                    "[aMotionStar] WARNING: Continuous data "
                                    "request failed!\n");
                        }
                        else {
                            fprintf(stderr,
                                    "[aMotionStar] Continuous data requested\n");
                        }
                    }

                    // Ensure that the position scaling factor has been set
                    // by this point.  If it has not been set, we are in big,
                    // big trouble since no useful data can be extracted from
                    // the server's data packets.
                    if ( m_xmtr_pos_scale == -1.0 ) {
                        fprintf(stderr,
                                "[aMotionStar] FATAL ERROR: Position scaling "
                                "factor unknown!\n");
                        retval = -4;
                    }
                    else {
                        fprintf(stderr, "[aMotionStar] Driver setup done\n\n");

                        if ( sys_status != NULL ) {
                            printSystemStatus(sys_status);
                            delete sys_status;
                        }

                        printDeviceStatus();

                        // The device setup has completed successfully, so we
                        // are now considered active.  The return value is
                        // left at 0 indicating successful startup.
                        m_active = true;
                    }
                }
            }
        }
    }
    // If no address has been set, we cannot start.
    else {
        retval = -3;
    }

    return retval;
}

// ----------------------------------------------------------------------------
// Stop the data flow (if it is in continuous mode), shut down the server and
// close the connection to it.
// ----------------------------------------------------------------------------
void
aMotionStar::stop () {
    stopData();
    shutdown();

    // Close the socket.
    if ( m_socket != -1 ) {
        close(m_socket);
        m_socket = -1;
    }

    // We reset the sequence number to 0 so that if the driver is restarted
    // later, the client will resync with the server starting with sequence
    // number 0.
    m_seq_num = 0;
}

// ----------------------------------------------------------------------------
// Based on the current run mode, a single sample is taken (run mode is
// BIRDNET::SINGLE_SHOT), or continuous samples are taken (run mode is
// BIRDNET::CONTINUOUS).
// ----------------------------------------------------------------------------
void
aMotionStar::sample () {
    BIRDNET::DATA_PACKET recv_pkt;

    // If we are running in single-shot mode, we need to request a data
    // packet from the server.  Otherwise, all incoming packets are data
    // packets contaning the information we want.
    if ( m_run_mode == BIRDNET::SINGLE_SHOT ) {
        BIRDNET::HEADER data_req(BIRDNET::MSG_SINGLE_SHOT);

        data_req.sequence = htons(m_seq_num);
        m_seq_num++;
        sendMsg(&data_req, sizeof(BIRDNET::HEADER));
    }

    // First, we need to read the header for the incoming data packet so we
    // know how much data to expect.
    getRsp(&recv_pkt.header, sizeof(BIRDNET::HEADER));

    // Record the current sequence number so that we know what to use the
    // next time we need to send a packet to the server.
    m_seq_num = ntohs(recv_pkt.header.sequence);

    if ( recv_pkt.header.error_code != 0 ) {
        printError(recv_pkt.header.error_code);
    }
    else {
        // Test the type of the packet read just to be safe.
        if ( recv_pkt.header.type == BIRDNET::DATA_PACKET_MULTI ) {
            char* base_ptr;
            BIRDNET::DATA_RECORD* rec_ptr;
            unsigned char format_code;
            unsigned int rec_data_words;
            size_t rec_data_size;

            getRsp(&recv_pkt.buffer, ntohs(recv_pkt.header.number_bytes));

            // Use a char* for doing pointer arithmetic.  It starts at the
            // beginning of the received packet's buffer field.
            base_ptr = (char*) &recv_pkt.buffer[0];

            for ( unsigned char bird = 0; bird < m_birds_active; bird++ ) {
                // Set the record pointer to the current data record's address
                // as defined by base_ptr.
                rec_ptr = (BIRDNET::DATA_RECORD*) base_ptr;

                // The least significant four bits of the data_info field
                // contain the number of words (2 bytes) of formatted data.
                // m_birds[bird]->data_words already has this value, but we
                // read it from the packet to be safe.
                rec_data_words = (rec_ptr->data_info) & 0x0f;
                rec_data_size  = rec_data_words * 2;

                if ( rec_data_words != m_birds[bird]->data_words ) {
                    fprintf(stderr,
                            "[aMotionStar] WARNING: Expecting %u data words from bird %u, got %u\n"
                            "                       You may have requested more birds than you\n"
                            "                       have connected, or the birds may not be\n"
                            "                       connected sequentially\n"
                            "                       Verify that your configuratoin is correct\n",
                            m_birds[bird]->data_words, bird, rec_data_words);
                }

                // Get the four most significant bits of the data_info field.
                // This gives the format code.  See page 134 of the Operation
                // Guide for more information.  m_birds[bird]->format already
                // has this set, but again, we read it from the current packet
                // just to be safe.  This is especially important because an
                // error may have occurred thus giving a format code of 15.
                format_code = (rec_ptr->data_info >> 4) & 0x0f;

                if ( format_code != m_birds[bird]->format ) {
                    fprintf(stderr,
                            "[aMotionStar] WARNING: Expecting format %u from "
                            "bird %u, got %u\n", m_birds[bird]->format, bird,
                            format_code);
                }

                // If the format code is 15, an error occurred in sampling the
                // data, and therefore the data block is invalid and should be
                // ignored.
                if ( format_code == 15 ) {
                    fprintf(stderr,
                            "[aMotionStar] An error occurred in sampling\n");
                }
                // Now that we have the size of the data block in the current
                // record, we can read it into the current bird's data block.
                // Since it uses a union, we can just read into its base
                // address.
                else {
                    memcpy(&m_birds[bird]->data, &rec_ptr->data,
                           rec_data_size);
                }

                // The size of each record may vary depending on the data
                // format in use.  Increment the address of the current
                // record by the statically known sizes and by the size of
                // the current record's data block (each bird may have a
                // different sized data block).
                base_ptr += sizeof(rec_ptr->address) +
                                sizeof(rec_ptr->data_info) + rec_data_size;

                // If the most significant bit is set in address, then there
                // is also button data for this record.
                if ( rec_ptr->address & 0x80 ) {
                    // Copy the button data into the current bird's info block.
                    m_birds[bird]->buttons[0] = rec_ptr->button_data[0];
                    m_birds[bird]->buttons[1] = rec_ptr->button_data[1];

                    // Increment the pointer to account for the button data.
                    base_ptr += sizeof(rec_ptr->button_data);
                }
            }
        }
        // It's unlikely that we will have received the wrong packet type at
        // this point, but this message will warn us if we did.
        else {
            fprintf(stderr,
                    "[aMotionStar] WARNING: Got unexpected packet type %u in "
                    "sample()!\n", recv_pkt.header.type);
        }
    }
}

// ----------------------------------------------------------------------------
// Stops the data flow if it is in continuous mode.
// ----------------------------------------------------------------------------
int
aMotionStar::stopData () {
    int status;

    status = 0;

    // If and only if the server is sending continuous data, we need to stop
    // it.
    if ( m_run_mode == BIRDNET::CONTINUOUS ) {
        BIRDNET::HEADER msg(BIRDNET::MSG_STOP_DATA), rsp;

        msg.sequence = htons(m_seq_num);
        m_seq_num++;

        // Send the MSG_STOP_DATA packet.
        status = sendMsg(&msg, sizeof(BIRDNET::HEADER));

        if ( status != 0 ) {
            fprintf(stderr,
                    "[aMotionStar] WARNING: Could not send message to stop "
                    "continuous data\n");
        }
        else {
            // Get the server's response.
            status = getRsp(&rsp, sizeof(BIRDNET::HEADER));

            // If getRsp() did not return 0, print a warning message stating
            // that the data flow could not be stopped.
            if ( status != 0 ) {
                fprintf(stderr,
                        "[aMotionStar] WARNING: Could not stop continuous data\n");
            }
            // Otherwise, the data flow has been stopped and we can return
            // success to the caller.
            else {
                fprintf(stderr, "[aMotionStar] Continuous data stopped\n");
            }
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Shut down the server chassis.
// ----------------------------------------------------------------------------
int
aMotionStar::shutdown () {
    BIRDNET::HEADER msg(BIRDNET::MSG_SHUT_DOWN), rsp;
    int status;

    msg.sequence = htons(m_seq_num);
    m_seq_num++;

    // Send the MSG_SHUT_DOWN packet.
    status = sendMsg(&msg, sizeof(BIRDNET::HEADER)) ;

    if ( status != 0 ) {
        fprintf(stderr,
                "[aMotionStar] WARNING: Could not send shutdown request\n");
    }
    else {
        // Get the server's response to the MSG_SHUT_DOWN packet.
        status = getRsp(&rsp, sizeof(BIRDNET::HEADER));

        // If one could not be read, print a warning message.
        if ( status != 0 ) {
            fprintf(stderr,
                    "[aMotionStar] WARNING: Could not shutdown server chassis\n");
        }
        // Otherwise, the server is now shut down, so the driver is inactive.
        else {
            fprintf(stderr, "[aMotionStar] Server chassis shut down\n");
            m_active = false;
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Get the current server address for the device.
// ----------------------------------------------------------------------------
void
aMotionStar::setRunMode (const BIRDNET::run_mode mode) {
    // If the driver is already active, we may need to do some communication
    // with the server before changing the run mode.
    if ( m_active ) {
        // If the old mode is BIRDNET::CONTINUOUS and the new mode is
        // BIRDNET::SINGLE_SHOT, we need to stop the incoming data first.
        if ( m_run_mode == BIRDNET::CONTINUOUS &&
             mode == BIRDNET::SINGLE_SHOT )
        {
            BIRDNET::HEADER msg(BIRDNET::MSG_STOP_DATA), rsp;

            msg.sequence = htons(m_seq_num);
            m_seq_num++;

            if ( sendMsg(&msg, sizeof(BIRDNET::HEADER)) == 0 ) {
                fprintf(stderr, "[aMotionStar] Continuous data stopped\n");
                getRsp(&rsp, sizeof(BIRDNET::HEADER));

                if ( rsp.error_code != 0 ) {
                    printError(rsp.error_code);
                }
            }
            else {
                fprintf(stderr,
                        "[aMotionStar] WARNING: Could not stop continuous "
                        "data before changing run modes!\n");
            }
        }
        // If the run mode was BIRDNET::SINGLE_SHOT and the new mode is
        // BIRDNET::CONTINUOUS, we need to request continuous data from the
        // server.
        else if ( m_run_mode == BIRDNET::SINGLE_SHOT &&
                  mode == BIRDNET::CONTINUOUS )
        {
            BIRDNET::HEADER msg(BIRDNET::MSG_RUN_CONTINUOUS), rsp;

            msg.sequence = htons(m_seq_num);
            m_seq_num++;

            if ( sendMsg(&msg, sizeof(BIRDNET::HEADER)) == 0 ) {
                fprintf(stderr, "[aMotionStar] Continuous data requested\n");
                getRsp(&rsp, sizeof(BIRDNET::HEADER));

                if ( rsp.error_code != 0 ) {
                    printError(rsp.error_code);
                }
            }
            else {
                fprintf(stderr,
                        "[aMotionStar] WARNING: Could not request continuous "
                        "data flow!\n");
            }
        }
    }

    m_run_mode = mode;
}

// ----------------------------------------------------------------------------
// Get the x position of the i'th bird.
// ----------------------------------------------------------------------------
float
aMotionStar::getXPos (const unsigned int i) const {
    float x_pos;
    FLOCK::data_format format;

    format = m_birds[i]->format;

    if ( format == FLOCK::POSITION || format == FLOCK::POSITION_ANGLES ||
         format == FLOCK::POSITION_MATRIX ||
         format == FLOCK::POSITION_QUATERNION )
    {
        // Ahh, the beauty of a union is is that this statement works for
        // any of these data formats since the memory address is the same in
        // all cases.
        x_pos = toXPos((unsigned char*) &m_birds[i]->data) / m_unit_conv;
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getXPos() for bird %u does not support data "
                "format %d\n", i, format);
        x_pos = 0.0;
    }

    return x_pos;
}

// ----------------------------------------------------------------------------
// Get the y position of the i'th bird.
// ----------------------------------------------------------------------------
float
aMotionStar::getYPos (const unsigned int i) const {
    float y_pos;
    FLOCK::data_format format;

    format = m_birds[i]->format;

    if ( format == FLOCK::POSITION || format == FLOCK::POSITION_ANGLES ||
         format == FLOCK::POSITION_MATRIX ||
         format == FLOCK::POSITION_QUATERNION )
    {
        // Ahh, the beauty of a union is is that this statement works for
        // any of these data formats since the memory address is the same in
        // all cases.
        y_pos = toYPos((unsigned char*) &m_birds[i]->data) / m_unit_conv;
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getYPos() for bird %u does not support data "
                "format %s\n", i, FLOCK::getFormatName(format).c_str());
        y_pos = 0.0;
    }

    return y_pos;
}

// ----------------------------------------------------------------------------
// Get the z position of the i'th bird.
// ----------------------------------------------------------------------------
float
aMotionStar::getZPos (const unsigned int i) const {
    float z_pos;
    FLOCK::data_format format;

    format = m_birds[i]->format;

    if ( format == FLOCK::POSITION || format == FLOCK::POSITION_ANGLES ||
         format == FLOCK::POSITION_MATRIX ||
         format == FLOCK::POSITION_QUATERNION )
    {
        // Ahh, the beauty of a union is is that this statement works for
        // any of these data formats since the memory address is the same in
        // all cases.
        z_pos = toZPos((unsigned char*) &m_birds[i]->data) / m_unit_conv;
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getZPos() for bird %u does not support data "
                "format %s\n", i, FLOCK::getFormatName(format).c_str());
        z_pos = 0.0;
    }

    return z_pos;
}

// ----------------------------------------------------------------------------
// Get the z rotation of the i'th bird.
// ----------------------------------------------------------------------------
float
aMotionStar::getZRot (const unsigned int i) const {
    float z_rot;
    FLOCK::data_format format;

    format = m_birds[i]->format;

    if ( format == FLOCK::ANGLES ) {
        z_rot = toZRot(&(m_birds[i]->data.ANGLES[0]));
    }
    else if ( format == FLOCK::POSITION_ANGLES ) {
        z_rot = toZRot(&(m_birds[i]->data.POSITION_ANGLES[6]));
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getZRot() for bird %u does not support data "
                "format %s\n", i, FLOCK::getFormatName(format).c_str());
        z_rot = 0.0;
    }

    return z_rot;
}

// ----------------------------------------------------------------------------
// Get the y rotation of the i'th bird.
// ----------------------------------------------------------------------------
float
aMotionStar::getYRot (const unsigned int i) const {
    float y_rot;
    FLOCK::data_format format;

    format = m_birds[i]->format;

    if ( format == FLOCK::ANGLES ) {
        y_rot = toYRot(&(m_birds[i]->data.ANGLES[0]));
    }
    else if ( format == FLOCK::POSITION_ANGLES ) {
        y_rot = toYRot(&(m_birds[i]->data.POSITION_ANGLES[6]));
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getYRot() for bird %u does not support data "
                "format %d\n", i, format);
        y_rot = 0.0;
    }

    return y_rot;
}

// ----------------------------------------------------------------------------
// Get the x rotation of the i'th bird.
// ----------------------------------------------------------------------------
float
aMotionStar::getXRot (const unsigned int i) const {
    float x_rot;
    FLOCK::data_format format;

    format = m_birds[i]->format;

    if ( format == FLOCK::ANGLES ) {
        x_rot = toXRot(&(m_birds[i]->data.ANGLES[0]));
    }
    else if ( format == FLOCK::POSITION_ANGLES ) {
        x_rot = toXRot(&(m_birds[i]->data.POSITION_ANGLES[6]));
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getXRot() for bird %u does not support data "
                "format %d\n", i, format);
        x_rot = 0.0;
    }

    return x_rot;
}

// ----------------------------------------------------------------------------
// Get the rotation angles of the requested bird as matrix elements that are
// then stored in the given array.
// ----------------------------------------------------------------------------
void
aMotionStar::getMatrixAngles (const unsigned int bird, float angles[3]) const {
    FLOCK::data_format format;

    format = m_birds[bird]->format;

    if ( format == FLOCK::MATRIX || format == FLOCK::POSITION_MATRIX ) {
        float x_val, y_val, z_val, radians;
        float y_cos;

        // Extract the values from the angle matrix that will be used to get
        // the actual rotation values.  The X value comes from element (2, 3);
        // the Y value comes from element (1, 3); and the Z value comes from
        // element (1, 2).  Refer to page 84 of the MotionStar Installation
        // and Operation Guide for the data format.  Data read is in network
        // byte order (MS byte, LS byte).

        // MATRIX format.
        if ( format == FLOCK::MATRIX ) {
            // Element (2, 3).
            x_val = toFloat(m_birds[bird]->data.MATRIX[14],
                            m_birds[bird]->data.MATRIX[15]);

            // Element (1, 3).
            y_val = toFloat(m_birds[bird]->data.MATRIX[12],
                            m_birds[bird]->data.MATRIX[13]);

            // Element (1, 2).
            z_val = toFloat(m_birds[bird]->data.MATRIX[6],
                            m_birds[bird]->data.MATRIX[7]);
        }
        // POSITION/MATRIX format.
        else {
            // Element (2, 3).
            x_val = toFloat(m_birds[bird]->data.POSITION_MATRIX[20],
                            m_birds[bird]->data.POSITION_MATRIX[21]);

            // Element (1, 3).
            y_val = toFloat(m_birds[bird]->data.POSITION_MATRIX[18],
                            m_birds[bird]->data.POSITION_MATRIX[19]);

            // Element (1, 2).
            z_val = toFloat(m_birds[bird]->data.POSITION_MATRIX[12],
                            m_birds[bird]->data.POSITION_MATRIX[13]);
        }

        // Get the y-axis rotation first.  It is the opposite of the arcsine
        // of element (1, 3) in the matrix.
        radians   = asin(-y_val);
        y_cos     = cos(radians);
        angles[1] = RAD2DEG(radians);

        // Get the x-axis rotation.  It is the arcsine of element (2, 3)
        // divided by the cosine of y.
        radians   = asin(x_val / y_cos);
        angles[0] = RAD2DEG(radians);

        // Get the z-axis rotation.  It is the arcsine of element (1, 2)
        // divided by the cosine of y.

        radians   = asin(z_val / y_cos);
        angles[2] = RAD2DEG(radians);
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getMatrixAngles() for bird %u does not support "
                "data format %d\n", bird, format);
        angles[0] = angles[1] = angles[2] = 0.0;
    }
}

// ----------------------------------------------------------------------------
// Get the rotation angles of the i'th bird as quaternion parameters that are
// then stored in the given array.
// ----------------------------------------------------------------------------
void
aMotionStar::getQuaternion (const unsigned int bird, float quat[4]) const {
    FLOCK::data_format format;

    format = m_birds[bird]->format;

    // Read the quaternion parameters from the data block.  Refer to page 92
    // of the MotionStar Installation and Operation Guide for details on the
    // data format.  Data is read in network byte order (MS byte, LS byte).

    // QUATERNION format.
    if ( format == FLOCK::QUATERNION ) {
        for ( int i = 0, word = 0; word < 8; i++, word += 2 ) {
            quat[i] = toFloat(m_birds[bird]->data.QUATERNION[word],
                              m_birds[bird]->data.QUATERNION[word + 1]);
        }
    }
    // POSITION/QUATERNION format.
    else if ( format == FLOCK::POSITION_QUATERNION ) {
        for ( int i = 0, word = 6; word < 14; i++, word += 2 ) {
            quat[i] = toFloat(m_birds[bird]->data.POSITION_QUATERNION[word],
                              m_birds[bird]->data.POSITION_QUATERNION[word + 1]);
        }
    }
    else {
        fprintf(stderr,
                "[aMotionStar] getQuaternion() for bird %u does not support "
                "data format %d\n", bird, format);
        quat[0] = quat[1] = quat[2] = quat[3] = 0.0;
    }
}

// ============================================================================
// Private methods.
// ============================================================================

// ----------------------------------------------------------------------------
// Send a wake-up call to the MotionStar server.
// ----------------------------------------------------------------------------
int
aMotionStar::sendWakeUp () {
    BIRDNET::HEADER msg(BIRDNET::MSG_WAKE_UP), rsp;
    int status;

    msg.sequence = htons(m_seq_num);
    m_seq_num++;

    status = sendMsg((void*) &msg, sizeof(msg));

    // If the wake-up packet could not be sent to the server, print a
    // warning message.
    if ( status != 0 ) {
        fprintf(stderr,
                "[aMotionStar] WARNING: Could not send wake-up message to "
                "server!\n");
    }
    else {
        status = getRsp((void*) &rsp, sizeof(rsp));

        // If we got a response but there was an error from the server, shut
        // it down and send the wake-up message again.  This is the
        // recommended procedure documented in the operation guide.
        if ( status == 0 && rsp.error_code != 0 ) {
            BIRDNET::HEADER shutdown_msg(BIRDNET::MSG_SHUT_DOWN);

            printError(rsp.error_code);
            fprintf(stderr,
                    "[aMotionStar] Reinitializing server and sending wake-up "
                    "call again\n");

            shutdown_msg.sequence = htons(m_seq_num);
            m_seq_num++;

            status = sendMsg((void*) &shutdown_msg, sizeof(shutdown_msg));
            status = getRsp((void*) &rsp, sizeof(rsp));

            if ( rsp.error_code != 0 ) {
                printError(rsp.error_code);
            }
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Get the system status.
// ----------------------------------------------------------------------------
BIRDNET::SYSTEM_STATUS*
aMotionStar::getSystemStatus () {
    BIRDNET::DATA_PACKET* sys_status;
    BIRDNET::SYSTEM_STATUS* status_info;

    sys_status = getDeviceStatus(0);

    // If nothing was read into the system status block, set status_info to
    // NULL to inform the caller that something went wrong.
    if ( sys_status == NULL ) {
        status_info = NULL;
    }
    // Otherwise, we can proceed.
    else {
        unsigned char flock_number;

        // Set the status info block to the address of the system status
        // packet's data buffer.  This will be returned to the caller.
        status_info = (BIRDNET::SYSTEM_STATUS*) &(sys_status->buffer[0]);

        // Save these values.
        // NOTE: Most of them are not currently being used, but they may prove
        // useful at a later time.
        flock_number        = status_info->flockNumber;
        m_chassis_id        = status_info->chassisNumber;
        m_chassis_dev_count = status_info->chassisDevices;
        m_cur_mrate         = convertMeasurementRate(status_info->measurementRate);

        // If this is the master chassis, get the extra system information
        // about all of the devices connected to the chassis.  This will tell
        // us how many birds there are in the system and fill the m_birds
        // vector.
//        if ( m_master ) {
            unsigned char* fbb_devices;
            FBB::Device* cur_dev;

            // Get the start address for the "array" of devices.  Element 0
            // of this "array" is the first byte after the end of the system
            // status block.
            fbb_devices = &sys_status->buffer[sizeof(BIRDNET::SYSTEM_STATUS)];

            for ( unsigned char i = 0; i < flock_number; i++ ) {
                // Create a new FBB Device object.
                cur_dev = new FBB::Device();

                // The ERC device is not put into the m_birds vector.  We do
                // not want to try reading data from it.
                if ( fbb_devices[i] & FBB::ERC ) {
                    m_erc_vec.push_back(cur_dev);

                    cur_dev->ert3_present = fbb_devices[i] & FBB::ERT3;
                    cur_dev->ert2_present = fbb_devices[i] & FBB::ERT2;
                    cur_dev->ert1_present = fbb_devices[i] & FBB::ERT1;
                    cur_dev->ert0_present = fbb_devices[i] & FBB::ERT0;
                }
                // Put all non-ERC devices in the m_birds vector.
                else {
                    m_birds.push_back(cur_dev);
                }

                cur_dev->accessible = fbb_devices[i] & FBB::ACCESS;
                cur_dev->running    = fbb_devices[i] & FBB::RUNNING;
                cur_dev->has_sensor = fbb_devices[i] & FBB::SENSOR;
                cur_dev->is_erc     = fbb_devices[i] & FBB::ERC;

                // This is how we handle remembering which device this is
                // after this point.
                cur_dev->addr = i + 1;
            }

            // If the number of birds connected to the server is less than
            // the number of birds we want to sample, we have a problem.
            if ( flock_number < m_birds_requested ) {
                fprintf(stderr,
                        "[aMotionStar] WARNING: Needed %u birds, but the "
                        "server only has %u\n", m_birds_requested,
                        flock_number);
            }
//        }
    }

    return status_info;
}

// ----------------------------------------------------------------------------
// Set the system status.
// ----------------------------------------------------------------------------
int
aMotionStar::setSystemStatus (BIRDNET::SYSTEM_STATUS* sys_status,
                              const unsigned char xmtr_num,
                              const char data_rate[6])
{
    if ( sys_status->transmitterNumber != xmtr_num ) {
        sys_status->transmitterNumber = xmtr_num;
        fprintf(stderr, "[aMotionStar] Settting active transmitter to %u\n",
                xmtr_num);
    }

    // Copy the contents of data_rate into the measuermentRate block.
    // XXX: This may be a little bit of overkill.  strncpy(3) is probably
    // sufficient.
    for ( int i = 0; i < 6; i++ ) {
        sys_status->measurementRate[i] = data_rate[i];
    }

    // Set the system status by setting the status for device 0.
    return setDeviceStatus(0, (char*) sys_status,
                           sizeof(BIRDNET::SYSTEM_STATUS));
}

// ----------------------------------------------------------------------------
// Read the configurations of all the birds and send our configuration data
// to them.
// ----------------------------------------------------------------------------
int
aMotionStar::configureBirds () {
    BIRDNET::BIRD_STATUS* bird_status;
    unsigned int bird_count;
    bool values_set;

    bird_count = 0;
    values_set = false;

    // Configure each of the birds on the server chassis.
    for ( unsigned int bird = 0; bird < m_birds.size(); bird++ ) {
        // First, read the current configuration for the bird in question.
        bird_status = getBirdStatus(bird);

        if ( bird_status == NULL ) {
            fprintf(stderr, "[aMotionStar] Could not get status for bird %u\n",
                    bird);
        }
        // If we got the status for bird, parse it and send a new
        // configuration based on the bird's current configuration and our
        // own desired configuration.
        else {
            // Read the position scaling factor from the first available
            // device.
            if ( ! values_set ) {
                getUnitInfo(bird, bird_status);

                // We have the values we need, and we don't want to set them
                // again.
                values_set = true;
            }

            // If we are still reading configuration information for
            // requested birds, handle the setup steps.
            if ( bird < m_birds_requested ) {
                unsigned char format;

                // XXX: Eventually, we would like to have all birds get their
                // format value set individually.
                switch (m_bird_format) {
                  // 0 words, mode 0.
                  case FLOCK::NO_BIRD_DATA:
                    format = 0x00;
                    m_birds[bird]->data_words = 0;
                    break;
                  // 3 words, mode 1.
                  case FLOCK::POSITION:
                    format = 0x31;
                    m_birds[bird]->data_words = 3;
                    break;
                  // 3 words, mode 2.
                  case FLOCK::ANGLES:
                    format = 0x32;
                    m_birds[bird]->data_words = 3;
                    break;
                  // 9 words, mode 3.
                  case FLOCK::MATRIX:
                    format = 0x93;
                    m_birds[bird]->data_words = 9;
                    break;
                  // 6 words, mode 4.
                  case FLOCK::POSITION_ANGLES:
                    format = 0x64;
                    m_birds[bird]->data_words = 6;
                    break;
                  // 12 words (hex c), mode 5.
                  case FLOCK::POSITION_MATRIX:
                    format = 0xc5;
                    m_birds[bird]->data_words = 12;
                    break;
                  // 0 words, mode 6.
                  case FLOCK::INVALID:
                    format = 0x06;
                    m_birds[bird]->data_words = 0;
                    break;
                  // 4 words, mode 7.
                  case FLOCK::QUATERNION:
                    format = 0x47;
                    m_birds[bird]->data_words = 4;
                    break;
                  // 7 words, mode 8.
                  case FLOCK::POSITION_QUATERNION:
                    format = 0x78;
                    m_birds[bird]->data_words = 7;
                    break;
                }

                // Set the current bird's format to the value in
                // m_bird_format, and set its hemisphere to the value in
                // m_hemisphere.  Fill in the other values with those just set
                // in the bird_status object.  There is less chance for error
                // this way.
                m_birds[bird]->format      = m_bird_format;
                m_birds[bird]->hemisphere  = m_hemisphere;
                m_birds[bird]->report_rate = bird_status->status.reportRate;
                m_birds[bird]->setup       = bird_status->status.setup;

                // Fill in the bird_status struct.
                bird_status->status.setup      |= FLOCK::APPEND_BUTTON_DATA;
                bird_status->status.dataFormat = format;
                bird_status->status.reportRate = m_report_rate;
                bird_status->status.hemisphere = m_hemisphere;

                // XXX: The second clause in the conditional is here so that
                // we can use the 0-based m_birds_active in the sample()
                // method and use the < operator for comparison.
                if ( m_birds[bird]->data_words > 0 &&
                     bird < m_birds_requested )
                {
                    m_birds_active++;
                }
            }
            // Otherwise, disable any birds we do not request.
            else {
                // Disable the current bird in the m_birds vector.
                m_birds[bird]->data_words  = 0;
                m_birds[bird]->format      = FLOCK::NO_BIRD_DATA;
                m_birds[bird]->report_rate = 1;
                m_birds[bird]->hemisphere  = m_hemisphere;

                // Fill in the bird_status struct with disabling values.
                bird_status->status.dataFormat = 0x00;
                bird_status->status.reportRate = 1;
            }

            // Finally, send the new configuration to the current bird.
            setBirdStatus(bird, bird_status);

            bird_count++;
            delete bird_status;
        }
    }

    fprintf(stderr,
            "[aMotionStar] Configured %d birds (%u requested, %u active, "
            "%u disabled)\n", bird_count, m_birds_requested, m_birds_active,
            bird_count - m_birds_active);

    return bird_count;
}

// ----------------------------------------------------------------------------
// Get the status of an individual bird.
// ----------------------------------------------------------------------------
BIRDNET::BIRD_STATUS*
aMotionStar::getBirdStatus (const unsigned char bird) {
    BIRDNET::DATA_PACKET* status;
    BIRDNET::BIRD_STATUS* bird_status;

    // The value in bird is the index into the m_birds vector.  Using that
    // element from the vector, we get the actual FBB address.
    status = getDeviceStatus(m_birds[bird]->addr);

    // If nothing was read, nothing can be returned.
    if ( status == NULL ) {
        bird_status = NULL;
    }
    else {
        // The requested bird's status descrpition begins at the returned
        // packet's data buffer.  This is what will be returned to the caller.
        bird_status = (BIRDNET::BIRD_STATUS*) &(status->buffer[0]);

        // The dataFormat field contains the number of words (2 bytes) in
        // this bird's formatted data packet.  It is in the most significant
        // four bits.  See page 127 of the MotionStar Operation Guide.
        m_birds[bird]->data_words = (bird_status->status.dataFormat >> 4) & 0x0f;

        // The least significant four bits of dataFormat contain the format
        // of the data this bird will send by default.  Again, refer to page
        // 127 of the MotionStar Operation Guide.
        switch (bird_status->status.dataFormat & 0x0f) {
          case 0:
            m_birds[bird]->format = FLOCK::NO_BIRD_DATA;
            break;
          case 1:
            m_birds[bird]->format = FLOCK::POSITION;
            break;
          case 2:
            m_birds[bird]->format = FLOCK::ANGLES;
            break;
          case 3:
            m_birds[bird]->format = FLOCK::MATRIX;
            break;
          case 4:
            m_birds[bird]->format = FLOCK::POSITION_ANGLES;
            break;
          case 5:
            m_birds[bird]->format = FLOCK::POSITION_MATRIX;
            break;
          case 6:
            fprintf(stderr,
                    "[aMotionStar] WARNING: Got INVALID data format for "
                    "bird %u\n", bird);
            m_birds[bird]->format = FLOCK::INVALID;
            break;
          case 7:
            m_birds[bird]->format = FLOCK::QUATERNION;
            break;
          case 8:
            m_birds[bird]->format = FLOCK::POSITION_QUATERNION;
            break;
          default:
            fprintf(stderr,
                    "[aMotionStar] WARNING: Got unknown data format %u for "
                    "bird %u\n", bird_status->status.dataFormat & 0x0f, bird);
            break;
        }

        switch (bird_status->status.hemisphere) {
          case 0:
            m_birds[bird]->hemisphere = FLOCK::FRONT_HEMISPHERE;
            break;
          case 1:
            m_birds[bird]->hemisphere = FLOCK::REAR_HEMISPHERE;
            break;
          case 2:
            m_birds[bird]->hemisphere = FLOCK::UPPER_HEMISPHERE;
            break;
          case 3:
            m_birds[bird]->hemisphere = FLOCK::LOWER_HEMISPHERE;
            break;
          case 4:
            m_birds[bird]->hemisphere = FLOCK::LEFT_HEMISPHERE;
            break;
          case 5:
            m_birds[bird]->hemisphere = FLOCK::RIGHT_HEMISPHERE;
            break;
          default:
            fprintf(stderr,
                    "[aMotionStar] WARNING: Got unknown data hemisphere %u "
                    "for bird %u\n", bird_status->status.dataFormat, bird);
            break;
        }

        m_birds[bird]->report_rate = bird_status->status.reportRate;
        m_birds[bird]->address     = bird_status->status.FBBaddress;
    }

    return bird_status;
}

// ----------------------------------------------------------------------------
// Set the status of an individual bird.
// ----------------------------------------------------------------------------
int
aMotionStar::setBirdStatus (const unsigned char bird,
                            BIRDNET::BIRD_STATUS* status)
{
    // The value in bird is the index into the m_birds vector.  Using that
    // entry, we get the actual FBB address.
    return setDeviceStatus(m_birds[bird]->addr, (char*) status,
                           sizeof(BIRDNET::BIRD_STATUS));
}

// ----------------------------------------------------------------------------
// Get the status of the requested FBB device.  The device number must be that
// of the device's address on the Fast Bird Bus.  Thus, a value of 0 is
// interpreted as a request for the overall system status.  The birds are
// addressed from 1 through 120.
// ----------------------------------------------------------------------------
BIRDNET::DATA_PACKET*
aMotionStar::getDeviceStatus (const unsigned char device) {
    int status;
    BIRDNET::HEADER msg(BIRDNET::MSG_GET_STATUS);
    BIRDNET::DATA_PACKET* rsp;

    // Set the xtype field to the given device number.
    msg.xtype    = device;
    msg.sequence = htons(m_seq_num++);;

    rsp = NULL;

    // Send the status request packet to the server.
    status = sendMsg((void*) &msg, sizeof(msg));

    if ( status != 0 ) {
        fprintf(stderr,
                "[aMotionStar] ERROR: Could not request status for device %u\n",
                device);
    }
    else {
        // Allocate the new data packet object that will be returned to the
        // caller.
        rsp = new BIRDNET::DATA_PACKET();

        // First read the header of the response.  It will contain the size
        // of the data part of the packet.
        status = getRsp((void*) &(rsp->header), sizeof(rsp->header));

        if ( status != 0 ) {
            fprintf(stderr,
                    "[aMotionStar] ERROR: Could not read status header for "
                    "device %u from server\n", device);
        }
        // If that succeeded, read the rest of the response using the size
        // in the header's number_bytes field.
        else {
            if ( rsp->header.error_code != 0 ) {
                printError(rsp->header.error_code);
            }

            status = getRsp((void*) &(rsp->buffer),
                            ntohs(rsp->header.number_bytes));

            if ( status != 0 ) {
                fprintf(stderr,
                        "[aMotionStar] ERROR: Could not read status data "
                        "buffer for device %u from server\n", device);
            }
        }
    }

    return rsp;
}

// ----------------------------------------------------------------------------
// Set the status of the requested FBB device using the given configuration
// block.  The device number must be that of the device's address on the Fast
// Bird Bus.  Thus, a value of 0 is interpreted as a configuation block for
// the overall system.  The birds are addressed from 1 through 120.
// ----------------------------------------------------------------------------
int
aMotionStar::setDeviceStatus (const unsigned char device, const char* buffer,
                              const unsigned short buffer_size)
{
    BIRDNET::DATA_PACKET msg;
    BIRDNET::HEADER rsp;
    size_t total_size;
    int status;

    // Get the total size of the packet that we construct based on the size
    // of the header and the size of the passed buffer.
    total_size = sizeof(BIRDNET::HEADER) + buffer_size;

    // Fill in the header bits.
    msg.header.type         = BIRDNET::MSG_SEND_SETUP;
    msg.header.xtype        = device;
    msg.header.sequence     = htons(m_seq_num++);
    msg.header.number_bytes = htons(buffer_size);

    // Copy the given buffer into the packet's data buffer.
    memcpy((void*) &msg.buffer[0], (void*) buffer, buffer_size);

    // Send the constructed packet to the server.
    status = sendMsg(&msg, total_size);

    if ( status != 0 ) {
        fprintf(stderr,
                "[aMotionStar] WARNING: Could not set device status for "
                "device %u: %s\n", device, strerror(errno));
    }
    else {
        status = getRsp(&rsp, sizeof(rsp));

        if ( status != 0 ) {
            fprintf(stderr,
                    "[aMotionStar] WARNING: Could not read server response to "
                    "device %u setup: %s\n", device, strerror(errno));
        }
        else if ( rsp.error_code != 0 ) {
            printError(rsp.error_code);
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Tell the MotionStar server to sample continuously.
// ----------------------------------------------------------------------------
int
aMotionStar::setContinuous () {
    BIRDNET::HEADER msg(BIRDNET::MSG_RUN_CONTINUOUS), rsp;
    int status;

    status = -1;

    if ( sendMsg(&msg, sizeof(BIRDNET::HEADER)) == 0 ) {
        if ( getRsp(&rsp, sizeof(BIRDNET::HEADER)) == 0 ) {
            if ( rsp.error_code != 0 ) {
                printError(rsp.error_code);
            }
            else {
                status = 0;
            }
        }
    }

    return status;
}

// ----------------------------------------------------------------------------
// Combine the two given bytes (passed as high byte and low byte respectively)
// into a single word in host byte order.  This is used for reading bytes from
// the packets and converting them into usable values.
// ----------------------------------------------------------------------------
short
aMotionStar::toShort (const char high_byte, const char low_byte) const {
    union {
        char	c[sizeof(short)];
        short	value;
    } result;

    // If this is a big-endian host, put the high byte in the first byte of
    // result.value and the low byte in the second byte.
    if ( m_big_endian ) {
        result.c[0] = high_byte;
        result.c[1] = low_byte;
    }
    // Otherwise, put the low byte in the first byte and the high byte in the
    // second byte.
    else {
        result.c[0] = low_byte;
        result.c[1] = high_byte;
    }

    return result.value;
}

// ----------------------------------------------------------------------------
// Convert the given two bytes into a single-precision floating-point value
// that falls within a defined range.  This is documented in the MotionStar
// Operation and Installation Guide.
// ----------------------------------------------------------------------------
float
aMotionStar::toFloat (const unsigned char high_byte,
                      const unsigned char low_byte)
    const
{
    unsigned short input, min_base, max_base;
    float min, max, result;

    // Construct the input from the given bytes.
    input = toShort(high_byte, low_byte);

    // Use the values documented on page 92 of the MotionStar Installation and
    // Operation Guide.
    min_base = 0x8000;
    min      = -1.0;
    max_base = 0x7fff;
    max      = 0.99996;

    // If the input is 0, the result is 0.
    if ( input == 0 ) {
        result = 0.0;
    }
    // If the input is greater than or equal to 0x8000, the result will be
    // negative and should be multiplied by min and divided by min_base.
    else if ( input >= min_base ) {
        result = input * min / min_base;
    }
    // Otherwise, the result is positive and should be multiplied by max and
    // divided by max_base.
    else {
        result = input * max / max_base;
    }

    return result;
}

// ----------------------------------------------------------------------------
// Convert the given 6-byte array of characters to a double-precision
// floating-point number representing the data rate.
// ----------------------------------------------------------------------------
double
aMotionStar::convertMeasurementRate (const unsigned char rate[6]) {
    double data_rate;
    char data_rate_a[7];

    // Copy the bytes in status_info->measurementRate into data_rate_a.  I
    // know this is probably being overly cautious, but getting the byte
    // ordering right is important.
    for ( int i = 0; i < 6; i++ ) {
        data_rate_a[i] = rate[i];
    }

    // Terminate the measurement rate string.
    data_rate_a[6] = '\0';

    // This conversion is described in the measurementRate row of the
    // General System Status table on page 121 of the MotionStar Operation and
    // Installation Guide.
    data_rate = atof(data_rate_a) / 1000.0;

    return data_rate;
}

// ----------------------------------------------------------------------------
// Convert the given double-precision floating-point number to a 6-byte array
// of characters representing the data rate.
// ----------------------------------------------------------------------------
void
aMotionStar::convertMeasurementRate (const double rate, std::string& str_rate)
{
    char rate_a[7];

    snprintf(rate_a, 7, "%06.0f", rate * 1000.0);
    str_rate = rate_a;
}

// ----------------------------------------------------------------------------
// Extract the information regarding the measurement units (e.g., inches) and
// the position scaling factor.
// ----------------------------------------------------------------------------
void
aMotionStar::getUnitInfo (const unsigned int bird,
                          const BIRDNET::BIRD_STATUS* bird_status)
{
    unsigned char high_byte, low_byte, units;

    high_byte = bird_status->status.scaling[0];
    low_byte  = bird_status->status.scaling[1];

    // The highest four bits of the high byte tell the measurement system
    // being used.
    units = high_byte >> 4;

    // Determine the measurement units from the value in units.
    if ( units == 0 ) {
        m_units     = BIRDNET::INCHES;
        m_unit_conv = 12.0;
    }
    else {
        fprintf(stderr,
                "[aMotionStar] WARNING: Unknown units value %u read from "
                "scaling factor on bird %u\n", units, bird);
    }

    // Mask out the most significant four bits of the high byte because they
    // are used only to tell the measurement system.
    high_byte &= 0x0f;

    m_xmtr_pos_scale = (float) toShort(high_byte, low_byte);
}

// ----------------------------------------------------------------------------
// Send the given message to the server.
// ----------------------------------------------------------------------------
int
aMotionStar::sendMsg (const void* packet, const size_t packet_size) {
    ssize_t bytes;
    int status;

    // Send the packet to the server.
    bytes = send(m_socket, packet, packet_size, 0);

    // Nothing was sent.
    if ( bytes == 0 ) {
        fprintf(stderr, "[aMotionStar] Sent 0 bytes to %s: %s\n",
               m_address.c_str(), strerror(errno));
        status = -2;
    }
    // An error occurred while trying to send the packet.
    else if ( bytes == -1 ) {
        fprintf(stderr, "[aMotionStar] Could not send message to %s: %s\n",
                m_address.c_str(), strerror(errno));
        status = -1;
    }
    // The packet was sent successfully.
    else {
        status = 0;
    }

    return status;
}

// ----------------------------------------------------------------------------
// Get the server's response to a sent message.
// ----------------------------------------------------------------------------
int
aMotionStar::getRsp (void* packet, const size_t packet_size) {
    ssize_t bytes;
    int status;

    // Get the packet from the server.
    bytes = recvn(packet, packet_size);

    // Nothing was read.
    if ( bytes == 0 ) {
        fprintf(stderr, "[aMotionStar] Read 0 bytes from %s: %s\n",
                m_address.c_str(), strerror(errno));
        status = -2;
    }
    // An error occurred while trying to receive the packet.
    else if ( bytes == -1 ) {
        fprintf(stderr, "[aMotionStar] Could not read message from %s: %s\n",
                m_address.c_str(), strerror(errno));
        status = -1;
    }
    // The packet was received successfully.
    else {
        status = 0;
    }

    return status;
}

// ----------------------------------------------------------------------------
// Read exactly packet_size bytes from the server.
// ----------------------------------------------------------------------------
ssize_t
aMotionStar::recvn (void* packet, const size_t packet_size, const int flags) {
    size_t count;
    ssize_t bytes;

    count = packet_size;

    while ( count > 0 ) {
        bytes = ::recv(m_socket, packet, packet_size, flags);

        // Read error.
        if ( bytes < 0 ) {
            // Restart the read process if we were interrupted by the OS.
            if ( errno == EINTR ) {
                continue;
            }
            // Otherwise, we have an error situation, so return the value
            // returned by recv(2).
            else  {
                break;
            }
        }
        // May have read EOF, so return bytes read so far.
        else if ( bytes == 0 ) {
            bytes = packet_size - count;
        }
        else {
            packet = (void*) ((char*) packet + bytes);
            count -= bytes;
        }
    }

    return bytes;
}

// ----------------------------------------------------------------------------
// Print the system status as read from the server.
// ----------------------------------------------------------------------------
void
aMotionStar::printSystemStatus (const BIRDNET::SYSTEM_STATUS* status) {
    unsigned char erc_addr, xmtr_num;
    unsigned int pad_width_full, pad_width_dot;
    char rev_str[8];
    float rev_num;

    // Set the padding for the dots that go between the description and the
    // corresponding value and for the full line width.  In this case, we are
    // using 8 more characters for the full line width to make it just wide
    // enough to accommodate the output data.
    pad_width_dot  = 52;
    pad_width_full = pad_width_dot + 8;

    // Refer to page 122 of the MotionStar Installation and Operation Guide
    // for information about the format of the transmitterNumber field.
    erc_addr = status->transmitterNumber >> 4;
    xmtr_num = status->transmitterNumber & 0x03;

    // Set left justification so that everything will be formatted properly.
    std::cout.setf(std::ios::left);

    // Print a short heading.
    std::cout << std::setw(pad_width_full) << std::setfill('=') << "" << "\n";
    std::cout << "Ascension MotionStar General System Status ("
              << (m_master ? "master" : "slave") << ")\n";
    std::cout << std::setw(pad_width_full) << std::setfill('-') << "" << "\n";

    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Number of Fast Bird Bus devices in system " << " "
              << (unsigned int) status->flockNumber << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Number of servers in system " << " "
              << (unsigned int) status->serverNumber << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Server chassis ID number " << " "
              << (unsigned int) status->chassisNumber << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Number of devices within chassis " << " "
              << (unsigned int) status->chassisDevices << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* FBB address of first device in chassis " << " "
              << (unsigned int) status->firstAddress << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Active transmitter FBB address " << " "
              << (unsigned int) erc_addr << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Active transmitter number " << " "
              << (unsigned int) xmtr_num << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Measurement rate " << " "
              << convertMeasurementRate(status->measurementRate) << "\n";
    sprintf(rev_str, "%u.%u", status->softwareRevision[0],
            status->softwareRevision[1]);
    rev_num = atof(rev_str);
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Server Software Revision " << " " << rev_num << "\n";

    // Finish off with another line of = signs.
    std::cout << std::setw(pad_width_full) << std::setfill('=') << "" << "\n";
    std::cout << std::flush;
}

// ----------------------------------------------------------------------------
// Print the status for all the devices that have not been disabled.
// ----------------------------------------------------------------------------
void
aMotionStar::printDeviceStatus () {
    unsigned int pad_width_full, pad_width_dot;

    // Set the padding for the dots that go between the description and the
    // corresponding value and for the full line width.  In this case, we are
    // using 21 more characters for the full line width to make it just wide
    // enough to accommodate the output data.
    pad_width_dot  = 35;
    pad_width_full = pad_width_dot + 21;

    // Set left justification so that everything will be formatted properly.
    std::cout.setf(std::ios::left);

    // Print a short heading.
    std::cout << std::endl;
    std::cout << std::setw(pad_width_full) << std::setfill('=') << "" << "\n";
    std::cout << "Ascension MotionStar Device Status\n";
    std::cout << std::setw(pad_width_full) << std::setfill('-') << "" << "\n";

    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Position scaling factor " << " " << m_xmtr_pos_scale
              << "\n";
    std::cout << std::setw(pad_width_dot) << std::setfill('.')
              << "* Rotation scaling factor " << " " << m_xmtr_rot_scale
              << "\n";

    // Loop over the birds that we are actually using and print their status.
    for ( unsigned int i = 0; i < m_birds_active; i++ ) {
        std::cout << "* Bird " << i + 1 << " (unit " << i << "):\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     FBB address " << " "
                  << (unsigned int) m_birds[i]->address << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     Accessible " << " "
                  << (m_birds[i]->accessible ? "YES" : "NO") << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     Running " << " "
                  << (m_birds[i]->running ? "YES" : "NO") << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     Data format " << " "
                  << FLOCK::getFormatName(m_birds[i]->format) << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     Hemisphere " << " "
                  << FLOCK::getHemisphereName(m_birds[i]->hemisphere)
                  << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     Report rate " << " "
                  << (unsigned int) m_birds[i]->report_rate << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     Sudden change filtering " << " "
                  << (m_birds[i]->setup & FLOCK::SUDDEN_OUTPUT_CHANGE ? "ON" :
                                                                        "OFF")
                  << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     Button data " << " "
                  << (m_birds[i]->setup & FLOCK::APPEND_BUTTON_DATA ? "ON" :
                                                                      "OFF")
                  << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     AC narrow filter " << " "
                  << (m_birds[i]->setup & FLOCK::AC_NARROW_FILTER ? "ON" :
                                                                    "OFF")
                  << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     AC wide filter " << " "
                  << (m_birds[i]->setup & FLOCK::AC_WIDE_FILTER ? "ON" : "OFF")
                  << "\n";
        std::cout << std::setw(pad_width_dot) << std::setfill('.')
                  << "*     DC filter " << " "
                  << (m_birds[i]->setup & FLOCK::DC_FILTER ? "ON" : "OFF")
                  << "\n";
    }

    // Finish off with another line of = signs.
    std::cout << std::setw(pad_width_full) << std::setfill('=') << "" << "\n";
    std::cout << std::flush;
}

// ----------------------------------------------------------------------------
// Print the error message that corresponds to the given error code.  The
// message is based on the table on page 140 of the MotionStar manual.
// ----------------------------------------------------------------------------
void
aMotionStar::printError (const unsigned char err_code) {
    // Map the error code to a human-readable string.  These messages are
    // based on the table on page 140 of the MotionStar manual.
    switch (err_code) {
      case 1:
        fprintf(stderr, "WARNING: Single packet missing (error 1)\n");
        break;
      case 2:
        fprintf(stderr, "WARNING: Two or more packets missing (error 2)\n");
        break;
      case 3:
        fprintf(stderr, "WARNING: Many, many packets missing (error 3)\n");
        break;
      case 4:
        fprintf(stderr, "WARNING: Packet sequence number repeated (error 4)\n");
        break;
      case 6:
        fprintf(stderr, "WARNING: Unexpected packet type received (error 6)\n");
//        fprintf(stderr, "         Illegal packet sequence number: %hu\n",
//                error_code_extension);
        fprintf(stderr, "         Re-syncing may be necessary!\n");
        break;
      case 7:
        fprintf(stderr,
                "WARNING: Packet contents could not be determined (error 7)\n");
        break;
      case 8:
        fprintf(stderr, "WARNING: Status requested for non-existant FBB "
                "device (error 8)\n");
        break;
      case 9:
        fprintf(stderr, "WARNING: Illegal setup data sent (error 9)\n");
        break;
      case 100:
        fprintf(stderr, "WARNING: System not ready (error 100)\n");
        break;
    }
}

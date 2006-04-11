/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#define MIN_TIMEOUT 0.1

#include <gadget/Devices/DriverConfig.h>
#include <stdio.h>
#include <vpr/System.h>
#include <gadget/Devices/Immersion/IBox/IBoxStandalone.h>

#include <boost/concept_check.hpp>

// ----------------------------------------------------------------------------
// Constructor:  This sets all the default values for the ibox.
// ----------------------------------------------------------------------------
IboxStandalone::IboxStandalone()
{
	port = NULL;
	overlap = 0;

	fastTimeOut.set(100, vpr::Interval::Msec);
	slowTimeOut.set(3000, vpr::Interval::Msec);


	/* Set all descr. strings to null strings */
	mSerial_number[0] = 0;
	mProduct_name[0] = 0;
	//mProduct_id[0] = 0;
	mModel_name[0] = 0;
	mComment[0] = 0;
	mParam_format[0] = 0;
	mVersion[0] = 0;
	mPackets_expected=0;
	
	mPacket.clear();
}
// ----------------------------------------------------------------------------
// Destructor. Deletes the port
// ----------------------------------------------------------------------------
IboxStandalone::~IboxStandalone()
{
	if ( port != NULL )
	{
		port->close();
		delete port;
		port = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

vpr::ReturnStatus IboxStandalone::connect(const std::string& port_name,
                                          long int baud)
{
	mPortName=port_name;
	mBaudRate = baud;

	port = new vpr::SerialPort(mPortName);
    port->setOpenReadWrite();
    if ( !port->open().success() )
	{
		std::cerr << "[IBox] Port: " << mPortName << " could not be opened!" << std::endl;
		return(vpr::ReturnStatus::Fail);
	}
	else
	{
		std::cerr << "[IBox] Success, Port: " << mPortName << " opened." << std::endl;
	}
    port->clearAll();
    port->enableLocalAttach();
    port->setBufferSize(0);
    port->setTimeout(0);
    port->setOutputBaudRate(mBaudRate); // Put me before input to be safe
	port->setInputBaudRate(mBaudRate);
    port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
    port->enableRead();
	std::cerr << "[IBox] Changed all of the port Settings." << std::endl;
	if ( autosynch()!=vpr::ReturnStatus::Succeed )
	{
		return(vpr::ReturnStatus::Fail);
	}
	std::cerr << "[IBox] Done with Autosync." << std::endl;
	begin();
	getInfo();
	return(vpr::ReturnStatus::Succeed);
}
vpr::ReturnStatus IboxStandalone::connect()
{
	if(mPortName.empty()) mPortName = std::string("/dev/ttyd4");
	if(mBaudRate==0) mBaudRate = 9600;

	port = new vpr::SerialPort(mPortName);
    port->setOpenReadWrite();
    if ( !port->open().success() )
	{
		std::cerr << "[IBox] Port: " << mPortName << " could not be opened!" << std::endl;
		return(vpr::ReturnStatus::Fail);
	}
	else
	{
		std::cerr << "[IBox] Success, Port: " << mPortName << " opened." << std::endl;
	}
    port->clearAll();
    port->enableLocalAttach();
    port->setBufferSize(0);
    port->setTimeout(0);
    port->setOutputBaudRate(mBaudRate); // Put me before input to be safe
	port->setInputBaudRate(mBaudRate);
    port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
    port->enableRead();
	std::cerr << "[IBox] Changed all of the port Settings." << std::endl;
	if ( autosynch()!=vpr::ReturnStatus::Succeed )
	{
		return(vpr::ReturnStatus::Fail);
	}
	std::cerr << "[IBox] Done with Autosync." << std::endl;
	begin();
	getInfo();
	return(vpr::ReturnStatus::Succeed);
}

unsigned char IboxStandalone::getCmdByte(int t, int anlg, int encd)
{
	unsigned char tByte;
	unsigned char anlgByte;
	unsigned char encdByte;
	if ( t )
	{
		tByte = TIMER_BIT;
	}
	else
	{
		tByte = 0;
	}

	if ( anlg > 4 )
	{
		anlgByte=ANALOG_BITS;
	}
	else if ( anlg > 2 )
	{
		anlgByte=ANALOG_HI_BIT;
	}
	else if ( anlg > 0 )
	{
		anlgByte=ANALOG_LO_BIT;
	}
	else
	{
		anlgByte=0;
	}

	if ( encd > 4 )
	{
		encdByte=ENCODER_BITS;
	}
	else if ( encd > 2 )
	{
		encdByte=ENCODER_HI_BIT;
	}
	else if ( encd > 0 )
	{
		encdByte=ENCODER_LO_BIT;
	}
	else
	{
		encdByte=0;
	}
	return(tByte | anlgByte | encdByte);
}

vpr::ReturnStatus IboxStandalone::getInfo()
{
	vpr::ReturnStatus status = vpr::ReturnStatus::Succeed;
	//Get Product Name from device, all Immersion products return there name
	//so that drivers could be used for more than one Immersion device dynamically
    if ( string_cmd(GET_PROD_NAME) != vpr::ReturnStatus::Succeed )
	{
		std::cerr << "	[IBox] Could not get Product Name!" << std::endl;
		status=vpr::ReturnStatus::Fail;
	}
	else
	{
		std::cerr << "	[IBox] Product Name: " << mProduct_name << std::endl;
	}
	//Get Product ID
	if ( string_cmd(GET_PROD_ID) != vpr::ReturnStatus::Succeed )
	{
		std::cerr << "	[IBox] Could not get Product ID!" << std::endl;
		status=vpr::ReturnStatus::Fail;
	}
	else
	{
		std::cerr << "	[IBox] Product ID: " << mProduct_id << std::endl;
	}
	//Get Model Name
	if ( string_cmd(GET_MODEL_NAME) != vpr::ReturnStatus::Succeed )
	{
		std::cerr << "	[IBox] Could not get Model Name!" << std::endl;
		status=vpr::ReturnStatus::Fail;
	}
	else
	{
		std::cerr << "	[IBox] Model Name: " << mModel_name << std::endl;
	}
	//Get Serial Number
	if ( string_cmd(GET_SERNUM) != vpr::ReturnStatus::Succeed )
	{
		std::cerr << "	[IBox] Could not get Serial Number!" << std::endl;
		status=vpr::ReturnStatus::Fail;
	}
	else
	{
		std::cerr << "	[IBox] Serial Number: " << mSerial_number << std::endl;
	}
	//Get Device Comments
	if ( string_cmd(GET_COMMENT) != vpr::ReturnStatus::Succeed )
	{
		std::cerr << "	[IBox] Could not get Comment!" << std::endl;
		status=vpr::ReturnStatus::Fail;
	}
	else
	{
		std::cerr << "	[IBox] Comment: " << mComment << std::endl;
	}

	//Get Parameter Format
	if ( string_cmd(GET_PRM_FORMAT) != vpr::ReturnStatus::Succeed )
	{
		std::cerr << "	[IBox] Could not get Parameter Format!" << std::endl;
		status=vpr::ReturnStatus::Fail;
	}
	else
	{
		std::cerr << "	[IBox] Parameter Format: " << mParam_format << std::endl;
	}

	//Get Version
	if ( string_cmd(GET_VERSION) != vpr::ReturnStatus::Succeed )
	{
		std::cerr << "	[IBox] Could not get Version!" << std::endl;
		status=vpr::ReturnStatus::Fail;
	}
	else
	{
		std::cerr << "	[IBox] Version: " << mVersion << std::endl;
	}
	return(status);
}

void   IboxStandalone::invalidate_fields()
{
	mTimer_updated = 0;
	mAnalog_updated[0] = 0;
	mAnalog_updated[1] = 0;
	mAnalog_updated[2] = 0;
	mAnalog_updated[3] = 0;
	mAnalog_updated[4] = 0;
	mAnalog_updated[5] = 0;
	mAnalog_updated[6] = 0;
	mAnalog_updated[7] = 0;
	mEncoder_updated[0] = 0;
	mEncoder_updated[1] = 0;
	mEncoder_updated[2] = 0;
	mEncoder_updated[3] = 0;
	mEncoder_updated[4] = 0;
	mEncoder_updated[5] = 0;
	mMarker_updated = 0;
}

vpr::ReturnStatus IboxStandalone::autosynch()
{
	vpr::Uint32 written;
	unsigned char temp[4];
	int trys = 0;

	end();
    port->write(SIGNON_STR, sizeof(SIGNON_STR) - 1, written);
	vpr::System::msleep(150); /* XXX: Artificial pause */

	port->read(&temp, 1, written, slowTimeOut);
	//Try for a specified numer of times
	while ( trys < 10 )
	{
        if ( temp[0] == 'I' )
		{
			port->read(&temp, 3, written, fastTimeOut);
            if ( temp[0] == 'M' && temp[1] == 'M' && temp[2] == 'C' )
			{
				return(vpr::ReturnStatus::Succeed);
			}
		}
		port->write(SIGNON_STR, sizeof(SIGNON_STR) - 1, written);
		vpr::System::msleep(150); /* XXX: Artificial pause */
		port->read(&temp, 1, written, slowTimeOut);
		trys++;
	}
    return(vpr::ReturnStatus::Fail);
}

int IboxStandalone::begin()
{
	vpr::Uint32 written;
	port->flushQueue(vpr::SerialTypes::IO_QUEUES);
	port->write(BEGIN_STR,sizeof(BEGIN_STR) - 1, written);
    if ( read_string(mProduct_id) == vpr::ReturnStatus::Succeed )
	{
		return true;
	}
	else
	{
		return false;
	}
}


int IboxStandalone::end()
{
	vpr::Uint32 written;
	port->flushQueue(vpr::SerialTypes::IO_QUEUES);
	if ( port->write(END_STR, sizeof(END_STR) - 1, written).success() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void IboxStandalone::disconnect()
{
	port->flushQueue(vpr::SerialTypes::IO_QUEUES);
	end();
	port->close();
}

vpr::ReturnStatus IboxStandalone::string_cmd(byte cmnd)
{
	
	vpr::Uint32 written;
	vpr::ReturnStatus result;

	char temp=cmnd;
    char* buffer = &temp;
    char ch[2];

	port->flushQueue(vpr::SerialTypes::IO_QUEUES);
	port->write(buffer, 1, written);
	port->read(ch, 1, written,fastTimeOut);
	if ( ch[0]!=*buffer )		//If the returned command does not match the sent command
	{
		vpr::System::msleep(150);		//We try one more time
		port->flushQueue(vpr::SerialTypes::IO_QUEUES);
        port->write(buffer, 1, written);
        if ( (port->read(ch, 1, written,slowTimeOut) != vpr::ReturnStatus::Succeed) || (ch[0]!=*buffer) )
		{
			return(vpr::ReturnStatus::Fail);
		}
	}
    switch ( cmnd )
	{
		case GET_PROD_NAME:
			result = read_string(mProduct_name);
			break;
		case GET_PROD_ID:
			result = read_string(mProduct_id);
			break;
		case GET_MODEL_NAME:
			result = read_string(mModel_name);
			break;
		case GET_SERNUM:
			result = read_string(mSerial_number);
			break;
		case GET_COMMENT:
			result = read_string(mComment);
			break;
		case GET_PRM_FORMAT:
			result = read_string(mParam_format);
			break;
		case GET_VERSION:
			result = read_string(mVersion);
			break;
		default:
			result = vpr::ReturnStatus::Succeed;
			break;
	}
	return result;
}

vpr::ReturnStatus IboxStandalone::read_string(char *str)
{
	vpr::Uint32 written;
	vpr::ReturnStatus status;
	char ch;

	status = port->read(&ch,1,written, fastTimeOut);
	if(status != vpr::ReturnStatus::Succeed)
	{
		return(vpr::ReturnStatus::Fail);
	}
	else
	{
		while ( status == vpr::ReturnStatus::Succeed )
		{
			*str++ = (byte) ch;
			status = port->read(&ch,1,written, fastTimeOut);
		}
	}
	return(vpr::ReturnStatus::Succeed);
}

vpr::ReturnStatus IboxStandalone::wait_update(int timer_flag, int num_analogs ,int num_encoders)
{
	std_cmd(timer_flag, num_analogs, num_encoders);
	return wait_packet();
}

vpr::ReturnStatus    IboxStandalone::wait_packet()
{
	while ( check_packet() == vpr::ReturnStatus::InProgress );
	return(vpr::ReturnStatus::Succeed);			//Might as well be void return
}

void IboxStandalone::std_cmd(int timer_flag, int analog_reports, int encoder_reports)
{
	vpr::Uint32 written;
	unsigned char cmnd = getCmdByte(timer_flag, analog_reports, encoder_reports);
	port->write(&cmnd, 1, written);
}

void IboxStandalone::simple_cfg_cmd(byte cmnd)
{
	vpr::Uint32 written;
	char temp = cmnd;
	char* buffer = &temp;
	port->write(buffer, sizeof(buffer), written);
}

vpr::ReturnStatus IboxStandalone::passwd_cmd(byte cmnd)
{
	char temp = cmnd;
	char* buffer = &temp;
	vpr::Uint16 size;
	vpr::Uint32 written;
	char ch[2];
	char reader[2];
	port->write(buffer, 1, written);
    port->read(ch, 1, written, fastTimeOut);
	while ( ch[0]!=cmnd )
	{
		if ( port->getBufferSize(size).failure() )
		{
			return(vpr::ReturnStatus::Fail);
		}

		if ( ch!=0 )
		{
			port->read(ch, 1, written, fastTimeOut);
		}
        port->read(ch, 1, written, slowTimeOut);
	}
	if ( ch[0]==cmnd )
	{
		port->write(mSerial_number, strlen(mSerial_number), written);
		port->write(NULL, 1, written);
        port->read(reader, 1, written, fastTimeOut);
		if ( (unsigned char) reader[0] == PASSWD_OK )
		{
			port->write(mCfg_args, 1, written);
			return(vpr::ReturnStatus::Succeed);
		}
		else return (vpr::ReturnStatus::Fail);
	}
	else return(vpr::ReturnStatus::Timeout);
}


void IboxStandalone::insert_marker(byte marker)
{
	simple_cfg_cmd(INSERT_MARKER);
	char temp = marker;
	char* buffer = &temp;
	vpr::Uint32 written;
	port->write( buffer, 1, written);
}

vpr::ReturnStatus IboxStandalone::get_home_ref()
{
	simple_cfg_cmd(GET_HOME_REF);
	return wait_packet();
}

vpr::ReturnStatus IboxStandalone::set_home_ref(int *homeref)
{
	mNum_cfg_args = 2*NUM_ENCODERS;
	mCfg_args[0] = homeref[0] >> 8;
	mCfg_args[1] = homeref[0] & 0x00FF;
	mCfg_args[2] = homeref[1] >> 8;
	mCfg_args[3] = homeref[1] & 0x00FF;
	mCfg_args[4] = homeref[2] >> 8;
	mCfg_args[5] = homeref[2] & 0x00FF;
	mCfg_args[6] = homeref[3] >> 8;
	mCfg_args[7] = homeref[3] & 0x00FF;
	mCfg_args[8] = homeref[4] >> 8;
	mCfg_args[9] = homeref[4] & 0x00FF;
	mCfg_args[10] = homeref[5] >> 8;
	mCfg_args[11] = homeref[5] & 0x00FF;
	return passwd_cmd(SET_HOME_REF);
}

vpr::ReturnStatus    IboxStandalone::go_home_pos()
{
	simple_cfg_cmd(HOME_POS);
	return wait_packet();
}

vpr::ReturnStatus    IboxStandalone::set_home_pos(int *homepos)
{
	mNum_cfg_args = 2*NUM_ENCODERS;
	mCfg_args[0] = homepos[0] >> 8;
	mCfg_args[1] = homepos[0] & 0x00FF;
	mCfg_args[2] = homepos[1] >> 8;
	mCfg_args[3] = homepos[1] & 0x00FF;
	mCfg_args[4] = homepos[2] >> 8;
	mCfg_args[5] = homepos[2] & 0x00FF;
	mCfg_args[6] = homepos[3] >> 8;
	mCfg_args[7] = homepos[3] & 0x00FF;
	mCfg_args[8] = homepos[4] >> 8;
	mCfg_args[9] = homepos[4] & 0x00FF;
	mCfg_args[10] = homepos[5] >> 8;
	mCfg_args[11] = homepos[5] & 0x00FF;
	return passwd_cmd(SET_HOME);
}

vpr::ReturnStatus    IboxStandalone::get_maxes()
{
	simple_cfg_cmd(GET_MAXES);
	return wait_packet();
}

vpr::ReturnStatus    IboxStandalone::factory_settings()
{
    mNum_cfg_args = 0;
    return(passwd_cmd(RESTORE_FACTORY));
}

void IboxStandalone::report_motion(int timer_flag, int analog_reports, int encoder_reports, int delay, byte active_btns, int *analog_deltas, int *encoder_deltas)
{
   boost::ignore_unused_variable_warning(active_btns);
	int i;
	vpr::Uint32 written;
	char temp;
	char* buffer;
	char  cmnd = getCmdByte(timer_flag, analog_reports, encoder_reports);

	//Command Byte 4F or CF
	temp = REPORT_MOTION;
	buffer = &temp;
	port->write(buffer, 1, written);

	//Minimum delay between packets, 16 bit integer, ticks are ~1 ms
	temp = (vpr::Uint16)delay;
	buffer = &temp;
	port->write(buffer, 1, written);

	//Send the command that we are to act like we are responding to	(Byte 3)
	buffer = &cmnd;
	port->write(buffer, 1, written);

	//Does each button click generate a packet
	temp = 0xFF;
	buffer = &temp;
	port->write(buffer, 1, written);

	//Send the minimum analog change to generate a packet
	port->write(analog_deltas, sizeof(analog_deltas), written);

	//Send the minimum encoder change to generate a packet
	for ( i=0;i<NUM_ENCODERS;i++ )
	{
		temp = encoder_deltas[i];
		buffer = &temp;
		port->write(buffer, sizeof(buffer), written);
	}
}

void   IboxStandalone::end_motion()
{
	vpr::Uint32 written;
	port->write(0, 1, written);
	port->flushQueue(vpr::SerialTypes::IO_QUEUES);
}

vpr::ReturnStatus    IboxStandalone::check_packet()
{
	vpr::Uint16 size;
	if ( build_packet()==vpr::ReturnStatus::Succeed )
	{
		return(parse_packet());
	}
	else
	{
		if ( port->getBufferSize(size).failure() )
		{
			return(vpr::ReturnStatus::Timeout);
		}
		else
		{
			return(vpr::ReturnStatus::InProgress);
		}
	}
}

vpr::ReturnStatus    IboxStandalone::check_motion()
{
	if ( build_packet()==vpr::ReturnStatus::Succeed )
	{
		return(parse_packet());
	}
	else
	{
		return(vpr::ReturnStatus::InProgress);
	}
}

vpr::ReturnStatus  IboxStandalone::build_packet()
{
	unsigned char ch;
	int cmdInt;
	vpr::Uint32 written;

    if ( mPacket.parsed )
	{
		if ( port->read(&ch, 1, written,fastTimeOut).failure() )
		{
			std::cerr << "[IBox] Failure to get first byte" << std::endl;
			return(vpr::ReturnStatus::Fail);
		}
		else
		{
			mPacket.cmd_byte = (byte) ch;				//Set the returned command byte, it will tell us what is in it
			cmdInt = ch;
            if ( cmdInt < 0x80 )		   //This is because the 7th bit is always set high when echoed back
			{
				std::cerr << "[IBox] Not a valid command!" << std::endl;
				mPacket.error = 1;
				return(vpr::ReturnStatus::Fail);
			}
			else
			{
				mPacket.parsed = 0;
				mPacket.error = 0;
				mPacket.data_ptr = mPacket.data;
				mPacket.num_bytes_needed = packet_size(cmdInt);
                mPackets_expected--;
                if ( mPacket.num_bytes_needed > 0 )
				{
					while ( port->read(&ch, 1, written,fastTimeOut).success() && --mPacket.num_bytes_needed != 0 )
					{
                        *(mPacket.data_ptr)++ = (byte) ch;
					}
				}
                return(vpr::ReturnStatus::Succeed);
			}
		}
	}
	return(vpr::ReturnStatus::InProgress);
}

vpr::ReturnStatus    IboxStandalone::parse_packet()
{
	byte cmnd = mPacket.cmd_byte;
	unsigned char bits, temp;
	byte *dp;
	int *p, *q;

	if ( mPacket.num_bytes_needed < 0 )
	{
		return(vpr::ReturnStatus::Fail);
	}
	if ( mPacket.error )
	{
		return(vpr::ReturnStatus::Fail);
	}

	invalidate_fields();
	dp = mPacket.data;
	if ( cmnd < CONFIG_MIN )
	{
		bits = *dp++;			//Bits = the second byte of the data
		mButtons = bits;		//mbuttons simply = this first byte
		p = mButton;
		*p++ = bits & 0x01;
		*p++ = bits & 0x02;
		*p++ = bits & 0x04;
		*p++ = bits & 0x08;
		*p++ = bits & 0x10;
		*p++ = bits & 0x20;
		*p++ = bits & 0x40;
		if ( cmnd & TIMER_BIT )
		{
			temp = *dp++ << 7;
			temp += *dp++;
			mTimer = temp;
			mTimer_updated = 1;
		}
		bits = (cmnd & ANALOG_BITS) >> 2;
		if ( bits-- )
		{
			p = mAnalog;
			q = mAnalog_updated;
			*p++ = *dp++ << 1;
			*p++ = *dp++ << 1;
			*q++ = 1, *q++ = 1;
			if ( bits-- )
			{
				*p++ = *dp++ << 1;
				*p++ = *dp++ << 1;
				*q++ = 1, *q++ = 1;
				if ( bits-- )
				{
					*p++ = *dp++ << 1;
					*p++ = *dp++ << 1;
					*p++ = *dp++ << 1;
					*p++ = *dp++ << 1;
					*q++ = 1, *q++ = 1;
					*q++ = 1, *q++ = 1;
				}
			}
			p = mAnalog;
			*p++ |= (*dp & 0x40 ? 1 : 0);
			*p++ |= (*dp & 0x20 ? 1 : 0);
			*p++ |= (*dp & 0x10 ? 1 : 0);
			*p++ |= (*dp & 0x08 ? 1 : 0);
			*p++ |= (*dp & 0x04 ? 1 : 0);
			*p++ |= (*dp & 0x02 ? 1 : 0);
			*p++ |= (*dp++ & 0x01 ? 1 : 0);
		}
		bits = cmnd & ENCODER_BITS;
		if ( bits-- )
		{
			p = mEncoder;
			q = mEncoder_updated;
			*p = *dp++ << 7;
			*p++ += *dp++;
			*p = *dp++ << 7;
			*p++ += *dp++;
			*q++ = 1, *q++ = 1;
			if ( bits-- )
			{
				*p = *dp++ << 7;
				*p++ += *dp++;
				*q++ = 1;
				if ( bits-- )
				{
					*p = *dp++ << 7;
					*p++ += *dp++;
					*p = *dp++ << 7;
					*p++ += *dp++;
					*p = *dp++ << 7;
					*p++ += *dp++;
					*q++ = 1, *q++ = 1, *q++ = 1;
				}
			}
		}
	}
	else
	{
		return(parse_cfg_packet());
	}
	mPacket.parsed = 1;
	//}
	return(vpr::ReturnStatus::Succeed);
}

vpr::ReturnStatus    IboxStandalone::parse_cfg_packet()
{
	byte *dp = mPacket.data;
	int *p;
	std::cout << "Parsing config!!!!" <<std::endl;
	switch ( mPacket.cmd_byte )
	{
		case GET_HOME_REF:
			p = mHome_ref;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			break;
		case GET_MAXES:
			p = mButton_supported;
			*p++ = *dp & 0x01; *p++ = *dp & 0x02;
			*p++ = *dp & 0x04; *p++ = *dp & 0x08;
			*p++ = *dp & 0x10; *p++ = *dp & 0x20;
			*p++ = *dp++ & 0x40;

			mMax_timer = *dp++ << 8;
			mMax_timer |= *dp++;

			p = mMax_analog;
			*p++ = *dp++; *p++ = *dp++; *p++ = *dp++;
			*p++ = *dp++; *p++ = *dp++; *p++ = *dp++;
			*p++ = *dp++; *p++ = *dp++;
			p = mMax_analog;
			*p++ |= (0x40 & *dp ? 0x01 : 0);
			*p++ |= (0x20 & *dp ? 0x01 : 0);
			*p++ |= (0x10 & *dp ? 0x01 : 0);
			*p++ |= (0x08 & *dp ? 0x01 : 0);
			*p++ |= (0x04 & *dp ? 0x01 : 0);
			*p++ |= (0x02 & *dp ? 0x01 : 0);
			*p++ |= (0x01 & *dp++ ? 0x01 : 0);

			p = mMax_encoder;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			*p = *dp++ << 8; *p++ += *dp++;
			break;
		case INSERT_MARKER:
			mMarker = *dp;
			mMarker_updated = 1;
			break;
		case HOME_POS:	/* No action needed */
		case REPORT_MOTION:
			break;
		default:
			return(vpr::ReturnStatus::Fail);
			break;
	}
	return(vpr::ReturnStatus::Succeed);
}

int    IboxStandalone::packet_size(int cmd)
{
	int size = 1;	/* Regular cmds always include buttons byte */
	int bits;

	if ( cmd < CONFIG_MIN )
	{
		if ( cmd & TIMER_BIT )
		{
			size += 2;
		}

		bits = cmd & ANALOG_BITS;			//Extract ANALOG bits and TEST
		if ( bits == ANALOG_LO_BIT )
		{
			size += 2 + 1;
		}
		else if ( bits == ANALOG_HI_BIT )
		{
			size += 4 + 1;
		}
		else if ( bits == ANALOG_BITS )
		{
			size += 8 + 1;
		}

		bits = cmd & ENCODER_BITS;			//Extract Encoder bits and TEST
		if ( bits == ENCODER_LO_BIT )
		{
			size += 2*2;
		}
		else if ( bits == ENCODER_HI_BIT )
		{
			size += 2*3;
		}
		else if ( bits == ENCODER_BITS )
		{
			size += 2*6;
		}
	}
	else switch ( cmd )
		{
			case GET_HOME_REF:
				size = 12;
				break;
			case SET_BAUD:
			case INSERT_MARKER:
				size = 1;
				break;
			case GET_MAXES:
				size = 24;
				break;
			case HOME_POS:
			case END_SESSION:
			case REPORT_MOTION:
				size = 0;
				break;
			case GET_PARAMS:
			case GET_PROD_NAME:
			case GET_PROD_ID:
			case GET_MODEL_NAME:
			case GET_SERNUM:
			case GET_COMMENT:
			case GET_PRM_FORMAT:
			case GET_VERSION:
				//case SET_PARAMS:
			case SET_HOME:
			case SET_HOME_REF:
			case RESTORE_FACTORY:
				size = -1;
				break;
		}

	return size;
}

int IboxStandalone::buttonFunc(int pos)
{
	return mButton[pos];
}

int IboxStandalone::analogFunc(int pos)
{
	return mAnalog[pos];
}

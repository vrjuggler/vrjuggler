// -------------------------------------------------------
// Ibox device driver
//
// Purpose:
//      ibox management class
//
// Author:
//      Josh Brown
//
// Created:
//      6/26/01
// -------------------------------------------------------

#define MIN_TIMEOUT 0.1


#include <vjConfig.h>
#include <stdio.h>
#include <Input/ibox/ibox2.h>


char SIGNON_STR[5] = "IMMC"; 
char BEGIN_STR[6] = "BEGIN";


// ----------------------------------------------------------------------------
// Constructor:  This sets all the default values for the ibox.
// ----------------------------------------------------------------------------
ibox2::ibox2() {
        port = NULL;
        slow_timeout = 3.0;
        fast_timeout = 0.1;
        overlap = 0;
        
        /* Set all descr. strings to null strings */
        serial_number[0] = 0;
        product_name[0] = 0;
        product_id[0] = 0;
        model_name[0] = 0;
        comment[0] = 0;
        param_format[0] = 0;
        version[0] = 0;

        /* By default, no error handlers are installed */
        BAD_PORT_handler = NULL;
        CANT_OPEN_handler = NULL;
        NO_HCI_handler = NULL;
        CANT_BEGIN_handler = NULL;
        TIMED_OUT_handler = NULL;
        BAD_PACKET_handler = NULL;
        BAD_VERSION_handler = NULL;
        BAD_FORMAT_handler = NULL;

        default_handler = NULL;
}
// ----------------------------------------------------------------------------
// Destructor. Deletes the port
// ----------------------------------------------------------------------------
ibox2::~ibox2(){
        delete port;
}

ibox2_result ibox2::connect(const std::string& port_name,  long int baud){
        ibox2_result result=SUCCESS;
        name = port_name;
        port = new vpr::SerialPort(name);
        port->setOpenReadWrite();
        if(port->open().success()){
                port->setTimeout((int)slow_timeout*10);
                port->setCharacterSize(vpr::SerialTypes::CS_BITS_8);
                      port->enableRead(); 
                port->setBufferSize(0);
                port->setOutputBaudRate(baud); // Put me before input to be safe
                port->setInputBaudRate(baud);
                if(autosynch()){
                        if(!begin()){
                                result = CANT_BEGIN;
                        }        
                }else{
                        result = NO_HCI;
                } 
        
        }else{
                result = CANT_OPEN_PORT;
        }
        return result;
}

ibox2_result ibox2::fancy_connect(const std::string& port_name, long int baud , void (*installer_fun)()){
        ibox2_result result;
        port = new vpr::SerialPort(name);
        port->setOpenReadWrite();
        (*installer_fun)();
        if(port->open().success()){
                port->setTimeout((int)slow_timeout*10);
                if(autosynch()){
                        if(!begin()){
                                result = CANT_BEGIN;
                        }
                }else{
                        result = NO_HCI;
                }
        }else{
                result = CANT_OPEN_PORT;
        }
        return result;
}        

ibox2_result ibox2::wait_update(int timer_flag, int num_analogs ,int num_encoders){
        std_cmd(timer_flag, num_analogs, num_encoders);        
        return wait_packet();
        
}

void ibox2::disconnect(){
        port->flushQueue(vpr::SerialTypes::IO_QUEUES);
        end();
        port->close();
}


void ibox2::std_cmd(int timer_flag, int analog_reports, int encoder_reports){
        vpr::Uint32 written;
        char cmnd = CMD_BYTE(timer_flag, analog_reports, encoder_reports);
        port->write(&cmnd, 1, written);
        if(!overlap){
                port->setTimeout(fast_timeout);
        }
}


void ibox2::simple_cfg_cmd(byte cmnd){
        vpr::Uint32 written;        
        char temp = cmnd;        
        char* buffer = &temp;
        port->write(buffer, sizeof(buffer), written);
        if(!overlap){
                port->setTimeout(fast_timeout);
        }

}        

ibox2_result ibox2::string_cmd(byte cmnd){
        vpr::Uint16 size;
        ibox2_result result;
        char temp= cmnd;
        char ch[2];
        vpr::Uint32 written;
        char* buffer = &temp;
        port->write(buffer, 1, written);
        port->setTimeout(fast_timeout);
        port->read(ch, 1, written);
        while(ch[0]!=cmnd){
                if(ch!=0) port->setTimeout(fast_timeout);
                if(port->getBufferSize(size).failure()) break; 
                port->read(ch, 1, written);
        }
        if(ch[0]!=cmnd) return error(TIMED_OUT);

        switch (cmnd)
        {
                case GET_PROD_NAME:
                        result = read_string(product_name);
                        break;
                case GET_PROD_ID:
                        result = read_string(product_id);
                        break;
                case GET_MODEL_NAME:
                        result = read_string(model_name);
                        break;
                case GET_SERNUM:
                        result = read_string(serial_number);
                        break;
                case GET_COMMENT:
                        result = read_string(comment);
                        break;
                case GET_PRM_FORMAT:
                        result = read_string(param_format);
                        break;
                case GET_VERSION:
                        result = read_string(version);
                        break;
                default:
                        result = SUCCESS;
                        break;
        }

        return result;
} 

ibox2_result ibox2::passwd_cmd(byte cmnd){
        char temp = cmnd;
        char* buffer = &temp;
        vpr::Uint16 size;        
        vpr::Uint32 written;
        char ch[2];        
        char reader[2];//was MAX_STRING_SIZE        
        port->write(buffer, 1, written);
        port->setTimeout(fast_timeout);
        
        port->read(ch, 1, written);
        while(ch[0]!=cmnd){
                if(ch!=0) port->setTimeout(fast_timeout);
                if(port->getBufferSize(size).failure()) break;
                port->read(ch, 1, written);
        }
        if(ch[0]==cmnd){
                port->write(serial_number, strlen(serial_number), written);
                port->write(NULL, 1, written);
                port->setTimeout(fast_timeout);
                port->read(reader, 1, written);
                if((unsigned char) reader[0] == PASSWD_OK){
                        port->write(cfg_args, 1, written);
                        return SUCCESS;
                }
                else return BAD_PASSWORD;
        }
        else return error(TIMED_OUT);
}


void ibox2::insert_marker(byte marker){
        simple_cfg_cmd(INSERT_MARKER);
        char temp = marker;        
        char* buffer = &temp;        
        vpr::Uint32 written;
        port->write( buffer, 1, written);
}

ibox2_result ibox2::get_params(byte *block, int *block_size){
        char ch[2];
        vpr::Uint16 size;        
        vpr::Uint32 written;
        char temp = GET_PARAMS;        
        char* send = &temp;        
        port->write(send, sizeof(GET_PARAMS), written);
        port->setTimeout(fast_timeout);
        port->read(ch, 1, written);
        while(ch[0]!=GET_PARAMS){
                if(ch!=0) port->setTimeout(fast_timeout);
                if(port->getBufferSize(size).failure()) break;
                port->read(ch, 1, written);
        }        
        if(ch[0]!= GET_PARAMS) return error(TIMED_OUT);
        *block_size = -1;
        return read_block(block, block_size);        

}

ibox2_result ibox2::set_params(byte *block, int block_size){
        int i;
        num_cfg_args = block_size;
        for(i=0;i<block_size; i++) cfg_args[i] = block[i];
        return passwd_cmd(SET_PARAMS);
}        

ibox2_result ibox2::get_home_ref(){
        simple_cfg_cmd(GET_HOME_REF);
        return wait_packet();
}

ibox2_result ibox2::set_home_ref(int *homeref){
        num_cfg_args = 2*NUM_ENCODERS;
        cfg_args[0] = homeref[0] >> 8;
        cfg_args[1] = homeref[0] & 0x00FF;
        cfg_args[2] = homeref[1] >> 8;
        cfg_args[3] = homeref[1] & 0x00FF;
        cfg_args[4] = homeref[2] >> 8;
        cfg_args[5] = homeref[2] & 0x00FF;
        cfg_args[6] = homeref[3] >> 8;
        cfg_args[7] = homeref[3] & 0x00FF;
        cfg_args[8] = homeref[4] >> 8;
        cfg_args[9] = homeref[4] & 0x00FF;
        cfg_args[10] = homeref[5] >> 8;
        cfg_args[11] = homeref[5] & 0x00FF;
        return passwd_cmd(SET_HOME_REF);
}

ibox2_result    ibox2::go_home_pos(){
        simple_cfg_cmd(HOME_POS);
        return wait_packet();
}

ibox2_result    ibox2::set_home_pos(int *homepos){
        num_cfg_args = 2*NUM_ENCODERS;
        cfg_args[0] = homepos[0] >> 8;
        cfg_args[1] = homepos[0] & 0x00FF;
        cfg_args[2] = homepos[1] >> 8;
        cfg_args[3] = homepos[1] & 0x00FF;
        cfg_args[4] = homepos[2] >> 8;
        cfg_args[5] = homepos[2] & 0x00FF;
        cfg_args[6] = homepos[3] >> 8;
        cfg_args[7] = homepos[3] & 0x00FF;
        cfg_args[8] = homepos[4] >> 8;
        cfg_args[9] = homepos[4] & 0x00FF;
        cfg_args[10] = homepos[5] >> 8;
        cfg_args[11] = homepos[5] & 0x00FF;
        return passwd_cmd(SET_HOME);
}

ibox2_result    ibox2::get_maxes(){
        simple_cfg_cmd(GET_MAXES);
        return wait_packet();
}

ibox2_result    ibox2::factory_settings(){
        ibox2_result result;
        num_cfg_args = 0;
        result = passwd_cmd(RESTORE_FACTORY);
        return result;
}

void            ibox2::report_motion(int timer_flag, int analog_reports, int encoder_reports, int delay, byte active_btns, int *analog_deltas, int *encoder_deltas){

        int i;
        vpr::Uint32 written;        
        char temp;
        char* buffer;        
        char         cmnd = CMD_BYTE(timer_flag, analog_reports, encoder_reports);
        temp = REPORT_MOTION;
        buffer = &temp;        
        port->write(buffer, 1, written);
        temp = delay >> 8;
        buffer = &temp;
        port->write(buffer, 1, written);
        temp = delay & 0x00FF;
        buffer = &temp;
        port->write(buffer, 1, written);
        buffer = &cmnd;
        port->write(buffer, 1, written);
        
        port->write(analog_deltas, sizeof(analog_deltas), written);

        for(i=0;i<NUM_ENCODERS;i++){
                temp = encoder_deltas[i];
                buffer = &temp;
                port->write(buffer, sizeof(buffer), written);
        }        
}

void   ibox2::end_motion(){
        vpr::Uint32 written;
        port->write(0, 1, written);
        port->flushQueue(vpr::SerialTypes::IO_QUEUES);
}

ibox2_result    ibox2::wait_packet(){
        ibox2_result result;
        port->setTimeout(fast_timeout);
        while( (result = check_packet()) == NO_PACKET_YET);
        return result;
}
        
ibox2_result    ibox2::check_packet(){
        ibox2_result result;
        vpr::Uint16 size;
        if(build_packet()){
                result = parse_packet();
                if(result == SUCCESS) return result;
                else return error(result);
        }else{
                if(port->getBufferSize(size).failure())
                        return error(TIMED_OUT);
                else
                        return NO_PACKET_YET;
        }
}
        
ibox2_result    ibox2::check_motion(){
        ibox2_result result;
        if(build_packet()){
                result = parse_packet();
                if(result==SUCCESS) return result;
                else return error(result);
        }
        else return NO_PACKET_YET;
}

int  ibox2::build_packet(){
        char ch;        
        int temp;
        vpr::Uint32 written;
        if(packet.parsed){
                if(port->read(&ch, 1, written).failure()) return 0;
                else{
                        packet.cmd_byte = (byte) ch;
                        temp = ch;
                        if(temp < 0x80){
                                packet.error = 1;
                                return 1;
                        }else{
                                packet.parsed = 0;
                                packet.error = 0;
                                packet.data_ptr = packet.data;
                                packet.num_bytes_needed = packet_size(temp);
                                packets_expected--;
                                port->setTimeout(fast_timeout);
                        }
                }
        }        
        else if (packet.num_bytes_needed > 0){
                while(port->read(&ch, 1, written).success()){
                        *(packet.data_ptr)++ = (byte) ch;
                        if(--packet.num_bytes_needed == 0) break;
                }
        }
        return (packet.num_bytes_needed <= 0);
}        

ibox2_result    ibox2::parse_packet(){
        char cmnd = packet.cmd_byte, bits, temp;
        ibox2_result result = SUCCESS;
        byte *dp;
        int *p, *q;
        
        if(packet.num_bytes_needed){
                if(packet.num_bytes_needed < 0)
                        result = BAD_PACKET;
                else return NO_PACKET_YET;
        }
        if(packet.error) result = error(BAD_PACKET);
        
        if(result == SUCCESS){
                invalidate_fields();
                dp = packet.data;
                if(cmnd < CONFIG_MIN){
                        bits = *dp++;
                        buttons = bits;
                        p = button;
                        *p++ = bits & 0x01;
                        *p++ = bits & 0x012;
                        *p++ = bits & 0x04;
                        *p++ = bits & 0x08;
                        *p++ = bits & 0x10;
                        *p++ = bits & 0x20;
                        *p++ = bits & 0x40;
                        if(cmnd & TIMER_BIT){
                                temp = *dp++ << 7;
                                temp += *dp++;
                                timer = temp;
                                timer_updated = 1;
                        }
                        bits = (cmnd & ANALOG_BITS) >> 2;
                        if(bits--){
                                p = analog;
                                q = analog_updated;
                                *p++ = *dp++ << 1;
                                *p++ = *dp++ << 1;
                                *q++ = 1, *q++ = 1;
                                if(bits--){
                                        *p++ = *dp++ << 1;
                                        *p++ = *dp++ << 1;
                                        *q++ = 1, *q++ = 1;
                                        if(bits--){
                                                *p++ = *dp++ << 1;
                                                *p++ = *dp++ << 1;
                                                *p++ = *dp++ << 1;
                                                *p++ = *dp++ << 1;
                                                *q++ = 1, *q++ = 1;
                                                *q++ = 1, *q++ = 1;
                                        }
                                }
                                p = analog;
                                *p++ |= (*dp & 0x40 ? 1 : 0);
                                *p++ |= (*dp & 0x20 ? 1 : 0);
                                *p++ |= (*dp & 0x10 ? 1 : 0);
                                *p++ |= (*dp & 0x08 ? 1 : 0);
                                *p++ |= (*dp & 0x04 ? 1 : 0);
                                *p++ |= (*dp & 0x02 ? 1 : 0);
                                *p++ |= (*dp++ & 0x01 ? 1 : 0);
                        }
                        bits = cmnd & ENCODER_BITS;
                        if (bits--){
                                p = encoder;
                                q = encoder_updated;
                                *p = *dp++ << 7;
                                *p++ += *dp++;
                                *p = *dp++ << 7;
                                *p++ += *dp++;
                                *q++ = 1, *q++ = 1;
                                if(bits--){
                                        *p = *dp++ << 7;
                                        *p++ += *dp++;
                                        *q++ = 1;
                                        if(bits--){
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
                else result = parse_cfg_packet();
                packet.parsed = 1;
        }
        return result;
}

ibox2_result    ibox2::parse_cfg_packet(){
        ibox2_result result = SUCCESS;
        byte *dp = packet.data;
        int *p;

        switch(packet.cmd_byte)
        {
                case GET_HOME_REF:
                        p = home_ref;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        break;
                case GET_MAXES:
                        p = button_supported;
                        *p++ = *dp & 0x01; *p++ = *dp & 0x02;
                        *p++ = *dp & 0x04; *p++ = *dp & 0x08;
                        *p++ = *dp & 0x10; *p++ = *dp & 0x20;
                        *p++ = *dp++ & 0x40;

                        max_timer = *dp++ << 8;
                        max_timer |= *dp++;

                        p = max_analog;
                        *p++ = *dp++; *p++ = *dp++; *p++ = *dp++;
                        *p++ = *dp++; *p++ = *dp++; *p++ = *dp++;
                        *p++ = *dp++; *p++ = *dp++;
                        p = max_analog;
                        *p++ |= (0x40 & *dp ? 0x01 : 0);
                        *p++ |= (0x20 & *dp ? 0x01 : 0);
                        *p++ |= (0x10 & *dp ? 0x01 : 0);
                        *p++ |= (0x08 & *dp ? 0x01 : 0);
                        *p++ |= (0x04 & *dp ? 0x01 : 0);
                        *p++ |= (0x02 & *dp ? 0x01 : 0);
                        *p++ |= (0x01 & *dp++ ? 0x01 : 0);

                        p = max_encoder;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        *p = *dp++ << 8; *p++ += *dp++;
                        break;
                case INSERT_MARKER:
                        marker = *dp;
                        marker_updated = 1;
                        break;
                case HOME_POS:  /* No action needed */
                case REPORT_MOTION:
                        break;
                default:
                        result = BAD_PACKET;
                        break;
        }

        return result;
}        

int    ibox2::packet_size(int cmd){
        int size = 1;   /* Regular cmds always include buttons byte */
        int bits;
        
        if (cmd < CONFIG_MIN)
        {
                if (cmd & TIMER_BIT) size += 2;
                bits = cmd & ANALOG_BITS;
                if (bits == ANALOG_LO_BIT) size += 2 + 1;
                else if (bits == ANALOG_HI_BIT) size += 4 + 1;
                else if (bits == ANALOG_BITS) size += 8 + 1;
                bits = cmd & ENCODER_BITS;
                if (bits == ENCODER_LO_BIT) size += 2*2;
                else if (bits == ENCODER_HI_BIT) size += 2*3;
                else if (bits == ENCODER_BITS) size += 2*6;
        }
        else switch (cmd)
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
                case SET_PARAMS:
                case SET_HOME:
                case SET_HOME_REF:
                case RESTORE_FACTORY:
                        size = -1;
                        break;
        }

        return size;
}

ibox2_result   ibox2:: read_string(char *str){
        vpr::Uint32 written;
        if(port->read(str, MAX_STRING_SIZE, written).success()) return SUCCESS;
        else return TIMED_OUT;

}

ibox2_result ibox2::read_block(byte *block, int *num_bytes){
        int count;
        vpr::Uint32 written;
        char *ch;
        vpr::Uint16 size;
        count = *num_bytes;
        *num_bytes = 0;
        port->read(ch, 1, written);
        while(port->getBufferSize(size).success()){
                if(count < 0) count = *ch;
                else{
                        if (*ch != -1){
                                *block++ = (byte) *ch;
                                (*num_bytes)++;
                                if(--count == 0) return SUCCESS;
                        }
                        port->read(ch, 1, written);        
                }
                return TIMED_OUT;
        }
}        

void   ibox2::invalidate_fields(){
        timer_updated = 0;
        analog_updated[0] = 0;
        analog_updated[1] = 0;
        analog_updated[2] = 0;
        analog_updated[3] = 0;
        analog_updated[4] = 0;
        analog_updated[5] = 0;
        analog_updated[6] = 0;
        analog_updated[7] = 0;
        encoder_updated[0] = 0;
        encoder_updated[1] = 0;
        encoder_updated[2] = 0;
        encoder_updated[3] = 0;
        encoder_updated[4] = 0;
        encoder_updated[5] = 0;
        marker_updated = 0;
}


ibox2_result    ibox2::error(ibox2_result condition){
        ibox2_result (*handler) ();
        
        /* These two are not really errors */
        if (condition == SUCCESS) return SUCCESS;
        if (condition == NO_PACKET_YET) return NO_PACKET_YET;

        /* These errors have handler pointers */
        if (condition == BAD_PORT_NUM)
                handler = BAD_PORT_handler;
        else if (condition== CANT_OPEN_PORT)
                handler = CANT_OPEN_handler;
        else if (condition == NO_HCI)
                handler = NO_HCI_handler;
        else if (condition == CANT_BEGIN)
                handler = CANT_BEGIN_handler;
        else if (condition == TIMED_OUT)
                handler = TIMED_OUT_handler;
        else if (condition == BAD_PACKET)
                handler = BAD_PACKET_handler;
        else if (condition == BAD_PASSWORD)
                handler = BAD_PASSWORD_handler;
        else if (condition == BAD_VERSION)
                handler = BAD_VERSION_handler;
        else if (condition == BAD_FORMAT)
                handler = BAD_FORMAT_handler;
        else handler = NULL;

        if (handler == NULL) handler = default_handler;
        if (handler == NULL) return condition;

        return (*handler)();
}

int ibox2::autosynch(){
        vpr::Uint16 size;
        ssize_t ch=0;
        vpr::Uint32 written;
        int signed_on=0;
        int timed_out=0;
        char *sign_ch = SIGNON_STR;
//        char temp[sizeof(SIGNON_STR) - 1];
        char temp;
        vpr::Status status;

        while(!signed_on && !port->getBufferSize(size).failure()){
                end();
                port->write(SIGNON_STR, sizeof(SIGNON_STR) - 1, written);
                vpr::System::msleep(15); /* XXX: Artificial pause */
                status = port->read(&temp, 1, written);

                while(status != vpr::Status::Failure && written != 0){
                        
                        if(temp == *sign_ch){
                                if(!*++sign_ch){
                                        signed_on=1;
                                        break;
                                }
                        }else{
                                sign_ch = SIGNON_STR;
                        }
                        status = port->read(&temp, 1, written);
                }
        }
        port->flushQueue(vpr::SerialTypes::IO_QUEUES);
        if(signed_on) return true;
        else return false;
}

int ibox2::begin(){
        int ch;
        vpr::Uint32 written;
//        char* temp;
        port->write(BEGIN_STR,sizeof(BEGIN_STR) - 1, written);
        port->setTimeout(slow_timeout);
        if(port->read(product_id, sizeof(product_id), written).success()){
                return true;
        }else{
                return false;
        }
}


int ibox2::end(){
        vpr::Uint32 written;

        char temp = END_SESSION;
        char* buffer = &temp; 
        if(port->write(buffer, sizeof(buffer), written).success()){
                return true;
        }else{
                return false;
        }
}

int ibox2::buttonFunc(int pos){
        return button[pos];
}

int ibox2::analogFunc(int pos){
        return analog[pos];
}

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * PyJuggler is (C) Copyright 2002, 2003 by Patrick Hartling
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

// Includes ====================================================================
#include <boost/python.hpp>
#include <gadget/Type/PositionProxy.h>
#include <gadgeteer-wrappers.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/DeviceInterface.h>
#include <gadget/Type/AnalogProxy.h>
#include <gadget/Type/EventWindow/Keys.h>
#include <gadget/Type/EventWindow/MouseEvent.h>
#include <gadget/Type/DigitalData.h>
#include <gadget/Type/EventWindow/KeyEvent.h>
#include <gadget/Type/EventWindow/Event.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Position.h>
#include <gadget/Type/DigitalProxy.h>
#include <gadget/Type/AnalogData.h>
#include <gadget/Type/PositionData.h>

// Using =======================================================================
using namespace boost::python;

// Declarations ================================================================


namespace  {


struct gadget_BaseDeviceInterface_Wrapper: gadget::BaseDeviceInterface
{
    gadget_BaseDeviceInterface_Wrapper(PyObject* self_, const gadget::BaseDeviceInterface & p0):
        gadget::BaseDeviceInterface(p0), self(self_) {}

    gadget_BaseDeviceInterface_Wrapper(PyObject* self_):
        gadget::BaseDeviceInterface(), self(self_) {}

    void refresh() {
        try
        {
            call_method< void >(self, "refresh");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_refresh() {
        gadget::BaseDeviceInterface::refresh();
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadgetWrapper_PositionInterface_getData_overloads_0_1, getData, 0, 1)

struct gadget_AnalogProxy_Wrapper: gadget::AnalogProxy
{
    gadget_AnalogProxy_Wrapper(PyObject* self_, const gadget::AnalogProxy & p0):
        gadget::AnalogProxy(p0), self(self_) {}

    gadget_AnalogProxy_Wrapper(PyObject* self_):
        gadget::AnalogProxy(), self(self_) {}

    void updateData() {
        try
        {
            call_method< void >(self, "updateData");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_updateData() {
        gadget::AnalogProxy::updateData();
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::AnalogProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Analog * p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Analog * p1) {
        gadget::AnalogProxy::set(p0, p1);
    }

    bool refresh() {
        try
        {
            return call_method< bool >(self, "refresh");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_refresh() {
        return gadget::AnalogProxy::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::AnalogProxy::getDeviceName();
    }

    bool isStupified() const {
        try
        {
            return call_method< bool >(self, "isStupified");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_isStupified() const {
        return gadget::AnalogProxy::isStupified();
    }

    PyObject* self;
};

struct gadget_DigitalProxy_Wrapper: gadget::DigitalProxy
{
    gadget_DigitalProxy_Wrapper(PyObject* self_, const gadget::DigitalProxy & p0):
        gadget::DigitalProxy(p0), self(self_) {}

    gadget_DigitalProxy_Wrapper(PyObject* self_):
        gadget::DigitalProxy(), self(self_) {}

    void updateData() {
        try
        {
            call_method< void >(self, "updateData");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_updateData() {
        gadget::DigitalProxy::updateData();
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::DigitalProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Digital * p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Digital * p1) {
        gadget::DigitalProxy::set(p0, p1);
    }

    bool refresh() {
        try
        {
            return call_method< bool >(self, "refresh");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_refresh() {
        return gadget::DigitalProxy::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::DigitalProxy::getDeviceName();
    }

    bool isStupified() const {
        try
        {
            return call_method< bool >(self, "isStupified");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_isStupified() const {
        return gadget::DigitalProxy::isStupified();
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_PositionProxy_getData_overloads_0_1, getData, 0, 1)

struct gadget_PositionProxy_Wrapper: gadget::PositionProxy
{
    gadget_PositionProxy_Wrapper(PyObject* self_, const gadget::PositionProxy & p0):
        gadget::PositionProxy(p0), self(self_) {}

    gadget_PositionProxy_Wrapper(PyObject* self_):
        gadget::PositionProxy(), self(self_) {}

    void updateData() {
        try
        {
            call_method< void >(self, "updateData");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_updateData() {
        gadget::PositionProxy::updateData();
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::PositionProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Position * p1) {
        try
        {
            call_method< void >(self, "set", p0, p1);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Position * p1) {
        gadget::PositionProxy::set(p0, p1);
    }

    bool refresh() {
        try
        {
            return call_method< bool >(self, "refresh");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_refresh() {
        return gadget::PositionProxy::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::PositionProxy::getDeviceName();
    }

    bool isStupified() const {
        try
        {
            return call_method< bool >(self, "isStupified");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_isStupified() const {
        return gadget::PositionProxy::isStupified();
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Analog_getAnalogData_overloads_0_1, getAnalogData, 0, 1)

struct gadget_Analog_Wrapper: gadget::Analog
{
    gadget_Analog_Wrapper(PyObject* self_):
        gadget::Analog(), self(self_) {}

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::Analog::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::Analog::readObject(p0);
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::Analog::config(p0);
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getBaseType() {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getBaseType");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKONWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getBaseType() {
        return gadget::Analog::getBaseType();
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Digital_getDigitalData_overloads_0_1, getDigitalData, 0, 1)

struct gadget_Digital_Wrapper: gadget::Digital
{
    gadget_Digital_Wrapper(PyObject* self_):
        gadget::Digital(), self(self_) {}

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::Digital::config(p0);
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getBaseType() {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getBaseType");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKONWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getBaseType() {
        return gadget::Digital::getBaseType();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::Digital::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::Digital::readObject(p0);
    }

    PyObject* self;
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(gadget_Position_getPositionData_overloads_0_1, getPositionData, 0, 1)

struct gadget_Position_Wrapper: gadget::Position
{
    gadget_Position_Wrapper(PyObject* self_):
        gadget::Position(), self(self_) {}

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        try
        {
            return call_method< bool >(self, "config", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return false;
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::Position::config(p0);
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getBaseType() {
        try
        {
            return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getBaseType");
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return std::string("UNKNOWN");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getBaseType() {
        return gadget::Position::getBaseType();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::Position::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::Position::readObject(p0);
    }

    PyObject* self;
};

struct gadget_KeyEvent_Wrapper: gadget::KeyEvent
{
    gadget_KeyEvent_Wrapper(PyObject* self_, const gadget::KeyEvent & p0):
        gadget::KeyEvent(p0), self(self_) {}

    gadget_KeyEvent_Wrapper(PyObject* self_, const gadget::EventType & p0, const gadget::Keys & p1, const int & p2, const long unsigned int & p3):
        gadget::KeyEvent(p0, p1, p2, p3), self(self_) {}

    gadget_KeyEvent_Wrapper(PyObject* self_, const gadget::EventType & p0, const gadget::Keys & p1, const int & p2, const long unsigned int & p3, char p4):
        gadget::KeyEvent(p0, p1, p2, p3, p4), self(self_) {}

    gadget_KeyEvent_Wrapper(PyObject* self_):
        gadget::KeyEvent(), self(self_) {}

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::KeyEvent::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::KeyEvent::readObject(p0);
    }

    PyObject* self;
};

struct gadget_MouseEvent_Wrapper: gadget::MouseEvent
{
    gadget_MouseEvent_Wrapper(PyObject* self_, const gadget::MouseEvent & p0):
        gadget::MouseEvent(p0), self(self_) {}

    gadget_MouseEvent_Wrapper(PyObject* self_, const gadget::EventType & p0, const gadget::Keys & p1, const int & p2, const int & p3, const int & p4, const int & p5, const int & p6, const long unsigned int & p7):
        gadget::MouseEvent(p0, p1, p2, p3, p4, p5, p6, p7), self(self_) {}

    gadget_MouseEvent_Wrapper(PyObject* self_):
        gadget::MouseEvent(), self(self_) {}

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::MouseEvent::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        try
        {
            return call_method< vpr::ReturnStatus >(self, "readObject", p0);
        }
        catch(error_already_set)
        {
            PyErr_Print();
        }

        return vpr::ReturnStatus::Fail;
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::MouseEvent::readObject(p0);
    }

    PyObject* self;
};



}// namespace 


// Module ======================================================================
BOOST_PYTHON_MODULE(gadget)
{
    class_< gadget::BaseDeviceInterface, gadget_BaseDeviceInterface_Wrapper >("BaseDeviceInterface", init<  >())
        .def(init< const gadget::BaseDeviceInterface & >())
        .def("init", &gadget::BaseDeviceInterface::init)
        .def("getProxyName", &gadget::BaseDeviceInterface::getProxyName)
        .def("isConnected", &gadget::BaseDeviceInterface::isConnected)
        .def("refreshAllDevices", &gadget::BaseDeviceInterface::refreshAllDevices)
        .staticmethod("refreshAllDevices")
        .def("refresh", &gadget::BaseDeviceInterface::refresh, &gadget_BaseDeviceInterface_Wrapper::default_refresh)
    ;

    class_< gadgetWrapper::AnalogInterface, boost::noncopyable >("AnalogInterface", init<  >())
        .def(init< gadgetWrapper::AnalogInterface & >())
        .def("init", &gadgetWrapper::AnalogInterface::init)
        .def("refresh", &gadgetWrapper::AnalogInterface::refresh)
        .def("getProxyName", &gadgetWrapper::AnalogInterface::getProxyName)
        .def("isConnected", &gadgetWrapper::AnalogInterface::isConnected)
        .def("getProxy", &gadgetWrapper::AnalogInterface::getProxy, return_internal_reference< 1 >())
        .def("getData", &gadgetWrapper::AnalogInterface::getData)
    ;

    class_< gadgetWrapper::DigitalInterface, boost::noncopyable >("DigitalInterface", init<  >())
        .def(init< gadgetWrapper::DigitalInterface & >())
        .def("init", &gadgetWrapper::DigitalInterface::init)
        .def("refresh", &gadgetWrapper::DigitalInterface::refresh)
        .def("getProxyName", &gadgetWrapper::DigitalInterface::getProxyName)
        .def("isConnected", &gadgetWrapper::DigitalInterface::isConnected)
        .def("getProxy", &gadgetWrapper::DigitalInterface::getProxy, return_internal_reference< 1 >())
        .def("getData", &gadgetWrapper::DigitalInterface::getData)
    ;

    class_< gadgetWrapper::PositionInterface, boost::noncopyable >("PositionInterface", init<  >())
        .def(init< gadgetWrapper::PositionInterface & >())
        .def("init", &gadgetWrapper::PositionInterface::init)
        .def("refresh", &gadgetWrapper::PositionInterface::refresh)
        .def("getProxyName", &gadgetWrapper::PositionInterface::getProxyName)
        .def("isConnected", &gadgetWrapper::PositionInterface::isConnected)
        .def("getProxy", &gadgetWrapper::PositionInterface::getProxy, return_internal_reference< 1 >())
        .def("getTimeStamp", &gadgetWrapper::PositionInterface::getTimeStamp)
        .def("getData", &gadgetWrapper::PositionInterface::getData, gadgetWrapper_PositionInterface_getData_overloads_0_1())
    ;

    class_< gadget::AnalogData >("AnalogData", init<  >())
        .def(init< const gadget::AnalogData & >())
        .def("getAnalog", &gadget::AnalogData::getAnalog)
        .def("setAnalog", &gadget::AnalogData::setAnalog)
    ;

    class_< gadget::DigitalData >("DigitalData", init<  >())
        .def(init< const gadget::DigitalData & >())
        .def("getDigital", &gadget::DigitalData::getDigital)
        .def("setDigital", &gadget::DigitalData::setDigital)
    ;

    class_< gadget::PositionData >("PositionData", init<  >())
        .def(init< const gadget::PositionData & >())
        .def_readwrite("mPosData", &gadget::PositionData::mPosData)
        .def("getPosition", &gadget::PositionData::getPosition)
        .def("setPosition", &gadget::PositionData::setPosition)
    ;

    class_< gadget::AnalogProxy, gadget_AnalogProxy_Wrapper >("AnalogProxy", init<  >())
        .def(init< const gadget::AnalogProxy & >())
        .def("getData", &gadget::AnalogProxy::getData)
        .def("getAnalogPtr", &gadget::AnalogProxy::getAnalogPtr, return_internal_reference< 1 >())
        .def("getUnit", &gadget::AnalogProxy::getUnit)
        .def("getChunkType", &gadget::AnalogProxy::getChunkType)
        .staticmethod("getChunkType")
        .def("updateData", &gadget::AnalogProxy::updateData, &gadget_AnalogProxy_Wrapper::default_updateData)
        .def("config", &gadget::AnalogProxy::config, &gadget_AnalogProxy_Wrapper::default_config)
        .def("set", &gadget::AnalogProxy::set, &gadget_AnalogProxy_Wrapper::default_set)
        .def("refresh", &gadget::AnalogProxy::refresh, &gadget_AnalogProxy_Wrapper::default_refresh)
        .def("getDeviceName", &gadget::AnalogProxy::getDeviceName, &gadget_AnalogProxy_Wrapper::default_getDeviceName)
        .def("isStupified", &gadget::AnalogProxy::isStupified, &gadget_AnalogProxy_Wrapper::default_isStupified)
    ;

    class_< gadget::DigitalProxy, gadget_DigitalProxy_Wrapper >("DigitalProxy", init<  >())
        .def(init< const gadget::DigitalProxy & >())
        .def("getData", &gadget::DigitalProxy::getData)
        .def("getDigitalData", &gadget::DigitalProxy::getDigitalData, return_internal_reference< 1 >())
        .def("getDigitalPtr", &gadget::DigitalProxy::getDigitalPtr, return_internal_reference< 1 >())
        .def("getUnit", &gadget::DigitalProxy::getUnit)
        .def("getChunkType", &gadget::DigitalProxy::getChunkType)
        .staticmethod("getChunkType")
        .def("updateData", &gadget::DigitalProxy::updateData, &gadget_DigitalProxy_Wrapper::default_updateData)
        .def("config", &gadget::DigitalProxy::config, &gadget_DigitalProxy_Wrapper::default_config)
        .def("set", &gadget::DigitalProxy::set, &gadget_DigitalProxy_Wrapper::default_set)
        .def("refresh", &gadget::DigitalProxy::refresh, &gadget_DigitalProxy_Wrapper::default_refresh)
        .def("getDeviceName", &gadget::DigitalProxy::getDeviceName, &gadget_DigitalProxy_Wrapper::default_getDeviceName)
        .def("isStupified", &gadget::DigitalProxy::isStupified, &gadget_DigitalProxy_Wrapper::default_isStupified)
    ;

    class_< gadget::PositionProxy, gadget_PositionProxy_Wrapper >("PositionProxy", init<  >())
        .def(init< const gadget::PositionProxy & >())
        .def("getTimeStamp", &gadget::PositionProxy::getTimeStamp)
        .def("getData", &gadget::PositionProxy::getData, gadget_PositionProxy_getData_overloads_0_1())
        .def("getPositionData", &gadget::PositionProxy::getPositionData, return_internal_reference< 1 >())
        .def("getUnit", &gadget::PositionProxy::getUnit)
        .def("getPositionPtr", &gadget::PositionProxy::getPositionPtr, return_internal_reference< 1 >())
        .def("getChunkType", &gadget::PositionProxy::getChunkType)
        .staticmethod("getChunkType")
        .def("updateData", &gadget::PositionProxy::updateData, &gadget_PositionProxy_Wrapper::default_updateData)
        .def("config", &gadget::PositionProxy::config, &gadget_PositionProxy_Wrapper::default_config)
        .def("set", &gadget::PositionProxy::set, &gadget_PositionProxy_Wrapper::default_set)
        .def("refresh", &gadget::PositionProxy::refresh, &gadget_PositionProxy_Wrapper::default_refresh)
        .def("getDeviceName", &gadget::PositionProxy::getDeviceName, &gadget_PositionProxy_Wrapper::default_getDeviceName)
        .def("isStupified", &gadget::PositionProxy::isStupified, &gadget_PositionProxy_Wrapper::default_isStupified)
    ;

    class_< gadget::Analog, boost::noncopyable, gadget_Analog_Wrapper >("Analog", init<  >())
        .def("getAnalogData", &gadget::Analog::getAnalogData, gadget_Analog_getAnalogData_overloads_0_1())
        .def("addAnalogSample", &gadget::Analog::addAnalogSample)
        .def("swapAnalogBuffers", &gadget::Analog::swapAnalogBuffers)
        .def("getAnalogDataBuffer", &gadget::Analog::getAnalogDataBuffer, return_internal_reference< 1 >())
        .def("writeObject", &gadget::Analog::writeObject, &gadget_Analog_Wrapper::default_writeObject)
        .def("readObject", &gadget::Analog::readObject, &gadget_Analog_Wrapper::default_readObject)
        .def("config", &gadget::Analog::config, &gadget_Analog_Wrapper::default_config)
        .def("getBaseType", &gadget::Analog::getBaseType, &gadget_Analog_Wrapper::default_getBaseType)
    ;

    scope* gadget_Digital_scope = new scope(
    class_< gadget::Digital, boost::noncopyable, gadget_Digital_Wrapper >("Digital", init<  >())
        .def("getDigitalData", &gadget::Digital::getDigitalData, gadget_Digital_getDigitalData_overloads_0_1())
        .def("addDigitalSample", &gadget::Digital::addDigitalSample)
        .def("swapDigitalBuffers", &gadget::Digital::swapDigitalBuffers)
        .def("getDigitalDataBuffer", &gadget::Digital::getDigitalDataBuffer, return_internal_reference< 1 >())
        .def("config", &gadget::Digital::config, &gadget_Digital_Wrapper::default_config)
        .def("getBaseType", &gadget::Digital::getBaseType, &gadget_Digital_Wrapper::default_getBaseType)
        .def("writeObject", &gadget::Digital::writeObject, &gadget_Digital_Wrapper::default_writeObject)
        .def("readObject", &gadget::Digital::readObject, &gadget_Digital_Wrapper::default_readObject)
    );

    enum_< gadget::Digital::State >("State")
        .value("TOGGLE_OFF", gadget::Digital::TOGGLE_OFF)
        .value("ON", gadget::Digital::ON)
        .value("OFF", gadget::Digital::OFF)
        .value("TOGGLE_ON", gadget::Digital::TOGGLE_ON)
    ;

    delete gadget_Digital_scope;

    scope* gadget_Event_scope = new scope(
    class_< gadget::Event, boost::noncopyable >("Event", no_init)
        .def("type", &gadget::Event::type, return_value_policy< copy_const_reference >())
        .def("setType", &gadget::Event::setType)
        .def("time", &gadget::Event::time, return_value_policy< copy_const_reference >())
    );
    // Temporary code for smart pointers
    objects::class_value_wrapper< 
      boost::shared_ptr< gadget::Event >, objects::make_ptr_instance< 
        gadget::Event, objects::pointer_holder< 
          boost::shared_ptr< gadget::Event >, gadget::Event >
      >
    >();
    delete gadget_Event_scope;

    class_< gadget::Position, boost::noncopyable, gadget_Position_Wrapper >("Position", init<  >())
        .def("getPositionData", &gadget::Position::getPositionData, gadget_Position_getPositionData_overloads_0_1())
        .def("addPositionSample", &gadget::Position::addPositionSample)
        .def("swapPositionBuffers", &gadget::Position::swapPositionBuffers)
        .def("getPositionDataBuffer", &gadget::Position::getPositionDataBuffer, return_internal_reference< 1 >())
        .def("config", &gadget::Position::config, &gadget_Position_Wrapper::default_config)
        .def("getBaseType", &gadget::Position::getBaseType, &gadget_Position_Wrapper::default_getBaseType)
        .def("writeObject", &gadget::Position::writeObject, &gadget_Position_Wrapper::default_writeObject)
        .def("readObject", &gadget::Position::readObject, &gadget_Position_Wrapper::default_readObject)
    ;

    scope* gadget_KeyEvent_scope = new scope(
    class_< gadget::KeyEvent, bases< gadget::Event > , gadget_KeyEvent_Wrapper >("KeyEvent", init<  >())
        .def(init< const gadget::KeyEvent & >())
        .def(init< const gadget::EventType &, const gadget::Keys &, const int &, const long unsigned int &, optional< char > >())
        .def("getKey", &gadget::KeyEvent::getKey, return_value_policy< copy_const_reference >())
        .def("getModifierMask", &gadget::KeyEvent::getModifierMask, return_value_policy< copy_const_reference >())
        .def("getKeyChar", &gadget::KeyEvent::getKeyChar, return_value_policy< copy_const_reference >())
        .def("writeObject", &gadget::KeyEvent::writeObject, &gadget_KeyEvent_Wrapper::default_writeObject)
        .def("readObject", &gadget::KeyEvent::readObject, &gadget_KeyEvent_Wrapper::default_readObject)
    );
    // Temporary code for smart pointers
    objects::class_value_wrapper< 
      boost::shared_ptr< gadget::KeyEvent >, objects::make_ptr_instance< 
        gadget::KeyEvent, objects::pointer_holder< 
          boost::shared_ptr< gadget::KeyEvent >, gadget::KeyEvent >
      >
    >();
    delete gadget_KeyEvent_scope;

    scope* gadget_MouseEvent_scope = new scope(
    class_< gadget::MouseEvent, bases< gadget::Event > , gadget_MouseEvent_Wrapper >("MouseEvent", init<  >())
        .def(init< const gadget::MouseEvent & >())
        .def(init< const gadget::EventType &, const gadget::Keys &, const int &, const int &, const int &, const int &, const int &, const long unsigned int & >())
        .def("getButton", &gadget::MouseEvent::getButton, return_value_policy< copy_const_reference >())
        .def("getX", &gadget::MouseEvent::getX, return_value_policy< copy_const_reference >())
        .def("getY", &gadget::MouseEvent::getY, return_value_policy< copy_const_reference >())
        .def("getGlobalX", &gadget::MouseEvent::getGlobalX, return_value_policy< copy_const_reference >())
        .def("getGlobalY", &gadget::MouseEvent::getGlobalY, return_value_policy< copy_const_reference >())
        .def("getState", &gadget::MouseEvent::getState, return_value_policy< copy_const_reference >())
        .def("writeObject", &gadget::MouseEvent::writeObject, &gadget_MouseEvent_Wrapper::default_writeObject)
        .def("readObject", &gadget::MouseEvent::readObject, &gadget_MouseEvent_Wrapper::default_readObject)
    );
    // Temporary code for smart pointers
    objects::class_value_wrapper< 
      boost::shared_ptr< gadget::MouseEvent >, objects::make_ptr_instance< 
        gadget::MouseEvent, objects::pointer_holder< 
          boost::shared_ptr< gadget::MouseEvent >, gadget::MouseEvent >
      >
    >();
    delete gadget_MouseEvent_scope;

    enum_< gadget::ButtonMask >("ButtonMask")
        .value("BUTTON5_MASK", gadget::BUTTON5_MASK)
        .value("BUTTON1_MASK", gadget::BUTTON1_MASK)
        .value("BUTTON2_MASK", gadget::BUTTON2_MASK)
        .value("BUTTON3_MASK", gadget::BUTTON3_MASK)
        .value("BUTTON4_MASK", gadget::BUTTON4_MASK)
    ;

    enum_< gadget::EventType >("EventType")
        .value("KeyPressEvent", gadget::KeyPressEvent)
        .value("MouseMoveEvent", gadget::MouseMoveEvent)
        .value("MouseButtonPressEvent", gadget::MouseButtonPressEvent)
        .value("NoEvent", gadget::NoEvent)
        .value("KeyReleaseEvent", gadget::KeyReleaseEvent)
        .value("MouseButtonReleaseEvent", gadget::MouseButtonReleaseEvent)
    ;

    enum_< gadget::Keys >("Keys")
        .value("KEY_E", gadget::KEY_E)
        .value("KEY_D", gadget::KEY_D)
        .value("KEY_G", gadget::KEY_G)
        .value("KEY_F", gadget::KEY_F)
        .value("KEY_A", gadget::KEY_A)
        .value("KEY_EXCLAM", gadget::KEY_EXCLAM)
        .value("KEY_C", gadget::KEY_C)
        .value("KEY_B", gadget::KEY_B)
        .value("KEY_M", gadget::KEY_M)
        .value("KEY_L", gadget::KEY_L)
        .value("KEY_O", gadget::KEY_O)
        .value("KEY_N", gadget::KEY_N)
        .value("KEY_I", gadget::KEY_I)
        .value("KEY_H", gadget::KEY_H)
        .value("KEY_K", gadget::KEY_K)
        .value("KEY_J", gadget::KEY_J)
        .value("KEY_U", gadget::KEY_U)
        .value("KEY_T", gadget::KEY_T)
        .value("KEY_W", gadget::KEY_W)
        .value("KEY_V", gadget::KEY_V)
        .value("KEY_Q", gadget::KEY_Q)
        .value("KEY_P", gadget::KEY_P)
        .value("KEY_S", gadget::KEY_S)
        .value("KEY_R", gadget::KEY_R)
        .value("KEY_AT", gadget::KEY_AT)
        .value("KEY_Y", gadget::KEY_Y)
        .value("KEY_X", gadget::KEY_X)
        .value("KEY_Z", gadget::KEY_Z)
        .value("KEY_ASCII_CIRCUM", gadget::KEY_ASCII_CIRCUM)
        .value("KEY_UP", gadget::KEY_UP)
        .value("KEY_UNKNOWN", gadget::KEY_UNKNOWN)
        .value("KEY_BRACKET_RIGHT", gadget::KEY_BRACKET_RIGHT)
        .value("MOUSE_NEGX", gadget::MOUSE_NEGX)
        .value("KEY_NUMBER_SIGN", gadget::KEY_NUMBER_SIGN)
        .value("KEY_DELETE", gadget::KEY_DELETE)
        .value("MOUSE_NEGY", gadget::MOUSE_NEGY)
        .value("KEY_SCROLL_LOCK", gadget::KEY_SCROLL_LOCK)
        .value("KEY_MENU", gadget::KEY_MENU)
        .value("KEY_APOSTROPHE", gadget::KEY_APOSTROPHE)
        .value("KEY_BAR", gadget::KEY_BAR)
        .value("KEY_DOLLAR", gadget::KEY_DOLLAR)
        .value("KEY_SPACE", gadget::KEY_SPACE)
        .value("KEY_SUPER_R", gadget::KEY_SUPER_R)
        .value("KEY_PAREN_LEFT", gadget::KEY_PAREN_LEFT)
        .value("KEY_RETURN", gadget::KEY_RETURN)
        .value("KEY_HELP", gadget::KEY_HELP)
        .value("KEY_PERCENT", gadget::KEY_PERCENT)
        .value("KEY_SYSREQ", gadget::KEY_SYSREQ)
        .value("KEY_F23", gadget::KEY_F23)
        .value("KEY_BACKTAB", gadget::KEY_BACKTAB)
        .value("KEY_BACKSPACE", gadget::KEY_BACKSPACE)
        .value("KEY_MINUS", gadget::KEY_MINUS)
        .value("KEY_PRINT", gadget::KEY_PRINT)
        .value("NO_MBUTTON", gadget::NO_MBUTTON)
        .value("KEY_SUPER_L", gadget::KEY_SUPER_L)
        .value("KEY_DOWN", gadget::KEY_DOWN)
        .value("KEY_SLASH", gadget::KEY_SLASH)
        .value("KEY_ESC", gadget::KEY_ESC)
        .value("KEY_SEMICOLON", gadget::KEY_SEMICOLON)
        .value("KEY_F34", gadget::KEY_F34)
        .value("KEY_F35", gadget::KEY_F35)
        .value("KEY_BRACKET_LEFT", gadget::KEY_BRACKET_LEFT)
        .value("KEY_NUM_LOCK", gadget::KEY_NUM_LOCK)
        .value("KEY_NONE", gadget::KEY_NONE)
        .value("KEY_END", gadget::KEY_END)
        .value("KEY_5", gadget::KEY_5)
        .value("KEY_4", gadget::KEY_4)
        .value("KEY_7", gadget::KEY_7)
        .value("KEY_6", gadget::KEY_6)
        .value("KEY_1", gadget::KEY_1)
        .value("KEY_0", gadget::KEY_0)
        .value("KEY_3", gadget::KEY_3)
        .value("KEY_2", gadget::KEY_2)
        .value("KEY_F30", gadget::KEY_F30)
        .value("KEY_9", gadget::KEY_9)
        .value("KEY_8", gadget::KEY_8)
        .value("KEY_F31", gadget::KEY_F31)
        .value("KEY_HYPER_L", gadget::KEY_HYPER_L)
        .value("KEY_PERIOD", gadget::KEY_PERIOD)
        .value("KEY_F32", gadget::KEY_F32)
        .value("KEY_QUOTE_LEFT", gadget::KEY_QUOTE_LEFT)
        .value("KEY_F33", gadget::KEY_F33)
        .value("KEY_PAUSE", gadget::KEY_PAUSE)
        .value("KEY_COLON", gadget::KEY_COLON)
        .value("KEY_F18", gadget::KEY_F18)
        .value("KEY_F19", gadget::KEY_F19)
        .value("KEY_F16", gadget::KEY_F16)
        .value("KEY_F17", gadget::KEY_F17)
        .value("KEY_F14", gadget::KEY_F14)
        .value("KEY_ENTER", gadget::KEY_ENTER)
        .value("KEY_F12", gadget::KEY_F12)
        .value("KEY_F13", gadget::KEY_F13)
        .value("KEY_F10", gadget::KEY_F10)
        .value("KEY_F11", gadget::KEY_F11)
        .value("KEY_F4", gadget::KEY_F4)
        .value("KEY_F5", gadget::KEY_F5)
        .value("KEY_F6", gadget::KEY_F6)
        .value("KEY_F7", gadget::KEY_F7)
        .value("LAST_KEY", gadget::LAST_KEY)
        .value("KEY_F1", gadget::KEY_F1)
        .value("KEY_F2", gadget::KEY_F2)
        .value("KEY_F3", gadget::KEY_F3)
        .value("KEY_GREATER", gadget::KEY_GREATER)
        .value("KEY_F8", gadget::KEY_F8)
        .value("KEY_F9", gadget::KEY_F9)
        .value("KEY_RIGHT", gadget::KEY_RIGHT)
        .value("KEY_BRACE_LEFT", gadget::KEY_BRACE_LEFT)
        .value("KEY_HYPER_R", gadget::KEY_HYPER_R)
        .value("KEY_ALT", gadget::KEY_ALT)
        .value("KEY_EQUAL", gadget::KEY_EQUAL)
        .value("KEY_CTRL", gadget::KEY_CTRL)
        .value("KEY_ASTERISK", gadget::KEY_ASTERISK)
        .value("KEY_SHIFT", gadget::KEY_SHIFT)
        .value("KEY_ASCII_TILDE", gadget::KEY_ASCII_TILDE)
        .value("KEY_CAPS_LOCK", gadget::KEY_CAPS_LOCK)
        .value("KEY_LESS", gadget::KEY_LESS)
        .value("KEY_AMPERSAND", gadget::KEY_AMPERSAND)
        .value("KEY_F27", gadget::KEY_F27)
        .value("KEY_F26", gadget::KEY_F26)
        .value("KEY_F25", gadget::KEY_F25)
        .value("KEY_F24", gadget::KEY_F24)
        .value("KEY_F15", gadget::KEY_F15)
        .value("KEY_HOME", gadget::KEY_HOME)
        .value("KEY_F21", gadget::KEY_F21)
        .value("KEY_F20", gadget::KEY_F20)
        .value("KEY_COMMA", gadget::KEY_COMMA)
        .value("KEY_ANY", gadget::KEY_ANY)
        .value("KEY_PAREN_RIGHT", gadget::KEY_PAREN_RIGHT)
        .value("KEY_F29", gadget::KEY_F29)
        .value("KEY_F28", gadget::KEY_F28)
        .value("KEY_UNDERSCORE", gadget::KEY_UNDERSCORE)
        .value("KEY_NEXT", gadget::KEY_NEXT)
        .value("MOUSE_POSX", gadget::MOUSE_POSX)
        .value("MOUSE_POSY", gadget::MOUSE_POSY)
        .value("KEY_QUESTION", gadget::KEY_QUESTION)
        .value("KEY_BACKSLASH", gadget::KEY_BACKSLASH)
        .value("KEY_INSERT", gadget::KEY_INSERT)
        .value("KEY_BRACE_RIGHT", gadget::KEY_BRACE_RIGHT)
        .value("KEY_TAB", gadget::KEY_TAB)
        .value("KEY_LEFT", gadget::KEY_LEFT)
        .value("KEY_QUOTE_DBL", gadget::KEY_QUOTE_DBL)
        .value("KEY_PLUS", gadget::KEY_PLUS)
        .value("KEY_PRIOR", gadget::KEY_PRIOR)
        .value("MBUTTON5", gadget::MBUTTON5)
        .value("MBUTTON4", gadget::MBUTTON4)
        .value("MBUTTON3", gadget::MBUTTON3)
        .value("MBUTTON2", gadget::MBUTTON2)
        .value("MBUTTON1", gadget::MBUTTON1)
        .value("KEY_F22", gadget::KEY_F22)
    ;

    enum_< gadget::ModiferMask >("ModiferMask")
        .value("ALT_MASK", gadget::ALT_MASK)
        .value("SHIFT_MASK", gadget::SHIFT_MASK)
        .value("CTRL_MASK", gadget::CTRL_MASK)
    ;

}

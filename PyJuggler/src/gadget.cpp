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
#include <gadget/Type/Position.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/DeviceInterface.h>
#include <gadget/Type/AnalogProxy.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/DigitalData.h>
#include <gadgeteer-wrappers.h>
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
        call_method< void >(self, "refresh");
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
        call_method< void >(self, "updateData");
    }

    void default_updateData() {
        gadget::AnalogProxy::updateData();
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::AnalogProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Analog * p1) {
        call_method< void >(self, "set", p0, p1);
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Analog * p1) {
        gadget::AnalogProxy::set(p0, p1);
    }

    bool refresh() {
        return call_method< bool >(self, "refresh");
    }

    bool default_refresh() {
        return gadget::AnalogProxy::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::AnalogProxy::getDeviceName();
    }

    bool isStupified() const {
        return call_method< bool >(self, "isStupified");
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
        call_method< void >(self, "updateData");
    }

    void default_updateData() {
        gadget::DigitalProxy::updateData();
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::DigitalProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Digital * p1) {
        call_method< void >(self, "set", p0, p1);
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Digital * p1) {
        gadget::DigitalProxy::set(p0, p1);
    }

    bool refresh() {
        return call_method< bool >(self, "refresh");
    }

    bool default_refresh() {
        return gadget::DigitalProxy::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::DigitalProxy::getDeviceName();
    }

    bool isStupified() const {
        return call_method< bool >(self, "isStupified");
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
        call_method< void >(self, "updateData");
    }

    void default_updateData() {
        gadget::PositionProxy::updateData();
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::PositionProxy::config(p0);
    }

    void set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Position * p1) {
        call_method< void >(self, "set", p0, p1);
    }

    void default_set(std::basic_string<char,std::char_traits<char>,std::allocator<char> > p0, gadget::Position * p1) {
        gadget::PositionProxy::set(p0, p1);
    }

    bool refresh() {
        return call_method< bool >(self, "refresh");
    }

    bool default_refresh() {
        return gadget::PositionProxy::refresh();
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getDeviceName");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getDeviceName() const {
        return gadget::PositionProxy::getDeviceName();
    }

    bool isStupified() const {
        return call_method< bool >(self, "isStupified");
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
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::Analog::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::Analog::readObject(p0);
    }

    bool config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::Analog::config(p0);
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getBaseType() {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getBaseType");
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
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::Digital::config(p0);
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getBaseType() {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getBaseType");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getBaseType() {
        return gadget::Digital::getBaseType();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::Digital::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
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
        return call_method< bool >(self, "config", p0);
    }

    bool default_config(boost::shared_ptr<jccl::ConfigChunk> p0) {
        return gadget::Position::config(p0);
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > getBaseType() {
        return call_method< std::basic_string<char,std::char_traits<char>,std::allocator<char> > >(self, "getBaseType");
    }

    std::basic_string<char,std::char_traits<char>,std::allocator<char> > default_getBaseType() {
        return gadget::Position::getBaseType();
    }

    vpr::ReturnStatus writeObject(vpr::ObjectWriter * p0) {
        return call_method< vpr::ReturnStatus >(self, "writeObject", p0);
    }

    vpr::ReturnStatus default_writeObject(vpr::ObjectWriter * p0) {
        return gadget::Position::writeObject(p0);
    }

    vpr::ReturnStatus readObject(vpr::ObjectReader * p0) {
        return call_method< vpr::ReturnStatus >(self, "readObject", p0);
    }

    vpr::ReturnStatus default_readObject(vpr::ObjectReader * p0) {
        return gadget::Position::readObject(p0);
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

}

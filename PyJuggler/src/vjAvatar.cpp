
// Includes ====================================================================
#include <boost/python.hpp>
#include <vjAvatarFactory.h>
#include <vjAvatar.h>

// Using =======================================================================
using namespace boost::python;

// Module ======================================================================
BOOST_PYTHON_MODULE(vjAvatar)
{
    scope* vjAvatar_scope = new scope(
    class_< vjAvatar, boost::noncopyable >("vjAvatar", no_init)
        .def("setPath", &vjAvatar::setPath)
        .def("contextInit", &vjAvatar::contextInit)
        .def("setContextType", &vjAvatar::setContextType)
        .def("isReady", &vjAvatar::isReady)
        .def("triggerAnimation", (bool (vjAvatar::*)(int) )&vjAvatar::triggerAnimation)
        .def("triggerAnimation", (bool (vjAvatar::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &) )&vjAvatar::triggerAnimation)
        .def("triggerAnimationCycle", (bool (vjAvatar::*)(int) )&vjAvatar::triggerAnimationCycle)
        .def("triggerAnimationCycle", (bool (vjAvatar::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &) )&vjAvatar::triggerAnimationCycle)
        .def("stopAnimation", (bool (vjAvatar::*)(int) )&vjAvatar::stopAnimation)
        .def("stopAnimation", (bool (vjAvatar::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &) )&vjAvatar::stopAnimation)
        .def("checkAnimation", (bool (vjAvatar::*)(int) )&vjAvatar::checkAnimation)
        .def("checkAnimation", (bool (vjAvatar::*)(const std::basic_string<char,std::char_traits<char>,std::allocator<char> > &) )&vjAvatar::checkAnimation)
        .def("stopAllAnimations", &vjAvatar::stopAllAnimations)
        .def("moveTo", &vjAvatar::moveTo)
        .def("isMoving", &vjAvatar::isMoving)
        .def("getCurrentAnimIDs", &vjAvatar::getCurrentAnimIDs)
        .def("update", &vjAvatar::update)
        .def("updateAnimations", &vjAvatar::updateAnimations)
        .def("updateMesh", &vjAvatar::updateMesh)
        .def("updateMove", &vjAvatar::updateMove)
        .def("render", (bool (vjAvatar::*)(vjAvatar::DrawMode) )&vjAvatar::render)
        .def("render", (bool (vjAvatar::*)() )&vjAvatar::render)
        .def("renderMesh", &vjAvatar::renderMesh)
        .def("renderSkeleton", &vjAvatar::renderSkeleton)
        .def("setTexture", &vjAvatar::setTexture)
        .def("setDrawMode", &vjAvatar::setDrawMode)
        .def("setLOD", &vjAvatar::setLOD)
        .def("setShadowColor", &vjAvatar::setShadowColor)
        .def("setPos", &vjAvatar::setPos)
        .def("setRot", (void (vjAvatar::*)(gmtl::Quat<float>) )&vjAvatar::setRot)
        .def("setRot", (void (vjAvatar::*)(gmtl::EulerAngle<float,gmtl::XYZ>) )&vjAvatar::setRot)
        .def("setScale", &vjAvatar::setScale)
        .def("getRotXYZ", &vjAvatar::getRotXYZ)
        .def("getPos", &vjAvatar::getPos)
        .def("getRotQuat", &vjAvatar::getRotQuat)
        .def("getScale", &vjAvatar::getScale)
        .def("setWCS", &vjAvatar::setWCS)
        .def("getAnimID", &vjAvatar::getAnimID)
        .def("markDirtyContext", &vjAvatar::markDirtyContext)
        .def("markCleanContext", &vjAvatar::markCleanContext)
        .def("hasDirtyContext", &vjAvatar::hasDirtyContext)
        .def("getBone", &vjAvatar::getBone, return_internal_reference<1>())
    );

    enum_< vjAvatar::ContextType >("ContextType")
        .value("CONTEXT_SINGLE", vjAvatar::CONTEXT_SINGLE)
        .value("CONTEXT_GL", vjAvatar::CONTEXT_GL)
    ;


    enum_< vjAvatar::DrawMode >("DrawMode")
        .value("DrawMesh", vjAvatar::DrawMesh)
        .value("DrawMeshForceTex", vjAvatar::DrawMeshForceTex)
        .value("DrawShadow", vjAvatar::DrawShadow)
        .value("DrawSkeleton", vjAvatar::DrawSkeleton)
    ;

    delete vjAvatar_scope;

    class_< vjAvatarFactory, boost::noncopyable >("vjAvatarFactory", init<  >())
        .def("newAvatar", &vjAvatarFactory::newAvatar, return_internal_reference< 1 >())
        .def("newBipedAvatar", &vjAvatarFactory::newBipedAvatar, return_internal_reference< 1 >())
        .def("addAvatar", &vjAvatarFactory::addAvatar)
        .def("removeAvatar", &vjAvatarFactory::removeAvatar)
        .def("listAvatars", &vjAvatarFactory::listAvatars)
        .def("addPath", &vjAvatarFactory::addPath)
        .def("contextInit", &vjAvatarFactory::contextInit)
        .def("TextureLoader", &vjAvatarFactory::TextureLoader, return_internal_reference< 1 >())
    ;

}

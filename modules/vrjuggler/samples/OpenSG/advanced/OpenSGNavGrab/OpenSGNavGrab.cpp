/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGTransformFields.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>

#if OSG_MAJOR_VERSION >= 2
#  include <OpenSG/OSGTypedGeoIntegralProperty.h>
#endif

#include <OpenSGNavGrab.h>


// ------------------------------ Public methods ------------------------------

OpenSGNavGrab::OpenSGNavGrab(vrj::Kernel* kern)
   : vrj::opensg::App(kern)
   , mVelocity(0.0f)
   , mIntersectColor(1.0f, 1.0f, 0.0f)
   , mGrabColor(1.0f, 0.0f, 1.0f)
{
   ;
}

OpenSGNavGrab::~OpenSGNavGrab()
{
   /* Do nothing. */ ;
}

void OpenSGNavGrab::setModelsToLoad(const std::vector<std::string>& fileNames)
{
/*
   for ( std::vector<std::string>::const_iterator i = fileNames.begin();
         i != fileNames.end();
         ++i )
   {
      std::cout << "[OpenSGNavGrab::setModelsToLoad()] Will load '"
                << *i << "'" << std::endl;
   }
*/
   mFilesToLoad = fileNames;
}

// Handle any initialization needed before API
void OpenSGNavGrab::init()
{
   mWandPos.init("VJWand");
   mButton0.init("VJButton0");
   mButton1.init("VJButton1");
   mButton2.init("VJButton2");

   // Call the base class init() implementation.  This is required for
   // subclasses of vrj::opensg::App that override vrj::opensg::App::init().
   vrj::opensg::App::init();
}

void OpenSGNavGrab::initScene()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_CRITICAL_LVL,
                        "OpenSGNavGrab::initScene() called.\n",
                        "OpenSGNavGrab::initScene() exiting.\n");

   // Set up the node highlighting state.
   initHighlight();

   mModelRoot = OSG::Node::create();
   mModelGroup = OSG::Group::create();

#if OSG_MAJOR_VERSION < 2
   CPEdit(mModelRoot, OSG::Node::CoreFieldMask);
#endif

   mModelRoot->setCore(mModelGroup);

   // Load the model to use.
   if ( mFilesToLoad.empty() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "[OpenSGNavGrab::initScene()] No model specified; creating box."
         << std::endl << vprDEBUG_FLUSH;

      // Box geometry: 2.5x2.5x2.5 (units are in feet)
      // Center point: (0,0,0)
      OSG::NodeRefPtr geom_node(OSG::makeBox(2.5f, 2.5f, 2.5f, 1, 1, 1));

      // Move it so that it would butt up against a wall five feet in front
      // of the user and another wall five feet to the left of the user.
      // If the application units were meters, this placement would be very
      // different.
      OSG::Matrix init_geom_pos;
      init_geom_pos.setTransform(OSG::Vec3f(-3.75f, 1.25f, -3.75f));

      mObjects.push_back(makeGrabbable(geom_node, init_geom_pos));
   }
   else
   {
      OSG::Vec3f distance_vec;
      OSG::Pnt3f::RealReturnType max_dist(0.0f);

      std::vector<OSG::NodeRefPtr> nodes;

      // Load all the models that the user told us to load.
      typedef std::vector<std::string>::iterator iter_type;
      for ( iter_type i = mFilesToLoad.begin(); i != mFilesToLoad.end(); ++i )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "[OpenSGNavGrab::initScene()] Loading '" << *i << "' ..."
            << std::endl << vprDEBUG_FLUSH;
         const OSG::Char8* file = (*i).c_str();
         OSG::NodeRefPtr geom_node(
#if OSG_MAJOR_VERSION < 2
            OSG::SceneFileHandler::the().read(file)
#else
            OSG::SceneFileHandler::the()->read(file)
#endif
         );

         nodes.push_back(geom_node);

         // Needed to get the volume set up after creating the geometry.
         geom_node->updateVolume();
         OSG::DynamicVolume volume = geom_node->getVolume();
         OSG::Pnt3f min, max;
         volume.getBounds(min, max);

         // Calculate the distance between min and max.  The largest distance
         // will be used for spacing out the objects.
         OSG::Pnt3f::RealReturnType dist(max.dist(min));

         // Keep track of the maximum-sized bounding volume.
         if ( dist > max_dist )
         {
            max_dist     = dist;
            distance_vec = max - min;
         }
      }

      std::cout << "distance_vec: " << distance_vec << std::endl;

      // If we have loaded any models, we will position them along the X-axis.
      // The extents of the line of models is based on the bounding volume of
      // the largest model and the number of models.  The line of models is
      // centered on the origin.  Hence, half the models will be have a
      // negative X translation value, and half will have a positive X
      // translation value.
      if ( ! nodes.empty() )
      {
         const std::vector<OSG::NodeRefPtr>::size_type num_objs(nodes.size());
         const OSG::Real32 interval_dist(distance_vec[0]);

         // Set the left extreme of the line of models.  The full extent of
         // the model range is interval_dist * num_objs.  To center things
         // on the origin, we divide that value by 2.  Since x_offset will
         // be used for the new center point of the model (see below), we
         // shift the starting point (interval_dist / 2) units so that some
         // model will end up centered on the origin.
         OSG::Real32 x_offset =
            -(interval_dist * OSG::Real32(num_objs) - interval_dist) / 2.0f;

         typedef std::vector<OSG::NodeRefPtr>::iterator iter_type;
         for ( iter_type i = nodes.begin();
               i != nodes.end();
               ++i, x_offset += interval_dist )
         {

            // Get the center point of the current node and change its X
            // component to be the current x_offset value.
            const OSG::DynamicVolume& volume = (*i)->getVolume();
            OSG::Pnt3f center;
            volume.getCenter(center);
            center[0] = x_offset;

            // The transformation matrix xform_mat will be the "home" position
            // for the model.
            OSG::Matrix xform_mat;
            xform_mat.setTranslate(center);
            mObjects.push_back(makeGrabbable(*i, xform_mat));
         }
      }
   }

   // --- Light setup --- //
   // - Add directional light for scene
   // - Create a beacon for it and connect to that beacon
   mLightNode   = OSG::Node::create();
   mLightBeacon = OSG::Node::create();
   OSG::DirectionalLightPtr light_core = OSG::DirectionalLight::create();
   OSG::TransformPtr light_beacon_core = OSG::Transform::create();

   // Set up light beacon
   OSG::Matrix light_pos;
   light_pos.setTransform(OSG::Vec3f(2.0f, 5.0f, 4.0f));

#if OSG_MAJOR_VERSION < 2
   CPEdit(light_beacon_core, OSG::Transform::MatrixFieldMask);
   CPEdit(mLightBeacon, OSG::Node::CoreFieldMask);
   CPEdit(mLightNode, OSG::Node::CoreFieldMask | OSG::Node::ChildrenFieldMask);
   CPEditAll(light_core);
#endif

   light_beacon_core->setMatrix(light_pos);

   mLightBeacon->setCore(light_beacon_core);

   // Set up light node
   mLightNode->setCore(light_core);
   mLightNode->addChild(mLightBeacon);

   light_core->setAmbient(0.9, 0.8, 0.8, 1);
   light_core->setDiffuse(0.6, 0.6, 0.6, 1);
   light_core->setSpecular(1, 1, 1, 1);
   light_core->setDirection(0, 0, 1);
   light_core->setBeacon(mLightNode);

   // --- Set up scene -- //
   // add the loaded scene to the light node, so that it is lit by the light
   mLightNode->addChild(mModelRoot);

   // create the root part of the scene
   mSceneRoot = OSG::Node::create();
   mSceneTransform = OSG::Transform::create();

#if OSG_MAJOR_VERSION < 2
   CPEdit(mSceneRoot, OSG::Node::CoreFieldMask | OSG::Node::ChildrenFieldMask);
#endif

   // Set up the root node
   mSceneRoot->setCore(mSceneTransform);
   mSceneRoot->addChild(mLightNode);
}

/** Called once per context at context creation */
void OpenSGNavGrab::contextInit()
{
   // Call the base class contextInit() implementation.  This is required for
   // subclasses of vrj::opensg::App that override
   // vrj::opensg::App::contextInit().
   vrj::opensg::App::contextInit();
   initGLState();
}

void OpenSGNavGrab::preFrame()
{
   static const float inc_vel(0.005f);
   static const float max_vel(0.05f);

   // Update velocity
   if ( mButton1->getData() == gadget::Digital::ON )
   {
      mVelocity += inc_vel;
   }
   else if ( mVelocity > 0.0f )
   {
      mVelocity -= inc_vel;
   }

   // Restrict range
   if ( mVelocity < 0.0f )
   {
      mVelocity = 0.0f;
   }
   if ( mVelocity > max_vel )
   {
      mVelocity = max_vel;
   }

   // Reset state when button 2 is toggled on.
   if ( mButton2->getData() == gadget::Digital::TOGGLE_ON )
   {
      reset();
   }

   vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
      << "Current velocity: " << mVelocity << std::endl << vprDEBUG_FLUSH;

   // Get the wand matrix in the units of this application.
   const gmtl::Matrix44f wand_mat(mWandPos->getData(getDrawScaleFactor()));
   updateGrabbing(wand_mat);
   updateNavigation(wand_mat);

   vrj::opensg::App::preFrame();
}

void OpenSGNavGrab::reset()
{
   mVelocity = 0.0f;

#if OSG_MAJOR_VERSION < 2
   CPEdit(mSceneTransform, OSG::Transform::MatrixFieldMask);

   mSceneTransform->getMatrix().setIdentity();
#else
   mSceneTransform->editMatrix().setIdentity();
#endif

   typedef std::vector<GrabObjectPtr>::iterator iter_type;
   for ( iter_type i = mObjects.begin(); i != mObjects.end(); ++i )
   {
#if OSG_MAJOR_VERSION < 2
      OSG::TransformPtr core = (*i)->getXformNode();
      CPEdit(core, OSG::Transform::MatrixFieldMask);
      core->getMatrix().setValue((*i)->getHomePos());
#else
      (*i)->getXformNode()->editMatrix().setValue((*i)->getHomePos());
#endif
   }
}

void OpenSGNavGrab::exit()
{
   mObjects.clear();

   mIntersectedObj = GrabObjectPtr();
   mGrabbedObj     = GrabObjectPtr();

   mSceneRoot         = OSG::NullFC;
   mSceneTransform    = OSG::NullFC;
   mModelRoot         = OSG::NullFC;
   mModelGroup        = OSG::NullFC;
   mLightNode         = OSG::NullFC;
   mLightBeacon       = OSG::NullFC;
   mHighlight         = OSG::NullFC;
   mHighlightNode     = OSG::NullFC;
   mHighlightPoints   = OSG::NullFC;
   mHighlightMaterial = OSG::NullFC;

   vrj::opensg::App::exit();
}

// ----------------------------- Private methods ------------------------------

void OpenSGNavGrab::initGLState()
{
   // OpenSG does not handle this yet.  Being smart about it is non-trivial.
   glEnable(GL_NORMALIZE);
}

OpenSGNavGrab::GrabObjectPtr
OpenSGNavGrab::makeGrabbable(OSG::NodeRefPtr model,
                             const OSG::Matrix& modelPos)
{
   OSG::TransformNodePtr model_xform_node = OSG::TransformNodePtr::create();

#if OSG_MAJOR_VERSION < 2
   OSG::TransformPtr core = model_xform_node;
   CPEdit(core, OSG::Transform::MatrixFieldMask);
   OSG::CPEditor e(model_xform_node.node(),
                   OSG::Node::CoreFieldMask | OSG::Node::ChildrenFieldMask);
   CPEdit(mModelRoot, OSG::Node::ChildrenFieldMask);
#endif

   model_xform_node->setMatrix(modelPos);
   model_xform_node.node()->addChild(model);

   mModelRoot->addChild(model_xform_node.node());

   return GrabObject::create(model_xform_node, modelPos);
}

void OpenSGNavGrab::updateGrabbing(const gmtl::Matrix44f& wandMatrix)
{
   // The navigation matrix is w_M_vw, so invert it for use here.
   const OSG::Matrix& nav_mat(mSceneTransform->getMatrix());
   OSG::Matrix vw_M_w;
   nav_mat.inverse(vw_M_w);

   // Create a GMTL version of vw_M_w so that we can perform the necessary
   // multiplication.
   gmtl::Matrix44f gmtl_vw_M_w;
   gmtl_vw_M_w.set(vw_M_w.getValues());

   // Transform the wand matrix from real world coordinates into virtual world
   // coordinates.
   const gmtl::Matrix44f wand_matrix = gmtl_vw_M_w * wandMatrix;

   // Extract the wand translation component as an OSG::Pnt3f for use below.
   // It's too bad that gmtl::makeTrans<T,U>() can't take an OSG::Pnt3f as its
   // T (translation type) parameter.
   const gmtl::Vec3f wand_pos = gmtl::makeTrans<gmtl::Vec3f>(wand_matrix);
   const OSG::Pnt3f wand_point(wand_pos[0], wand_pos[1], wand_pos[2]);

   // Only perform the intersection testing when we are not already grabbing
   // an object.
   if ( mGrabbedObj.get() == NULL )
   {
      GrabObjectPtr intersect_obj;

      // Find the first object--if any--in mObjects with which the wand
      // intersects.
      typedef std::vector<GrabObjectPtr>::iterator iter_type;
      for ( iter_type o = mObjects.begin(); o != mObjects.end(); ++o )
      {
         const OSG::DynamicVolume& bbox =
            (*o)->getXformNode().node()->getVolume();

         if ( bbox.intersect(wand_point) )
         {
            intersect_obj = *o;
            break;
         }
      }

      // If the intersected object changed since the last frame, we need to
      // update things.
      if ( intersect_obj != mIntersectedObj )
      {
         // If mIntersectedObj was not NULL, then we need to remove the
         // bounding box rendering that is now out of date.
         if ( mIntersectedObj.get() != NULL )
         {
#if OSG_MAJOR_VERSION < 2
            OSG::CPEditor e(mIntersectedObj->getXformNode().node(),
                            OSG::Node::ChildrenFieldMask);
#endif
            mIntersectedObj->getXformNode().node()->subChild(mHighlightNode);
         }

         mIntersectedObj = intersect_obj;

         // If mIntersectedObj is non-NULL, we have selected a new object.
         // Create a new bounding box around that object.
         if ( mIntersectedObj.get() != NULL )
         {
            mHighlight = mIntersectedObj->getXformNode().node()->getChild(0);

#if OSG_MAJOR_VERSION < 2
            CPEdit(mHighlightMaterial, OSG::SimpleMaterial::DiffuseFieldMask);
            OSG::CPEditor e(mIntersectedObj->getXformNode().node(),
                            OSG::Node::ChildrenFieldMask);
#endif

            // Set the highlight color to mIntersectColor.
            mHighlightMaterial->setDiffuse(mIntersectColor);

            mIntersectedObj->getXformNode().node()->addChild(mHighlightNode);

            updateHighlight();
         }
      }
   }

   // Enable grabbing only when no other object is currently grabbed, when
   // the wand button is intersecting an object, and when button 0 is pressed.
   if ( mIntersectedObj.get() != NULL && mGrabbedObj.get() == NULL &&
        mButton0->getData() == gadget::Digital::ON )
   {
      mGrabbedObj = mIntersectedObj;

#if OSG_MAJOR_VERSION < 2
      CPEdit(mHighlightMaterial, OSG::SimpleMaterial::DiffuseFieldMask);
#endif

      // Set the highlight color to mGrabColor.
      mHighlightMaterial->setDiffuse(mGrabColor);
   }
   // We cannot be grabbing anything unless button 0 is pressed.
   else if ( mButton0->getData() != gadget::Digital::ON )
   {
      // If we are dropping a grabbed object, then set the highlight color
      // back to mIntersectColor.
      if ( mGrabbedObj.get() != NULL )
      {
#if OSG_MAJOR_VERSION < 2
      CPEdit(mHighlightMaterial, OSG::SimpleMaterial::DiffuseFieldMask);
#endif

         mHighlightMaterial->setDiffuse(mIntersectColor);
      }

      mGrabbedObj = GrabObjectPtr();
   }

   // If mGrabbedObj is non-NULL, then we are grabbing the object pointed at
   // by mGrabbedObj.
   if ( mGrabbedObj.get() != NULL )
   {
#if OSG_MAJOR_VERSION < 2
      OSG::TransformPtr core = mGrabbedObj->getXformNode();
      CPEdit(core, OSG::Transform::MatrixFieldMask);
      mGrabbedObj->getXformNode()->getMatrix().setTranslate(wand_point);
#else
      mGrabbedObj->getXformNode()->editMatrix().setTranslate(wand_point);
#endif
   }
}

void OpenSGNavGrab::updateNavigation(const gmtl::Matrix44f& wandMatrix)
{
   // Travel in model
   // - Find forward direction of wand
   // - Translate along that direction
   const gmtl::Vec3f z_dir(0.0f, 0.0f, mVelocity);
   const gmtl::Vec3f trans = wandMatrix * z_dir;

   OSG::Matrix trans_mat(OSG::Matrix::identity());
   trans_mat.setTranslate(trans[0], trans[1], trans[2]);

#if OSG_MAJOR_VERSION < 2
   CPEdit(mSceneTransform, OSG::Transform::MatrixFieldMask);
   mSceneTransform->getMatrix().multLeft(trans_mat);
#else
   mSceneTransform->editMatrix().multLeft(trans_mat);
#endif
}

// This implementation is adapted from
// OSG::SimpleSceneManager::highlightChanged() in OpenSG 1.4.0.
void OpenSGNavGrab::initHighlight()
{
   if ( mHighlightMaterial == OSG::NullFC )
   {
      mHighlightMaterial = OSG::SimpleMaterial::create();

#if OSG_MAJOR_VERSION < 2
      CPEdit(mHighlightMaterial, OSG::SimpleMaterial::LitFieldMask);
#endif

      mHighlightMaterial->setLit(false);
   }

   if ( mHighlightNode == OSG::NullFC )
   {
#if OSG_MAJOR_VERSION < 2
      OSG::GeoPTypesPtr type       = OSG::GeoPTypesUI8::create();
      OSG::GeoPLengthsPtr lens     = OSG::GeoPLengthsUI32::create();
#else
      OSG::GeoPTypesUI8Ptr type    = OSG::GeoPTypesUI8::create();
      OSG::GeoPLengthsUI32Ptr lens = OSG::GeoPLengthsUI32::create();
#endif
      OSG::GeoIndicesUI32Ptr index = OSG::GeoIndicesUI32::create();
      mHighlightPoints             = OSG::GeoPositions3f::create();
      OSG::GeometryPtr geo         = OSG::Geometry::create();
      mHighlightNode               = OSG::Node::create();

#if OSG_MAJOR_VERSION < 2
      CPEditAll(type);
      CPEditAll(lens);
      CPEditAll(index);
      CPEditAll(mHighlightPoints);
      CPEditAll(geo);
      CPEdit(mHighlightNode, OSG::Node::CoreFieldMask);
#endif

      type->push_back(GL_LINE_STRIP);
      type->push_back(GL_LINES);

      lens->push_back(10);
      lens->push_back(6);

#if OSG_MAJOR_VERSION < 2
      OSG::GeoIndicesUI32::StoredFieldType* index_field =
         index->getFieldPtr();
#else
      OSG::GeoIndicesUI32::StoredFieldType* index_field =
         index->editFieldPtr();
#endif

      index_field->push_back(0);
      index_field->push_back(1);
      index_field->push_back(3);
      index_field->push_back(2);
      index_field->push_back(0);
      index_field->push_back(4);
      index_field->push_back(5);
      index_field->push_back(7);
      index_field->push_back(6);
      index_field->push_back(4);

      index_field->push_back(1);
      index_field->push_back(5);
      index_field->push_back(2);
      index_field->push_back(6);
      index_field->push_back(3);
      index_field->push_back(7);

      mHighlightPoints->push_back(OSG::Pnt3f(-1, -1, -1));
      mHighlightPoints->push_back(OSG::Pnt3f( 1, -1, -1));
      mHighlightPoints->push_back(OSG::Pnt3f(-1,  1, -1));
      mHighlightPoints->push_back(OSG::Pnt3f( 1,  1, -1));
      mHighlightPoints->push_back(OSG::Pnt3f(-1, -1,  1));
      mHighlightPoints->push_back(OSG::Pnt3f( 1, -1,  1));
      mHighlightPoints->push_back(OSG::Pnt3f(-1,  1,  1));
      mHighlightPoints->push_back(OSG::Pnt3f( 1,  1,  1));

      geo->setTypes(type);
      geo->setLengths(lens);
      geo->setIndices(index);
      geo->setPositions(mHighlightPoints);
      geo->setMaterial(mHighlightMaterial);

      mHighlightNode->setCore(geo);
   }
}

// This implementation is adapted from
// OSG::SimpleSceneManager::updateHighlight() in OpenSG 1.4.0.
void OpenSGNavGrab::updateHighlight()
{
   if ( mHighlight == OSG::NullFC )
   {
      return;
   }

   const OSG::DynamicVolume& vol = mHighlight->getVolume();

   OSG::Pnt3f min, max;
   vol.getBounds(min, max);

#if OSG_MAJOR_VERSION < 2
   CPEditAll(mHighlightPoints);
   OSG::CPEditor e(mHighlightNode->getCore(),
                   OSG::Geometry::PositionsFieldMask);
#endif

   mHighlightPoints->setValue(OSG::Pnt3f(min[0], min[1], min[2]), 0);
   mHighlightPoints->setValue(OSG::Pnt3f(max[0], min[1], min[2]), 1);
   mHighlightPoints->setValue(OSG::Pnt3f(min[0], max[1], min[2]), 2);
   mHighlightPoints->setValue(OSG::Pnt3f(max[0], max[1], min[2]), 3);
   mHighlightPoints->setValue(OSG::Pnt3f(min[0], min[1], max[2]), 4);
   mHighlightPoints->setValue(OSG::Pnt3f(max[0], min[1], max[2]), 5);
   mHighlightPoints->setValue(OSG::Pnt3f(min[0], max[1], max[2]), 6);
   mHighlightPoints->setValue(OSG::Pnt3f(max[0], max[1], max[2]), 7);

#if OSG_MAJOR_VERSION >= 2
   OSG::commitChanges();
#endif
}

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#include <OpenSG/OSGMatrix.h>
#include <OpenSG/OSGSimpleGeometry.h>
#include <OpenSG/OSGDirectionalLight.h>
#include <OpenSG/OSGSceneFileHandler.h>
#include <OpenSG/OSGTransformFields.h>

#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>

#include <OpenSGNavGrab.h>


// ------------------------------ Public methods ------------------------------

OpenSGNavGrab::OpenSGNavGrab(vrj::Kernel* kern)
   : vrj::OpenSGApp(kern)
   , mVelocity(0.0f)
   , mIntersectedObj(NULL)
   , mGrabbedObj(NULL)
   , mIntersectColor(1.0f, 1.0f, 0.0f)
   , mGrabColor(1.0f, 0.0f, 1.0f)
{
   ;
}

OpenSGNavGrab::~OpenSGNavGrab()
{
   for ( std::vector<GrabObject*>::iterator i = mObjects.begin();
         i != mObjects.end();
         ++i )
   {
      delete *i;
   }
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
   // subclasses of vrj::OpenSGApp that override vrj::OpenSGApp::init().
   vrj::OpenSGApp::init();
}

void OpenSGNavGrab::initScene()
{
   vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_CRITICAL_LVL,
                        "OpenSGNavGrab::initScene() called.\n",
                        "OpenSGNavGrab::initScene() exiting.\n");

   // Set up the node highlighting state.
   initHighlight();

   mModelRoot = osg::Node::create();
   mModelGroup = osg::Group::create();

   osg::beginEditCP(mModelRoot);
      mModelRoot->setCore(mModelGroup);
   osg::endEditCP(mModelRoot);

   // Load the model to use.
   if ( mFilesToLoad.empty() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "[OpenSGNavGrab::initScene()] No model specified; creating box."
         << std::endl << vprDEBUG_FLUSH;

      // Box geometry: 2.5x2.5x2.5 (units are in feet)
      // Center point: (0,0,0)
      osg::NodePtr geom_node = osg::makeBox(2.5f, 2.5f, 2.5f, 1, 1, 1);
      osg::addRefCP(geom_node);

      // Move it so that it would butt up against a wall five feet in front
      // of the user and another wall five feet to the left of the user.
      // If the application units were meters, this placement would be very
      // different.
      osg::Matrix init_geom_pos;
      init_geom_pos.setTransform(osg::Vec3f(-3.75f, 1.25f, -3.75f));

      mObjects.push_back(makeGrabbable(geom_node, init_geom_pos));
   }
   else
   {
      osg::Vec3f distance_vec;
      osg::Pnt3f::RealReturnType max_dist(0.0f);

      std::vector<osg::NodePtr> nodes;

      // Load all the models that the user told us to load.
      for ( std::vector<std::string>::iterator i = mFilesToLoad.begin();
            i != mFilesToLoad.end();
            ++i )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "[OpenSGNavGrab::initScene()] Loading '" << *i << "' ..."
            << std::endl << vprDEBUG_FLUSH;
         osg::NodePtr geom_node =
            osg::SceneFileHandler::the().read((osg::Char8*) (*i).c_str());
         osg::addRefCP(geom_node);

         nodes.push_back(geom_node);

         // Needed to get the volume set up after creating the geometry.
         geom_node->updateVolume();
         osg::DynamicVolume volume = geom_node->getVolume();
         osg::Pnt3f min, max;
         volume.getBounds(min, max);

         // Calculate the distance between min and max.  The largest distance
         // will be used for spacing out the objects.
         osg::Pnt3f::RealReturnType dist(max.dist(min));

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
         const std::vector<osg::NodePtr>::size_type num_objs(nodes.size());
         const osg::Real32 interval_dist(distance_vec[0]);

         // Set the left extreme of the line of models.  The full extent of
         // the model range is interval_dist * num_objs.  To center things
         // on the origin, we divide that value by 2.  Since x_offset will
         // be used for the new center point of the model (see below), we
         // shift the starting point (interval_dist / 2) units so that some
         // model will end up centered on the origin.
         osg::Real32 x_offset =
            -(interval_dist * osg::Real32(num_objs) - interval_dist) / 2.0f;

         for ( std::vector<osg::NodePtr>::iterator i = nodes.begin();
               i != nodes.end();
               ++i, x_offset += interval_dist )
         {

            // Get the center point of the current node and change its X
            // component to be the current x_offset value.
            const osg::DynamicVolume& volume = (*i)->getVolume();
            osg::Pnt3f center;
            volume.getCenter(center);
            center[0] = x_offset;

            // The transformation matrix xform_mat will be the "home" position
            // for the model.
            osg::Matrix xform_mat;
            xform_mat.setTranslate(center);
            mObjects.push_back(makeGrabbable(*i, xform_mat));
         }
      }
   }

   // --- Light setup --- //
   // - Add directional light for scene
   // - Create a beacon for it and connect to that beacon
   mLightNode   = osg::Node::create();
   mLightBeacon = osg::Node::create();
   osg::DirectionalLightPtr light_core = osg::DirectionalLight::create();
   osg::TransformPtr light_beacon_core = osg::Transform::create();

   // Set up light beacon
   osg::Matrix light_pos;
   light_pos.setTransform(osg::Vec3f(2.0f, 5.0f, 4.0f));

   osg::beginEditCP(light_beacon_core, osg::Transform::MatrixFieldMask);
      light_beacon_core->setMatrix(light_pos);
   osg::endEditCP(light_beacon_core, osg::Transform::MatrixFieldMask);

   osg::beginEditCP(mLightBeacon);
      mLightBeacon->setCore(light_beacon_core);
   osg::endEditCP(mLightBeacon);

   // Set up light node
   osg::addRefCP(mLightNode);
   osg::beginEditCP(mLightNode);
      mLightNode->setCore(light_core);
      mLightNode->addChild(mLightBeacon);
   osg::endEditCP(mLightNode);

   osg::beginEditCP(light_core);
      light_core->setAmbient(0.9, 0.8, 0.8, 1);
      light_core->setDiffuse(0.6, 0.6, 0.6, 1);
      light_core->setSpecular(1, 1, 1, 1);
      light_core->setDirection(0, 0, 1);
      light_core->setBeacon(mLightNode);
   osg::endEditCP(light_core);

   // --- Set up scene -- //
   // add the loaded scene to the light node, so that it is lit by the light

   osg::beginEditCP(mLightNode);
      mLightNode->addChild(mModelRoot);
   osg::endEditCP(mLightNode);

   // create the root part of the scene
   mSceneRoot = osg::Node::create();
   mSceneTransform = osg::Transform::create();

   // Set up the root node
   osg::beginEditCP(mSceneRoot);
      mSceneRoot->setCore(mSceneTransform);
      mSceneRoot->addChild(mLightNode);
   osg::endEditCP(mSceneRoot);
}

/** Called once per context at context creation */
void OpenSGNavGrab::contextInit()
{
   // Call the base class contextInit() implementation.  This is required for
   // subclasses of vrj::OpenSGApp that override vrj::OpenSGApp::contextInit().
   vrj::OpenSGApp::contextInit();
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
}

void OpenSGNavGrab::reset()
{
   mVelocity = 0.0f;

   osg::beginEditCP(mSceneTransform, osg::Transform::MatrixFieldMask);
      mSceneTransform->getMatrix().setIdentity();
   osg::endEditCP(mSceneTransform, osg::Transform::MatrixFieldMask);

   for ( std::vector<GrabObject*>::iterator i = mObjects.begin();
         i != mObjects.end();
         ++i )
   {
      osg::beginEditCP((*i)->xformCore, osg::Transform::MatrixFieldMask);
         (*i)->xformCore->getMatrix().setValue((*i)->homePos);
      osg::endEditCP((*i)->xformCore, osg::Transform::MatrixFieldMask);
   }
}

// ----------------------------- Private methods ------------------------------

void OpenSGNavGrab::initGLState()
{
    // OpenSG does not handle this yet.  Being smart about it is non-trivial.
    glEnable(GL_NORMALIZE);
}

OpenSGNavGrab::GrabObject* OpenSGNavGrab::makeGrabbable(osg::NodePtr model,
                                                        const osg::Matrix& modelPos)
{
   osg::TransformPtr model_xform_core = osg::Transform::create();
   osg::beginEditCP(model_xform_core, osg::Transform::MatrixFieldMask);
      model_xform_core->setMatrix(modelPos);
   osg::endEditCP(model_xform_core, osg::Transform::MatrixFieldMask);

   osg::NodePtr model_xform_node = osg::Node::create();
   osg::beginEditCP(model_xform_node);
      model_xform_node->setCore(model_xform_core);
      model_xform_node->addChild(model);
   osg::endEditCP(model_xform_node);

   osg::beginEditCP(mModelRoot, osg::Node::ChildrenFieldMask);
      mModelRoot->addChild(model_xform_node);
   osg::endEditCP(mModelRoot, osg::Node::ChildrenFieldMask);

   return new GrabObject(model_xform_node, model_xform_core, modelPos);
}

void OpenSGNavGrab::updateGrabbing(const gmtl::Matrix44f& wandMatrix)
{
   // The navigation matrix is w_M_vw, so invert it for use here.
   const osg::Matrix& nav_mat(mSceneTransform->getMatrix());
   osg::Matrix vw_M_w;
   nav_mat.inverse(vw_M_w);

   // Create a GMTL version of vw_M_w so that we can perform the necessary
   // multiplication.
   gmtl::Matrix44f gmtl_vw_M_w;
   gmtl_vw_M_w.set(vw_M_w.getValues());

   // Transform the wand matrix from real world coordinates into virtual world
   // coordinates.
   const gmtl::Matrix44f wand_matrix = gmtl_vw_M_w * wandMatrix;

   // Extract the wand translation component as an osg::Pnt3f for use below.
   // It's too bad that gmtl::makeTrans<T,U>() can't take an osg::Pnt3f as its
   // T (translation type) parameter.
   const gmtl::Vec3f wand_pos = gmtl::makeTrans<gmtl::Vec3f>(wand_matrix);
   const osg::Pnt3f wand_point(wand_pos[0], wand_pos[1], wand_pos[2]);

   // Only perform the intersection testing when we are not already grabbing
   // an object.
   if ( mGrabbedObj == NULL )
   {
      GrabObject* intersect_obj(NULL);

      // Find the first object--if any--in mObjects with which the wand
      // intersects.
      for ( std::vector<GrabObject*>::iterator o = mObjects.begin();
            o != mObjects.end();
            ++o )
      {
         const osg::DynamicVolume& bbox = (*o)->xformNode->getVolume();

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
         if ( mIntersectedObj != NULL )
         {
            osg::beginEditCP(mIntersectedObj->xformNode,
                             osg::Node::ChildrenFieldMask);
               mIntersectedObj->xformNode->subChild(mHighlightNode);
            osg::endEditCP(mIntersectedObj->xformNode,
                           osg::Node::ChildrenFieldMask);
         }

         mIntersectedObj = intersect_obj;

         // If mIntersectedObj is non-NULL, we have selected a new object.
         // Create a new bounding box around that object.
         if ( mIntersectedObj != NULL )
         {
            mHighlight = mIntersectedObj->xformNode->getChild(0);

            // Set the highlight color to mIntersectColor.
            osg::beginEditCP(mHighlightMaterial,
                             osg::SimpleMaterial::DiffuseFieldMask);
               mHighlightMaterial->setDiffuse(mIntersectColor);
            osg::endEditCP(mHighlightMaterial,
                           osg::SimpleMaterial::DiffuseFieldMask);

            osg::beginEditCP(mIntersectedObj->xformNode,
                             osg::Node::ChildrenFieldMask);
               mIntersectedObj->xformNode->addChild(mHighlightNode);
            osg::endEditCP(mIntersectedObj->xformNode,
                           osg::Node::ChildrenFieldMask);
            updateHighlight();
         }
      }
   }

   // Enable grabbing only when no other object is currently grabbed, when
   // the wand button is intersecting an object, and when button 0 is pressed.
   if ( mIntersectedObj != NULL && mGrabbedObj == NULL &&
        mButton0->getData() == gadget::Digital::ON )
   {
      mGrabbedObj = mIntersectedObj;

      // Set the highlight color to mGrabColor.
      osg::beginEditCP(mHighlightMaterial,
                       osg::SimpleMaterial::DiffuseFieldMask);
         mHighlightMaterial->setDiffuse(mGrabColor);
      osg::endEditCP(mHighlightMaterial,
                     osg::SimpleMaterial::DiffuseFieldMask);
   }
   // We cannot be grabbing anything unless button 0 is pressed.
   else if ( mButton0->getData() != gadget::Digital::ON )
   {
      // If we are dropping a grabbed object, then set the highlight color
      // back to mIntersectColor.
      if ( mGrabbedObj != NULL )
      {
         osg::beginEditCP(mHighlightMaterial,
                          osg::SimpleMaterial::DiffuseFieldMask);
            mHighlightMaterial->setDiffuse(mIntersectColor);
         osg::endEditCP(mHighlightMaterial,
                        osg::SimpleMaterial::DiffuseFieldMask);
      }

      mGrabbedObj = NULL;
   }

   // If mGrabbedObj is non-NULL, then we are grabbing the object pointed at
   // by mGrabbedObj.
   if ( mGrabbedObj != NULL )
   {
      osg::beginEditCP(mGrabbedObj->xformCore, osg::Transform::MatrixFieldMask);
         mGrabbedObj->xformCore->getMatrix().setTranslate(wand_point);
      osg::endEditCP(mGrabbedObj->xformCore, osg::Transform::MatrixFieldMask);
   }
}

void OpenSGNavGrab::updateNavigation(const gmtl::Matrix44f& wandMatrix)
{
   // Travel in model
   // - Find forward direction of wand
   // - Translate along that direction
   const gmtl::Vec3f z_dir(0.0f, 0.0f, mVelocity);
   const gmtl::Vec3f trans = wandMatrix * z_dir;

   osg::Matrix trans_mat(osg::Matrix::identity());
   trans_mat.setTranslate(trans[0], trans[1], trans[2]);

   osg::beginEditCP(mSceneTransform, osg::Transform::MatrixFieldMask);
      mSceneTransform->getMatrix().multLeft(trans_mat);
   osg::endEditCP(mSceneTransform, osg::Transform::MatrixFieldMask);
}

// This implementation is adapted from
// osg::SimpleSceneManager::highlightChanged() in OpenSG 1.4.0.
void OpenSGNavGrab::initHighlight()
{
   if ( mHighlightMaterial == osg::NullFC )
   {
      mHighlightMaterial = osg::SimpleMaterial::create();

      osg::beginEditCP(mHighlightMaterial);
         mHighlightMaterial->setLit(false);
      osg::endEditCP(mHighlightMaterial);
   }

   if ( mHighlightNode == osg::NullFC )
   {
      osg::GeoPTypesPtr type = osg::GeoPTypesUI8::create();
      osg::beginEditCP(type);
         type->push_back(GL_LINE_STRIP);
         type->push_back(GL_LINES);
      osg::endEditCP(type);

      osg::GeoPLengthsPtr lens = osg::GeoPLengthsUI32::create();
      osg::beginEditCP(lens);
         lens->push_back(10);
         lens->push_back(6);
      osg::endEditCP(lens);

      osg::GeoIndicesUI32Ptr index = osg::GeoIndicesUI32::create();
      osg::beginEditCP(index);
         index->getFieldPtr()->push_back(0);
         index->getFieldPtr()->push_back(1);
         index->getFieldPtr()->push_back(3);
         index->getFieldPtr()->push_back(2);
         index->getFieldPtr()->push_back(0);
         index->getFieldPtr()->push_back(4);
         index->getFieldPtr()->push_back(5);
         index->getFieldPtr()->push_back(7);
         index->getFieldPtr()->push_back(6);
         index->getFieldPtr()->push_back(4);

         index->getFieldPtr()->push_back(1);
         index->getFieldPtr()->push_back(5);
         index->getFieldPtr()->push_back(2);
         index->getFieldPtr()->push_back(6);
         index->getFieldPtr()->push_back(3);
         index->getFieldPtr()->push_back(7);
      osg::endEditCP(index);

      mHighlightPoints = osg::GeoPositions3f::create();
      osg::beginEditCP(mHighlightPoints);
         mHighlightPoints->push_back(osg::Pnt3f(-1, -1, -1));
         mHighlightPoints->push_back(osg::Pnt3f( 1, -1, -1));
         mHighlightPoints->push_back(osg::Pnt3f(-1,  1, -1));
         mHighlightPoints->push_back(osg::Pnt3f( 1,  1, -1));
         mHighlightPoints->push_back(osg::Pnt3f(-1, -1,  1));
         mHighlightPoints->push_back(osg::Pnt3f( 1, -1,  1));
         mHighlightPoints->push_back(osg::Pnt3f(-1,  1,  1));
         mHighlightPoints->push_back(osg::Pnt3f( 1,  1,  1));
      osg::endEditCP(mHighlightPoints);

      osg::GeometryPtr geo = osg::Geometry::create();
      osg::beginEditCP(geo);
         geo->setTypes(type);
         geo->setLengths(lens);
         geo->setIndices(index);
         geo->setPositions(mHighlightPoints);
         geo->setMaterial(mHighlightMaterial);
      osg::endEditCP(geo);
      osg::addRefCP(geo);

      mHighlightNode = osg::Node::create();
      osg::beginEditCP(mHighlightNode);
         mHighlightNode->setCore(geo);
      osg::endEditCP(mHighlightNode);
      osg::addRefCP(mHighlightNode);
   }
}

// This implementation is adapted from
// osg::SimpleSceneManager::updateHighlight() in OpenSG 1.4.0.
void OpenSGNavGrab::updateHighlight()
{
   if ( mHighlight == osg::NullFC )
   {
      return;
   }

   const osg::DynamicVolume& vol = mHighlight->getVolume();

   osg::Pnt3f min, max;
   vol.getBounds(min, max);

   osg::beginEditCP(mHighlightPoints);
      mHighlightPoints->setValue(osg::Pnt3f(min[0], min[1], min[2]), 0);
      mHighlightPoints->setValue(osg::Pnt3f(max[0], min[1], min[2]), 1);
      mHighlightPoints->setValue(osg::Pnt3f(min[0], max[1], min[2]), 2);
      mHighlightPoints->setValue(osg::Pnt3f(max[0], max[1], min[2]), 3);
      mHighlightPoints->setValue(osg::Pnt3f(min[0], min[1], max[2]), 4);
      mHighlightPoints->setValue(osg::Pnt3f(max[0], min[1], max[2]), 5);
      mHighlightPoints->setValue(osg::Pnt3f(min[0], max[1], max[2]), 6);
      mHighlightPoints->setValue(osg::Pnt3f(max[0], max[1], max[2]), 7);
   osg::endEditCP(mHighlightPoints);

   osg::beginEditCP(mHighlightNode->getCore(),
                    osg::Geometry::PositionsFieldMask);
   osg::endEditCP(mHighlightNode->getCore(),
                  osg::Geometry::PositionsFieldMask);
}

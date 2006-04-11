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

   mModelRoot = OSG::Node::create();
   mModelGroup = OSG::Group::create();

   OSG::beginEditCP(mModelRoot);
      mModelRoot->setCore(mModelGroup);
   OSG::endEditCP(mModelRoot);

   // Load the model to use.
   if ( mFilesToLoad.empty() )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
         << "[OpenSGNavGrab::initScene()] No model specified; creating box."
         << std::endl << vprDEBUG_FLUSH;

      // Box geometry: 2.5x2.5x2.5 (units are in feet)
      // Center point: (0,0,0)
      OSG::NodePtr geom_node = OSG::makeBox(2.5f, 2.5f, 2.5f, 1, 1, 1);
      OSG::addRefCP(geom_node);

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

      std::vector<OSG::NodePtr> nodes;

      // Load all the models that the user told us to load.
      for ( std::vector<std::string>::iterator i = mFilesToLoad.begin();
            i != mFilesToLoad.end();
            ++i )
      {
         vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
            << "[OpenSGNavGrab::initScene()] Loading '" << *i << "' ..."
            << std::endl << vprDEBUG_FLUSH;
         OSG::NodePtr geom_node =
            OSG::SceneFileHandler::the().read((OSG::Char8*) (*i).c_str());
         OSG::addRefCP(geom_node);

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
         const std::vector<OSG::NodePtr>::size_type num_objs(nodes.size());
         const OSG::Real32 interval_dist(distance_vec[0]);

         // Set the left extreme of the line of models.  The full extent of
         // the model range is interval_dist * num_objs.  To center things
         // on the origin, we divide that value by 2.  Since x_offset will
         // be used for the new center point of the model (see below), we
         // shift the starting point (interval_dist / 2) units so that some
         // model will end up centered on the origin.
         OSG::Real32 x_offset =
            -(interval_dist * OSG::Real32(num_objs) - interval_dist) / 2.0f;

         for ( std::vector<OSG::NodePtr>::iterator i = nodes.begin();
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

   OSG::beginEditCP(light_beacon_core, OSG::Transform::MatrixFieldMask);
      light_beacon_core->setMatrix(light_pos);
   OSG::endEditCP(light_beacon_core, OSG::Transform::MatrixFieldMask);

   OSG::beginEditCP(mLightBeacon);
      mLightBeacon->setCore(light_beacon_core);
   OSG::endEditCP(mLightBeacon);

   // Set up light node
   OSG::addRefCP(mLightNode);
   OSG::beginEditCP(mLightNode);
      mLightNode->setCore(light_core);
      mLightNode->addChild(mLightBeacon);
   OSG::endEditCP(mLightNode);

   OSG::beginEditCP(light_core);
      light_core->setAmbient(0.9, 0.8, 0.8, 1);
      light_core->setDiffuse(0.6, 0.6, 0.6, 1);
      light_core->setSpecular(1, 1, 1, 1);
      light_core->setDirection(0, 0, 1);
      light_core->setBeacon(mLightNode);
   OSG::endEditCP(light_core);

   // --- Set up scene -- //
   // add the loaded scene to the light node, so that it is lit by the light

   OSG::beginEditCP(mLightNode);
      mLightNode->addChild(mModelRoot);
   OSG::endEditCP(mLightNode);

   // create the root part of the scene
   mSceneRoot = OSG::Node::create();
   mSceneTransform = OSG::Transform::create();

   // Set up the root node
   OSG::beginEditCP(mSceneRoot);
      mSceneRoot->setCore(mSceneTransform);
      mSceneRoot->addChild(mLightNode);
   OSG::endEditCP(mSceneRoot);
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

   OSG::beginEditCP(mSceneTransform, OSG::Transform::MatrixFieldMask);
      mSceneTransform->getMatrix().setIdentity();
   OSG::endEditCP(mSceneTransform, OSG::Transform::MatrixFieldMask);

   for ( std::vector<GrabObject*>::iterator i = mObjects.begin();
         i != mObjects.end();
         ++i )
   {
      OSG::beginEditCP((*i)->xformCore, OSG::Transform::MatrixFieldMask);
         (*i)->xformCore->getMatrix().setValue((*i)->homePos);
      OSG::endEditCP((*i)->xformCore, OSG::Transform::MatrixFieldMask);
   }
}

// ----------------------------- Private methods ------------------------------

void OpenSGNavGrab::initGLState()
{
   GLfloat light0_ambient[]  = { 0.1f,  0.1f,  0.1f, 1.0f };
   GLfloat light0_diffuse[]  = { 0.7f,  0.7f,  0.7f, 1.0f };
   GLfloat light0_specular[] = { 1.0f,  1.0f,  1.0f, 1.0f };
   GLfloat light0_position[] = { 0.5f, 0.75f, 0.75f, 0.0f };

   GLfloat mat_ambient[]   = { 0.7f, 0.7f, 0.7f, 1.0f };
   GLfloat mat_diffuse[]   = { 1.0f, 0.5f, 0.8f, 1.0f };
   GLfloat mat_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat mat_shininess[] = { 50.0f };
//   GLfloat mat_emission[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   GLfloat no_mat[]        = { 0.0f, 0.0f, 0.0f, 1.0f };

   glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
   glShadeModel(GL_SMOOTH);

   // OpenSG does not handle this yet.  Being smart about it is non-trivial.
   glEnable(GL_NORMALIZE);
}

OpenSGNavGrab::GrabObject* OpenSGNavGrab::makeGrabbable(OSG::NodePtr model,
                                                        const OSG::Matrix& modelPos)
{
   OSG::TransformPtr model_xform_core = OSG::Transform::create();
   OSG::beginEditCP(model_xform_core, OSG::Transform::MatrixFieldMask);
      model_xform_core->setMatrix(modelPos);
   OSG::endEditCP(model_xform_core, OSG::Transform::MatrixFieldMask);

   OSG::NodePtr model_xform_node = OSG::Node::create();
   OSG::beginEditCP(model_xform_node);
      model_xform_node->setCore(model_xform_core);
      model_xform_node->addChild(model);
   OSG::endEditCP(model_xform_node);

   OSG::beginEditCP(mModelRoot, OSG::Node::ChildrenFieldMask);
      mModelRoot->addChild(model_xform_node);
   OSG::endEditCP(mModelRoot, OSG::Node::ChildrenFieldMask);

   return new GrabObject(model_xform_node, model_xform_core, modelPos);
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
   if ( mGrabbedObj == NULL )
   {
      GrabObject* intersect_obj(NULL);

      // Find the first object--if any--in mObjects with which the wand
      // intersects.
      for ( std::vector<GrabObject*>::iterator o = mObjects.begin();
            o != mObjects.end();
            ++o )
      {
         const OSG::DynamicVolume& bbox = (*o)->xformNode->getVolume();

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
            OSG::beginEditCP(mIntersectedObj->xformNode,
                             OSG::Node::ChildrenFieldMask);
               mIntersectedObj->xformNode->subChild(mHighlightNode);
            OSG::endEditCP(mIntersectedObj->xformNode,
                           OSG::Node::ChildrenFieldMask);
         }

         mIntersectedObj = intersect_obj;

         // If mIntersectedObj is non-NULL, we have selected a new object.
         // Create a new bounding box around that object.
         if ( mIntersectedObj != NULL )
         {
            mHighlight = mIntersectedObj->xformNode->getChild(0);

            // Set the highlight color to mIntersectColor.
            OSG::beginEditCP(mHighlightMaterial,
                             OSG::SimpleMaterial::DiffuseFieldMask);
               mHighlightMaterial->setDiffuse(mIntersectColor);
            OSG::endEditCP(mHighlightMaterial,
                           OSG::SimpleMaterial::DiffuseFieldMask);

            OSG::beginEditCP(mIntersectedObj->xformNode,
                             OSG::Node::ChildrenFieldMask);
               mIntersectedObj->xformNode->addChild(mHighlightNode);
            OSG::endEditCP(mIntersectedObj->xformNode,
                           OSG::Node::ChildrenFieldMask);
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
      OSG::beginEditCP(mHighlightMaterial,
                       OSG::SimpleMaterial::DiffuseFieldMask);
         mHighlightMaterial->setDiffuse(mGrabColor);
      OSG::endEditCP(mHighlightMaterial,
                     OSG::SimpleMaterial::DiffuseFieldMask);
   }
   // We cannot be grabbing anything unless button 0 is pressed.
   else if ( mButton0->getData() != gadget::Digital::ON )
   {
      // If we are dropping a grabbed object, then set the highlight color
      // back to mIntersectColor.
      if ( mGrabbedObj != NULL )
      {
         OSG::beginEditCP(mHighlightMaterial,
                          OSG::SimpleMaterial::DiffuseFieldMask);
            mHighlightMaterial->setDiffuse(mIntersectColor);
         OSG::endEditCP(mHighlightMaterial,
                        OSG::SimpleMaterial::DiffuseFieldMask);
      }

      mGrabbedObj = NULL;
   }

   // If mGrabbedObj is non-NULL, then we are grabbing the object pointed at
   // by mGrabbedObj.
   if ( mGrabbedObj != NULL )
   {
      OSG::beginEditCP(mGrabbedObj->xformCore, OSG::Transform::MatrixFieldMask);
         mGrabbedObj->xformCore->getMatrix().setTranslate(wand_point);
      OSG::endEditCP(mGrabbedObj->xformCore, OSG::Transform::MatrixFieldMask);
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

   OSG::beginEditCP(mSceneTransform, OSG::Transform::MatrixFieldMask);
      mSceneTransform->getMatrix().multLeft(trans_mat);
   OSG::endEditCP(mSceneTransform, OSG::Transform::MatrixFieldMask);
}

// This implementation is adapted from
// OSG::SimpleSceneManager::highlightChanged() in OpenSG 1.4.0.
void OpenSGNavGrab::initHighlight()
{
   if ( mHighlightMaterial == OSG::NullFC )
   {
      mHighlightMaterial = OSG::SimpleMaterial::create();

      OSG::beginEditCP(mHighlightMaterial);
         mHighlightMaterial->setLit(false);
      OSG::endEditCP(mHighlightMaterial);
   }

   if ( mHighlightNode == OSG::NullFC )
   {
      OSG::GeoPTypesPtr type = OSG::GeoPTypesUI8::create();
      OSG::beginEditCP(type);
         type->push_back(GL_LINE_STRIP);
         type->push_back(GL_LINES);
      OSG::endEditCP(type);

      OSG::GeoPLengthsPtr lens = OSG::GeoPLengthsUI32::create();
      OSG::beginEditCP(lens);
         lens->push_back(10);
         lens->push_back(6);
      OSG::endEditCP(lens);

      OSG::GeoIndicesUI32Ptr index = OSG::GeoIndicesUI32::create();
      OSG::beginEditCP(index);
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
      OSG::endEditCP(index);

      mHighlightPoints = OSG::GeoPositions3f::create();
      OSG::beginEditCP(mHighlightPoints);
         mHighlightPoints->push_back(OSG::Pnt3f(-1, -1, -1));
         mHighlightPoints->push_back(OSG::Pnt3f( 1, -1, -1));
         mHighlightPoints->push_back(OSG::Pnt3f(-1,  1, -1));
         mHighlightPoints->push_back(OSG::Pnt3f( 1,  1, -1));
         mHighlightPoints->push_back(OSG::Pnt3f(-1, -1,  1));
         mHighlightPoints->push_back(OSG::Pnt3f( 1, -1,  1));
         mHighlightPoints->push_back(OSG::Pnt3f(-1,  1,  1));
         mHighlightPoints->push_back(OSG::Pnt3f( 1,  1,  1));
      OSG::endEditCP(mHighlightPoints);

      OSG::GeometryPtr geo = OSG::Geometry::create();
      OSG::beginEditCP(geo);
         geo->setTypes(type);
         geo->setLengths(lens);
         geo->setIndices(index);
         geo->setPositions(mHighlightPoints);
         geo->setMaterial(mHighlightMaterial);
      OSG::endEditCP(geo);
      OSG::addRefCP(geo);

      mHighlightNode = OSG::Node::create();
      OSG::beginEditCP(mHighlightNode);
         mHighlightNode->setCore(geo);
      OSG::endEditCP(mHighlightNode);
      OSG::addRefCP(mHighlightNode);
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

   OSG::beginEditCP(mHighlightPoints);
      mHighlightPoints->setValue(OSG::Pnt3f(min[0], min[1], min[2]), 0);
      mHighlightPoints->setValue(OSG::Pnt3f(max[0], min[1], min[2]), 1);
      mHighlightPoints->setValue(OSG::Pnt3f(min[0], max[1], min[2]), 2);
      mHighlightPoints->setValue(OSG::Pnt3f(max[0], max[1], min[2]), 3);
      mHighlightPoints->setValue(OSG::Pnt3f(min[0], min[1], max[2]), 4);
      mHighlightPoints->setValue(OSG::Pnt3f(max[0], min[1], max[2]), 5);
      mHighlightPoints->setValue(OSG::Pnt3f(min[0], max[1], max[2]), 6);
      mHighlightPoints->setValue(OSG::Pnt3f(max[0], max[1], max[2]), 7);
   OSG::endEditCP(mHighlightPoints);

   OSG::beginEditCP(mHighlightNode->getCore(),
                    OSG::Geometry::PositionsFieldMask);
   OSG::endEditCP(mHighlightNode->getCore(),
                  OSG::Geometry::PositionsFieldMask);
}

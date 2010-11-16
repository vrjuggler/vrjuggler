/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_OSG_APP_PAGER_H_
#define _VRJ_OSG_APP_PAGER_H_

#include <vrj/vrjConfig.h>

#include <osgDB/DatabasePager>
#include <osgDB/ImagePager>

#include <vrj/Draw/OSG/App.h>

namespace vrj
{

namespace osg
{

/** \class vrj::osg::AppPager AppPager.h vrj/Draw/OSG/AppPager.h
 *
 * @since 2.3.20
 */
class AppPager : public vrj::osg::App
{
public:
   AppPager(Kernel* kern = NULL)
      : vrj::osg::App(kern)
   {
   }

   virtual ~AppPager()
   {
   }

   virtual void initScene()
   {
      mDatabasePager = new osgDB::DatabasePager;
      mImagePager = new osgDB::ImagePager;

      mDatabasePager->registerPagedLODs(getScene()); 
   }

   virtual void configSceneView(osgUtil::SceneView* newSceneViewer)
   {
      vrj::osg::App::configSceneView(newSceneViewer);

      newSceneViewer->getCullVisitor()->setDatabaseRequestHandler(mDatabasePager);
      mDatabasePager->setCompileGLObjectsForContextID(
         newSceneViewer->getState()->getContextID(), true
      );
   }

   virtual void contextPostDraw()
   {
      const double available_time(1.0 / 10.0);

      ::osg::ref_ptr< ::osg::FrameStamp > frame_stamp(getFrameStamp());
      if (frame_stamp)
      {
         mDatabasePager->compileGLObjects(*sceneViewer->get()->getState(),
                                          available_time);
         sceneViewer->get()->flushDeletedGLObjects(available_time);
      }

      mDatabasePager->signalEndFrame();
      vrj::osg::App::contextPostDraw();
   }

protected:
   virtual void update()
   {
      vrj::osg::App::update();

      ::osg::ref_ptr< ::osg::FrameStamp > frame_stamp(getFrameStamp());
      mDatabasePager->updateSceneGraph(*frame_stamp);
      mImagePager->updateSceneGraph(*frame_stamp);

      mDatabasePager->signalBeginFrame(frame_stamp);
   }

private:
   ::osg::ref_ptr<osgDB::DatabasePager> mDatabasePager;
   ::osg::ref_ptr<osgDB::ImagePager> mImagePager;
};

} // End of osg namespace

} // End of vrj namespace


#endif /* _VRJ_OSG_APP_PAGER_H_ */

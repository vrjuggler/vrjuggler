#include <gadget/Filter/Position/PositionCalibrationFilter.h>
#include <gadget/Filter/Position/PositionFilterFactory.h>
#include <jccl/Config/ConfigElement.h>
#include <jccl/Config/ParseUtil.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <cppdom/cppdom.h>
#include <gmtl/Math.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Output.h>
#include <gmtl/VecOps.h>

#include <sstream>
#include <fstream>

namespace gadget
{

   GADGET_REGISTER_POSFILTER_CREATOR( PositionCalibrationFilter );
   
   PositionCalibrationFilter::~PositionCalibrationFilter()
   {
      delete[] mAlphaVec;
      for (size_t i = 0; i < mTable.size(); ++i)
      {
         delete[] mWMatrix[i];
      }
      delete[] mWMatrix;
   }
   
   std::string
   PositionCalibrationFilter::getElementType()
   {
      return std::string("position_calibration_filter");
   }
   
   bool
   PositionCalibrationFilter::config(jccl::ConfigElementPtr e)
   {
      vprASSERT(e->getID() == PositionCalibrationFilter::getElementType());
      vprDEBUG_OutputGuard(vprDBG_ALL, vprDBG_VERB_LVL,
                           std::string("PositionCalibrationFilter::config:  ") +
                           e->getFullName() + std::string(":") + e->getID() + 
                           std::string("\n"),
                    std::string("PositionCalibrationFilter::config: done.\n") );
      std::string file_name = e->getProperty<std::string>("calibration_file",0);
      mFileName = jccl::ParseUtil::expandFileName(file_name, std::string("") );

      // Parse the calibration file.
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config] Parsing " << mFileName << "\n"
         << vprDEBUG_FLUSH;

      cppdom::ContextPtr context( new cppdom::Context() );
      cppdom::DocumentPtr document( new cppdom::Document(context) );
      
      try
      {
         document->loadFile(mFileName);
      }
      catch (cppdom::Error e)
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
            << "[PositionCalibrationFilter::config] Unable to load calibration "
            << "file " << mFileName << ".  " << e.getString() << " at "
            << e.getInfo() << vprDEBUG_FLUSH;
         return false;
      }

      cppdom::NodePtr root = document->getChild("CalibrationTable");
      if (NULL == root.get())
      {
         vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
            << "[PositionCalibrationFilter::config] Bad calibration file "
            << mFileName << "; could not retrieve the root node "
            << "'CalibrationTable'.  " << vprDEBUG_FLUSH;
         return false;
      }
      
      cppdom::NodeList offset_list;
      offset_list = root->getChildren("Offset");
      cppdom::NodeListIterator itr;
      
      for (itr = offset_list.begin(); itr != offset_list.end(); ++itr)
      {
         gmtl::Vec3f real_position;
         gmtl::Vec3f dev_position;
         
         if ( (*itr)->hasAttribute("X") && 
              (*itr)->hasAttribute("Y") && 
              (*itr)->hasAttribute("Z") )
         {
            real_position.set( (*itr)->getAttribute("X").getValue<float>(),
                               (*itr)->getAttribute("Y").getValue<float>(),
                               (*itr)->getAttribute("Z").getValue<float>() );
            std::istringstream offset_stream((*itr)->getCdata());
            //XXX:  Error checking on the Cdata needs to be more robust.
            offset_stream >> dev_position[gmtl::Xelt];
            offset_stream >> dev_position[gmtl::Yelt];
            offset_stream >> dev_position[gmtl::Zelt];

            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
               << "[PositionCalibrationFilter::config()] Added "
               << real_position << " --> " << dev_position << " "
               << "to the table.\n"
               << vprDEBUG_FLUSH;
            
            mTable.push_back( std::make_pair(real_position, dev_position) );
         }
         else
         {
            //XXX:  Make this more informative.
            vprDEBUG(vprDBG_ERROR, vprDBG_CONFIG_LVL)
               << "[PositionCalibrationFilter::config()] Element doesn't "
               << "contain a real position attribute; skipping.\n"
               << vprDEBUG_FLUSH;
         }
      }

      // Now that we have the calibration table, compute the W Matrix and then
      // solve for the Alpha Vector.

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Calibration table parsed; "
         << "preparing W Matrix...\n" << vprDEBUG_FLUSH;

      // The "W" Matrix is an NxN matrix where N = mTable.size()
      // It is composed of elements computed using the w(p) function:
      // w[j](p) = sqrt( length(p - p[j])^2 + R^2 )
      // where 10 <= R^2 <= 1000.
      // The matrix looks like:
      // ( w[1](p[1]) w[2](p[1]) w[3](p[1]) ... w[N](p[1]) )
      // ( w[1](p[2]) w[2](p[2]) w[3](p[2]) ... w[N](p[2]) )
      // ( w[1](p[3]) w[2](p[3]) w[3](p[3]) ... w[N](p[3]) )
      // (                      .                          )
      // (                      .                          )
      // (                      .                          )
      // ( w[1](p[N]) w[2](p[N]) w[3](p[N]) ... w[N](p[N]) )

      vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
         << "[PositionCalibrationFilter::config()] The W matrix is " 
         << mTable.size() << "x" << mTable.size() << ".\n" 
         << vprDEBUG_FLUSH;
      
      float r_squared(100.0f);
      float r(10.0f);
      mWMatrix = new float*[mTable.size()];
      for (unsigned int i = 0; i < mTable.size(); ++i)
      {
         mWMatrix[i] = new float[mTable.size()];
         for (unsigned int j = 0; j < mTable.size(); ++j)
         {
            if (i == j)
            {
               // Shortcut; the diagonal will always be equal to R.
               mWMatrix[i][j] = r;
            }
            else
            {
               // XXX: This is a workaround for GMTL CVS Head which supports template 
               //      meta-programming for vectors; unfortunately, 
               //      gmtl::length(v1 - v2) will not compile since the types are 
               //      NOT gmtl::Vec.
               gmtl::Vec3f difference = mTable[i].second - mTable[j].second;
               float length = gmtl::length( difference );
               mWMatrix[i][j] = gmtl::Math::sqrt( 
                                length * length + 
                                r_squared );
            }
            vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
               << "[PositionCalibrationFilter::config()] Assigning " << mWMatrix[i][j]
               << " to mWMatrix( " << i << ", " << j << ").\n"
               << vprDEBUG_FLUSH;
         }
      }

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Finished preparing the W matrix.\n"
         << "[PositionCalibrationFilter::config()] Solving for the alpha vector...\n"
         << vprDEBUG_FLUSH;

      // Our LU Decomposition handles one direction at a time.
      float* alpha_x = new float[mTable.size()];
      float* alpha_y = new float[mTable.size()];
      float* alpha_z = new float[mTable.size()];

      int* permutation = new int[mTable.size()];
      
      for (size_t i = 0; i < mTable.size(); ++i)
      {
         alpha_x[i] = mTable[i].first[0];
         alpha_y[i] = mTable[i].first[1];
         alpha_z[i] = mTable[i].first[2];
         permutation[i] = 0;
      }
      
      // Solve for each component of the alpha vector.
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Performing LU Decomposition on "
         << "the W Matrix...\n"
         << vprDEBUG_FLUSH;
      luDecomposition(mWMatrix, mTable.size(), permutation);
      
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Solving for Alpha X...\n"
         << vprDEBUG_FLUSH;
      luBacksubstitution(mWMatrix, mTable.size(), permutation, alpha_x);
      
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Solving for Alpha Y...\n"
         << vprDEBUG_FLUSH;
      luBacksubstitution(mWMatrix, mTable.size(), permutation, alpha_y);
      
      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Solving for Alpha Z...\n"
         << vprDEBUG_FLUSH;
      luBacksubstitution(mWMatrix, mTable.size(), permutation, alpha_z);
      
      // Copy the alpha vector into mAlphaVec.

      mAlphaVec = new gmtl::Vec3f[mTable.size()];
      for (size_t i = 0; i < mTable.size(); ++i)
      {
         mAlphaVec[i][0] = alpha_x[i];
         mAlphaVec[i][1] = alpha_y[i];
         mAlphaVec[i][2] = alpha_z[i];
         vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
            << "[PositionCalibrationFilter::config()] AlphaVec[" << i << "] "
            << ": " << mAlphaVec[i] << ".\n"
            << vprDEBUG_FLUSH;
      }

      vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
         << "[PositionCalibrationFilter::config()] Alpha Vector found.\n"
         << vprDEBUG_FLUSH;
      
      // Clean up memory

      delete[] permutation;
      delete[] alpha_x;
      delete[] alpha_y;
      delete[] alpha_z;

      return true;  
   }

   void
   PositionCalibrationFilter::apply(std::vector< PositionData >& posSample)
   {
      std::vector< PositionData >::iterator itr;
      for (itr = posSample.begin(); itr != posSample.end(); ++itr)
      {
         // Prepare the position matrix for Hardy's multi-quadric method.
         // 
         // Right now, we only calibrate the position, not the orientation, so
         // first, we need to pull out the translation matrix of posSample.
         // We do this using the standard method:  pull the rotation matrix R out
         // of the transformation matrix Tr and multiply R-inverse by Tr, ie,
         // inverse(R) * Tr = T
         // and since rotation matrices are orthogonal,
         // inverse(R) = transpose(R), so...
         // transpose(R) * Tr = T
         gmtl::Matrix44f rotation(itr->getPosition());
         rotation[0][3] = 0;
         rotation[1][3] = 0;
         rotation[2][3] = 0;
         rotation[3][3] = 1;
         gmtl::Matrix44f translation = gmtl::transpose(rotation) * itr->getPosition();
         gmtl::Vec3f tracked_pos( translation[0][3], 
                                  translation[1][3], 
                                  translation[2][3] );

         // Now, we apply the following to the tracked position:
         // real_pos = alpha[j] * w[j](tracked_pos) + ... + alpha[N] * 
         // w[N](tracked_pos)
         // where w[j](p) = sqrt( length( p - p[j] )^2 + R^2 )
         // where 10 <= R^2 <= 1000.
         gmtl::Vec3f real_pos;
         float r_squared = 100.0f;
         for (unsigned int i = 0; i < mTable.size(); ++i)
         {
            // XXX: This is a workaround for GMTL CVS Head which supports template 
            //      meta-programming for vectors; unfortunately, 
            //      gmtl::length(v1 - v2) will not compile since the types are 
            //      NOT gmtl::Vec.
            gmtl::Vec3f difference = tracked_pos = mTable[i].second;
            float length = gmtl::length(difference);
            real_pos = mAlphaVec[i] * gmtl::Math::sqrt( length * length + r_squared ); 
         }

         vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
            << "[PositionCalibrationFilter::apply()] Replaced " << translation
            << " with ";
         
         // Now we clobber the old transformation and replace it with a translation to
         // our real position.
         translation[0][3] = real_pos[0];
         translation[1][3] = real_pos[1];
         translation[2][3] = real_pos[2];
         translation[3][3] = 1;
         
         vprDEBUG(vprDBG_ALL, vprDBG_DETAILED_LVL)
            << translation << "\n"
            << vprDEBUG_FLUSH;

         // Rebuild the position sample (transformation matrix).
         itr->setPosition(rotation * translation);
      }
   }
     
   void 
   PositionCalibrationFilter::luBacksubstitution(float** decomposedA, 
                                                 unsigned int size, 
                                                 int* permutation, float* solution)
   {
      int ii(0);
      int ip(0);
      float sum(0.0f);

      for (int i = 0; i < size; ++i) 
      {
         ip = permutation[i];
         sum = solution[ip];
         solution[ip] = solution[i];
         if (ii != 0)
         {
            for (int j = ii - 1; j < i; ++j)
            { 
               sum -= decomposedA[i][j] * solution[j];
            }
         }
         else if (sum != 0.0)
         {
            ii = i + 1;
         }
         solution[i] = sum;
      }
      for (int i = size - 1; i >= 0; --i)
      {
         sum = solution[i];
         for (int j = i + 1; j < size; ++j)
         {
            sum -= decomposedA[i][j] * solution[j];
         }
         solution[i] = sum / decomposedA[i][i];
      }
   }
   
   void 
   PositionCalibrationFilter::luDecomposition(float** matrix, unsigned int size, 
                                              int* permutation) 
   {
      const float TINY(0.0f);
      int imax(0);
      int i(0);
      int j(0);
      int k(0);
      
      float big(0.0f);
      float dum(0.0f);
      float sum(0.0f);
      float temp(0.0f);

      float* vv = new float[size];
      
      for (i = 0; i < size; ++i)
      {
         for (j = 0; j < size; ++j)
         {
            temp = gmtl::Math::abs(matrix[i][j]);
            if (temp > big)
            {
               big = temp;
            }
         }
         if (0.0 == big)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_VERB_LVL)
               << "[PositionCalibrationFilter::luDecomposition()] Singular matrix "
               << "given.\n"
               << vprDEBUG_FLUSH;
         }
         vv[i] = 1.0 / big;
      }
      for (j = 0; j < size; ++j)
      {
         for (i = 0; i < j; ++i)
         {
            sum = matrix[i][j];
            for (k = 0; k < i; ++k)
            {
               sum -= matrix[i][k] * matrix[k][j];
            }
            matrix[i][j] = sum;
         }
         big = 0.0;
         for (i = j; i < size; ++i)
         {
            sum = matrix[i][j];
            for (k = 0; k < j; ++k)
            {
               sum -= matrix[i][k] * matrix[k][j];
            }
            matrix[i][j] = sum;
            dum = vv[i] * gmtl::Math::abs(sum);
            if (dum >= big)
            {
               big = dum;
               imax = i;
            }
         }
         if (j != imax)
         {
            for (k = 0; k < size; ++k)
            {
               dum = matrix[imax][k];
               matrix[imax][k] = matrix[j][k];
               matrix[j][k] = dum;
            }
            vv[imax] = vv[j];
         }
         permutation[j] = imax;
         if (0.0 == matrix[j][j])
         {
            matrix[j][j] = TINY;
         }
         if (j != size - 1)
         {
            dum = 1.0 / (matrix[j][j]);
            for (i = j + 1; i < size; ++i)
            {
               matrix[i][j] *= dum;
            }
         }
      }
      delete[] vv;
   }
}

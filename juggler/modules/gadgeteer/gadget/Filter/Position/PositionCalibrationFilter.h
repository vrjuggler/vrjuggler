#ifndef _GADGET_POSITION_CALIBRATION_FILTER_H_
#define _GADGET_POSITION_CALIBRATION_FILTER_H_

#include <gadget/gadgetConfig.h>
#include <gadget/Filter/Position/PositionFilter.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <gadget/Type/PositionData.h>
#include <gadget/Util/Debug.h>

#include <gmtl/Vec.h>

#include <map>
#include <string>
#include <vector>

namespace gadget
{
   /**
    * A calibration filter that uses a calibration table to correct 
    * electromagnetic interference error from the tracker.
    */
   class GADGET_CLASS_API PositionCalibrationFilter : public PositionFilter
   {
   public:
      
      /**
       * Default Ctor
       */
      PositionCalibrationFilter()
         : mWMatrix(NULL), mAlphaVec(NULL)
      {}

      ~PositionCalibrationFilter();

      bool config(jccl::ConfigElementPtr e);

      void apply(std::vector< PositionData >& posSample);

      static std::string getElementType();
      
   private:

      /// The calibration table.
      std::vector< std::pair< gmtl::Vec3f, gmtl::Vec3f > > mTable;

      /// The name of the calibration file.
      std::string                                         mFileName;

      /// The "W" matrix.
      /// The "W" Matrix is an NxN matrix where N = mTable.size()
      /// It is composed of elements computed using the w(p) function:
      /// w[j](p) = sqrt( length(p - p[j]) + R*R )
      /// where 10 <= R*R <= 1000.
      /// The matrix looks like:
      /// ( w[1](p[1]) w[2](p[1]) w[3](p[1]) ... w[N](p[1]) )
      /// ( w[1](p[2]) w[2](p[2]) w[3](p[2]) ... w[N](p[2]) )
      /// ( w[1](p[3]) w[2](p[3]) w[3](p[3]) ... w[N](p[3]) )
      /// (                      .                          )
      /// (                      .                          )
      /// (                      .                          )
      /// ( w[1](p[N]) w[2](p[N]) w[3](p[N]) ... w[N](p[N]) )
      float**                                             mWMatrix;

      /// The Alpha Vector.
      gmtl::Vec3f*                                        mAlphaVec;
      
      /**
       * Performs Backsubstitution on a matrix; designed to be used
       * in conjunction with the LU Decomposition algorithm.  Both
       * this and the LU Decomposition algorithm are used to solve
       * the set of n linear equations A * X = B as described in
       * _Numerical Recipes in C++_.
       *
       * @param   decomposedA    The LU Decomposition of A.
       * @param   size           The size of the square matrix A.
       * @param   permutation    The permutation vector as returned by
       *                         LU Decomposition.
       * @param   solution       The solution matrix B.
       *
       * @return     solution will be replaced with the solution matrix X.
       */
      void luBacksubstitution(float** decomposedA, int size, 
                              int* permutation, float* solution);

      /**
       * Performs LU Decomposition on a set of n linear equations.  
       * This is designed to solve the set of equations:
       * A * x = B.  LU Decomposition does this by
       * L * U = A.
       * => (L * U) * x = B
       * => L * (U * x) = B
       * 
       * This routine actually does the LU Decomposition of a rowwise permutation of 
       * the given matrix, and then hands that to luBacksubstitution for completion,
       * as described in _Numerical Recipes in C_.  
       *
       * @note    matrix will be destroyed through this process!
       * 
       * @param   matrix         the matrix to decompose.
       * @param   size           the size of the square matrix.
       * @param   permutation    the permutation vector.
       */
      void luDecomposition(float** matrix, int size, int* permutation);

   };
}
#endif

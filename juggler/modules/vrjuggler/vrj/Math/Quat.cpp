#include <Math/vjQuat.h>

void vjQuat::makeQuat(vjMatrix& mat)
{
   float tr, s;
   int i, j, k;
   static int nxt[3] = {VJ_Y, VJ_Z, VJ_X};

   tr = mat.mat[0][0] + mat.mat[1][1] + mat.mat[2][2];

   if (tr > 0.0)
   {
      s = sqrt(tr + 1.0);
      vec[VJ_W] = s*0.5;
      s = 0.5/s;

      vec[VJ_X] = (mat.mat[1][2] - mat.mat[2][1])*s;
      vec[VJ_Y] = (mat.mat[2][0] - mat.mat[0][2])*s;
      vec[VJ_Y] = (mat.mat[0][1] - mat.mat[1][0])*s; 
   }
   else
   {
      i = VJ_X;
      if (mat.mat[VJ_Y][VJ_Y] > mat.mat[VJ_X][VJ_X]) i = VJ_Y;
      if (mat.mat[VJ_Z][VJ_Z] > mat.mat[i][i]) i = VJ_Z;

      j = nxt[i]; k = nxt[j];

      s = sqrt((mat.mat[i][j] - (mat.mat[j][j]+mat.mat[k][k])) + 1.0f);

      vec[i] = s*0.5;
      s = 0.5/s;
      vec[VJ_W] = (mat.mat[j][k] - mat.mat[k][j])*s;
      vec[j] = (mat.mat[i][j] + mat.mat[j][i])*s;
      vec[k] = (mat.mat[i][k] + mat.mat[k][i])*s;
   }
}


// From Adv Anim and Rendering Tech. Pg 364
//
void vjQuat::slerp(float t, const vjQuat& p, const vjQuat& q)
{
   double omega, cosom, sinom, sclp, sclq;
   int i;

   cosom = p[VJ_X]*q[VJ_X] + p[VJ_Y]*q[VJ_Y] + p[VJ_Z]*q[VJ_Z] + p[VJ_W]*q[VJ_W];

   if ((1.0 + cosom) > VJ_QUAT_EPSILON)
   {
      if ((1.0 - cosom) > VJ_QUAT_EPSILON)
      {
         omega = acos(cosom);
         sinom = sin(omega);
         sclp  = sin((1.0 - t)*omega)/sinom;
         sclq = sin(t*omega)/sinom;
      }
      else
      {
         sclp = 1.0 - t;
         sclq = t;
      }
      for (i=0; i<4; i++)
         vec[i] = sclp*p[i] + sclq*q[i];

   }
   else
   {
      vec[VJ_X] = -p[VJ_Y];
      vec[VJ_Y] = p[VJ_X];
      vec[VJ_Z] = -p[VJ_W];
      vec[VJ_W] = p[VJ_Z];

      sclp = sin((1.0 - t)*M_PI_2);
      sclq = sin(t*M_PI_2);

      for (i=0; i<3; i++)
         vec[i] = sclp*p[i] + sclq*vec[i];
   }
}

void vjQuat::squad(float _t, const vjQuat& _q1, const vjQuat& _q2, const vjQuat& _a, const vjQuat& _b)
{;}


#include <Math/vjPlane.h>
#include <Math/vjVec3.h>
#include <Math/vjSeg.h>

// Program to test the functionality of the vjSeg class
//
//
int main(void)
{
   // Create some planes and segs to use in the tests
   const vjVec3 x_axis(1,0,0);
   const vjVec3 y_axis(0,1,0);
   const vjVec3 z_axis(0,0,1);
   const vjVec3 origin(0,0,0);
   vjPlane zy_plane; zy_plane.makePts(z_axis,origin,y_axis);

   // Test finding collision with a triangle
   vjSeg seg_through, seg_parallel, seg_miss;
   float t_dist;
   bool hit;

   seg_through.makePts(vjVec3(1,0.25,0.25), vjVec3(-1,0.25,0.25) );    // Passes through the plane at the origin
   seg_parallel.makePts(x_axis, (x_axis+z_axis));                 // Is parallel to the plane
   seg_miss.makePts(vjVec3(1,1,1), vjVec3(-1,0.5,0.5));  // Misses

   hit = seg_through.isectTriangle(z_axis,origin,y_axis,&t_dist);
   cout << "Testing isectTriangle (through): ";
   if(hit && VJ_IS_ZERO(t_dist - 1.0f))
      cout << "Passed.\n";
   else
      cout << "FAILED!!!!\n";

   hit = seg_parallel.isectTriangle(z_axis,origin,y_axis,&t_dist);
   cout << "Testing isectTriangle (parallel): ";
   if(!hit)
      cout << "Passed.\n";
   else
      cout << "FAILED!!!!\n";

   hit = seg_miss.isectTriangle(z_axis,origin,y_axis,&t_dist);
   cout << "Testing isectTriangle (miss): ";
   if(!hit)
      cout << "Passed.\n";
   else
      cout << "FAILED!!!!\n";



}

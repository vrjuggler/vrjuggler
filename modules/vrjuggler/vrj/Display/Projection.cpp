#include <Kernel/vjProjection.h>

ostream& operator<<(ostream& out, vjProjection& proj)
{
   out << "surface: ";
   switch(proj.surface)
   {
   case vjProjection::LEFT:
      out << "Left";
      break;
   case vjProjection::RIGHT:
      out << "Right";
      break;
   case vjProjection::FRONT:
      out << "Front";
      break;
   case vjProjection::FLOOR:
      out << "Floor";
      break;
   case vjProjection::BACK:
      out << "Back";
      break;
   case vjProjection::CEILING:
      out << "Ceiling";
      break;
   case vjProjection::USER:
      out << "User";
      break;
   }
   out << endl;
   
   out << "eye: ";
   switch(proj.eye)
   {
   case vjProjection::LEFT:
      out << "Left";
      break;
   case vjProjection::RIGHT:
      out << "Right";
      break;
   }
   out << endl;
   return out;
}

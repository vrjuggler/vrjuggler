
#include "Image.h"
#include "TgaImporter.h"

void main()
{
   TgaImporter i;
   Image image;
   i.import( "bokbokbok.tga", image );
}

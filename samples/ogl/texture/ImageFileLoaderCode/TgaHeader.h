#ifndef TGA_DATA_STRUCTURE
#define TGA_DATA_STRUCTURE

// This code contributed from software located:
// http://www.bigfoot.com/~KevinMeinert/html/software/index.html
// -- Kevin Meinert, kevinmeinert@bigfoot.com
//////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	//
	//	For more information about the original Truevision TGA(tm) file format,
	//	or for additional information about the new extensions to the
	//	Truevision TGA file, refer to the "Truevision TGA File Format
	//	Specification Version 2.0" available from Truevision or your
	//	Truevision dealer.
	//
	//  FILE STRUCTURE FOR THE ORIGINAL TRUEVISION TGA FILE				
	//	  FIELD 1 :	NUMBER OF CHARACTERS IN ID FIELD (1 BYTES)	
	//	  FIELD 2 :	COLOR MAP TYPE (1 BYTES)			
	//	  FIELD 3 :	IMAGE TYPE CODE (1 BYTES)			
	//					= 0	NO IMAGE DATA INCLUDED		
	//					= 1	UNCOMPRESSED, COLOR-MAPPED IMAGE
	//					= 2	UNCOMPRESSED, TRUE-COLOR IMAGE	
	//					= 3	UNCOMPRESSED, BLACK AND WHITE IMAGE
	//					= 9	RUN-LENGTH ENCODED COLOR-MAPPED IMAGE
	//					= 10 RUN-LENGTH ENCODED TRUE-COLOR IMAGE
	//					= 11 RUN-LENGTH ENCODED BLACK AND WHITE IMAGE
	//	  FIELD 4 :	COLOR MAP SPECIFICATION	(5 BYTES)		
	//				4.1 : COLOR MAP ORIGIN (2 BYTES)	
	//				4.2 : COLOR MAP LENGTH (2 BYTES)	
	//				4.3 : COLOR MAP ENTRY SIZE (2 BYTES)	
	//	  FIELD 5 :	IMAGE SPECIFICATION (10 BYTES)			
	//				5.1 : X-ORIGIN OF IMAGE (2 BYTES)	
	//				5.2 : Y-ORIGIN OF IMAGE (2 BYTES)	
	//				5.3 : WIDTH OF IMAGE (2 BYTES)		
	//				5.4 : HEIGHT OF IMAGE (2 BYTES)		
	//				5.5 : IMAGE PIXEL SIZE (1 BYTE)		
	//				5.6 : IMAGE DESCRIPTOR BYTE (1 BYTE) 	
	//	  FIELD 6 :	IMAGE ID FIELD (LENGTH SPECIFIED BY FIELD 1)	
	//	  FIELD 7 :	COLOR MAP DATA (BIT WIDTH SPECIFIED BY FIELD 4.3 AND
	//				NUMBER OF COLOR MAP ENTRIES SPECIFIED IN FIELD 4.2)
	//	  FIELD 8 :	IMAGE DATA FIELD (WIDTH AND HEIGHT SPECIFIED IN
	//				FIELD 5.3 AND 5.4)				
	//////////////////////////////////////////////////////////////////////////

	//
	// Define byte counts associated with extension areas for various
	// versions of the TGA specification.
	
	#define	EXT_SIZE_20	495			// verison 2.0 extension size 
	#define CBUFSIZE	2048		// size of copy buffer 
	#define RLEBUFSIZ	512			// size of largest possible RLE packet 

	

	// Original header found in the TGA 1.0 spec
	struct TGAHeader
	{
		unsigned char	idLength;		// length of ID string 
		unsigned char	mapType;		// color map type, if 1 then there is a palette. if 0, then there is no palette.
		unsigned char	imageType;		// image type code 
		unsigned short	mapStart;		// starting index of map 
		unsigned short	mapLength;		// size of map in elements (next field is the element size.
		unsigned char	mapDepth;		// width of map in bits 8, 15, 16, 24, 32
		unsigned short	xOrigin;		// x-origin of image 
		unsigned short	yOrigin;		// y-origin of image 
		unsigned short	imageWidth;		// width of image 
		unsigned short	imageHeight;	// height of image 
		unsigned char	pixelDepth;		// bits per pixel 
		unsigned char	imageDesc;		// image descriptor 
		//1.) bits 0-3 contain the number of attribute bits per pixel. (16, and 32bit only) alpha channel, overlay or interrupt bits...
		//2.) bits 4,5 image origin location (0,0) of image. 0,0 is origin at lower corner...
		//3.) bits 6,7 unused set to 0
	};

	//char				idString[256];	// image ID string 
	
	// Then data. . . .

	// Then the extended header found in TGA 2.0 spec
	struct TGAExtendedHeader
	{
		unsigned short	extSize;		// extension area size (should be 495)
		char			author[41];		// name of the author of the image 
		char			authorCom[324];	// author's comments 
		unsigned short	month;			// date-time stamp 
		unsigned short	day;
		unsigned short	year;
		unsigned short	hour;
		unsigned short	minute;
		unsigned short	second;
		char			jobID[41];		// job identifier 
		unsigned short	jobHours;		// job elapsed time 
		unsigned short	jobMinutes;
		unsigned short	jobSeconds;
		char			softID[41];		// software identifier/program name 
		unsigned short	versionNum;		// software version designation 
		unsigned char	versionLet;
		unsigned long	keyColor;		// key color value as A:R:G:B 
		unsigned short	pixNumerator;	// pixel aspect ratio 
		unsigned short	pixDenominator;
		unsigned short	gammaNumerator;	// gamma correction factor 
		unsigned short	gammaDenominator;
		unsigned long	colorCorrectOffset;	// offset to color correction table 
		unsigned long	stampOffset;	// offset to postage stamp data 
		unsigned long	scanLineOffset;	// offset to scan line table 
		unsigned char	alphaAttribute;	// alpha attribute description 
		unsigned long	*scanLineTable;	// address of scan line offset table 
		unsigned char	stampWidth;		// width of postage stamp 
		unsigned char	stampHeight;	// height of postage stamp 
		void*			postStamp;		// address of postage stamp data 
		unsigned short	*colorCorrectTable;
	};
	
	// in a TGA 2.0 file, this structure will be 26 bytes from the end of the file.
	// if signature == "TRUEVISION-XFILE.\0" then the file really is a TGA2.0, 
	// and may carry an extended header (if extAreaOffset > 0).
	struct TGAFooter{
		unsigned long	extAreaOffset;	// extension area offset 
		unsigned long	devDirOffset;	// developer directory offset 
		char			signature[18];	// signature string	
	};

	// for use by the TGA 2.0 developer area stuff
	struct TGADeveloperAreaTag
	{
		unsigned short	tagValue;		// ID number of the tag.
		unsigned long	tagOffset;		// Offset location of the tag data.
		unsigned long	tagSize;		// Size of the tag in bytes.
	};

#endif

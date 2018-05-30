// image format detection solution is found at https://oroboro.com/image-format-magic-bytes/
enum ImageFileType
{
   IMAGE_FILE_JPG,      // joint photographic experts group - .jpeg or .jpg
   IMAGE_FILE_PNG,      // portable network graphics
   IMAGE_FILE_GIF,      // graphics interchange format 
   IMAGE_FILE_TIFF,     // tagged image file format
   IMAGE_FILE_BMP,      // Microsoft bitmap format
   IMAGE_FILE_WEBP,     // Google WebP format, a type of .riff file
   IMAGE_FILE_ICO,      // Microsoft icon format
   IMAGE_FILE_INVALID,  // unidentified image types.
};

ImageFileType getImageTypeByMagic( const u8* data, u32 len )
{
   if ( len < 16 ) return IMAGE_FILE_INVALID;
 
   // .jpg:  FF D8 FF
   // .png:  89 50 4E 47 0D 0A 1A 0A
   // .gif:  GIF87a      
   //        GIF89a
   // .tiff: 49 49 2A 00
   //        4D 4D 00 2A
   // .bmp:  BM 
   // .webp: RIFF ???? WEBP 
   // .ico   00 00 01 00
   //        00 00 02 00 ( cursor files )
 
   switch ( data[0] )
   {
      case (u8)'\xFF':
         return ( !strncmp( (const char*)data, "\xFF\xD8\xFF", 3 )) ? 
            IMAGE_FILE_JPG : IMAGE_FILE_INVALID;
 
      case (u8)'\x89':
         return ( !strncmp( (const char*)data, 
                            "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A", 8 )) ?
            IMAGE_FILE_PNG : IMAGE_FILE_INVALID;
 
      case 'G':
         return ( !strncmp( (const char*)data, "GIF87a", 6 ) || 
                  !strncmp( (const char*)data, "GIF89a", 6 ) ) ? 
            IMAGE_FILE_GIF : IMAGE_FILE_INVALID;
 
      case 'I':
         return ( !strncmp( (const char*)data, "\x49\x49\x2A\x00", 4 )) ? 
            IMAGE_FILE_TIFF : IMAGE_FILE_INVALID;
 
      case 'M':
         return ( !strncmp( (const char*)data, "\x4D\x4D\x00\x2A", 4 )) ? 
             IMAGE_FILE_TIFF : IMAGE_FILE_INVALID;
 
      case 'B':
         return (( data[1] == 'M' )) ? 
             IMAGE_FILE_BMP : IMAGE_FILE_INVALID;
 
      case 'R':
         if ( strncmp( (const char*)data,     "RIFF", 4 )) 
            return IMAGE_FILE_INVALID;
         if ( strncmp( (const char*)(data+8), "WEBP", 4 )) 
            return IMAGE_FILE_INVALID;
         return IMAGE_FILE_WEBP;
 
      case '\0':
         if ( !strncmp( (const char*)data, "\x00\x00\x01\x00", 4 )) 
            return IMAGE_FILE_ICO;
         if ( !strncmp( (const char*)data, "\x00\x00\x02\x00", 4 )) 
            return IMAGE_FILE_ICO;
         return IMAGE_FILE_INVALID;
 
      default:
         return IMAGE_FILE_INVALID;
   }
}
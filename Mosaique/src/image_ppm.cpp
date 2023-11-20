#include "image_ppm.h"

void ignorer_commentaires(FILE * f)
{
  unsigned char c;
  while((c=fgetc(f)) == '#')
    while((c=fgetc(f)) != '\n');
  fseek(f, -sizeof(unsigned char), SEEK_CUR);
}

OCTET* load_pgm( std::string path, int* dimX, int* dimY )
{
    FILE* f_image;
    int max_grey_val, taille_image;
    OCTET* Img;

    if( ( f_image = fopen( path.c_str(), "rb" ) ) == NULL )
    {
        printf("\nPas d'acces en lecture sur l'image %s \n", path.c_str() );
        return NULL;
    }
    else
    {
        fscanf( f_image, "P5 " );
        ignorer_commentaires( f_image );
        fscanf( f_image, "%d %d %d%*c", dimX, dimY, &max_grey_val );

        taille_image = (*dimX) * (*dimY);

        try_malloc( Img, OCTET, taille_image, NULL );

        if( ( fread( (OCTET*) Img, sizeof(OCTET), taille_image, f_image ) ) != (size_t) taille_image )
        {
            printf("\nErreur de lecture de l'image %s \n", path.c_str() );
            return NULL;
        }

        fclose(f_image);
    }

    return Img;
}

OCTET* load_ppm( std::string path, int* dimX, int* dimY )
{
    FILE* f_image;
    int max_grey_val, taille_image;
    OCTET* Img;

    if( ( f_image = fopen( path.c_str(), "rb" ) ) == NULL )
    {
        printf("\nPas d'acces en lecture sur l'image %s \n", path.c_str() );
        return NULL;
    }
    else
    {
        fscanf( f_image, "P6 " );
        ignorer_commentaires( f_image );
        fscanf( f_image, "%d %d %d%*c", dimX, dimY, &max_grey_val );

        taille_image = (*dimX) * (*dimY) * 3;

        try_malloc( Img, OCTET, taille_image, NULL );

        if( ( fread( (OCTET*) Img, sizeof(OCTET), taille_image, f_image ) ) != (size_t) taille_image )
        {
            printf("\nErreur de lecture de l'image %s \n", path.c_str() );
            return NULL;
        }

        fclose(f_image);
    }

    return Img;
}

OCTET* load_Img( std::string path, int* dimX, int* dimY )
{
   if( endsWith( path, ".pgm" ) )
      return load_pgm( path, dimX, dimY );

   if( endsWith( path, ".ppm" ) )
      return load_ppm( path, dimX, dimY );

   return NULL;
}

int write_pgm( OCTET* ImgOut, std::string ImgOutStr, int dimX, int dimY )
{
   FILE* f_image;
   int taille_image = dimX * dimY;

   if( ( f_image = fopen( ImgOutStr.c_str(), "wb" ) ) == NULL )
   {
      printf("\nPas d'acces en ecriture sur l'image %s \n", ImgOutStr.c_str() );
      return 1;
   }
   else
   {
      fprintf(f_image,"P5\r") ;                               
      fprintf(f_image,"%d %d\r255\r", dimX, dimY ) ;

      if( ( fwrite( (OCTET*) ImgOut, sizeof(OCTET), taille_image, f_image ) ) != (size_t) taille_image )
      {
         printf("\nErreur de l'ecriture de l'image %s \n", ImgOutStr.c_str() );
         return 1;
      }

      fclose(f_image);
   }

   return 0;
}

int write_ppm( OCTET* ImgOut, std::string ImgOutStr, int dimX, int dimY )
{
   FILE* f_image;
   int taille_image = dimX * dimY * 3;

   if( ( f_image = fopen( ImgOutStr.c_str(), "wb" ) ) == NULL )
   {
      printf("\nPas d'acces en ecriture sur l'image %s \n", ImgOutStr.c_str() );
      return 1;
   }
   else
   {
      fprintf(f_image,"P6\r") ;                              
      fprintf(f_image,"%d %d\r255\r", dimX, dimY ) ;

      if( ( fwrite( (OCTET*) ImgOut, sizeof(OCTET), taille_image, f_image ) ) != (size_t) taille_image )
      {
         printf("\nErreur de l'ecriture de l'image %s \n", ImgOutStr.c_str() );
         return 1;
      }

      fclose(f_image);
   }

   return 0;
}

int write_Img( OCTET* ImgOut, std::string ImgOutStr, int dimX, int dimY )
{
   if( endsWith( ImgOutStr, ".pgm" ) )
      return write_pgm( ImgOut, ImgOutStr, dimX, dimY );

   if( endsWith( ImgOutStr, ".ppm" ) )
      return write_ppm( ImgOut, ImgOutStr, dimX, dimY );

   return 1;
}

bool endsWith(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}
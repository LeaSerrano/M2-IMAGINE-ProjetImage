#include "image_ppm.h"
#include "DataBase_Grey.h"

class mosaique_Grey
{
    OCTET* ImgIn;
    std::string ImgInStr;

    OCTET* ImgOut; 
    std::string ImgOutStr;

    int dimImgX;
    int dimImgY;

    int dimX;
    int dimY;

    public : 

    mosaique_Grey( char* ImgInStr, char* ImgOutStr, int dimX, int dimY ) 
        : ImgIn( NULL ), ImgInStr( std::string( ImgInStr ) ), ImgOut( NULL ), ImgOutStr( std::string( ImgOutStr ) ), 
          dimImgX( -1 ), dimImgY( -1 ), dimX( dimX ), dimY( dimY ) {}
    
    mosaique_Grey( std::string ImgInStr, std::string ImgOutStr, int dimX, int dimY ) 
        : ImgIn( NULL ), ImgInStr( ImgInStr ),  ImgOut( NULL ), ImgOutStr( ImgOutStr ), 
          dimImgX( -1 ), dimImgY( -1 ), dimX( dimX ), dimY( dimY ) {}
    
    ~mosaique_Grey()
    {
        if( ImgIn )
            free( ImgIn );

        if( ImgOut )
            free( ImgOut );  
    }

    OCTET* get_ImgIn();
    OCTET* get_ImgOut();

    int set_ImgInStr( std::string ImgStr );
    int set_ImgInStr( char* ImgStr );
    int set_ImgOutStr( std::string ImgStr );
    int set_ImgOutStr( char* ImgStr );

    int get_dimImgX();
    int get_dimImgY();

    int get_dimX();
    int get_dimY();
    
    int set_dimX( int dim );
    int set_dimY( int dim );

    int load_ImgIn();
    int write_ImgOut();

    void copy_In( OCTET* Img );
    void copy_Out( OCTET* Img );
    void copy_In2Out();
    void copy_Out2In();

    double PSNR( OCTET* Img1, OCTET* Img2, int dimImgX, int dimImgY );
    int replace_block_PSNR( OCTET* blockIn, DataBase_Grey& DB );
    int generate_ImgOut_PSNR( DataBase_Grey& DB );

    int generate_ImgOut_mean( DataBase_Grey& DB );

    int generate_ImgOut_variance( DataBase_Grey& DB );

    int copy_used_imagettes( std::string path, std::string format, DataBase_Grey& DB );
};
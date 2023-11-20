#include "image_ppm.h"

class DataBase_Color 
{
    std::string pathToDB;

    std::vector<OCTET*> Imgs;

    int dimX; 
    int dimY;

    public :

    DataBase_Color( std::string pathToDB, int dimX, int dimY )
        : pathToDB( pathToDB ), dimX( dimX ), dimY( dimY ) {}
    
    DataBase_Color( char* pathToDB, int dimX, int dimY )
        : pathToDB( std::string(pathToDB) ), dimX( dimX ), dimY( dimY ) {}

    ~DataBase_Color()
    {
        for( unsigned long Img = 0; Img < Imgs.size(); Img++ )
            free( Imgs[Img] );
    }

    unsigned int get_Imgs_length();
    OCTET* get_Img( int idx );

    int get_dimX();
    int get_dimY();
    
    int set_dimX( int dim );
    int set_dimY( int dim );

    OCTET* redim_Img( OCTET* Img, int dimImgX, int dimImgY );

    int load_DataBase_Color();

    int compute_imagettes_mean( std::vector<double>& ImgsMean );
    int compute_imagettes_variance( std::vector<double>& ImgsMean, std::vector<double>& ImgsVariance );
};
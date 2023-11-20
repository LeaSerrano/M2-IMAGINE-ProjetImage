#include "DataBase_Grey.h"
#include <filesystem>
namespace fs = std::filesystem;

unsigned int DataBase_Grey::get_Imgs_length() { return Imgs.size(); }

OCTET* DataBase_Grey::get_Img( int idx ) { return ( idx < 0 || ( unsigned int ) idx >= get_Imgs_length() )?(NULL):(Imgs[idx]); }

int DataBase_Grey::get_dimX() { return dimX; }
int DataBase_Grey::get_dimY() { return dimY; }

int DataBase_Grey::set_dimX( int dim ) 
{
    if( dim < 2 )
        return 1;

    dimX = dim;

    return 0;
}

int DataBase_Grey::set_dimY( int dim ) 
{
    if( dim < 2 )
        return 1;

    dimY = dim;

    return 0;
}

OCTET* DataBase_Grey::redim_Img( OCTET* Img, int dimImgX, int dimImgY )
{
    if( !Img )
        return NULL;

    OCTET* newImg;
    try_malloc( newImg, OCTET, dimImgX*dimImgY, NULL );

    for( int y = 0; y < dimY; y++ )
        for( int x = 0; x < dimX; x++ )
        {
            double percBegX = (float)x/(float)dimX;
            double percEndX = (float)(x+1)/(float)dimX;
            double percBegY = (float)y/(float)dimY;
            double percEndY = (float)(y+1)/(float)dimY;

            int xBeg = floor(percBegX*dimImgX);
            int xEnd = ceil(percEndX*dimImgX);
            int yBeg = floor(percBegY*dimImgY);
            int yEnd = ceil(percEndY*dimImgY);

            double mean = 0.;

            for( int ImgX = xBeg; ImgX < xEnd; ImgX++ )
                for( int ImgY = yBeg; ImgY < yEnd; ImgY++ )
                    mean += (double) Img[ ImgY * dimImgX + ImgX ];
            
            mean = mean / (double) ( ( xEnd - xBeg + 1 ) * ( yEnd - yBeg + 1) );

            newImg[ y * dimX + x ] = mean;
        }

    free( Img );
    return newImg;
}   

int DataBase_Grey::load_DataBase_Grey()
{
    int dimImgX, dimImgY;
    OCTET* newImg;

    for( const auto& entry : fs::directory_iterator( pathToDB ) )
    {
        std::string path( entry.path().u8string() );

        if( endsWith( path, ".pgm" ) )
        {
            newImg = redim_Img( load_Img( path, &dimImgX, &dimImgY ), dimImgX, dimImgY );

            if( !newImg )
                std::cout << "Failed to load " << path << "\n";
            else
                Imgs.push_back( newImg );
        }
        else if( endsWith( path, ".ppm" ) )
        {
            OCTET* imagetteColor = load_Img( path, &dimImgX, &dimImgY );

            if( !imagetteColor )
            {
                std::cout << "Failed to load " << path << "\n";
                continue;
            }

            int size = dimImgX * dimImgY;

            OCTET* imagetteGrey = (OCTET*) malloc( sizeof(OCTET) * size );

            if( !imagetteGrey )
            {
                std::cout << "Allocation Error !\n";
                continue;
            } 

            int pixel = 0;

            for( ;pixel < size; pixel += 4 )
            {
                imagetteGrey[pixel] = round( 0.299 * (double) imagetteColor[pixel*3] 
                                           + 0.587 * (double) imagetteColor[pixel*3+1] 
                                           + 0.114 * (double) imagetteColor[pixel*3+2] );

                imagetteGrey[pixel+1] = round( 0.299 * (double) imagetteColor[(pixel+1)*3] 
                                             + 0.587 * (double) imagetteColor[(pixel+1)*3+1] 
                                             + 0.114 * (double) imagetteColor[(pixel+1)*3+2] );

                imagetteGrey[pixel+2] = round( 0.299 * (double) imagetteColor[(pixel+2)*3] 
                                             + 0.587 * (double) imagetteColor[(pixel+2)*3+1] 
                                             + 0.114 * (double) imagetteColor[(pixel+2)*3+2] );

                imagetteGrey[pixel+3] = round( 0.299 * (double) imagetteColor[(pixel+3)*3] 
                                             + 0.587 * (double) imagetteColor[(pixel+3)*3+1] 
                                             + 0.114 * (double) imagetteColor[(pixel+3)*3+2] );
            }

            for( ; pixel < size; pixel++ )
                imagetteGrey[pixel] = round( 0.299 * (double) imagetteColor[pixel*3] 
                                           + 0.587 * (double) imagetteColor[pixel*3+1] 
                                           + 0.114 * (double) imagetteColor[pixel*3+2] );
                
            free( imagetteColor );

            OCTET* newImg = redim_Img( imagetteGrey, dimImgX, dimImgY );

            if( !newImg )
                std::cout << "Failed to load " << path << "\n";
            else
                Imgs.push_back( newImg );
        }
        else
        {
            CImg<float> img;
            img.load( path.c_str() );

            if( img.is_empty() )    
            {
                std::cout << "Failed to load " << path << "\n";
                continue;
            }   

            dimImgX = img.width();
            dimImgY = img.height();

            int spectrum = img.spectrum();
            int size = img.size()/spectrum;
            float* data = img.data();

            OCTET* imagette = (OCTET*) malloc( sizeof(OCTET) * (img.size()/spectrum) );

            if( !imagette )
            {
                std::cout << "Allocation Error !\n";
                continue;
            } 

            int pixel = 0;

            if( spectrum == 1 )
            {
                for( ; pixel < size; pixel += 8 ) 
                {
                    imagette[pixel  ] = data[pixel  ];
                    imagette[pixel+1] = data[pixel+1];
                    imagette[pixel+2] = data[pixel+2];
                    imagette[pixel+3] = data[pixel+3];
                    imagette[pixel+4] = data[pixel+4];
                    imagette[pixel+5] = data[pixel+5];
                    imagette[pixel+6] = data[pixel+6];
                    imagette[pixel+7] = data[pixel+7];
                }

                for( ; pixel < size; pixel++ ) 
                    imagette[pixel] = data[pixel];
            }
            else if( spectrum == 3 )
            {
                for( ; pixel < size; pixel += 4 ) 
                {
                    imagette[pixel] = round( 0.299 * data[pixel] + 0.587 * data[pixel+size] + 0.114 * data[pixel+2*size] );
                    imagette[pixel+1] = round( 0.299 * data[pixel+1] + 0.587 * data[pixel+1+size] + 0.114 * data[pixel+1+2*size] );
                    imagette[pixel+2] = round( 0.299 * data[pixel+2] + 0.587 * data[pixel+2+size] + 0.114 * data[pixel+2+2*size] );
                    imagette[pixel+3] = round( 0.299 * data[pixel+3] + 0.587 * data[pixel+3+size] + 0.114 * data[pixel+3+2*size] );
                }

                for( ; pixel < size; pixel++ ) 
                    imagette[pixel] = round( 0.299 * data[pixel] + 0.587 * data[pixel+size] + 0.114 * data[pixel+2*size] );
            }

            img.clear();

            newImg = redim_Img( imagette, dimImgX, dimImgY );

            if( !newImg )
                std::cout << "Failed to load " << path << "\n";
            else
                Imgs.push_back( newImg );
        }

        //std::cout << path << std::endl;
    }

    return 0;
}

int DataBase_Grey::compute_imagettes_mean( std::vector<double>& ImgsMean )
{
    int taille_imagette = dimX * dimY;
    unsigned int nb_Imgs = get_Imgs_length();
    ImgsMean.resize( nb_Imgs );

    for( unsigned int Img_idx = 0; Img_idx < nb_Imgs; Img_idx++ )
    {
        OCTET* Img = get_Img( Img_idx );
        int pixel = 0;
        double mean = 0.;

        for( ; pixel < taille_imagette; pixel += 8 )
        {
            int pixel01 = Img[pixel  ] + Img[pixel+1];
            int pixel23 = Img[pixel+2] + Img[pixel+3];
            int pixel45 = Img[pixel+4] + Img[pixel+5];
            int pixel67 = Img[pixel+6] + Img[pixel+7];

            pixel01 += pixel23;
            pixel45 += pixel67;

            pixel01 += pixel45;

            mean += (double) pixel01;
        }

        for( ; pixel < taille_imagette; pixel++ )
            mean += (double) Img[pixel];

        mean /= (double) taille_imagette;

        ImgsMean[Img_idx] = mean;
    }

    return 0;
}

int DataBase_Grey::compute_imagettes_variance( std::vector<double>& ImgsMean, std::vector<double>& ImgsVariance )
{
    int taille_imagette = dimX * dimY;
    unsigned int nb_Imgs = get_Imgs_length();
    ImgsVariance.resize( nb_Imgs );

    compute_imagettes_mean( ImgsMean );

    for( unsigned int Img_idx = 0; Img_idx < nb_Imgs; Img_idx++ )
    {
        OCTET* Img = get_Img( Img_idx );
        int pixel = 0;

        double mean = ImgsMean[Img_idx];
        double variance = 0.;

        for( ; pixel < taille_imagette; pixel += 8 )
        {
            double pixel0 = (double) Img[pixel  ] - mean;
            double pixel1 = (double) Img[pixel+1] - mean;
            double pixel2 = (double) Img[pixel+2] - mean;
            double pixel3 = (double) Img[pixel+3] - mean;
            double pixel4 = (double) Img[pixel+4] - mean;
            double pixel5 = (double) Img[pixel+5] - mean;
            double pixel6 = (double) Img[pixel+6] - mean;
            double pixel7 = (double) Img[pixel+7] - mean;

            pixel0 *= pixel0;
            pixel1 *= pixel1;
            pixel2 *= pixel2;
            pixel3 *= pixel3;
            pixel4 *= pixel4;
            pixel5 *= pixel5;
            pixel6 *= pixel6;
            pixel7 *= pixel7;

            pixel0 += pixel1;
            pixel2 += pixel3;
            pixel4 += pixel5;
            pixel6 += pixel7;
            
            pixel0 += pixel2;
            pixel4 += pixel6;

            pixel0 += pixel4;

            variance += pixel0;
        }

        for( ; pixel < taille_imagette; pixel++ )
            variance += pow(Img[pixel] - mean, 2.);
        
        variance /= (double) taille_imagette;

        ImgsVariance[Img_idx] = variance;
    }

    return 0;
}
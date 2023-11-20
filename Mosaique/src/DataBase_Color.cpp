#include "DataBase_Color.h"
#include <filesystem>
namespace fs = std::filesystem;

unsigned int DataBase_Color::get_Imgs_length() { return Imgs.size(); }

OCTET* DataBase_Color::get_Img( int idx ) { return ( idx < 0 || ( unsigned int ) idx >= get_Imgs_length() )?(NULL):(Imgs[idx]); }

int DataBase_Color::get_dimX() { return dimX; }
int DataBase_Color::get_dimY() { return dimY; }

int DataBase_Color::set_dimX( int dim ) 
{
    if( dim < 2 )
        return 1;

    dimX = dim;

    return 0;
}

int DataBase_Color::set_dimY( int dim ) 
{
    if( dim < 2 )
        return 1;

    dimY = dim;

    return 0;
}


OCTET* DataBase_Color::redim_Img( OCTET* Img, int dimImgX, int dimImgY )
{
    if( dimX == dimImgX && dimY == dimImgY )
        return Img;
    
    if( !Img )
        return NULL;

    OCTET* newImg;
    try_malloc( newImg, OCTET, dimImgX*dimImgY*3, NULL );

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

            double mean[3];
            mean[0] = 0.;
            mean[1] = 0.;
            mean[2] = 0.;

            for( int ImgX = xBeg; ImgX < xEnd; ImgX++ )
                for( int ImgY = yBeg; ImgY < yEnd; ImgY++ )
                {
                    mean[0] += (double) Img[ ( ImgY * dimImgX + ImgX ) * 3 ];
                    mean[1] += (double) Img[ ( ImgY * dimImgX + ImgX ) * 3 + 1 ];
                    mean[2] += (double) Img[ ( ImgY * dimImgX + ImgX ) * 3 + 2 ];
                }

            mean[0] /= (double) ( ( xEnd - xBeg  ) * ( yEnd - yBeg) );
            mean[1] /= (double) ( ( xEnd - xBeg ) * ( yEnd - yBeg) );
            mean[2] /= (double) ( ( xEnd - xBeg  ) * ( yEnd - yBeg ) );

            newImg[ ( y * dimX + x ) * 3 ] = mean[0];
            newImg[ ( y * dimX + x ) * 3 + 1 ] = mean[1];
            newImg[ ( y * dimX + x ) * 3 + 2 ] = mean[2];
        }
    
    free( Img );
    return newImg;
}   

int DataBase_Color::load_DataBase_Color()
{
    int dimImgX, dimImgY;
    OCTET* newImg;
    
    for( const auto& entry : fs::directory_iterator( pathToDB ) )
    {
        std::string path( entry.path().u8string() );

        if( endsWith( path, ".ppm" ) )
        {
            newImg = redim_Img( load_Img( path, &dimImgX, &dimImgY ), dimImgX, dimImgY );

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

            OCTET* newImg = (OCTET*) malloc( sizeof(OCTET) * size * 3 );

            if( !newImg )
            {
                std::cout << "Allocation Error !\n";
                continue;
            } 

            // std::cout << img.size() << " & " << img.width() << " & " << img.height() <<  " & " << img.spectrum() << "\n";

            int pixel = 0;

            if( spectrum == 1 )
            {
                for( ; pixel < size; pixel += 3 ) 
                {
                    newImg[pixel*3] = data[pixel];
                    newImg[pixel*3+1] = data[pixel];
                    newImg[pixel*3+2] = data[pixel];

                    newImg[(pixel+1)*3] = data[pixel+1];
                    newImg[(pixel+1)*3+1] = data[pixel+1];
                    newImg[(pixel+1)*3+2] = data[pixel+1];

                    newImg[(pixel+2)*3] = data[pixel+2];
                    newImg[(pixel+2)*3+1] = data[pixel+2];
                    newImg[(pixel+2)*3+2] = data[pixel+2];
                }

                for( ; pixel < size; pixel++ ) 
                {
                    newImg[pixel*3] = data[pixel];
                    newImg[pixel*3+1] = data[pixel];
                    newImg[pixel*3+2] = data[pixel];
                }
            }
            else if( spectrum == 3 )
            {
                for( ; pixel < size; pixel += 3 ) 
                {
                    newImg[pixel*3] = data[pixel];
                    newImg[pixel*3+1] = data[pixel+size];
                    newImg[pixel*3+2] = data[pixel+size*2];

                    newImg[(pixel+1)*3] = data[pixel+1];
                    newImg[(pixel+1)*3+1] = data[pixel+1+size];
                    newImg[(pixel+1)*3+2] = data[pixel+1+size*2];

                    newImg[(pixel+2)*3] = data[pixel+2];
                    newImg[(pixel+2)*3+1] = data[pixel+2+size];
                    newImg[(pixel+2)*3+2] = data[pixel+2+size*2];
                }
                
                for( ; pixel < size; pixel++ ) 
                {
                    newImg[pixel*3] = data[pixel];
                    newImg[pixel*3+1] = data[pixel+size];
                    newImg[pixel*3+2] = data[pixel+size*2];
                }
            }

            newImg = redim_Img( newImg, dimImgX, dimImgY );

            if( !newImg )
                std::cout << "Failed to load " << path << "\n";
            else
                Imgs.push_back( newImg );
        }

        //std::cout << path << std::endl;
    }

    return 0;
}

int DataBase_Color::compute_imagettes_mean( std::vector<double>& ImgsMean )
{
    int taille_imagette = dimX * dimY;
    unsigned int nb_Imgs = get_Imgs_length();
    ImgsMean.resize( nb_Imgs * 3 );

    for( unsigned int Img_idx = 0; Img_idx < nb_Imgs; Img_idx++ )
    {
        OCTET* Img = get_Img( Img_idx );
        int pixel = 0;

        double mean[3];
        mean[0] = 0.;
        mean[1] = 0.;
        mean[2] = 0.;

        for( ; pixel < taille_imagette*3; pixel += 12 )
        {
            double meanR1 = (double) ( Img[ pixel ] + Img[ pixel + 3 ] );
            double meanG1 = (double) ( Img[ pixel + 1 ] + Img[ pixel + 4 ] );
            double meanB1 = (double) ( Img[ pixel + 2 ] + Img[ pixel + 5 ] );

            double meanR2 = (double) ( Img[ pixel + 6 ] + Img[ pixel + 9 ] );
            double meanG2 = (double) ( Img[ pixel + 7 ] + Img[ pixel + 10 ] );
            double meanB2 = (double) ( Img[ pixel + 8 ] + Img[ pixel + 11 ] );

            meanR1 += meanR2;
            meanG1 += meanG2;
            meanB1 += meanB2;

            mean[0] += meanR1;
            mean[1] += meanG1;
            mean[2] += meanB1;
        }

        for( ; pixel < taille_imagette*3; pixel += 3 )
        {
            mean[0] += (double) Img[ pixel ];
            mean[1] += (double) Img[ pixel + 1 ];
            mean[2] += (double) Img[ pixel + 2 ];
        }

        mean[0] /= (double) taille_imagette;
        mean[1] /= (double) taille_imagette;
        mean[2] /= (double) taille_imagette;

        ImgsMean[Img_idx*3] = mean[0];
        ImgsMean[Img_idx*3+1] = mean[1];
        ImgsMean[Img_idx*3+2] = mean[2];
    }

    return 0;
}

int DataBase_Color::compute_imagettes_variance( std::vector<double>& ImgsMean, std::vector<double>& ImgsVariance )
{
    int taille_imagette = dimX * dimY;
    unsigned int nb_Imgs = get_Imgs_length();
    ImgsVariance.resize( nb_Imgs * 3 );

    compute_imagettes_mean( ImgsMean );

    for( unsigned int Img_idx = 0; Img_idx < nb_Imgs; Img_idx++ )
    {
        OCTET* Img = get_Img( Img_idx );
        int pixel = 0;

        double meanR = ImgsMean[Img_idx*3];
        double meanG = ImgsMean[Img_idx*3+1];
        double meanB = ImgsMean[Img_idx*3+2];
        
        double variance[3];
        variance[0] = 0.;
        variance[1] = 0.;
        variance[2] = 0.;

        for( ; pixel < taille_imagette*3; pixel += 12 )
        {
            double variance1R = (double) Img[pixel  ] - meanR;
            double variance1G = (double) Img[pixel+1] - meanG;
            double variance1B = (double) Img[pixel+2] - meanB;
            double variance2R = (double) Img[pixel+3] - meanR;
            double variance2G = (double) Img[pixel+4] - meanG;
            double variance2B = (double) Img[pixel+5] - meanB;
            double variance3R = (double) Img[pixel+6] - meanR;
            double variance3G = (double) Img[pixel+7] - meanG;
            double variance3B = (double) Img[pixel+8] - meanB;
            double variance4R = (double) Img[pixel+9] - meanR;
            double variance4G = (double) Img[pixel+10] - meanG;
            double variance4B = (double) Img[pixel+11] - meanB;

            variance1R *= variance1R;
            variance1G *= variance1G;
            variance1B *= variance1B;
            variance2R *= variance2R;
            variance2G *= variance2G;
            variance2B *= variance2B;
            variance3R *= variance3R;
            variance3G *= variance3G;
            variance3B *= variance3B;
            variance4R *= variance4R;
            variance4G *= variance4G;
            variance4B *= variance4B;

            variance1R += variance2R;
            variance1G += variance2G;
            variance1B += variance2B;

            variance3R += variance4R;
            variance3G += variance4G;
            variance3B += variance4B;

            variance1R += variance3R;
            variance1G += variance3G;
            variance1B += variance3B;

            variance[0] += variance1R;
            variance[1] += variance1G;
            variance[2] += variance1B;
        }

        for( ; pixel < taille_imagette*3; pixel += 3 )
        {
            variance[0] += pow(Img[pixel] - meanR, 2.);
            variance[1] += pow(Img[pixel+1] - meanG, 2.);
            variance[2] += pow(Img[pixel+2] - meanB, 2.);
        }
        
        variance[0] /= (double) taille_imagette;
        variance[1] /= (double) taille_imagette;
        variance[2] /= (double) taille_imagette;

        ImgsVariance[Img_idx*3] = variance[0];
        ImgsVariance[Img_idx*3+1] = variance[1];
        ImgsVariance[Img_idx*3+2] = variance[2];
    }

    return 0;
}
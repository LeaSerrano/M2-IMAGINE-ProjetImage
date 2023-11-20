#include "mosaique_Grey.h"

OCTET* mosaique_Grey::get_ImgIn() { return ImgIn; }
OCTET* mosaique_Grey::get_ImgOut() { return ImgOut; }

int mosaique_Grey::set_ImgInStr( std::string ImgStr )
{
    OCTET* OldImgIn = ImgIn;
    std::string OldImgInStr( ImgInStr );

    ImgInStr = ImgStr;

    if( load_ImgIn() )
    {
        ImgIn = OldImgIn;
        ImgInStr = OldImgInStr;
        load_ImgIn();
        return 1;
    }
    
    free( ImgIn );

    return 0;
}

int mosaique_Grey::set_ImgInStr( char* ImgStr )
{
    if( !ImgStr )
        return 1;

    OCTET* OldImgIn = ImgIn;
    std::string OldImgInStr( ImgInStr );

    ImgInStr = ImgStr;

    if( load_ImgIn() )
    {
        ImgIn = OldImgIn;
        ImgInStr = OldImgInStr;
        load_ImgIn();
        return 1;
    }
    
    free( ImgIn );

    return 0;
}

int mosaique_Grey::set_ImgOutStr( std::string ImgStr ) 
{ 
    ImgOutStr = ImgStr; 
    return 0; 
}

int mosaique_Grey::set_ImgOutStr( char* ImgStr )
{ 
    if( !ImgStr )
        return 1;
    
    ImgOutStr = std::string(ImgStr); 
    return 0; 
}

int mosaique_Grey::get_dimImgX() { return dimImgX; }
int mosaique_Grey::get_dimImgY() { return dimImgY; }

int mosaique_Grey::get_dimX() { return dimX; }
int mosaique_Grey::get_dimY() { return dimY; }

int mosaique_Grey::set_dimX( int dim ) 
{
    if( dim < 2 )
        return 1;

    if( ImgIn && dim%dimImgX != 0 )
        return 1;

    dimX = dim;

    return 0;
}

int mosaique_Grey::set_dimY( int dim ) 
{
    if( dim < 2 )
        return 1;

    if( ImgIn && dim%dimImgY != 0 )
        return 1;

    dimY = dim;

    return 0;
}

int mosaique_Grey::load_ImgIn()
{
    if( ImgInStr.empty() )
        Error("Aucun chemin pour l'image d'entrée n'est renseigné", 1 );

    if( endsWith( ImgInStr, ".pgm") )
    {
        ImgIn = load_Img( ImgInStr, &dimImgX, &dimImgY );

        if( !ImgIn )
            Error("L'image pgm n'a pas pu être créée", 1 );
    }
    else 
    {
        CImg<float> img;
        img.load( ImgInStr.c_str() );

        if( img.is_empty() )    
            Error("L'image CImg n'a pas pu être créée", 1 );      

        dimImgX = img.width();
        dimImgY = img.height();

        int spectrum = img.spectrum();
        int size = img.size()/spectrum;
        float* data = img.data();

        try_malloc( ImgIn, OCTET, (int) (img.size()/spectrum), 1 );

        if( !ImgIn )
            Error("L'image d'entrée n'a pas pu être créée", 1 );

        int pixel = 0;

        if( spectrum == 1 )
        {
            for( ; pixel < size; pixel += 8 ) 
                {
                    ImgIn[pixel  ] = data[pixel  ];
                    ImgIn[pixel+1] = data[pixel+1];
                    ImgIn[pixel+2] = data[pixel+2];
                    ImgIn[pixel+3] = data[pixel+3];
                    ImgIn[pixel+4] = data[pixel+4];
                    ImgIn[pixel+5] = data[pixel+5];
                    ImgIn[pixel+6] = data[pixel+6];
                    ImgIn[pixel+7] = data[pixel+7];
                }

                for( ; pixel < size; pixel++ ) 
                    ImgIn[pixel] = data[pixel];
        }
        else if( spectrum == 3 )
        {
            for( ; pixel < size; pixel += 4 ) 
                {
                    ImgIn[pixel] = round( 0.299 * data[pixel] + 0.587 * data[pixel+size] + 0.114 * data[pixel+2*size] );
                    ImgIn[pixel+1] = round( 0.299 * data[pixel+1] + 0.587 * data[pixel+1+size] + 0.114 * data[pixel+1+2*size] );
                    ImgIn[pixel+2] = round( 0.299 * data[pixel+2] + 0.587 * data[pixel+2+size] + 0.114 * data[pixel+2+2*size] );
                    ImgIn[pixel+3] = round( 0.299 * data[pixel+3] + 0.587 * data[pixel+3+size] + 0.114 * data[pixel+3+2*size] );
                }

                for( ; pixel < size; pixel++ ) 
                    ImgIn[pixel] = round( 0.299 * data[pixel] + 0.587 * data[pixel+size] + 0.114 * data[pixel+2*size] );
        }
    }

    free( ImgOut );

    try_malloc( ImgOut, OCTET, dimImgX * dimImgY, 1 );

    return 0;
}

void mosaique_Grey::copy_In( OCTET* Img )
{
    if( !Img )
        return; 

    int size = dimImgX * dimImgY;
    int pixel = 0;

    for( ; pixel < size; pixel += 16 )
    {
        Img[pixel  ] = ImgIn[pixel  ];
        Img[pixel+1] = ImgIn[pixel+1];
        Img[pixel+2] = ImgIn[pixel+2];
        Img[pixel+3] = ImgIn[pixel+3];
        Img[pixel+4] = ImgIn[pixel+4];
        Img[pixel+5] = ImgIn[pixel+5];
        Img[pixel+6] = ImgIn[pixel+6];
        Img[pixel+7] = ImgIn[pixel+7];
        Img[pixel+8] = ImgIn[pixel+8];
        Img[pixel+9] = ImgIn[pixel+9];
        Img[pixel+10] = ImgIn[pixel+10];
        Img[pixel+11] = ImgIn[pixel+11];
        Img[pixel+12] = ImgIn[pixel+12];
        Img[pixel+13] = ImgIn[pixel+13];
        Img[pixel+14] = ImgIn[pixel+14];
        Img[pixel+15] = ImgIn[pixel+15];
    }
    
    for( ; pixel < size; pixel++ )
        Img[pixel] = ImgIn[pixel];
}

void mosaique_Grey::copy_Out( OCTET* Img )
{
    if( !Img )
        return; 

    int size = dimImgX * dimImgY;
    int pixel = 0;

    for( ; pixel < size; pixel += 16 )
    {
        Img[pixel  ] = ImgOut[pixel  ];
        Img[pixel+1] = ImgOut[pixel+1];
        Img[pixel+2] = ImgOut[pixel+2];
        Img[pixel+3] = ImgOut[pixel+3];
        Img[pixel+4] = ImgOut[pixel+4];
        Img[pixel+5] = ImgOut[pixel+5];
        Img[pixel+6] = ImgOut[pixel+6];
        Img[pixel+7] = ImgOut[pixel+7];
        Img[pixel+8] = ImgOut[pixel+8];
        Img[pixel+9] = ImgOut[pixel+9];
        Img[pixel+10] = ImgOut[pixel+10];
        Img[pixel+11] = ImgOut[pixel+11];
        Img[pixel+12] = ImgOut[pixel+12];
        Img[pixel+13] = ImgOut[pixel+13];
        Img[pixel+14] = ImgOut[pixel+14];
        Img[pixel+15] = ImgOut[pixel+15];
    }
    
    for( ; pixel < size; pixel++ )
        Img[pixel] = ImgOut[pixel];
}

void mosaique_Grey::copy_In2Out() { copy_In( ImgOut ); }


void mosaique_Grey::copy_Out2In() { copy_Out( ImgIn ); }


int mosaique_Grey::write_ImgOut()
{
    if( !ImgOut )
        return 1;

    if( endsWith( ImgOutStr, ".pgm" ) )
        return write_Img( ImgOut, ImgOutStr, dimImgX, dimImgY );
    else 
    {
        CImg<OCTET> img( ImgOut, dimImgX, dimImgY, 1, 1);
        
        img.save( ImgOutStr.c_str() );
    }

    return 0;
}

double mosaique_Grey::PSNR( OCTET* Img1, OCTET* Img2, int dimImgX, int dimImgY )
{
    double EQM = 0.;
    int taille_image = dimImgX * dimImgY;
    int pixel = 0;
	
    for( ; pixel < taille_image; pixel += 8 )
    {
        int pixel0 = Img1[pixel  ] - Img2[pixel  ];
        int pixel1 = Img1[pixel+1] - Img2[pixel+1];
        int pixel2 = Img1[pixel+2] - Img2[pixel+2];
        int pixel3 = Img1[pixel+3] - Img2[pixel+3];
        int pixel4 = Img1[pixel+4] - Img2[pixel+4];
        int pixel5 = Img1[pixel+5] - Img2[pixel+5];
        int pixel6 = Img1[pixel+6] - Img2[pixel+6];
        int pixel7 = Img1[pixel+7] - Img2[pixel+7];

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

        EQM += ( double ) pixel0;
    }

    for( ; pixel < taille_image; pixel++ )
        EQM += pow(Img1[pixel]- Img2[pixel], 2.);

    EQM /= (double) taille_image;

    double PSNR = 10. * log10( pow(255., 2.)/EQM ) ;

    return PSNR;
}

int mosaique_Grey::replace_block_PSNR( OCTET* blockIn, DataBase_Grey& DB )
{
    double better_PSNR = PSNR( blockIn, DB.get_Img(0), dimX, dimY );
    int best_Img = 0;

    unsigned int nb_Imgs = DB.get_Imgs_length();

    for( unsigned int Img = 1; Img < nb_Imgs; Img++ )
    {
        double new_PSNR = PSNR( blockIn, DB.get_Img(Img), dimX, dimY );

        if( new_PSNR > better_PSNR )
        {
            better_PSNR = new_PSNR;
            best_Img = Img;
        }
    }

    //std::cout << best_Img <<  " & " << better_PSNR << " & " << nb_Imgs <<"\n";

    return best_Img;
}

int mosaique_Grey::generate_ImgOut_PSNR( DataBase_Grey& DB )
{
    OCTET* blockIn;
    try_malloc( blockIn, OCTET, dimX * dimY, 1 );

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                    blockIn[ blockInY * dimX + blockInX ] = ImgIn[ ( blockInY + y ) * dimImgX + ( blockInX + x ) ];

            int best_Img_idx = replace_block_PSNR( blockIn, DB );

            OCTET* best_Img = DB.get_Img( best_Img_idx );

            for( int best_ImgY = 0; best_ImgY < dimY; best_ImgY++ )
                for( int best_ImgX = 0; best_ImgX < dimX; best_ImgX++ )
                    ImgOut[ ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ] = best_Img[ best_ImgY * dimX + best_ImgX ];
        }
    
    free( blockIn );
    return 0;
}

int mosaique_Grey::generate_ImgOut_mean( DataBase_Grey& DB )
{ 
    int taille_imagette = dimX * dimY;

    std::vector<double> ImgsMean;
    unsigned int nb_Imgs = DB.get_Imgs_length();
    
    DB.compute_imagettes_mean( ImgsMean );

    double blockMean;
    double bestMeanDiff;
    int best_Img_idx;

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            blockMean = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                    blockMean += (double) ImgIn[ ( blockInY + y ) * dimImgX + ( blockInX + x ) ];
            
            blockMean /= (double) taille_imagette;

            bestMeanDiff = abs( blockMean - ImgsMean[0] );
            best_Img_idx = 0;

            for( unsigned int Img = 1; Img < nb_Imgs; Img++ ) // To Peel twice
            {
                double newDiff = abs( blockMean - ImgsMean[Img] );

                if( bestMeanDiff > newDiff )
                {
                    bestMeanDiff = newDiff;
                    best_Img_idx = Img;
                }
            }

            OCTET* best_Img = DB.get_Img( best_Img_idx );

            for( int best_ImgY = 0; best_ImgY < dimY; best_ImgY++ )
                for( int best_ImgX = 0; best_ImgX < dimX; best_ImgX++ )
                    ImgOut[ ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ] = best_Img[ best_ImgY * dimX + best_ImgX ];
        }
    
    return 0;
}

int mosaique_Grey::generate_ImgOut_variance( DataBase_Grey& DB )
{ 
    int taille_imagette = dimX * dimY;

    std::vector<double> ImgsMean;
    std::vector<double> ImgsVariance;
    unsigned int nb_Imgs = DB.get_Imgs_length();
    
    DB.compute_imagettes_variance( ImgsMean, ImgsVariance );

    double blockMean;
    double blockVariance;
    double bestVarDiff;
    int best_Img_idx;

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            blockMean = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                    blockMean += (double) ImgIn[ ( blockInY + y ) * dimImgX + ( blockInX + x ) ];
            
            blockMean /= (double) taille_imagette;

            blockVariance = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                    blockVariance += pow(ImgIn[ ( blockInY + y ) * dimImgX + ( blockInX + x ) ] - blockMean, 2.);
            
            blockVariance /= (double) taille_imagette;

            bestVarDiff = abs( blockMean + ImgsMean[0] ) + abs( blockVariance - ImgsVariance[0] );
            best_Img_idx = 0;

            for( unsigned int Img = 1; Img < nb_Imgs; Img++ ) // To Peel twice
            {
                double newDiff = abs( blockMean + ImgsMean[Img] ) + abs( blockVariance - ImgsVariance[Img] );

                if( bestVarDiff > newDiff )
                {
                    bestVarDiff = newDiff;
                    best_Img_idx = Img;
                }
            }

            OCTET* best_Img = DB.get_Img( best_Img_idx );

            for( int best_ImgY = 0; best_ImgY < dimY; best_ImgY++ )
                for( int best_ImgX = 0; best_ImgX < dimX; best_ImgX++ )
                    ImgOut[ ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ] = best_Img[ best_ImgY * dimX + best_ImgX ];
        }
    
    return 0;
}

int mosaique_Grey::copy_used_imagettes( std::string path, std::string format, DataBase_Grey& DB )
{
    unsigned int nb_Imgs = DB.get_Imgs_length();

    dimImgX = dimX;
    dimImgY = dimY;

    for( unsigned int Img = 0; Img < nb_Imgs; Img++ )
    {
        ImgOut = DB.get_Img( Img );

        ImgOutStr = path + std::to_string(Img) + format;
        
        write_ImgOut();
    }

    return 0;
}
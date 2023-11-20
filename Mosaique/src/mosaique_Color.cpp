#include "mosaique_Color.h"

OCTET* mosaique_Color::get_ImgIn() { return ImgIn; }
OCTET* mosaique_Color::get_ImgOut() { return ImgOut; }

int mosaique_Color::set_ImgInStr( std::string ImgStr )
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

int mosaique_Color::set_ImgInStr( char* ImgStr )
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

int mosaique_Color::set_ImgOutStr( std::string ImgStr ) 
{ 
    ImgOutStr = ImgStr; 
    return 0; 
}

int mosaique_Color::set_ImgOutStr( char* ImgStr )
{ 
    if( !ImgStr )
        return 1;
    
    ImgOutStr = std::string(ImgStr); 
    return 0; 
}

int mosaique_Color::get_dimImgX() { return dimImgX; }
int mosaique_Color::get_dimImgY() { return dimImgY; }

int mosaique_Color::get_dimX() { return dimX; }
int mosaique_Color::get_dimY() { return dimY; }

int mosaique_Color::set_dimX( int dim ) 
{
    if( dim < 2 )
        return 1;

    if( ImgIn && dim%dimImgX != 0 )
        return 1;

    dimX = dim;

    return 0;
}

int mosaique_Color::set_dimY( int dim ) 
{
    if( dim < 2 )
        return 1;

    if( ImgIn && dim%dimImgY != 0 )
        return 1;

    dimY = dim;

    return 0;
}

int mosaique_Color::load_ImgIn()
{
    if( ImgInStr.empty() )
        Error("Aucun chemin pour l'image d'entrée n'est renseigné", 1 );

    if( endsWith( ImgInStr, ".ppm") )
    {
        ImgIn = load_Img( ImgInStr, &dimImgX, &dimImgY );

        if( !ImgIn )
            Error("L'image ppm n'a pas pu être créée", 1 );
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

        try_malloc( ImgIn, OCTET, (int) (size*3), 1 );

        if( !ImgIn )
            Error("L'image d'entrée n'a pas pu être créée", 1 );

        if( spectrum == 1 )
        {
            for( int pixel = 0; pixel < size; pixel++ ) 
                ImgIn[pixel*3] = ImgIn[pixel*3+1] = ImgIn[pixel*3+2] = data[pixel];
        }
        else if( spectrum == 3 )
        {
            for( int pixel = 0; pixel < size; pixel++ ) 
            {
                ImgIn[pixel*3] = data[pixel];
                ImgIn[pixel*3+1] = data[pixel+size];
                ImgIn[pixel*3+2] = data[pixel+size*2];
            }
        }

    }

    free( ImgOut );

    try_malloc( ImgOut, OCTET, dimImgX * dimImgY * 3, 1 );

    return 0;
}

void mosaique_Color::copy_In( OCTET* Img )
{
    if( !Img )
        return; 

    int size = dimImgX * dimImgY * 3;
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

void mosaique_Color::copy_Out( OCTET* Img )
{
    if( !Img )
        return; 

    int size = dimImgX * dimImgY * 3;
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

void mosaique_Color::copy_In2Out() { copy_In( ImgOut ); }


void mosaique_Color::copy_Out2In() { copy_Out( ImgIn ); }


int mosaique_Color::write_ImgOut()
{
    if( !ImgOut )
        return 1;

    if( endsWith( ImgOutStr, ".ppm" ) )
        return write_Img( ImgOut, ImgOutStr, dimImgX, dimImgY );
    else 
    {
        int size = dimImgX * dimImgY;
        OCTET* CImg_img;

        try_malloc(CImg_img, OCTET, size*3, 1 );

        int pixel = 0;

        for( ; pixel < size; pixel += 4 )
        {
            CImg_img[pixel] = ImgOut[pixel*3];
            CImg_img[pixel+size] = ImgOut[pixel*3+1];
            CImg_img[pixel+size*2] = ImgOut[pixel*3+2];

            CImg_img[pixel+1] = ImgOut[(pixel+1)*3];
            CImg_img[pixel+1+size] = ImgOut[(pixel+1)*3+1];
            CImg_img[pixel+1+size*2] = ImgOut[(pixel+1)*3+2];

            CImg_img[pixel+2] = ImgOut[(pixel+2)*3];
            CImg_img[pixel+2+size] = ImgOut[(pixel+2)*3+1];
            CImg_img[pixel+2+size*2] = ImgOut[(pixel+2)*3+2];

            CImg_img[pixel+3] = ImgOut[(pixel+3)*3];
            CImg_img[pixel+3+size] = ImgOut[(pixel+3)*3+1];
            CImg_img[pixel+3+size*2] = ImgOut[(pixel+3)*3+2];
        }

        for( ; pixel < size; pixel++ )
        {
            CImg_img[pixel] = ImgOut[pixel*3];
            CImg_img[pixel+size] = ImgOut[pixel*3+1];
            CImg_img[pixel+size*2] = ImgOut[pixel*3+2];
        }

        CImg<OCTET> img( CImg_img, dimImgX, dimImgY, 1, 3);

        img.save( ImgOutStr.c_str() );

        free( CImg_img );
    }

    return 0;
}

double mosaique_Color::PSNR( OCTET* Img1, OCTET* Img2, int dimImgX, int dimImgY )
{
    double EQM = 0.;
    int taille_image = dimImgX * dimImgY * 3;
    int subpixel = 0;
	
    for( ; subpixel < taille_image; subpixel += 8 )
    {
        int subpixel0 = Img1[subpixel  ] - Img2[subpixel  ];
        int subpixel1 = Img1[subpixel+1] - Img2[subpixel+1];
        int subpixel2 = Img1[subpixel+2] - Img2[subpixel+2];
        int subpixel3 = Img1[subpixel+3] - Img2[subpixel+3];
        int subpixel4 = Img1[subpixel+4] - Img2[subpixel+4];
        int subpixel5 = Img1[subpixel+5] - Img2[subpixel+5];
        int subpixel6 = Img1[subpixel+6] - Img2[subpixel+6];
        int subpixel7 = Img1[subpixel+7] - Img2[subpixel+7];

        subpixel0 *= subpixel0;
        subpixel1 *= subpixel1;
        subpixel2 *= subpixel2;
        subpixel3 *= subpixel3;
        subpixel4 *= subpixel4;
        subpixel5 *= subpixel5;
        subpixel6 *= subpixel6;
        subpixel7 *= subpixel7;

        subpixel0 += subpixel1;
        subpixel2 += subpixel3;
        subpixel4 += subpixel5;
        subpixel6 += subpixel7;
        
        subpixel0 += subpixel2;
        subpixel4 += subpixel6;

        subpixel0 += subpixel4;

        EQM += ( double ) subpixel0;
    }

    for( ; subpixel < taille_image; subpixel++ )
        EQM += pow(Img1[subpixel]- Img2[subpixel], 2.);

    EQM /= (double) taille_image;

    double PSNR = 10. * log10( pow(255., 2.)/EQM ) ;

    return PSNR;
}

int mosaique_Color::replace_block_PSNR( OCTET* blockIn, DataBase_Color& DB )
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

int mosaique_Color::generate_ImgOut_PSNR( DataBase_Color& DB )
{
    OCTET* blockIn;
    try_malloc( blockIn, OCTET, dimX * dimY * 3, 1 );

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                {
                    blockIn[ ( blockInY * dimX + blockInX ) * 3 ] = ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 ];
                    blockIn[ ( blockInY * dimX + blockInX ) * 3 + 1] = ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 1];
                    blockIn[ ( blockInY * dimX + blockInX ) * 3 + 2] = ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 2];
                }

            int best_Img_idx = replace_block_PSNR( blockIn, DB );

            OCTET* best_Img = DB.get_Img( best_Img_idx );

            for( int best_ImgY = 0; best_ImgY < dimY; best_ImgY++ )
                for( int best_ImgX = 0; best_ImgX < dimX; best_ImgX++ )
                {
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 ] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 ];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 1] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 1];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 2] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 2 ];
                }
        }
    
    free( blockIn );
    return 0;
}

int mosaique_Color::generate_ImgOut_mean( DataBase_Color& DB )
{ 
    int taille_imagette = dimX * dimY;

    std::vector<double> ImgsMean;
    unsigned int nb_Imgs = DB.get_Imgs_length();
    
    DB.compute_imagettes_mean( ImgsMean );

    double blockMean[3];
    double bestMeanDiff;
    int best_Img_idx;

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            blockMean[0] = 0.;
            blockMean[1] = 0.;
            blockMean[2] = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                {
                    blockMean[0] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 ];
                    blockMean[1] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 1];
                    blockMean[2] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 2];
                }
            
            blockMean[0] /= (double) taille_imagette;
            blockMean[1] /= (double) taille_imagette;
            blockMean[2] /= (double) taille_imagette;

            bestMeanDiff = sqrt( pow( blockMean[0] - ImgsMean[0], 2.) + pow( blockMean[1] - ImgsMean[1], 2.) + pow( blockMean[2] - ImgsMean[2], 2.) );
            best_Img_idx = 0;

            for( unsigned int Img = 1; Img < nb_Imgs; Img++ ) // To Peel twice
            {
                double newDiff = sqrt( pow( blockMean[0] - ImgsMean[Img*3], 2.) + pow( blockMean[1] - ImgsMean[Img*3+1], 2.) + pow( blockMean[2] - ImgsMean[Img*3+2], 2.) );

                if( bestMeanDiff > newDiff )
                {
                    bestMeanDiff = newDiff;
                    best_Img_idx = Img;
                }
            }

            OCTET* best_Img = DB.get_Img( best_Img_idx );

            for( int best_ImgY = 0; best_ImgY < dimY; best_ImgY++ )
                for( int best_ImgX = 0; best_ImgX < dimX; best_ImgX++ )
                {
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 ] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 ];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 1] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 1];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 2] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 2 ];
                }
        }
    
    return 0;
}

int mosaique_Color::generate_ImgOut_variance( DataBase_Color& DB )
{ 
    int taille_imagette = dimX * dimY;

    std::vector<double> ImgsMean;
    std::vector<double> ImgsVariance;
    unsigned int nb_Imgs = DB.get_Imgs_length();
    
    DB.compute_imagettes_variance( ImgsMean, ImgsVariance );

    double blockMean[3];
    double blockVariance[3];
    double bestVarDiff;
    int best_Img_idx;

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            blockMean[0] = 0.;
            blockMean[1] = 0.;
            blockMean[2] = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                {
                    blockMean[0] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 ];
                    blockMean[1] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 1];
                    blockMean[2] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 2];
                }
            
            blockMean[0] /= (double) taille_imagette;
            blockMean[1] /= (double) taille_imagette;
            blockMean[2] /= (double) taille_imagette;

            blockVariance[0] = 0.;
            blockVariance[1] = 0.;
            blockVariance[2] = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                {
                    blockVariance[0] += pow(ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 ] - blockMean[0], 2.);
                    blockVariance[1] += pow(ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 1 ] - blockMean[1], 2.);
                    blockVariance[2] += pow(ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 2 ] - blockMean[2], 2.);
                }
            
            blockVariance[0] /= (double) taille_imagette;
            blockVariance[1] /= (double) taille_imagette;
            blockVariance[2] /= (double) taille_imagette;

            bestVarDiff = sqrt( pow( blockMean[0] - ImgsMean[0], 2.) + pow( blockMean[1] - ImgsMean[1], 2.) + pow( blockMean[2] - ImgsMean[2], 2.) )
                        + sqrt( pow( blockVariance[0] - ImgsVariance[0], 2.) + pow( blockVariance[1] - ImgsVariance[1], 2.) + pow( blockVariance[2] - ImgsVariance[2], 2.) );
            best_Img_idx = 0;

            for( unsigned int Img = 1; Img < nb_Imgs; Img++ ) // To Peel twice
            {
                double newDiff = sqrt( pow( blockMean[0] - ImgsMean[Img*3], 2.) + pow( blockMean[1] - ImgsMean[Img*3+1], 2.) + pow( blockMean[2] - ImgsMean[Img*3+2], 2.) )
                               + sqrt( pow( blockVariance[0] - ImgsVariance[Img*3], 2.) + pow( blockVariance[1] - ImgsVariance[Img*3+1], 2.) + pow( blockVariance[2] - ImgsVariance[Img*3+2], 2.) );

                if( bestVarDiff > newDiff )
                {
                    bestVarDiff = newDiff;
                    best_Img_idx = Img;
                }
            }

            OCTET* best_Img = DB.get_Img( best_Img_idx );

            for( int best_ImgY = 0; best_ImgY < dimY; best_ImgY++ )
                for( int best_ImgX = 0; best_ImgX < dimX; best_ImgX++ )
                {
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 ] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 ];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 1] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 1];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 2] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 2 ];
                }
        }
    
    return 0;
}

double mosaique_Color::compute_diff_weighs( double* blockMean, double* ImgMean )
{
    double diffR = abs( blockMean[0] - ImgMean[0] );
    double diffG = abs( blockMean[1] - ImgMean[1] );
    double diffB = abs( blockMean[2] - ImgMean[2] );

    double first, second, third;

    if( diffR < diffG )
    {
        if( diffG < diffB )
        {
            first = diffB;
            second = diffG;
            third = diffR;
        }
        else 
        {
            first = diffG;

            if( diffR < diffB )
            {
                second = diffB;
                third = diffR;
            }
            else 
            {
                second = diffR;
                third = diffB;
            }
        }
    }
    else 
    {
        if( diffG > diffB )
        {
            first = diffR;
            second = diffG;
            third = diffB;
        }
        else  
        {
            third = diffG;

            if( diffR < diffB )
            {
                first = diffB;
                second = diffR;
            }
            else 
            {
                first = diffR;
                second = diffB;
            }
        }
    }

    return first*3. + second*2. + third;
}

int mosaique_Color::generate_ImgOut_weighs( DataBase_Color& DB )
{ 
    int taille_imagette = dimX * dimY;

    std::vector<double> ImgsMean;
    unsigned int nb_Imgs = DB.get_Imgs_length();
    
    DB.compute_imagettes_mean( ImgsMean );

    double blockMean[3];
    double bestMeanDiff;
    int best_Img_idx;

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            blockMean[0] = 0.;
            blockMean[1] = 0.;
            blockMean[2] = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                {
                    blockMean[0] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 ];
                    blockMean[1] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 1];
                    blockMean[2] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 2];
                }
            
            blockMean[0] /= (double) taille_imagette;
            blockMean[1] /= (double) taille_imagette;
            blockMean[2] /= (double) taille_imagette;

            bestMeanDiff = compute_diff_weighs( blockMean, &ImgsMean[0] );
            best_Img_idx = 0;

            for( unsigned int Img = 1; Img < nb_Imgs; Img++ ) 
            {
                double newDiff = compute_diff_weighs( blockMean, &ImgsMean[Img*3] );

                if( bestMeanDiff > newDiff )
                {
                    bestMeanDiff = newDiff;
                    best_Img_idx = Img;
                }
            }

            OCTET* best_Img = DB.get_Img( best_Img_idx );

            for( int best_ImgY = 0; best_ImgY < dimY; best_ImgY++ )
                for( int best_ImgX = 0; best_ImgX < dimX; best_ImgX++ )
                {
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 ] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 ];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 1] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 1];
                    ImgOut[ ( ( best_ImgY + y ) * dimImgX + ( best_ImgX + x ) ) * 3 + 2] = best_Img[ ( best_ImgY * dimX + best_ImgX ) * 3 + 2 ];
                }
        }
    
    return 0;
}

int mosaique_Color::generate_ImgOut_outline( DataBase_Grey& DB )
{
    mosaique_Grey MPGM( ImgInStr, ImgOutStr, dimX, dimY );
    MPGM.load_ImgIn();
    MPGM.generate_ImgOut_PSNR( DB );

    OCTET* GreyImg;
    try_malloc( GreyImg, OCTET, dimImgX * dimImgY, 1 );
    MPGM.copy_Out( GreyImg );

    int taille_imagette = dimX * dimY;

    double blockMean[3];

    for( int y = 0; y < dimImgY; y += dimY )
        for( int x = 0; x < dimImgX; x += dimX ) 
        {
            blockMean[0] = 0.;
            blockMean[1] = 0.;
            blockMean[2] = 0.;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                {
                    blockMean[0] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 ];
                    blockMean[1] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 1];
                    blockMean[2] += (double) ImgIn[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 2];
                }
            
            blockMean[0] /= (double) taille_imagette;
            blockMean[1] /= (double) taille_imagette;
            blockMean[2] /= (double) taille_imagette;

            for( int blockInY = 0; blockInY < dimY; blockInY++ )
                for( int blockInX = 0; blockInX < dimX; blockInX++ )
                {
                    OCTET curGrey = GreyImg[ ( blockInY + y ) * dimImgX + ( blockInX + x ) ];

                    ImgOut[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 ] =  (double) ( curGrey * blockMean[0] ) / 255.;
                    ImgOut[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 1] = (double) ( curGrey * blockMean[1] ) / 255.;
                    ImgOut[ ( ( blockInY + y ) * dimImgX + ( blockInX + x ) ) * 3 + 2] = (double) ( curGrey * blockMean[2] ) / 255.;
                }

                // + différence moyenne entre ImgOut et ImgIn
        }
    
    free( GreyImg );
    
    return 0;
}

int mosaique_Color::copy_used_imagettes( std::string path, std::string format, DataBase_Color& DB )
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
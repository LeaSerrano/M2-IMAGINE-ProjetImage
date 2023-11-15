#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include "image_ppm.h"
#include <random>
#include <algorithm>
#include <complex>
#include <vector>
#include <cmath>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////     AUXILIAIRE     ///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void get_Mean_Var( std::vector< OCTET > Arr, float& Mean, float& Var )
{
    float mean = 0.;

    for( uint i = 0; i < Arr.size(); i++ )
        mean += Arr[i];

    mean = mean / (float) Arr.size();

    float var = 0.;

    for( uint i = 0; i < Arr.size(); i++ )
        var += pow( Arr[i] - mean, 2. );

    var = var / (float) ( Arr.size() - 1 );

    Mean = mean;
    Var = var;
}

float clip( float n, float lower = 0, float upper = 255 ) 
{
  float min = (n < upper)?(n):(upper);
  return (lower<min)?(min):(lower);
}

// référence transformée de fourier rapide 2D : https://www.tutorialspoint.com/cplusplus-perform-to-a-2d-fft-inplace-given-a-complex-2d-array
// idée de chat GPT
float* FFT2D_Coeff( OCTET* ImgIn, int nH, int nW )
{
    int nTaille = nH * nW;
    float nTaille_sqrt = sqrt( nTaille );
    float PI_2 = 2. * M_PI;

    float* reel = (float*) malloc( sizeof(float) * nTaille );
    float* imag = (float*) malloc( sizeof(float) * nTaille );
    float* ImgRed = (float*) malloc( sizeof(float) * nTaille );
    float* coeff = (float*) malloc( sizeof(float) * nTaille );

    for( int i = 0; i < nH; i++ )
        for( int j = 0; j < nW; j++ )
        {
            reel[i*nW+j] = 0;
            imag[i*nW+j] = 0;
            ImgRed[i*nW+j] = ((float)ImgIn[i*nW+j]) / nTaille_sqrt;
        }

    float i_val, j_val, k_val, l_val;
    i_val = j_val = k_val = l_val = 0.;
    float i_pas = PI_2 /((float)nH);
    float j_pas = PI_2 /((float)nW);

    for( int i = 0; i < nH; i++, i_val += i_pas )
    {
        for( int j = 0; j < nW; j++, j_val += j_pas ) // Two outer loops iterate on output data.
        {
            for( int k = 0; k < nH; k++, k_val += i_val )
            {
                for( int l = 0; l < nW; l++, l_val += j_val ) // Two inner loops iterate on input data.
                {
                    float val = l_val + k_val;
                    reel[i*nW+j] += ImgRed[k*nW+l] * cos( val );
                    imag[i*nW+j] -= ImgRed[k*nW+l] * sin( val );
                }

                l_val = 0.;
            }
            
            k_val = 0.;
            coeff[i*nW+j] = sqrt( reel[i*nW+j] * reel[i*nW+j] + imag[i*nW+j] * imag[i*nW+j] ) / ((float)nTaille);  
            // Output of FFT[i*nW+j] : reel[i*nW+j] + i * imag[i*nW+j] 
        }

        j_val = 0.;
        printf("%d\n", i);
    }

    free(reel); free(imag); free(ImgRed);

    return coeff;
}

bool Index_Is_In_Vector( std::vector< uint >& indices, int i, int j )
{
    for( uint k = 0; k < indices.size(); k += 2 )
        if( indices[k] == i && indices[k+1] == j )
            return true;
    
    return false;
}

void extrairePatch(OCTET *ImgIn, OCTET *ImgOut, int y, int x, int tailleFenetre, int nW, int nH) {
    int m = tailleFenetre / 2;

    for (int j = -m; j <= m; ++j) {
        for (int i = -m; i <= m; ++i) {
            int newY = y + j;
            int newX = x + i;

            if (newY >= 0 && newY < nH && newX >= 0 && newX < nW) {
                ImgOut[(j + m) * tailleFenetre + (i + m)] = ImgIn[newY * nW + newX];
            }
        }
    }
}


float calculerMesureSimilarite(OCTET* patch1, OCTET* patch2, int tailleFenetre, float sigma) {
    float sum = 0;

    for(int i = 0; i < tailleFenetre * tailleFenetre; i++) {
        sum += exp(-(patch1[i] - patch2[i]) * (patch1[i] - patch2[i]) / (2 * sigma * sigma));
    }

    return sum;
}


float calculerPoids(float mesureSimilarite, float sigma, float h) {
    float denom = sigma * sigma * h * h;
    if (denom != 0.0) {
        return exp(-mesureSimilarite / denom);
    }

    return 0.0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////     MOYENNEUR     ///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Moyenneur_G( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        {
            int nb_vois_att = 0;
            uint pixel_sum = 0;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                    {
                        pixel_sum += ImgIn[(i+k)*nW+(j+l)];
                        nb_vois_att++;
                    }
            
            float old_val = ImgIn[i*nW+j];
            float new_val = (float) pixel_sum / (float) nb_vois_att;
            
            ImgOut[i*nW+j] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_MOY_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Moyenneur_RGB( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float intensite = 1. ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        {
            int nb_vois_att = 0;
            uint pixel_sum_R = 0;
            uint pixel_sum_G = 0;
            uint pixel_sum_B = 0;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                    {
                        pixel_sum_R += ImgIn[((i+k)*nW+(j+l))*3  ];
                        pixel_sum_G += ImgIn[((i+k)*nW+(j+l))*3+1];
                        pixel_sum_B += ImgIn[((i+k)*nW+(j+l))*3+2];
                        nb_vois_att++;
                    }
            
            float old_val_R = ImgIn[(i*nW+j)*3];
            float old_val_G = ImgIn[(i*nW+j)*3+1];
            float old_val_B = ImgIn[(i*nW+j)*3+2];
            float new_val_R = (float) pixel_sum_R / (float) nb_vois_att;
            float new_val_G = (float) pixel_sum_G / (float) nb_vois_att;
            float new_val_B = (float) pixel_sum_B / (float) nb_vois_att;
            
            ImgOut[(i*nW+j)*3] = round( clip( intensite*new_val_R + (1.-intensite)*old_val_R, 0., 255. ) );
            ImgOut[(i*nW+j)*3+1] = round( clip( intensite*new_val_G + (1.-intensite)*old_val_G, 0., 255. ) );
            ImgOut[(i*nW+j)*3+2] = round( clip( intensite*new_val_B + (1.-intensite)*old_val_B, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_MOY_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Moyenneur( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int voisins = atoi( argv[5] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 7 )
    {
        intensite = atof( argv[6] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 )      Moyenneur_G( cNomImgLue, cNomImgLueLocation, OutDir, voisins, intensite );
    else if( strcmp( extension, "ppm" ) == 0 ) Moyenneur_RGB( cNomImgLue, cNomImgLueLocation, OutDir, voisins, intensite );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////     MEDIAN     /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Median_G( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        {
            std::vector< OCTET > vals;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                        vals.push_back( ImgIn[(i+k)*nW+(j+l)]);
            
            std::sort( vals.begin(), vals.end() );
            
            float old_val = ImgIn[i*nW+j];
            float new_val = vals[( round(pow( voisins*2 + 1, 2. )) - 1)/2];
            
            ImgOut[i*nW+j] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_MED_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());
                                     
    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Median_RGB( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        {
            std::vector< OCTET > vals_R;
            std::vector< OCTET > vals_G;
            std::vector< OCTET > vals_B;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                    {
                        vals_R.push_back( ImgIn[((i+k)*nW+(j+l))*3] );
                        vals_G.push_back( ImgIn[((i+k)*nW+(j+l))*3+1] );
                        vals_B.push_back( ImgIn[((i+k)*nW+(j+l))*3+2] );
                    }
            
            std::sort( vals_R.begin(), vals_R.end() );
            std::sort( vals_G.begin(), vals_G.end() );
            std::sort( vals_B.begin(), vals_B.end() );
            
            float old_val_R = ImgIn[(i*nW+j)*3];
            float old_val_G = ImgIn[(i*nW+j)*3+1];
            float old_val_B = ImgIn[(i*nW+j)*3+2];
            float new_val_R = vals_R[(round(pow( voisins*2 + 1, 2. )) - 1)/2];
            float new_val_G = vals_G[(round(pow( voisins*2 + 1, 2. )) - 1)/2];
            float new_val_B = vals_B[(round(pow( voisins*2 + 1, 2. )) - 1)/2];
            
            ImgOut[(i*nW+j)*3] = round( clip( intensite*new_val_R + (1.-intensite)*old_val_R, 0., 255. ) );
            ImgOut[(i*nW+j)*3+1] = round( clip( intensite*new_val_G + (1.-intensite)*old_val_G, 0., 255. ) );
            ImgOut[(i*nW+j)*3+2] = round( clip( intensite*new_val_B + (1.-intensite)*old_val_B, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_MOY_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".ppm") ).c_str() );

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Median( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int voisins = atoi( argv[5] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 7 )
    {
        intensite = atof( argv[6] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 )      Median_G( cNomImgLue, cNomImgLueLocation, OutDir, voisins, intensite );
    else if( strcmp( extension, "ppm" ) == 0 ) Median_RGB( cNomImgLue, cNomImgLueLocation, OutDir, voisins, intensite );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////     WIENER     /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Wiener_G( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float Var_Bruit, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        {
            std::vector< OCTET > vals;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                        vals.push_back( ImgIn[(i+k)*nW+(j+l)]);
            
            float Mean, Var;

            get_Mean_Var( vals, Mean, Var );
            
            float old_val = ImgIn[i*nW+j];
            float new_val = Mean + ( ((float)ImgIn[i*nW+j]) + Mean ) * ( Var / ( Var + Var_Bruit ) );
            
            ImgOut[i*nW+j] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    std::string Var_BruitStr = std::to_string(Var_Bruit);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_WIE_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + Var_BruitStr.substr(0, Var_BruitStr.find(".") + 2) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Wiener_RGB( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float Var_Bruit, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        {
            std::vector< OCTET > vals_R;
            std::vector< OCTET > vals_G;
            std::vector< OCTET > vals_B;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                    {
                        vals_R.push_back( ImgIn[((i+k)*nW+(j+l))*3] );
                        vals_G.push_back( ImgIn[((i+k)*nW+(j+l))*3+1] );
                        vals_B.push_back( ImgIn[((i+k)*nW+(j+l))*3+2] );
                    }
            
            float Mean_R, Mean_G, Mean_B, Var_R, Var_G, Var_B;

            get_Mean_Var( vals_R, Mean_R, Var_R );
            get_Mean_Var( vals_G, Mean_G, Var_G );
            get_Mean_Var( vals_B, Mean_B, Var_B );
            
            float old_val_R = ImgIn[(i*nW+j)*3];
            float old_val_G = ImgIn[(i*nW+j)*3+1];
            float old_val_B = ImgIn[(i*nW+j)*3+2];
            float new_val_R = Mean_R + ( ((float)ImgIn[(i*nW+j)*3]) + Mean_R ) * ( Var_R / ( Var_R + Var_Bruit ) );
            float new_val_G = Mean_G + ( ((float)ImgIn[(i*nW+j)*3+1]) + Mean_G ) * ( Var_G / ( Var_G + Var_Bruit ) );
            float new_val_B = Mean_B + ( ((float)ImgIn[(i*nW+j)*3+2]) + Mean_B ) * ( Var_B / ( Var_B + Var_Bruit ) );
            
            ImgOut[(i*nW+j)*3] = round( clip( intensite*new_val_R + (1.-intensite)*old_val_R, 0., 255. ) );
            ImgOut[(i*nW+j)*3+1] = round( clip( intensite*new_val_G + (1.-intensite)*old_val_G, 0., 255. ) );
            ImgOut[(i*nW+j)*3+2] = round( clip( intensite*new_val_B + (1.-intensite)*old_val_B, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    std::string Var_BruitStr = std::to_string(Var_Bruit);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_WIE_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + Var_BruitStr.substr(0, Var_BruitStr.find(".") + 2) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Wiener( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int voisins = atoi( argv[5] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float Var_Bruit = atof( argv[6] );
    if( Var_Bruit <= 0 )
    {
        printf("Variance négative\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 8 )
    {
        intensite = atof( argv[7] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 ) Wiener_G( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Var_Bruit, intensite );
    else if( strcmp( extension, "ppm" ) == 0 ) Wiener_RGB( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Var_Bruit, intensite );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////     GAUSSIEN     ////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gaussien_G( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float Mean, float Var, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    int nWF = voisins*2+1;
    
    float* filter = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( ((float)k) - Mean, 2 ) + pow( ((float)l) - Mean, 2 ) ) / ( 2. * Var ) ) / (2. * M_PI * Var );

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        { 
            float pixel_sum = 0;
            float Fsum = 0.;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                    {
                        pixel_sum += ((float)ImgIn[(i+k)*nW+(j+l)]) * filter[(k+voisins)*nWF+(l+voisins)];
                        Fsum += filter[(k+voisins)*nWF+(l+voisins)];
                    }
            
            float old_val = ImgIn[i*nW+j];
            float new_val = pixel_sum / Fsum;
            
            ImgOut[i*nW+j] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    std::string MeanStr = std::to_string(Mean);
    std::string VarStr = std::to_string(Var);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_GAU_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + MeanStr.substr(0, MeanStr.find(".") + 2) + std::string("_") 
                                     + VarStr.substr(0, VarStr.find(".") + 2) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free( filter );
}

void Gaussien_RGB( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float Mean, float Var, float intensite = 1. ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    int nWF = voisins*2+1;
    
    float* filter = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( ((float)k) - Mean, 2 ) + pow( ((float)l) - Mean, 2 ) ) / ( 2. * Var ) ) / (2. * M_PI * Var );

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        { 
            float pixel_sum_R = 0;
            float pixel_sum_G = 0;
            float pixel_sum_B = 0;
            float Fsum = 0;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                    {
                        pixel_sum_R += ImgIn[((i+k)*nW+(j+l))*3] * filter[(k+voisins)*nWF+(l+voisins)];
                        pixel_sum_G += ImgIn[((i+k)*nW+(j+l))*3+1] * filter[(k+voisins)*nWF+(l+voisins)];
                        pixel_sum_B += ImgIn[((i+k)*nW+(j+l))*3+2] * filter[(k+voisins)*nWF+(l+voisins)];
                        Fsum += filter[(k+voisins)*nWF+(l+voisins)];
                    }
            
            float old_val_R = ImgIn[(i*nW+j)*3];
            float old_val_G = ImgIn[(i*nW+j)*3+1];
            float old_val_B = ImgIn[(i*nW+j)*3+2];
            float new_val_R = pixel_sum_R / Fsum;
            float new_val_G = pixel_sum_G / Fsum;
            float new_val_B = pixel_sum_B / Fsum;
            
            ImgOut[(i*nW+j)*3] = round( clip( intensite*new_val_R + (1.-intensite)*old_val_R, 0., 255. ) );
            ImgOut[(i*nW+j)*3+1] = round( clip( intensite*new_val_G + (1.-intensite)*old_val_G, 0., 255. ) );
            ImgOut[(i*nW+j)*3+2] = round( clip( intensite*new_val_B + (1.-intensite)*old_val_B, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    std::string MeanStr = std::to_string(Mean);
    std::string VarStr = std::to_string(Var);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_GAU_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + MeanStr.substr(0, MeanStr.find(".") + 2) + std::string("_") 
                                     + VarStr.substr(0, VarStr.find(".") + 2) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free( filter );
}

void Gaussien( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int voisins = atoi( argv[5] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }
    
    float Mean, Var;

    if( argc >= 7 )
        Mean = atof( argv[6] );
    else   
        Mean = 0.;

    if( argc >= 8 )
    {
        Var = atof( argv[7] );

        if( Var <= 0 )
        {
            printf("Attention : Variance négative ou nulle( mise à 1 )\n");
            Var = 1.;
        }
    }
    else   
        Var = 1.;
    

    float intensite = 1.;
    if( argc >= 9)
    {
        intensite = atof( argv[8] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 )      Gaussien_G( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Mean, Var, intensite );
    else if( strcmp( extension, "ppm" ) == 0 ) Gaussien_RGB( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Mean, Var, intensite );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////     WIENER AVEC FOURNIER     //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Fournier_G( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float Var_Bruit, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    int nWF = voisins*2+1;
    float* coeff = FFT2D_Coeff( ImgIn, nH, nW );

    float* filter = (float*) malloc( sizeof(float) * nTaille );
    for( int i = 0; i < nH; i++ )
        for( int j = 0; j < nW; j++ )
        {
            float poids = coeff[i*nW+j] / ( coeff[i*nW+j] + Var_Bruit );
            filter[i*nW+j] = poids / ( poids + 1 );
        }
    
    for( int i = 0; i < nH; i++ )
        for( int j = 0; j < nW; j++ )
        {
            float somme = 0.;
            float somme_poids = 0.;

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                    {
                        float poids = filter[(i+k)*nW+(j+l)];
                        somme += poids * ImgIn[(i+k)*nW+(j+l)];
                        somme_poids += poids;
                    }

            float old_val = ImgIn[i*nW+j];
            float new_val = somme / somme_poids;
            
            ImgOut[i*nW+j] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    std::string Var_BruitStr = std::to_string(Var_Bruit);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_FOU_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + Var_BruitStr.substr(0, Var_BruitStr.find(".") + 2) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free(coeff); free(filter);
}

void Fournier( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int voisins = atoi( argv[5] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float Var_Bruit = atof( argv[6] );
    if( Var_Bruit <= 0 )
    {
        printf("Variance négative\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 8 )
    {
        intensite = atof( argv[7] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 ) Fournier_G( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Var_Bruit, intensite );
    else if( strcmp( extension, "ppm" ) == 0 ); //Fournier_RGB( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Var_Bruit, intensite );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////     GRADIENT     ////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gradient_G( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, OCTET Seuil, bool use_MOY, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut, *ImgGrad;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(ImgGrad, OCTET, nTaille);

    int nWF = voisins*2+1;
    
    float* filter = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    float* filter_default = ( float* ) malloc( sizeof(float) * 9 );

    for( int k = -1; k <= 1; k++ )
        for( int l = -1; l <= 1; l++ )
            filter_default[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    for (int i = 0; i < nH-1; i++) 
        for (int j = 0; j < nW-1; j++) 
        {
            float val = sqrt( pow( ImgIn[(i+1)*nW+j]- ImgIn[i*nW+j] , 2 ) + pow( ImgIn[i*nW+j]- ImgIn[i*nW+j+1], 2 ) );
            ImgGrad[i*nW+j] = (val<(float)Seuil)?(0):(255);
        }
    
    for( int i = 0; i < nH; i++ )
        for( int j = 0; j < nW; j++ )
        {
            OCTET cur_color = ImgGrad[i*nW+j];

            std::vector< uint > visited;
            visited.clear();
            visited.push_back( i );
            visited.push_back( j );

            for( uint k = 0; k < visited.size(); k += 2 )
            {
                int cur_i = visited[k];
                int cur_j = visited[k+1];

                if( cur_i + 1 <= i + voisins && cur_i + 1 < nH 
                 && ImgGrad[(cur_i + 1)*nW+cur_j] == cur_color 
                 && !Index_Is_In_Vector( visited, cur_i+1, cur_j ) )
                {
                    visited.push_back( cur_i + 1 );
                    visited.push_back( cur_j );
                }
                if( cur_i - 1 >= i - voisins && cur_i - 1 >= 0 
                 && ImgGrad[(cur_i - 1)*nW+cur_j] == cur_color 
                 && !Index_Is_In_Vector( visited, cur_i-1, cur_j ) )
                {
                    visited.push_back( cur_i - 1 );
                    visited.push_back( cur_j );
                }
                if( cur_j + 1 <= j + voisins && cur_j + 1 < nW 
                 && ImgGrad[cur_i*nW+cur_j+1] == cur_color 
                 && !Index_Is_In_Vector( visited, cur_i, cur_j+1 ) )
                {
                    visited.push_back( cur_i );
                    visited.push_back( cur_j + 1);
                }
                if( cur_j - 1 >= j - voisins && cur_j - 1 >= 0 
                 && ImgGrad[cur_i*nW+cur_j-1] == cur_color 
                 && !Index_Is_In_Vector( visited, cur_i, cur_j-1 ) )
                {
                    visited.push_back( cur_i );
                    visited.push_back( cur_j - 1 );
                }
            }
            
            float old_val = ImgIn[i*nW+j];
            float new_val;

            if( visited.size() == 2 )
            {
                if( use_MOY )
                {
                    int pixel_sum = 0;
                    int nb_vois_att = 0;

                    for( int k = -1; k <= 1; k++ )
                        for( int l = -1; l <= 1; l++ )
                            if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                            {
                                pixel_sum += ImgIn[(i+k)*nW+(j+l)];
                                nb_vois_att++;
                            }
                    
                    new_val = (float) pixel_sum / (float) nb_vois_att;
                }
                else 
                {
                    new_val = 0.;
                    float total_weight = 0;

                    for( int k = -1; k <= 1; k++ )
                        for( int l = -1; l <= 1; l++ )
                            if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                            {
                                new_val += ImgIn[(i+k)*nW+(j+l)] * filter_default[(k+1)*3+(l+1)];
                                total_weight += filter_default[(k+1)*3+(l+1)];
                            }
                    
                    new_val /= total_weight;
                }
                
            }
            else 
            {
                float moy = 0.;

                if( use_MOY )
                {
                    for( uint k = 0; k < visited.size(); k += 2 )
                        moy += (float) ImgIn[visited[k]*nW+visited[k+1]];

                    moy = moy / (float) (visited.size()/2);
                }
                else 
                {
                    float total_weight = 0;

                    for( uint k = 0; k < visited.size(); k += 2 )
                    {
                        moy += (float) ImgIn[visited[k]*nW+visited[k+1]] * filter[(visited[k]-i+voisins)*nWF+(visited[k+1]-j+voisins)];
                        total_weight += filter[(visited[k]-i+voisins)*nWF+(visited[k+1]-j+voisins)];
                    }

                    moy /= total_weight;
                }
                
                new_val = moy;
            }

            ImgOut[i*nW+j] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_GRA_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + std::to_string(Seuil) + std::string("_") 
                                     + std::string((use_MOY)?"moy":"gra") + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free( ImgGrad ); free(ImgOut); free( filter ); free( filter_default );
}

void Gradient_RGB( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, OCTET Seuil, bool use_MOY, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut, *ImgGrad;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(ImgGrad, OCTET, nTaille*3);

    int nWF = voisins*2+1;
    
    float* filter = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    float* filter_default = ( float* ) malloc( sizeof(float) * 9 );

    for( int k = -1; k <= 1; k++ )
        for( int l = -1; l <= 1; l++ )
            filter_default[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    for (int i = 0; i < nH-1; i++) 
        for (int j = 0; j < nW-1; j++) 
            for( uint d = 0; d < 3; d++ )
            {
                float val = sqrt( pow( ImgIn[((i+1)*nW+j)*3+d]- ImgIn[(i*nW+j)*3+d] , 2 ) + pow( ImgIn[(i*nW+j+1)*3+d]- ImgIn[(i*nW+j)*3+d], 2 ) );
                ImgGrad[(i*nW+j)*3+d] = (val<(float)Seuil)?(0):(255);
            }
    
    for( int i = 0; i < nH; i++ )
        for( int j = 0; j < nW; j++ )
            for( uint d = 0; d < 3; d++ )
            {
                OCTET cur_color = ImgGrad[(i*nW+j)*3+d];

                std::vector< uint > visited;
                visited.clear();
                visited.push_back( i );
                visited.push_back( j );

                for( uint k = 0; k < visited.size(); k += 2 )
                {
                    int cur_i = visited[k];
                    int cur_j = visited[k+1];

                    if( cur_i + 1 <= i + voisins && cur_i + 1 < nH 
                    && ImgGrad[((cur_i + 1)*nW+cur_j)*3+d] == cur_color 
                    && !Index_Is_In_Vector( visited, cur_i+1, cur_j ) )
                    {
                        visited.push_back( cur_i + 1 );
                        visited.push_back( cur_j );
                    }
                    if( cur_i - 1 >= i - voisins && cur_i - 1 >= 0 
                    && ImgGrad[((cur_i - 1)*nW+cur_j)*3+d] == cur_color 
                    && !Index_Is_In_Vector( visited, cur_i-1, cur_j ) )
                    {
                        visited.push_back( cur_i - 1 );
                        visited.push_back( cur_j );
                    }
                    if( cur_j + 1 <= j + voisins && cur_j + 1 < nW 
                    && ImgGrad[(cur_i*nW+cur_j+1)*3+d] == cur_color 
                    && !Index_Is_In_Vector( visited, cur_i, cur_j+1 ) )
                    {
                        visited.push_back( cur_i );
                        visited.push_back( cur_j + 1);
                    }
                    if( cur_j - 1 >= j - voisins && cur_j - 1 >= 0 
                    && ImgGrad[(cur_i*nW+cur_j-1)*3+d] == cur_color 
                    && !Index_Is_In_Vector( visited, cur_i, cur_j-1 ) )
                    {
                        visited.push_back( cur_i );
                        visited.push_back( cur_j - 1 );
                    }
                }
                
                float old_val = ImgIn[(i*nW+j)*3+d];
                float new_val;

                if( visited.size() == 2 )
                {
                    if( use_MOY )
                    {
                        int pixel_sum = 0;
                        int nb_vois_att = 0;

                        for( int k = -1; k <= 1; k++ )
                            for( int l = -1; l <= 1; l++ )
                                if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                                {
                                    pixel_sum += ImgIn[((i+k)*nW+(j+l))*3+d];
                                    nb_vois_att++;
                                }
                        
                        new_val = (float) pixel_sum / (float) nb_vois_att;
                    }
                    else 
                    {
                        new_val = 0.;
                        float total_weight = 0;

                        for( int k = -1; k <= 1; k++ )
                            for( int l = -1; l <= 1; l++ )
                                if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW )
                                {
                                    new_val += ImgIn[((i+k)*nW+(j+l))*3+d] * filter_default[(k+1)*3+(l+1)];
                                    total_weight += filter_default[(k+1)*3+(l+1)];
                                }

                        new_val /= total_weight;
                    }   
                    
                }
                else 
                {
                    float moy = 0.;

                    if( use_MOY )
                    {
                        for( uint k = 0; k < visited.size(); k += 2 )
                            moy += (float) ImgIn[(visited[k]*nW+visited[k+1])*3+d];

                        moy = moy / (float) (visited.size()/2);
                    }
                    else 
                    {
                        float total_weight = 0;

                        for( uint k = 0; k < visited.size(); k += 2 )
                        {
                            moy += (float) ImgIn[(visited[k]*nW+visited[k+1])*3+d]* filter[(visited[k]-i+voisins)*nWF+(visited[k+1]-j+voisins)];
                            total_weight += filter[(visited[k]-i+voisins)*nWF+(visited[k+1]-j+voisins)];
                        }

                        moy /= total_weight;
                    }
                    
                    new_val = moy;
                }

                ImgOut[(i*nW+j)*3+d] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
            }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_GRA_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + std::to_string(Seuil) + std::string("_") 
                                     + std::string((use_MOY)?"moy":"gra") + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free( ImgGrad ); free(ImgOut); free( filter ); free( filter_default );
}

void Gradient( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int voisins = atoi( argv[5] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    OCTET Seuil = (OCTET) round( clip( atof( argv[6] ), 0., 255. ) );

    bool use_MOY = (argv[7][0] == '0')?false:true;

    float intensite = 1.;
    if( argc >= 9 )
    {
        intensite = atof( argv[8] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 )       Gradient_G( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Seuil, use_MOY, intensite );
    else if( strcmp( extension, "ppm" ) == 0 )  Gradient_RGB( cNomImgLue, cNomImgLueLocation, OutDir, voisins, Seuil, use_MOY, intensite );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////     MOYENNEUR PONDEREE     ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pondere_G( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float power, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
        {
            float pixel_sum = 0;
            float weight_sum = 0;
            OCTET cur_color = ImgIn[i*nW+j];

            for( int k = -voisins; k <= voisins; k++ )
                for( int l = -voisins; l <= voisins; l++ )
                    if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW && !( k == 0 && l == 0 ) )
                    {
                        float power_res = pow( abs( ImgIn[(i+k)*nW+(j+l)] - cur_color ), power );
                        if( power_res <= 0 )
                            power_res = 1;
                        float weight = 1 / power_res;
                        pixel_sum += ImgIn[(i+k)*nW+j+l] * weight;
                        weight_sum += weight;
                    }
            
            float old_val = ImgIn[i*nW+j];
            float new_val = pixel_sum / weight_sum;
            
            ImgOut[i*nW+j] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
        }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    std::string powerStr = std::to_string(power);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_PON_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + powerStr.substr(0, powerStr.find(".") + 2) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Pondere_RGB( char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int voisins, float power, float intensite ) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    for( int i = 0; i < nH; i++ ) 
        for( int j = 0; j < nW; j++ ) 
            for( uint d = 0; d < 3; d++ )
            {
                float pixel_sum = 0;
                float weight_sum = 0;
                OCTET cur_color = ImgIn[(i*nW+j)*3+d];

                for( int k = -voisins; k <= voisins; k++ )
                    for( int l = -voisins; l <= voisins; l++ )
                        if( (i+k) >= 0 && (i+k) < nH && (j+l) >= 0 && (j+l) < nW && !( k == 0 && l == 0 ) )
                        {
                            float power_res = pow( abs( ImgIn[((i+k)*nW+(j+l))*3+d] - cur_color ), power );
                            if( power_res <= 0 )
                                power_res = 1;
                            float weight = 1 / power_res;
                            pixel_sum += ImgIn[((i+k)*nW+j+l)*3+d] * weight;
                            weight_sum += weight;
                        }
                
                float old_val = ImgIn[(i*nW+j)*3+d];
                float new_val = pixel_sum / weight_sum;
                
                ImgOut[(i*nW+j)*3+d] = round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
            }

    char cNomImgEcrite[250];
    std::string intensiteStr = std::to_string(intensite);
    std::string powerStr = std::to_string(power);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_PON_") 
                                     + std::to_string(voisins) + std::string("_") 
                                     + powerStr.substr(0, powerStr.find(".") + 2) + std::string("_") 
                                     + intensiteStr.substr(0, intensiteStr.find(".") + 2) 
                                     + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Pondere( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int voisins = atoi( argv[5] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float power = atof( argv[6] );
    if( power < 0 )
    {
        printf("Attention : puissance négatif\n");
        power = 1.;
    }

    float intensite = 1.;
    if( argc >= 8 )
    {
        intensite = atof( argv[7] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 )       Pondere_G( cNomImgLue, cNomImgLueLocation, OutDir, voisins, power, intensite );
    else if( strcmp( extension, "ppm" ) == 0 )  Pondere_RGB( cNomImgLue, cNomImgLueLocation, OutDir, voisins, power, intensite );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////     NON LOCAL MEANS     /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void NonLocalMeans_G(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, float sigma, float h, int tailleFenetre) {

    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

     OCTET *patchActuel;
    allocation_tableau(patchActuel, OCTET, tailleFenetre * tailleFenetre);
    
    OCTET *patchVoisin;
    allocation_tableau(patchVoisin, OCTET, tailleFenetre * tailleFenetre);

    int m = tailleFenetre/2;

    for (int y = m; y < nH - m; ++y) {
        for (int x = m; x < nW - m; ++x) {
            float sommePoids = 0.0;
            float sommePoidsValeurs = 0.0;

            extrairePatch(ImgIn, patchActuel, y, x, tailleFenetre, nW, nH);

            for (int j = -m; j <= m; ++j) {
                for (int i = -m; i <= m; ++i) {
                    extrairePatch(ImgIn, patchVoisin, y + j, x + i, tailleFenetre, nW, nH);

                    float mesureSimilarite = calculerMesureSimilarite(patchActuel, patchVoisin, tailleFenetre, sigma);
                    float poids = calculerPoids(mesureSimilarite, sigma, h);

                    sommePoids += poids;
                    sommePoidsValeurs += poids * ImgIn[(y + j) * nW + (x + i)];
                }
            }

            ImgOut[y * nW + x] = sommePoidsValeurs / sommePoids;

        }
    }

    char cNomImgEcrite[250];
    std::string sigmaStr = std::to_string(sigma);
    std::string hStr = std::to_string(h);
    strcpy(cNomImgEcrite, std::string( std::string(OutDir) + cNomImgLue + std::string("_NLM_") 
                                     + std::to_string(m) + std::string("_") 
                                     + sigmaStr.substr(0, sigmaStr.find(".") + 2) + std::string("_") 
                                     + hStr.substr(0, hStr.find(".") + 2) 
                                     + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn); free(ImgOut);
}

void NonLocalMeans( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    float sigma = atof( argv[5] );

    float h = atof( argv[6] );

    int tailleFenetre = atoi( argv[7] );
    if( tailleFenetre <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    if( strcmp( extension, "pgm" ) == 0 )      NonLocalMeans_G( cNomImgLue, cNomImgLueLocation, OutDir, sigma, h, tailleFenetre);
    //else if( strcmp( extension, "ppm" ) == 0 );
    else   
        printf("Extension %s inconnue.\n", extension );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////     MAIN     //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum FILTER
{
    MOYENNEUR,
    MEDIAN, 
    WIENER,
    FOURNIER,
    GAUSSIEN,
    GRADIENT,
    PONDERE,
    NONLOCALMEANS,
    NB_FILTERS
};

void (*filter_functions[NB_FILTERS]) ( int, char**, char*, char*, char*, char* ) = { Moyenneur, Median, Wiener, Fournier, Gaussien, Gradient, Pondere, NonLocalMeans };

char* TAG[NB_FILTERS] = { (char*) "MOY", (char*) "MED", (char*) "WIE", (char*) "FOU", (char*) "GAU", (char*) "GRA", (char*) "PON", (char*) "NLM" };

int getMode( char* modeStr, int argc )
{
    int mode = 0;

    if( !strcmp( modeStr, TAG[MOYENNEUR] ) )
    {
        if( argc < 6 )
        {
            printf("Usage: DirIn ImageIn DirOut MOY #voisins intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        mode = MOYENNEUR;
    }
    else if( !strcmp( modeStr, TAG[MEDIAN] ) )
    {
        if( argc < 6 )
        {
            printf("Usage: DirIn ImageIn DirOut MED #voisins intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        mode = MEDIAN;
    }
    else if( !strcmp( modeStr, TAG[WIENER] ))
    {
        if( argc < 7 )
        {
            printf("Usage: DirIn ImageIn DirOut WIE #voisins variance_bruit intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        mode = WIENER;
    }
    else if( !strcmp( modeStr, TAG[FOURNIER] ) )
    {
        if( argc < 7 )
        {
            printf("Usage: DirIn ImageIn DirOut FOU #voisins variance_bruit intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        printf("Attention : cette méthode est longue et inefficace\n");
        mode = FOURNIER;
    }
    else if( !strcmp( modeStr, TAG[GAUSSIEN] ) )
    {
        if( argc < 6 )
        {
            printf("Usage: DirIn ImageIn DirOut GAU #voisins moyenne variance intensité( entre 0 et 1 )\n"); 
            exit(1);
        }
        
        mode = GAUSSIEN;
    }
    else if( !strcmp( modeStr, TAG[GRADIENT] ) )
    {
        if( argc < 8 )
        {
            printf("Usage: DirIn ImageIn DirOut GRA #voisins Seuil utiliser_moyenneur intensité( entre 0 et 1 )\n"); 
            exit(1);
        }
        
        mode = GRADIENT;
    }
    else if( !strcmp( modeStr, TAG[PONDERE] ) )
    {
        if( argc < 7 )
        {
            printf("Usage: DirIn ImageIn DirOut PON #voisins puissance intensité( entre 0 et 1 )\n"); 
            exit(1);
        }
        
        mode = PONDERE;
    }
    else if( !strcmp( modeStr, TAG[NONLOCALMEANS] ) )
    {
        if( argc < 7 )
        {
            printf("Usage: DirIn ImageIn DirOut NLM ponderation tailleFenetreDeRecherche tailleFenetre\n"); 
            exit(1);
        }
        
        mode = NONLOCALMEANS;
    }
    else 
    {
        printf("Le mode %s n'existe pas !\nModes valides :", modeStr );
        
        for( int i = 0; i < NB_FILTERS-1; i++ )
            printf("%s,", TAG[i] );

        printf("%s\n", TAG[NB_FILTERS-1]);
        exit(1);
    }

    return mode;
}

int main(int argc, char* argv[])
{
    char cDirImgLue[250], cNomImgLue[250];

    if( argc < 5 ) 
    {
        printf("Usage: DirIn ImageIn DirOut Mode_Débruitage\n"
                       "MOY : filtre moyenneur ( args : #voisins intensite? )\n"
                       "MED : filtre médian ( args : #voisins intensite? )\n"
                       "WIE : filtre de Wiener ( args : #voisins variance_du_bruit intensite? )\n"
                       "FOU : filtre de Wiener avec une transformation de fournier ( à éviter, compiler avec -O3 )\n"
                       "GAU : filtre gaussien ( args : #voisins moyenne? variance? intensite? )\n"
                       "GRA : filtre utilisant la carte de gradient seuillée ( args : #voisins seuil intensite? )\n"
                       "PON : filtre moyenneur pondéré ( args : #voisins intensite? )\n"
                       "NLM : algorithme non local means (args : ponderation tailleFenetreDeRecherche tailleFenetre)\n"); 
        exit(1);
    }
    else if( strlen( argv[4] ) != 3 )
    {
        printf("Le mode %s n'existe pas !\nModes valides :", argv[4]);
        
        for( int i = 0; i < NB_FILTERS-1; i++ )
            printf("%s,", TAG[i] );

        printf("%s\n", TAG[NB_FILTERS-1]);
        exit(1);
    }

    int mode = getMode( argv[4], argc );

    sscanf (argv[1],"%s",cDirImgLue);
    sscanf (argv[2],"%s",cNomImgLue);

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    cNomImgLue[longueur - 4] = '\0';

    (*filter_functions[mode])( argc, argv, cDirImgLue, cNomImgLue, cDernieresLettres, argv[3] );

    return 0;
}
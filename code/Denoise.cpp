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

///////////////////////////////////////////////////////////////////////     AUXILIAIRE     ///////////////////////////////////////////////////////////////////////


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

float clip( float n, float lower, float upper ) 
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

///////////////////////////////////////////////////////////////////////     MOYENNEUR     ///////////////////////////////////////////////////////////////////////

void Moyenneur_G( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float intensite ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Denoise/Moyenneur/") + cNomImgLue + std::string("_Moyenneur_") + std::to_string(voisins) + std::string("_") + std::to_string(intensite) + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Moyenneur_RGB( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float intensite = 1. ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Ppm/Denoise/Moyenneur/") + cNomImgLue + std::string("_Moyenneur_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Moyenneur( int argc, char** argv, char* cNomImgLue, char* extension )
{   
    char cNomImgLueLocation[256];

    int voisins = atoi( argv[3] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 5 )
    {
        intensite = atof( argv[4] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Pgm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Moyenneur_G( cNomImgLue, cNomImgLueLocation, voisins, intensite );
    }
    else if( strcmp( extension, "ppm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Ppm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Moyenneur_RGB( cNomImgLue, cNomImgLueLocation, voisins, intensite );
    }
    else   
        printf("Extension %s inconnue.\n", extension );
}

///////////////////////////////////////////////////////////////////////     MEDIAN     ///////////////////////////////////////////////////////////////////////

void Median_G( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float intensite ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Denoise/Median/") + cNomImgLue + std::string("_Median_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Median_RGB( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float intensite ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Ppm/Denoise/Median/") + cNomImgLue + std::string("_Median_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Median( int argc, char** argv, char* cNomImgLue, char* extension )
{   
    char cNomImgLueLocation[256];

    int voisins = atoi( argv[3] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 5 )
    {
        intensite = atof( argv[4] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Pgm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Median_G( cNomImgLue, cNomImgLueLocation, voisins, intensite );
    }
    else if( strcmp( extension, "ppm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Ppm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Median_RGB( cNomImgLue, cNomImgLueLocation, voisins, intensite );
    }
    else   
        printf("Extension %s inconnue.\n", extension );
}

///////////////////////////////////////////////////////////////////////     WIENER     ///////////////////////////////////////////////////////////////////////

void Wiener_G( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float Var_Bruit, float intensite ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Denoise/Wiener/") + cNomImgLue + std::string("_Wiener_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(Var_Bruit).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Wiener_RGB( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float Var_Bruit, float intensite ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Ppm/Denoise/Wiener/") + cNomImgLue + std::string("_Wiener_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(Var_Bruit).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".pgm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Wiener( int argc, char** argv, char* cNomImgLue, char* extension )
{   
    char cNomImgLueLocation[256];

    int voisins = atoi( argv[3] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float Var_Bruit = atof( argv[4] );
    if( Var_Bruit <= 0 )
    {
        printf("Variance négative\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 6 )
    {
        intensite = atof( argv[5] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Pgm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Wiener_G( cNomImgLue, cNomImgLueLocation, voisins, Var_Bruit, intensite );
    }
    else if( strcmp( extension, "ppm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Ppm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Wiener_RGB( cNomImgLue, cNomImgLueLocation, voisins, Var_Bruit, intensite );
    }
    else   
        printf("Extension %s inconnue.\n", extension );
}

///////////////////////////////////////////////////////////////////////     GAUSSIEN     ///////////////////////////////////////////////////////////////////////

void Gaussien_G( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float Mean, float Var, float intensite ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Denoise/Gaussien/") + cNomImgLue + std::string("_Gaussien_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(Mean).c_str() + std::string("_") + std::to_string(Var).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free( filter );
}

void Gaussien_RGB( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float Mean, float Var, float intensite = 1. ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Ppm/Denoise/Gaussien/") + cNomImgLue + std::string("_Gaussien_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(Mean).c_str() + std::string("_") + std::to_string(Var).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".ppm") ).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free( filter );
}

void Gaussien( int argc, char** argv, char* cNomImgLue, char* extension )
{   
    char cNomImgLueLocation[256];

    int voisins = atoi( argv[3] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float Mean, Var;

    if( argc >= 5 )
        Mean = atof( argv[4] );
    else   
        Mean = 0.;

    if( argc >= 6 )
    {
        Var = atof( argv[5] );

        if( Var <= 0 )
        {
            printf("Attention : Variance négative ou nulle( mise à 1 )\n");
            Var = 1.;
        }
    }
    else   
        Var = 1.;
    

    float intensite = 1.;
    if( argc >= 7 )
    {
        intensite = atof( argv[6] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Pgm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Gaussien_G( cNomImgLue, cNomImgLueLocation, voisins, Mean, Var, intensite );
    }
    else if( strcmp( extension, "ppm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Ppm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Gaussien_RGB( cNomImgLue, cNomImgLueLocation, voisins, Mean, Var, intensite );
    }
    else   
        printf("Extension %s inconnue.\n", extension );
}

///////////////////////////////////////////////////////////////////////     WIENER AVEC FOURNIER     ///////////////////////////////////////////////////////////////////////

void Fournier_G( char *cNomImgLue, char *cNomImgLueLocation, int voisins, float Var_Bruit, float intensite ) 
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
    strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Denoise/Wiener/") + cNomImgLue + std::string("_Fournier_Wiener_") + std::to_string(voisins).c_str() + std::string("_") + std::to_string(Var_Bruit).c_str() + std::string("_") + std::to_string(intensite).c_str() + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free(coeff); free(filter);
}

void Fournier( int argc, char** argv, char* cNomImgLue, char* extension )
{   
    char cNomImgLueLocation[256];

    int voisins = atoi( argv[3] );
    if( voisins <= 0 )
    {
        printf("Nombre de voisins négatifs\n");
        exit(1);
    }

    float Var_Bruit = atof( argv[4] );
    if( Var_Bruit <= 0 )
    {
        printf("Variance négative\n");
        exit(1);
    }

    float intensite = 1.;
    if( argc >= 6 )
    {
        intensite = atof( argv[5] );
        if( intensite < 0 ) intensite = 0.;
        if( intensite > 1 ) intensite = 1.;
    }

    if( strcmp( extension, "pgm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Pgm/") + cNomImgLue + std::string(".") + extension ).c_str());
        Fournier_G( cNomImgLue, cNomImgLueLocation, voisins, Var_Bruit, intensite );
    }
    else if( strcmp( extension, "ppm" ) == 0 ) 
    {
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Ppm/") + cNomImgLue + std::string(".") + extension ).c_str());
        //Fournier_RGB( cNomImgLue, cNomImgLueLocation, voisins, Var_Bruit, intensite );
    }
    else   
        printf("Extension %s inconnue.\n", extension );
}

///////////////////////////////////////////////////////////////////////     MAIN     ///////////////////////////////////////////////////////////////////////

enum FILTER
{
    MOYENNEUR,
    MEDIAN, 
    WIENER,
    FOURNIER,
    GAUSSIEN,
    NB_FILTERS
};

void (*filter_functions[NB_FILTERS]) ( int, char**, char*, char* ) = { Moyenneur, Median, Wiener, Fournier, Gaussien };

char* TAG[NB_FILTERS] = { (char*) "MOY", (char*) "MED", (char*) "WIE", (char*) "FOU", (char*) "GAU" };

int getMode( char* modeStr, int argc )
{
    int mode = 0;

    if( !strcmp( modeStr, TAG[MOYENNEUR] ) )
    {
        if( argc < 4 )
        {
            printf("Usage: ImageIn MOY #voisins intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        mode = MOYENNEUR;
    }
    else if( !strcmp( modeStr, TAG[MEDIAN] ) )
    {
        if( argc < 4 )
        {
            printf("Usage: ImageIn MED #voisins intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        mode = MEDIAN;
    }
    else if( !strcmp( modeStr, TAG[WIENER] ))
    {
        if( argc < 5 )
        {
            printf("Usage: ImageIn WIE #voisins variance_bruit intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        mode = WIENER;
    }
    else if( !strcmp( modeStr, TAG[FOURNIER] ) )
    {
        if( argc < 5 )
        {
            printf("Usage: ImageIn FOU #voisins variance_bruit intensité( entre 0 et 1 )\n"); 
            exit(1);
        }

        printf("Attention : cette méthode est longue et inefficace\n");
        mode = FOURNIER;
    }
    else if( !strcmp( modeStr, TAG[GAUSSIEN] ) )
    {
        if( argc < 4 )
        {
            printf("Usage: ImageIn GAU #voisins moyenne variance intensité( entre 0 et 1 )\n"); 
            exit(1);
        }
        
        mode = GAUSSIEN;
    }
    else 
    {
        printf("Le mode %s n'existe pas !\nModes valides : MOY, MED, WIE, GAU\n", modeStr );
        exit(1);
    }

    return mode;
}

int main(int argc, char* argv[])
{
    char cNomImgLue[250];

    if( argc < 3 ) 
    {
        printf("Usage: ImageIn Mode_Débruitage\n"
                       "MOY : filtre moyenneur ( args : #voisins intensite? )\n"
                       "MED : filtre médian ( args : #voisins intensite? )\n"
                       "WIE : filtre de Wiener ( args : #voisins variance_du_bruit intensite? )\n"
                       "FOU : filtre de Wiener avec une transformation de fournier ( à éviter, compiler avec -O3 )\n"
                       "GAU : filtre gaussien ( args : #voisins moyenne? variance? intensite? )\n"); 
        exit(1);
    }
    else if( strlen( argv[2] ) != 3 )
    {
        printf("Le mode n'est pas valide.\nModes valides : MOY, MED, WIE, GAU\n");
        exit(1);
    }

    int mode = getMode( argv[2], argc );

    sscanf (argv[1],"%s",cNomImgLue);

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    cNomImgLue[longueur - 4] = '\0';

    (*filter_functions[mode])( argc, argv, cNomImgLue, cDernieresLettres );

    return 0;
}
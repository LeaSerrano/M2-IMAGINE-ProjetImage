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
#include <fstream>
#include <ctime>

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

void Moyenneur_G( char *cNomImgLue, /*char *cNomImgLueLocation, char* OutDir*/char* cNomImgEcrite, int voisins, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Moyenneur_RGB( char *cNomImgLue, char* cNomImgEcrite, int voisins, float intensite = 1. )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Moyenneur(char* cNomImgLue, char* cNomImgEcrite, float nbVoisins, float intensite )
{

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )      Moyenneur_G( cNomImgLue, cNomImgEcrite, nbVoisins, intensite );
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) Moyenneur_RGB( cNomImgLue, cNomImgEcrite, nbVoisins, intensite );
    else   
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////     MEDIAN     /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Median_G( char *cNomImgLue, char* cNomImgEcrite, int voisins, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

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
                                     
    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Median_RGB( char *cNomImgLue, char* cNomImgEcrite, int voisins, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Median(char* cNomImgLue, char* cNomImgEcrite, float nbVoisins, float intensite )
{   

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )      Median_G( cNomImgLue, cNomImgEcrite, nbVoisins, intensite );
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) Median_RGB( cNomImgLue, cNomImgEcrite, nbVoisins, intensite );
    else   
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////     WIENER     /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Wiener_G( char *cNomImgLue, char* cNomImgEcrite, int voisins, float Var_Bruit, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Wiener_RGB( char *cNomImgLue, char* cNomImgEcrite, int voisins, float Var_Bruit, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Wiener(char* cNomImgLue, char* cNomImgEcrite, float nbVoisins, float variance, float intensite )
{   
    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 ) Wiener_G( cNomImgLue, /*cNomImgLueLocation, OutDir*/ cNomImgEcrite, nbVoisins, variance, intensite );
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) Wiener_RGB( cNomImgLue, /*cNomImgLueLocation, OutDir*/ cNomImgEcrite, nbVoisins, variance, intensite );
    else   
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////     GAUSSIEN     ////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Gaussien_Denoise_G( char *cNomImgLue, char* cNomImgEcrite, int voisins, float Mean, float Var, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free( filter );
}

void Gaussien_Denoise_RGB( char *cNomImgLue, char* cNomImgEcrite, int voisins, float Mean, float Var, float intensite = 1. )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut); free( filter );
}

void Gaussien_Denoise( char* cNomImgLue, char* cNomImgEcrite, float nbVoisins, float moyenne, float variance, float intensite)
{   

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp(cDernieresLettres, "pgm" ) == 0 )      Gaussien_Denoise_G( cNomImgLue, cNomImgEcrite, nbVoisins, moyenne, variance, intensite );
    else if( strcmp(cDernieresLettres, "ppm" ) == 0 ) Gaussien_Denoise_RGB( cNomImgLue, cNomImgEcrite, nbVoisins, moyenne, variance, intensite );
    else   
        printf("Extension %s inconnue.\n", cDernieresLettres );
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

void Gradient_G( char *cNomImgLue, char* cNomImgEcrite, int voisins, bool use_MOY, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut, *ImgGrad;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(ImgGrad, OCTET, nTaille);

    int nWF = voisins*2+1;
    
    float* filter = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    float* filter_default = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter_default[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    float* ImgGradF = (float*) malloc( sizeof( float ) * nTaille );
    std::vector<float> vals;

    for (int i = 0; i < nH-1; i++) 
        for (int j = 0; j < nW-1; j++) 
        {
            float val = sqrt( pow( ImgIn[(i+1)*nW+j]- ImgIn[i*nW+j] , 2 ) + pow( ImgIn[i*nW+j]- ImgIn[i*nW+j+1], 2 ) );
            vals.push_back( val );
            ImgGradF[i*nW+j] = val;
        }

    std::sort( vals.begin(), vals.end() );
    
    float Seuil = vals[ nTaille/2 ];

    for (int i = 0; i < nH-1; i++) 
        for (int j = 0; j < nW-1; j++) 
            ImgGrad[i*nW+j] = (ImgGradF[i*nW+j]<Seuil)?(0):(255);
    
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

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free( ImgGrad ); free(ImgOut); free( filter ); free( filter_default );free( ImgGradF );
}

void Gradient_RGB( char *cNomImgLue, char* cNomImgEcrite, int voisins, bool use_MOY, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut, *ImgGrad;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);
    allocation_tableau(ImgGrad, OCTET, nTaille);

    int nWF = voisins*2+1;
    
    float* filter = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    float* filter_default = ( float* ) malloc( sizeof(float) * nWF * nWF );

    for( int k = -voisins; k <= voisins; k++ )
        for( int l = -voisins; l <= voisins; l++ )
            filter_default[(k+voisins)*nWF+(l+voisins)] = exp( - ( pow( k, 2 ) +  pow( l, 2 )  ) / 2. ) / ( 2. * M_PI );

    float* ImgGradF = (float*) malloc( sizeof( float ) * nTaille );
    std::vector<float> vals[3];

    for (int i = 0; i < nH-1; i++) 
        for (int j = 0; j < nW-1; j++) 
            for( uint d = 0; d < 3; d++ )
            {
                float val = sqrt( pow( ImgIn[((i+1)*nW+j)*3+d]- ImgIn[(i*nW+j)*3+d] , 2 ) + pow( ImgIn[(i*nW+j+1)*3+d]- ImgIn[(i*nW+j)*3+d], 2 ) );
                vals[d].push_back( val );
                ImgGradF[(i*nW+j)*3+d] = val;
            }
    for( uint d = 0; d < 3; d++ )
        std::sort( vals[d].begin(), vals[d].end() );
    
    float SeuilR = vals[0][ nTaille/6 ];
    float SeuilG = vals[1][ nTaille/6 ];
    float SeuilB = vals[2][ nTaille/6 ];

    for (int i = 0; i < nH-1; i++) 
        for (int j = 0; j < nW-1; j++) 
        {
            ImgGrad[(i*nW+j)*3] = (ImgGradF[(i*nW+j)*3]<SeuilR)?(0):(255);
            ImgGrad[(i*nW+j)*3+1] = (ImgGradF[(i*nW+j)*3+1]<SeuilG)?(0):(255);
            ImgGrad[(i*nW+j)*3+2] = (ImgGradF[(i*nW+j)*3+2]<SeuilB)?(0):(255);
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

                ImgOut[(i*nW+j)*3+d] = ImgGrad[(i*nW+j)*3+d];//round( clip( intensite*new_val + (1.-intensite)*old_val, 0., 255. ) );
            }

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free( ImgGrad ); free(ImgOut); free( filter ); free( filter_default ); free( ImgGradF );
}

void Gradient(char* cNomImgLue, char* cNomImgEcrite, float nbVoisins, float moyenne, float intensite)
{

    bool use_MOY = false;
    if (moyenne > 0) {
        use_MOY = true;
    }

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )       Gradient_G( cNomImgLue, cNomImgEcrite, nbVoisins, use_MOY, intensite );
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 )  Gradient_RGB( cNomImgLue, cNomImgEcrite, nbVoisins, use_MOY, intensite );
    else   
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////     MOYENNEUR PONDEREE     ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pondere_G( char *cNomImgLue, char* cNomImgEcrite, int voisins, float power, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Pondere_RGB( char *cNomImgLue, char* cNomImgEcrite, int voisins, float power, float intensite )
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW * 3;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

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

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
}

void Pondere(char* cNomImgLue, char* cNomImgEcrite, float nbVoisins, float puissance, float intensite/*char* extension, char* OutDir*/ )
{   

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )       Pondere_G( cNomImgLue, cNomImgEcrite, nbVoisins, puissance, intensite );
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 )  Pondere_RGB( cNomImgLue, cNomImgEcrite, nbVoisins, puissance, intensite );
    else   
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////     NON LOCAL MEANS     /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void calculNonLocalMeans(int m, int nH, int nW, int tailleFenetre,float sigma, float h, OCTET *ImgIn, OCTET *ImgOut) {

    OCTET *patchActuel;
    allocation_tableau(patchActuel, OCTET, tailleFenetre * tailleFenetre);
    
    OCTET *patchVoisin;
    allocation_tableau(patchVoisin, OCTET, tailleFenetre * tailleFenetre);

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
}

void NonLocalMeans_G(char *cNomImgLue, char* cNomImgEcrite, float sigma, float h, int tailleFenetre) {

    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);


    int m = tailleFenetre/2;
    calculNonLocalMeans(m, nH, nW, tailleFenetre, sigma, h, ImgIn, ImgOut);

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn); free(ImgOut);
}

void NonLocalMeans_RGB(char *cNomImgLue, char* cNomImgEcrite, float sigma, float h, int tailleFenetre) {

    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    OCTET *tabIdR, *tabIdG, *tabIdB;

    allocation_tableau(tabIdR, OCTET, nTaille);
    allocation_tableau(tabIdG, OCTET, nTaille);
    allocation_tableau(tabIdB, OCTET, nTaille);

    planR(tabIdR, ImgIn, nTaille);
    planV(tabIdG, ImgIn, nTaille);
    planB(tabIdB, ImgIn, nTaille);

    OCTET *traitementR, *traitementG, *traitementB;

    allocation_tableau(traitementR, OCTET, nTaille);
    allocation_tableau(traitementG, OCTET, nTaille);
    allocation_tableau(traitementB, OCTET, nTaille);

    int m = tailleFenetre/2;

    calculNonLocalMeans(m, nH, nW, tailleFenetre, sigma, h, tabIdR, traitementR);
    calculNonLocalMeans(m, nH, nW, tailleFenetre, sigma, h, tabIdG, traitementG);
    calculNonLocalMeans(m, nH, nW, tailleFenetre, sigma, h, tabIdB, traitementB);

    for (int elt=0; elt < nTaille3; elt+=3)
    {
      ImgOut[elt] = traitementR[elt/3];
      ImgOut[elt+1] = traitementG[elt/3];
      ImgOut[elt+2] = traitementB[elt/3];
    }
    
    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);
}

void NonLocalMeans(char* cNomImgLue, char* cNomImgEcrite, float ponderation, float tailleFenetreRecherche, float tailleFenetre)
{

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )      NonLocalMeans_G( cNomImgLue, cNomImgEcrite, ponderation, tailleFenetreRecherche, tailleFenetre);
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) NonLocalMeans_RGB( cNomImgLue, cNomImgEcrite, ponderation, tailleFenetreRecherche, tailleFenetre);
    else   
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

void calculBruitPoivreEtSel(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, float proportion) {
    srand(time(NULL));

    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            float randNb = (float)(rand()) / (float)(RAND_MAX);

            if (randNb < proportion / 2.0) {
                ImgOut[i * nW + j] = 0;
            } else if (randNb > 1 - proportion / 2.0) {
                ImgOut[i * nW + j] = 255;
            } else {
                ImgOut[i * nW + j] = ImgIn[i * nW + j];
            }
        }
    }
}


void PoivreEtSel_G(char *cNomImgLue, char* cNomImgEcrite, float proportion)
{
    int nH, nW, nTaille;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    calculBruitPoivreEtSel(ImgIn, ImgOut, nW, nH, proportion);

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void PoivreEtSel_RGB(char *cNomImgLue, char* cNomImgEcrite ,float proportion)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;


    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille3);


    OCTET *tabIdR, *tabIdG, *tabIdB;

    allocation_tableau(tabIdR, OCTET, nTaille);
    allocation_tableau(tabIdG, OCTET, nTaille);
    allocation_tableau(tabIdB, OCTET, nTaille);

    planR(tabIdR, ImgIn, nTaille);
    planV(tabIdG, ImgIn, nTaille);
    planB(tabIdB, ImgIn, nTaille);

    OCTET *traitementR, *traitementG, *traitementB;

    allocation_tableau(traitementR, OCTET, nTaille);
    allocation_tableau(traitementG, OCTET, nTaille);
    allocation_tableau(traitementB, OCTET, nTaille);

    calculBruitPoivreEtSel(tabIdR, traitementR, nW, nH, proportion);
    calculBruitPoivreEtSel(tabIdG, traitementG, nW, nH, proportion);
    calculBruitPoivreEtSel(tabIdB, traitementB, nW, nH, proportion);

    for (int elt=0; elt < nTaille3; elt+=3)
    {
        ImgOut[elt] = traitementR[elt/3];
        ImgOut[elt+1] = traitementG[elt/3];
        ImgOut[elt+2] = traitementB[elt/3];
    }

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void PoivreEtSel(char* cNomImgLue, char* cNomImgEcrite,  float proportion)
{

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )      PoivreEtSel_G( cNomImgLue, cNomImgEcrite, proportion);
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) PoivreEtSel_RGB( cNomImgLue, cNomImgEcrite, proportion);
    else
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

///////////////////////////////////////////////////////////////////////     GAUSSIEN     ///////////////////////////////////////////////////////////////////////

float genererNombreGaussien(int moyenne, int ecartType) {

    float nb1 = (float)(rand())/(float)(RAND_MAX);
    float nb2 = (float)(rand())/(float)(RAND_MAX);

    float z = sqrt(-2 * log(nb1)) * cos(2 * M_PI * nb2);

    return (float)(z * ecartType + moyenne);
}

void calculBruitGaussien(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, float moyenne, float ecartType, std::vector<float> &distribution) {

    distribution.clear();
    std::srand(std::time(0));

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW; j++)
        {

            float gaussien = genererNombreGaussien(moyenne, ecartType);

            float val = ImgIn[i*nW+j] + gaussien;
            distribution.push_back(gaussien);

            if (val < 0) {
                ImgOut[i * nW + j] = 0;
            } else if (val> 255) {
                ImgOut[i * nW + j] = 255;
            }
            else {
                ImgOut[i*nW+j] = val;
            }
        }
    }
}

void Gaussien_Noise_G(char *cNomImgLue, char *cNomImgEcrite, float moyenne, float ecartType)
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    std::vector<float> distribution;
    calculBruitGaussien(ImgIn, ImgOut, nW, nH, moyenne, ecartType, distribution);

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Gaussien_Noise_RGB(char *cNomImgLue, char *cNomImgEcrite, float moyenne, float ecartType)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;


    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    OCTET *tabIdR, *tabIdG, *tabIdB;

    allocation_tableau(tabIdR, OCTET, nTaille);
    allocation_tableau(tabIdG, OCTET, nTaille);
    allocation_tableau(tabIdB, OCTET, nTaille);

    planR(tabIdR, ImgIn, nTaille);
    planV(tabIdG, ImgIn, nTaille);
    planB(tabIdB, ImgIn, nTaille);

    OCTET *traitementR, *traitementG, *traitementB;

    allocation_tableau(traitementR, OCTET, nTaille);
    allocation_tableau(traitementG, OCTET, nTaille);
    allocation_tableau(traitementB, OCTET, nTaille);

    std::vector<float> distributionR;
    std::vector<float> distributionG;
    std::vector<float> distributionB;
    calculBruitGaussien(tabIdR, traitementR, nW, nH, moyenne, ecartType, distributionR);
    calculBruitGaussien(tabIdG, traitementG, nW, nH, moyenne, ecartType, distributionG);
    calculBruitGaussien(tabIdB, traitementB, nW, nH, moyenne, ecartType, distributionB);

    for (int elt=0; elt < nTaille3; elt+=3)
    {
        ImgOut[elt] = traitementR[elt/3];
        ImgOut[elt+1] = traitementG[elt/3];
        ImgOut[elt+2] = traitementB[elt/3];
    }

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void Gaussien_Noise(char* cNomImgLue, char* cNomImgEcrite, float moyenne, float ecartType )
{

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )      Gaussien_Noise_G( cNomImgLue, cNomImgEcrite, moyenne, ecartType);
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) Gaussien_Noise_RGB( cNomImgLue, cNomImgEcrite, moyenne, ecartType);
    else
        printf("Extension %s inconnue.\n", cDernieresLettres );
}


///////////////////////////////////////////////////////////////////////     POISSON     ///////////////////////////////////////////////////////////////////////

int genererNombrePoisson(float moyenne) {
    float L = exp(-moyenne);
    int k = 0;
    float p = 1.0;

    do {
        k++;
        float u = (float)(rand()) / (float)(RAND_MAX);
        p *= u;
    } while (p > L);

    return k - 1;
}

void calculBruitPoisson(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, float moyenne, std::vector<float> &distribution) {

    distribution.clear();
    std::srand(std::time(0));

    for (int i=0; i < nH; i++)
    {
        for (int j=0; j < nW; j++)
        {

            float gaussien = genererNombrePoisson(moyenne);

            float val = ImgIn[i*nW+j] + gaussien;
            distribution.push_back(gaussien);

            if (val < 0) {
                ImgOut[i * nW + j] = 0;
            } else if (val> 255) {
                ImgOut[i * nW + j] = 255;
            }
            else {
                ImgOut[i*nW+j] = val;
            }
        }
    }
}

void Poisson_G(char *cNomImgLue, char* cNomImgEcrite, float moyenne)
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    std::vector<float> distribution;
    calculBruitPoisson(ImgIn, ImgOut, nW, nH, moyenne, distribution);

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Poisson_RGB(char *cNomImgLue, char* cNomImgEcrite, float moyenne)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;


    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    OCTET *tabIdR, *tabIdG, *tabIdB;

    allocation_tableau(tabIdR, OCTET, nTaille);
    allocation_tableau(tabIdG, OCTET, nTaille);
    allocation_tableau(tabIdB, OCTET, nTaille);

    planR(tabIdR, ImgIn, nTaille);
    planV(tabIdG, ImgIn, nTaille);
    planB(tabIdB, ImgIn, nTaille);

    OCTET *traitementR, *traitementG, *traitementB;

    allocation_tableau(traitementR, OCTET, nTaille);
    allocation_tableau(traitementG, OCTET, nTaille);
    allocation_tableau(traitementB, OCTET, nTaille);

    std::vector<float> distributionR;
    std::vector<float> distributionG;
    std::vector<float> distributionB;
    calculBruitPoisson(tabIdR, traitementR, nW, nH, moyenne, distributionR);
    calculBruitPoisson(tabIdG, traitementG, nW, nH, moyenne, distributionG);
    calculBruitPoisson(tabIdB, traitementB, nW, nH, moyenne, distributionB);

    for (int elt=0; elt < nTaille3; elt+=3)
    {
        ImgOut[elt] = traitementR[elt/3];
        ImgOut[elt+1] = traitementG[elt/3];
        ImgOut[elt+2] = traitementB[elt/3];
    }

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);
}

void Poisson(char* cNomImgLue, char* cNomImgEcrite, float moyenne )
{

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp(cDernieresLettres, "pgm" ) == 0 )      Poisson_G( cNomImgLue, cNomImgEcrite, moyenne);
    else if( strcmp(cDernieresLettres, "ppm" ) == 0 ) Poisson_RGB( cNomImgLue, cNomImgEcrite, moyenne);
    else
        printf("Extension %s inconnue.\n", cDernieresLettres);
}


////////////////////////////////s///////////////////////////////////////     IMPULSIF     ///////////////////////////////////////////////////////////////////////

void calculBruitImpulsif(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, float facteur) {

    srand(time(NULL));

    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int randValue = rand() % 100;

            if (randValue < facteur) {
                int val = ImgIn[i * nW + j] / 2;

                if (val < 0) {
                    ImgOut[i * nW + j] = 0;
                } else if (val > 255) {
                    ImgOut[i * nW + j] = 255;
                } else {
                    ImgOut[i * nW + j] = val;
                }
            }
            else {
                ImgOut[i * nW + j] = ImgIn[i * nW + j];
            }
        }
    }
}

void Impulsif_G(char *cNomImgLue, char* cNomImgEcrite, int facteur)
{
    int nH, nW, nTaille;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);
    calculBruitImpulsif(ImgIn, ImgOut, nW, nH, facteur);

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Impulsif_RGB(char *cNomImgLue, char* cNomImgEcrite, int facteur)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    OCTET *tabIdR, *tabIdG, *tabIdB;

    allocation_tableau(tabIdR, OCTET, nTaille);
    allocation_tableau(tabIdG, OCTET, nTaille);
    allocation_tableau(tabIdB, OCTET, nTaille);

    planR(tabIdR, ImgIn, nTaille);
    planV(tabIdG, ImgIn, nTaille);
    planB(tabIdB, ImgIn, nTaille);

    OCTET *traitementR, *traitementG, *traitementB;

    allocation_tableau(traitementR, OCTET, nTaille);
    allocation_tableau(traitementG, OCTET, nTaille);
    allocation_tableau(traitementB, OCTET, nTaille);

    calculBruitImpulsif(tabIdR, traitementR, nW, nH, facteur);
    calculBruitImpulsif(tabIdG, traitementG, nW, nH, facteur);
    calculBruitImpulsif(tabIdB, traitementB, nW, nH, facteur);

    for (int elt=0; elt < nTaille3; elt+=3)
    {
        ImgOut[elt] = traitementR[elt/3];
        ImgOut[elt+1] = traitementG[elt/3];
        ImgOut[elt+2] = traitementB[elt/3];
    }

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void Impulsif(char* cNomImgLue, char* cNomImgEcrite, float facteur )
{

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )      Impulsif_G( cNomImgLue, cNomImgEcrite, facteur);
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) Impulsif_RGB( cNomImgLue, cNomImgEcrite, facteur);
    else
        printf("Extension %s inconnue.\n", cDernieresLettres );
}


///////////////////////////////////////////////////////////////////////     SPECKLE     ///////////////////////////////////////////////////////////////////////


void calculBruitSpeckle(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, int intensite) {
    srand(time(NULL));

    for (int i = 0; i < nH; i++) {
        for (int j = 0; j < nW; j++) {
            int speckle = -intensite + rand() % (intensite - (-intensite) + 1);

            int val = ImgIn[i*nW+j] + speckle;

            if (val < 0) {
                ImgOut[i * nW + j] = 0;
            } else if (val > 255) {
                ImgOut[i * nW + j] = 255;
            } else {
                ImgOut[i * nW + j] = val;
            }
        }
    }
}

void Speckle_G(char *cNomImgLue, char* cNomImgEcrite, int intensite)
{
    int nH, nW, nTaille;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    calculBruitSpeckle(ImgIn, ImgOut, nW, nH, intensite);

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Speckle_RGB(char *cNomImgLue, char* cNomImgEcrite, int intensite)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille3);

    OCTET *tabIdR, *tabIdG, *tabIdB;

    allocation_tableau(tabIdR, OCTET, nTaille);
    allocation_tableau(tabIdG, OCTET, nTaille);
    allocation_tableau(tabIdB, OCTET, nTaille);

    planR(tabIdR, ImgIn, nTaille);
    planV(tabIdG, ImgIn, nTaille);
    planB(tabIdB, ImgIn, nTaille);

    OCTET *traitementR, *traitementG, *traitementB;

    allocation_tableau(traitementR, OCTET, nTaille);
    allocation_tableau(traitementG, OCTET, nTaille);
    allocation_tableau(traitementB, OCTET, nTaille);

    calculBruitSpeckle(tabIdR, traitementR, nW, nH, intensite);
    calculBruitSpeckle(tabIdG, traitementG, nW, nH, intensite);
    calculBruitSpeckle(tabIdB, traitementB, nW, nH, intensite);

    for (int elt=0; elt < nTaille3; elt+=3)
    {
        ImgOut[elt] = traitementR[elt/3];
        ImgOut[elt+1] = traitementG[elt/3];
        ImgOut[elt+2] = traitementB[elt/3];
    }

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void Speckle(char* cNomImgLue, char* cNomImgEcrite, float intensite )
{

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if( strcmp( cDernieresLettres, "pgm" ) == 0 )      Speckle_G( cNomImgLue, cNomImgEcrite, intensite);
    else if( strcmp( cDernieresLettres, "ppm" ) == 0 ) Speckle_RGB( cNomImgLue, cNomImgEcrite, intensite);
    else
        printf("Extension %s inconnue.\n", cDernieresLettres );
}

//PSNR

float calculPSNR_G(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn1, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille);
    lire_image_pgm(cNomImgLue2, ImgIn2, nTaille);

    float EQM = 0, PSNR;

    for (int i=0; i < nTaille; i++) {
        EQM += pow((ImgIn1[i]-ImgIn2[i]), 2);
    }

    EQM/= nTaille;

    PSNR = 10*log10(pow(255, 2)/EQM);

    return PSNR;
}

float calculPSNR_RGB(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille, nTaille3;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = 3 * nTaille;

    allocation_tableau(ImgIn1, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue2, ImgIn2, nTaille);

    float EQM_R = 0, EQM_G = 0, EQM_B = 0, PSNR_R, PSNR_G, PSNR_B;

    for (int i = 0; i < nTaille3; i += 3) {
        EQM_R += pow((ImgIn1[i] - ImgIn2[i]), 2);
        EQM_G += pow((ImgIn1[i + 1] - ImgIn2[i + 1]), 2);
        EQM_B += pow((ImgIn1[i + 2] - ImgIn2[i + 2]), 2);
    }

    EQM_R /= nTaille;
    EQM_G /= nTaille;
    EQM_B /= nTaille;

    PSNR_R = 10 * log10(pow(255, 2) / EQM_R);
    PSNR_G = 10 * log10(pow(255, 2) / EQM_G);
    PSNR_B = 10 * log10(pow(255, 2) / EQM_B);

    float PSNR = (PSNR_R + PSNR_G + PSNR_B) / 3.0;

    return PSNR;
}


//SNR

float calculSNR_G(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn1, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille);
    lire_image_pgm(cNomImgLue2, ImgIn2, nTaille);

    float puissanceSignalUtiles = 0.0;
    float puissanceBruit = 0.0;

    for (int i = 0; i < nTaille; i++) {
        puissanceSignalUtiles += pow(ImgIn1[i], 2);
        puissanceBruit += pow(ImgIn1[i] - ImgIn2[i], 2);
    }

    puissanceSignalUtiles /= nTaille;
    puissanceBruit /= nTaille;

    float snr = 10 * log10(puissanceSignalUtiles / puissanceBruit);

    return snr;
}

float calculSNR_RGB(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille, nTaille3;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn1, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue2, ImgIn2, nTaille);

    float puissanceSignalUtiles_R = 0.0;
    float puissanceSignalUtiles_G = 0.0;
    float puissanceSignalUtiles_B = 0.0;

    float puissanceBruit_R = 0.0;
    float puissanceBruit_G = 0.0;
    float puissanceBruit_B = 0.0;

    for (int i = 0; i < nTaille3; i += 3) {
        puissanceSignalUtiles_R += pow(ImgIn1[i], 2);
        puissanceSignalUtiles_G += pow(ImgIn1[i+1], 2);
        puissanceSignalUtiles_B += pow(ImgIn1[i+2], 2);

        puissanceBruit_R += pow(ImgIn1[i] - ImgIn2[i], 2);
        puissanceBruit_G += pow(ImgIn1[i+1] - ImgIn2[i+1], 2);
        puissanceBruit_B += pow(ImgIn1[i+2] - ImgIn2[i+2], 2);
    }

    puissanceSignalUtiles_R /= nTaille;
    puissanceSignalUtiles_G /= nTaille;
    puissanceSignalUtiles_B /= nTaille;

    puissanceBruit_R /= nTaille;
    puissanceBruit_G /= nTaille;
    puissanceBruit_B /= nTaille;

    float snr_R = 10 * log10(puissanceSignalUtiles_R / puissanceBruit_R);
    float snr_G = 10 * log10(puissanceSignalUtiles_G / puissanceBruit_G);
    float snr_B = 10 * log10(puissanceSignalUtiles_B / puissanceBruit_B);

    float snr = (snr_R + snr_G + snr_B)/3.0;

    return snr;
}


//SSIM

const double C1 = 6.5025, C2 = 58.5225; // Constants

float calculMoyenne(OCTET *image, int nTaille) {
    float somme = 0.0;

    for (int i = 0; i < nTaille; i++) {
        somme += image[i];
    }

    return somme / nTaille;
}

float calculVariance(OCTET *image, float moyenne, int nTaille) {
    float sommeCarres = 0.0;

    for (int i = 0; i < nTaille; i++) {
        sommeCarres += pow(image[i] - moyenne, 2);
    }

    return sommeCarres / nTaille;
}

float calculCovariance(OCTET *img1, OCTET *img2, float moyenne1, float moyenne2, int nTaille) {
    float sommeProduits = 0.0;

    for (int i = 0; i < nTaille; i++) {
        sommeProduits += (img1[i] - moyenne1) * (img2[i] - moyenne2);
    }

    return sommeProduits / nTaille;
}

float calculSSIM_G(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn1, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille);
    lire_image_pgm(cNomImgLue2, ImgIn2, nTaille);

    float muX = calculMoyenne(ImgIn1, nTaille);
    float muY = calculMoyenne(ImgIn2, nTaille);

    float sigmaX = sqrt(calculVariance(ImgIn1, muX, nTaille));
    float sigmaY = sqrt(calculVariance(ImgIn2, muY, nTaille));

    float sigmaXY = calculCovariance(ImgIn1, ImgIn2, muX, muY, nTaille);

    float num = (2 * muX * muY + C1) * (2 * sigmaXY + C2);
    float denom = (muX * muX + muY * muY + C1) * (sigmaX * sigmaX + sigmaY * sigmaY + C2);

    return num / denom;
}

float calculSSIM_RGB(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille, nTaille3;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn1, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue2, ImgIn2, nTaille);

    OCTET *ImgIn1_R, *ImgIn1_G, *ImgIn1_B;
    OCTET *ImgIn2_R, *ImgIn2_G, *ImgIn2_B;

    allocation_tableau(ImgIn1_R, OCTET, nTaille);
    allocation_tableau(ImgIn1_G, OCTET, nTaille);
    allocation_tableau(ImgIn1_B, OCTET, nTaille);

    allocation_tableau(ImgIn2_R, OCTET, nTaille);
    allocation_tableau(ImgIn2_G, OCTET, nTaille);
    allocation_tableau(ImgIn2_B, OCTET, nTaille);

    for (int i = 0; i < nTaille; i++) {
        ImgIn1_R[i] = ImgIn1[3 * i];
        ImgIn1_G[i] = ImgIn1[3 * i + 1];
        ImgIn1_B[i] = ImgIn1[3 * i + 2];

        ImgIn2_R[i] = ImgIn2[3 * i];
        ImgIn2_G[i] = ImgIn2[3 * i + 1];
        ImgIn2_B[i] = ImgIn2[3 * i + 2];
    }

    // Calculer les moyennes pour chaque canal
    float muX_R = calculMoyenne(ImgIn1_R, nTaille);
    float muY_R = calculMoyenne(ImgIn2_R, nTaille);

    float muX_G = calculMoyenne(ImgIn1_G, nTaille);
    float muY_G = calculMoyenne(ImgIn2_G, nTaille);

    float muX_B = calculMoyenne(ImgIn1_B, nTaille);
    float muY_B = calculMoyenne(ImgIn2_B, nTaille);

    // Calculer les écarts types pour chaque canal
    float sigmaX_R = sqrt(calculVariance(ImgIn1_R, muX_R, nTaille));
    float sigmaY_R = sqrt(calculVariance(ImgIn2_R, muY_R, nTaille));

    float sigmaX_G = sqrt(calculVariance(ImgIn1_G, muX_G, nTaille));
    float sigmaY_G = sqrt(calculVariance(ImgIn2_G, muY_G, nTaille));

    float sigmaX_B = sqrt(calculVariance(ImgIn1_B, muX_B, nTaille));
    float sigmaY_B = sqrt(calculVariance(ImgIn2_B, muY_B, nTaille));

    // Calculer les covariances pour chaque canal
    float sigmaXY_R = calculCovariance(ImgIn1_R, ImgIn2_R, muX_R, muY_R, nTaille);
    float sigmaXY_G = calculCovariance(ImgIn1_G, ImgIn2_G, muX_G, muY_G, nTaille);
    float sigmaXY_B = calculCovariance(ImgIn1_B, ImgIn2_B, muX_B, muY_B, nTaille);

    // Calculer le SSIM pour chaque canal
    float num_R = (2 * muX_R * muY_R + C1) * (2 * sigmaXY_R + C2);
    float denom_R = (muX_R * muX_R + muY_R * muY_R + C1) * (sigmaX_R * sigmaY_R + C2);

    float num_G = (2 * muX_G * muY_G + C1) * (2 * sigmaXY_G + C2);
    float denom_G = (muX_G * muX_G + muY_G * muY_G + C1) * (sigmaX_G * sigmaY_G + C2);

    float num_B = (2 * muX_B * muY_B + C1) * (2 * sigmaXY_B + C2);
    float denom_B = (muX_B * muX_B + muY_B * muY_B + C1) * (sigmaX_B * sigmaY_B + C2);

    // Calculer la moyenne des SSIM sur les trois canaux
    float SSIM = (num_R / denom_R + num_G / denom_G + num_B / denom_B) / 3.0;

    // Libérer la mémoire allouée pour les tableaux des canaux
    free(ImgIn1_R);
    free(ImgIn1_G);
    free(ImgIn1_B);

    free(ImgIn2_R);
    free(ImgIn2_G);
    free(ImgIn2_B);

    return SSIM;
}


//RMSE

float calculRMSE_G(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn1, OCTET, nTaille);
    lire_image_pgm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille);
    lire_image_pgm(cNomImgLue2, ImgIn2, nTaille);

    float sommeCarresErreurs = 0.0;

    for (int i = 0; i < nTaille; i++) {
        sommeCarresErreurs += pow(ImgIn1[i] - ImgIn2[i], 2);
    }

    float rmse = sqrt(sommeCarresErreurs / nTaille);

    return rmse;
}

float calculRMSE_RGB(char* cNomImgLue1, char* cNomImgLue2) {
    OCTET *ImgIn1, *ImgIn2;
    int nH, nW, nTaille, nTaille3;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLue1, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn1, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue1, ImgIn1, nTaille);

    allocation_tableau(ImgIn2, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue2, ImgIn2, nTaille);

    float sommeCarresErreurs_R = 0.0;
    float sommeCarresErreurs_G = 0.0;
    float sommeCarresErreurs_B = 0.0;

    for (int i = 0; i < nTaille3; i += 3) {
        sommeCarresErreurs_R += pow(ImgIn1[i] - ImgIn2[i], 2);
        sommeCarresErreurs_G += pow(ImgIn1[i + 1] - ImgIn2[i + 1], 2);
        sommeCarresErreurs_B += pow(ImgIn1[i + 2] - ImgIn2[i + 2], 2);
    }

    float rmse_R = sqrt(sommeCarresErreurs_R / nTaille);
    float rmse_G = sqrt(sommeCarresErreurs_G / nTaille);
    float rmse_B = sqrt(sommeCarresErreurs_B / nTaille);

    float rmse = (rmse_R + rmse_G + rmse_B) / 3.0;

    return rmse;
}

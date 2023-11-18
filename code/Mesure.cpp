#include <stdio.h>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include "image_ppm.h"


//PSNR

float calculPSNR(OCTET *ImgIn, OCTET *ImgIn2, int nTaille) {
    float EQM = 0, PSNR;

    for (int i=0; i < nTaille; i++) {
        EQM += pow((ImgIn[i]-ImgIn2[i]), 2);
    }

    EQM/= nTaille;

    PSNR = 10*log10(pow(255, 2)/EQM);

    return PSNR;
}


//SNR

float calculSNR(OCTET *ImgIn, OCTET *ImgIn2, int nTaille) {
    float puissanceSignalUtiles = 0.0;
    float puissanceBruit = 0.0;

    for (int i = 0; i < nTaille; i++) {
        puissanceSignalUtiles += pow(ImgIn[i], 2);
        puissanceBruit += pow(ImgIn[i] - ImgIn2[i], 2);
    }

    puissanceSignalUtiles /= nTaille;
    puissanceBruit /= nTaille;

    float snr = 10 * log10(puissanceSignalUtiles / puissanceBruit);

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

float calculSSIM(OCTET *ImgIn, OCTET *ImgIn2, int nTaille) {
    float muX = calculMoyenne(ImgIn, nTaille);
    float muY = calculMoyenne(ImgIn2, nTaille);

    float sigmaX = sqrt(calculVariance(ImgIn, muX, nTaille));
    float sigmaY = sqrt(calculVariance(ImgIn2, muY, nTaille));

    float sigmaXY = calculCovariance(ImgIn, ImgIn2, muX, muY, nTaille);

    float num = (2 * muX * muY + C1) * (2 * sigmaXY + C2);
    float denom = (muX * muX + muY * muY + C1) * (sigmaX * sigmaX + sigmaY * sigmaY + C2);

    return num / denom;
}


//RMSE

float calculRMSE(OCTET *ImgIn, OCTET *ImgIn2, int nTaille) {
    float sommeCarresErreurs = 0.0;

    for (int i = 0; i < nTaille; i++) {
        sommeCarresErreurs += pow(ImgIn[i] - ImgIn2[i], 2);
    }

    float rmse = sqrt(sommeCarresErreurs / nTaille);

    return rmse;
}

enum MEASURES
{
    PSNR,
    SNR, 
    SSIM, 
    RMSE, 
    NB_MEASURES
};

float (*measure_functions[NB_MEASURES]) ( OCTET*, OCTET*, int ) = { calculPSNR, calculSNR, calculSSIM, calculRMSE };

char* TAG[NB_MEASURES] = { (char*) "PSNR", (char*) "SNR", (char*) "SSIM", (char*) "RMSE" };

int main(int argc, char* argv[])
{
    if( argc < 4 ) 
    {
        printf("Usage: Img1 Img2 mode txtFile?\n"); 
        exit(1);
    }

    OCTET* ImgIn1, *ImgIn2;
    int nH1, nW1, nH2, nW2;
    char cDernieresLettres[4];

    int longueur = strlen(argv[1]);
    strcpy(cDernieresLettres, argv[1] + longueur - 3);

    if( !strcmp( cDernieresLettres, "pgm" ) ) 
    {
        lire_nb_lignes_colonnes_image_pgm(argv[1], &nH1, &nW1);
        allocation_tableau(ImgIn1, OCTET, nH1 * nW1);
        lire_image_pgm(argv[1], ImgIn1, nH1 * nW1);

        longueur = strlen(argv[2]);
        strcpy(cDernieresLettres, argv[2] + longueur - 3);
        if( strcmp( cDernieresLettres, "pgm" ) ) 
        {
            printf("format d'image incompatible\n");
            exit(1);
        }

        lire_nb_lignes_colonnes_image_pgm(argv[2], &nH2, &nW2);
        if( nH1 != nH2 || nW1 != nW2 )
        {
            printf("taille d'image incompatible\n");
            exit(1);
        }

        allocation_tableau(ImgIn2, OCTET, nH1 * nW1);
        lire_image_pgm(argv[2], ImgIn2, nH1 * nW1);
    }
    if( !strcmp( cDernieresLettres, "ppm" ) ) 
    {
        lire_nb_lignes_colonnes_image_ppm(argv[1], &nH1, &nW1);
        allocation_tableau(ImgIn1, OCTET, nH1 * nW1  *3);
        lire_image_ppm(argv[1], ImgIn1, nH1 * nW1 * 3);

        longueur = strlen(argv[2]);
        strcpy(cDernieresLettres, argv[2] + longueur - 3);
        if( strcmp( cDernieresLettres, "ppm" ) ) 
        {
            printf("format d'image incompatible\n");
            exit(1);
        }

        lire_nb_lignes_colonnes_image_ppm(argv[2], &nH2, &nW2);
        if( nH1 != nH2 || nW1 != nW2 )
        {
            printf("taille d'image incompatible\n");
            exit(1);
        }

        allocation_tableau(ImgIn2, OCTET, nH1 * nW1 * 3);
        lire_image_ppm(argv[2], ImgIn2, nH1 * nW1);
    }

    int mode = -1;
    for( uint i = 0; i < NB_MEASURES; i++ )
        if( !strcmp( argv[3], TAG[i] ) )
            mode = i;
    
    if( mode == -1 )
    {
        printf("mode %s invalide.\nMode valide : ", argv[3] );

        for( uint i = 0; i < NB_MEASURES-1; i++ )
            printf("%s, ", TAG[i]);

        printf("%s\n", TAG[NB_MEASURES-1] );
    }

    float res = (*measure_functions[mode])( ImgIn1, ImgIn2, (strcmp( cDernieresLettres, "ppm" ))?(nH1*nW1):(nH1*nW1*3) );

    printf("%s entre %s et %s : %f\n", TAG[mode], argv[1], argv[2], res );

    if( argc >= 5 )
    {
        FILE* f = fopen(argv[4], "a" );
        if( !f )
        {
            printf("Le fichier %s n'a pas pu être ouvert\n", argv[4] );
            exit(1);
        }

        fprintf( f, "%f\n", res );

        fclose(f);
    }

    return 0;
}
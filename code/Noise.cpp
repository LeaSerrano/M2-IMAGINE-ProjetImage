#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include "image_ppm.h"
#include <random>
#include "Mesure.cpp"
#include <fstream>
#include <ctime>

///////////////////////////////////////////////////////////////////////     POIVRE ET SEL     ///////////////////////////////////////////////////////////////////////


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


void PoivreEtSel_G(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, float proportion)
{
    int nH, nW, nTaille;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    calculBruitPoivreEtSel(ImgIn, ImgOut, nW, nH, proportion);

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_PoivreEtSel_") + std::to_string(proportion).c_str() + std::string(".pgm")).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void PoivreEtSel_RGB(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir ,float proportion)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;


    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

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

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_PoivreEtSel_") + std::to_string(proportion).c_str() + std::string(".ppm")).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void PoivreEtSel( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    float proportion = atof( argv[5] );

    if( strcmp( extension, "pgm" ) == 0 )      PoivreEtSel_G( cNomImgLue, cNomImgLueLocation, OutDir, proportion);
    else if( strcmp( extension, "ppm" ) == 0 ) PoivreEtSel_RGB( cNomImgLue, cNomImgLueLocation, OutDir, proportion);
    else   
        printf("Extension %s inconnue.\n", extension );
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

void Gaussien_G(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, float moyenne, float ecartType)
{
   int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    std::vector<float> distribution;
    calculBruitGaussien(ImgIn, ImgOut, nW, nH, moyenne, ecartType, distribution);

    //calculDistributionGaussienne(distribution, moyenne, ecartType, nTaille);

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Gaussien_") + std::to_string(moyenne).c_str() + std::string("_") + std::to_string(ecartType).c_str() + std::string(".pgm")).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Gaussien_RGB(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, float moyenne, float ecartType)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;


   lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

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

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Gaussien_") + std::to_string(moyenne).c_str() + std::string("_") + std::to_string(ecartType).c_str() + std::string(".ppm")).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void Gaussien( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    float moyenne = atof( argv[5] );
    float ecartType = atof( argv[6] );

    if( strcmp( extension, "pgm" ) == 0 )      Gaussien_G( cNomImgLue, cNomImgLueLocation, OutDir, moyenne, ecartType);
    else if( strcmp( extension, "ppm" ) == 0 ) Gaussien_RGB( cNomImgLue, cNomImgLueLocation, OutDir, moyenne, ecartType);
    else   
        printf("Extension %s inconnue.\n", extension );
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

void Poisson_G(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, float moyenne) 
{
    int nH, nW, nTaille;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    std::vector<float> distribution;
    calculBruitPoisson(ImgIn, ImgOut, nW, nH, moyenne, distribution);

    //calculDistributionPoisson(distribution, facteur, nTaille);

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Poisson_") + std::to_string(moyenne).c_str() + std::string(".pgm")).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Poisson_RGB(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, float moyenne) 
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;


   lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

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

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Poisson_") + std::to_string(moyenne).c_str() + std::string(".ppm")).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);
}

void Poisson( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    float moyenne = atof( argv[5] );

    if( strcmp( extension, "pgm" ) == 0 )      Poisson_G( cNomImgLue, cNomImgLueLocation, OutDir, moyenne);
    else if( strcmp( extension, "ppm" ) == 0 ) Poisson_RGB( cNomImgLue, cNomImgLueLocation, OutDir, moyenne);
    else   
        printf("Extension %s inconnue.\n", extension );
}


///////////////////////////////////////////////////////////////////////     IMPULSIF     ///////////////////////////////////////////////////////////////////////

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

void Impulsif_G(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int facteur) 
{
    int nH, nW, nTaille;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);
    calculBruitImpulsif(ImgIn, ImgOut, nW, nH, facteur);

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Impulsif_") + std::to_string(facteur).c_str() + std::string(".pgm")).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Impulsif_RGB(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int facteur) 
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

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

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Impulsif_") + std::to_string(facteur).c_str() + std::string(".ppm")).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void Impulsif( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int facteur = atoi( argv[5] );

    if( strcmp( extension, "pgm" ) == 0 )      Impulsif_G( cNomImgLue, cNomImgLueLocation, OutDir, facteur);
    else if( strcmp( extension, "ppm" ) == 0 ) Impulsif_RGB( cNomImgLue, cNomImgLueLocation, OutDir, facteur);
    else   
        printf("Extension %s inconnue.\n", extension );
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

void Speckle_G(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int intensite) 
{
    int nH, nW, nTaille;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;

    allocation_tableau(ImgIn, OCTET, nTaille);
    lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

    allocation_tableau(ImgOut, OCTET, nTaille);

    calculBruitSpeckle(ImgIn, ImgOut, nW, nH, intensite);

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Speckle_") + std::to_string(intensite).c_str() + std::string(".pgm")).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut);
}

void Speckle_RGB(char *cNomImgLue, char *cNomImgLueLocation, char* OutDir, int intensite) 
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    lire_nb_lignes_colonnes_image_ppm(cNomImgLueLocation, &nH, &nW);

    nTaille = nH * nW;
    nTaille3 = nTaille * 3;

    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLueLocation, ImgIn, nH * nW);

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


    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string(OutDir) + cNomImgLue + std::string("_Speckle_") + std::to_string(intensite).c_str() + std::string(".ppm")).c_str());

    ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);

    free(ImgIn); free(ImgOut); free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);

}

void Speckle( int argc, char** argv, char* cDirImgLue, char* cNomImgLue, char* extension, char* OutDir )
{   
    char cNomImgLueLocation[256];
    strcpy(cNomImgLueLocation, std::string(std::string(cDirImgLue) + cNomImgLue + std::string(".") + extension ).c_str());

    int intensite = atoi( argv[5] );

    if( strcmp( extension, "pgm" ) == 0 )      Speckle_G( cNomImgLue, cNomImgLueLocation, OutDir, intensite);
    else if( strcmp( extension, "ppm" ) == 0 ) Speckle_RGB( cNomImgLue, cNomImgLueLocation, OutDir, intensite);
    else   
        printf("Extension %s inconnue.\n", extension );
}

///////////////////////////////////////////////////////////////////////     MAIN     ///////////////////////////////////////////////////////////////////////

enum FILTER
{
    POIVREETSEL,
    GAUSSIEN,
    POISSON,
    IMPULSIF,
    SPECKLE, 
    NB_FILTERS
};

void (*filter_functions[NB_FILTERS]) ( int, char**, char*, char*, char*, char* ) = { PoivreEtSel, Gaussien, Poisson, Impulsif, Speckle };

char* TAG[NB_FILTERS] = { (char*) "PES", (char*) "GAU", (char*) "POI", (char*) "IMP", (char*) "SPE" };

int getMode( char* modeStr, int argc )
{
    int mode = 0;

    if( !strcmp( modeStr, TAG[POIVREETSEL] ) )
    {
        if( argc < 5 )
        {
            printf("Usage: DirIn ImageIn DirOut PES proportion\n"); 
            exit(1);
        }

        mode = POIVREETSEL;
    }
    else if( !strcmp( modeStr, TAG[GAUSSIEN] ) )
    {
        if( argc < 6 )
        {
            printf("Usage: DirIn ImageIn DirOut GAU moyenne ecart-type\n"); 
            exit(1);
        }

        mode = GAUSSIEN;
    }
    else if( !strcmp( modeStr, TAG[POISSON] ))
    {
        if( argc < 6 )
        {
            printf("Usage: DirIn ImageIn DirOut POI moyenne\n"); 
            exit(1);
        }

        mode = POISSON;
    }
    else if( !strcmp( modeStr, TAG[IMPULSIF] ) )
    {
        if( argc < 5 )
        {
            printf("Usage: DirIn ImageIn DirOut IMP facteur\n"); 
            exit(1);
        }

        mode = IMPULSIF;
    }
    else if( !strcmp( modeStr, TAG[SPECKLE] ) )
    {
        if( argc < 5 )
        {
            printf("Usage: DirIn ImageIn DirOut SPE intensite\n"); 
            exit(1);
        }
        
        mode = SPECKLE;
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
    char cDirImgLue[250], cNomImgLue[250];

    if( argc < 5 ) 
    {
        printf("Usage: DirIn ImageIn DirOut Mode_Débruitage\n"
                       "PES : bruit poivre et sel ( args : proportion )\n"
                       "GAU : bruit gaussien ( args : moyenne, ecart-type )\n"
                       "POI : bruit de poisson ( args : moyenne)\n"
                       "IMP : bruit impulsif ( args : facteur )\n"
                       "SPE : bruit speckle ( args : intensite )\n"); 
        exit(1);
    }
    else if( strlen( argv[4] ) != 3 )
    {
        printf("Le mode n'est pas valide.\nModes valides : PES, GAU, POI, IMP, SPE\n");
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
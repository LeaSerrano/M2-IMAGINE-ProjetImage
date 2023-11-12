#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include "image_ppm.h"
#include <random>

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


void bruitPoivreEtSel(char *cNomImgLue, char *cNomImgLueLocation, float proportion, bool estImgGrise)
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    if (estImgGrise) {
        lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

        nTaille = nH * nW;

        allocation_tableau(ImgIn, OCTET, nTaille);
        lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

        allocation_tableau(ImgOut, OCTET, nTaille);

        calculBruitPoivreEtSel(ImgIn, ImgOut, nW, nH, proportion);
    }
    else {
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

        free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);
    
    }

    if (estImgGrise) {
        folder = "Pgm";
        extension = "pgm";
    }
    else {
        folder = "Ppm";
        extension = "ppm";
    }

    char cNomImgEcrite[250];
    strcpy(cNomImgEcrite, std::string(std::string("Images/") + folder + std::string("/Noise/Salt_and_Pepper/") + cNomImgLue + std::string("_PoivreEtSel_") + std::to_string(proportion).c_str() + std::string(".") + extension).c_str());

    if (estImgGrise) {
        ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    }
    else {
        ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    }

    free(ImgIn); free(ImgOut);
}

float genererNombreGaussien(int moyenne, int ecartType, int p) {

    float nb1 = (float)(rand())/(float)(RAND_MAX);
    float nb2 = (float)(rand())/(float)(RAND_MAX);

    float z = sqrt(-2 * log(nb1)) * cos(2 * M_PI * nb2);

    return (float)(z * ecartType + moyenne);
}

void calculBruitGaussien(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, float moyenne, float ecartType) {

    for (int i=0; i < nH; i++) 
    {
        for (int j=0; j < nW; j++) 
        {
            float gaussien = genererNombreGaussien(moyenne, ecartType, ImgIn[i*nW+j]);

            int val = ImgIn[i*nW+j] + gaussien;

            if (val < 0) {
                ImgOut[i * nW + j] = 0;
            } else if (val> 255) {
                ImgOut[i * nW + j] = 255;
            }   
            else 
                ImgOut[i*nW+j] = val;
        }
    }
}

void bruitGaussien(char *cNomImgLue, char *cNomImgLueLocation, int moyenne, int ecartType, std::string name, bool estImgGrise) {

    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    if (estImgGrise) {
        lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

        nTaille = nH * nW;

        allocation_tableau(ImgIn, OCTET, nTaille);
        lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

        allocation_tableau(ImgOut, OCTET, nTaille);

        calculBruitGaussien(ImgIn, ImgOut, nW, nH, moyenne, ecartType);
    }
    else {
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
    
        calculBruitGaussien(tabIdR, traitementR, nW, nH, moyenne, ecartType);
        calculBruitGaussien(tabIdG, traitementG, nW, nH, moyenne, ecartType);
        calculBruitGaussien(tabIdB, traitementB, nW, nH, moyenne, ecartType);

        for (int elt=0; elt < nTaille3; elt+=3)
        {
          ImgOut[elt] = traitementR[elt/3];
          ImgOut[elt+1] = traitementG[elt/3];
          ImgOut[elt+2] = traitementB[elt/3];
        }

        free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);
    
    }

    if (estImgGrise) {
        folder = "Pgm";
        extension = "pgm";
    }
    else {
        folder = "Ppm";
        extension = "ppm";
    }

    char cNomImgEcrite[250];
    
    strcpy(cNomImgEcrite, std::string(std::string("Images/") + folder + std::string("/Noise/") + name + std::string("/") + cNomImgLue + std::string("_") + name + std::string("_") + std::to_string(moyenne).c_str() + std::string("_") + std::to_string(ecartType).c_str() + std::string(".") + extension).c_str());

    if (estImgGrise) {
        ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    }
    else {
        ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    }

    free(ImgIn); free(ImgOut);
    
}

void bruitPoisson(char *cNomImgLue, char *cNomImgLueLocation, float facteur, std::string name, bool estImgGrise) 
{
    bruitGaussien(cNomImgLue, cNomImgLueLocation, facteur, facteur, name, estImgGrise);
}

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

void bruitImpulsif(char *cNomImgLue, char *cNomImgLueLocation, int facteur, bool estImgGrise) 
{

    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    if (estImgGrise) {
        lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

        nTaille = nH * nW;

        allocation_tableau(ImgIn, OCTET, nTaille);
        lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

        allocation_tableau(ImgOut, OCTET, nTaille);
        calculBruitImpulsif(ImgIn, ImgOut, nW, nH, facteur);
    }
    else {
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

        free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);
    }

    if (estImgGrise) {
        folder = "Pgm";
        extension = "pgm";
    }
    else {
        folder = "Ppm";
        extension = "ppm";
    }

    char cNomImgEcrite[250];
    
    strcpy(cNomImgEcrite, std::string(std::string("Images/") + folder + std::string("/Noise/Impulsif/") + cNomImgLue + std::string("_Impulsif_") + std::to_string(facteur).c_str() + std::string(".") + extension).c_str());

    if (estImgGrise) {
        ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    }
    else {
        ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    }

    free(ImgIn); free(ImgOut);
}

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

void bruitSpeckle(char *cNomImgLue, char *cNomImgLueLocation, int intensite, bool estImgGrise) 
{
    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    if (estImgGrise) {
        lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

        nTaille = nH * nW;

        allocation_tableau(ImgIn, OCTET, nTaille);
        lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

        allocation_tableau(ImgOut, OCTET, nTaille);

        calculBruitSpeckle(ImgIn, ImgOut, nW, nH, intensite);
    }
    else {
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

        free(tabIdR); free(tabIdG); free(tabIdB); free(traitementR); free(traitementG); free(traitementB);
    }

    if (estImgGrise) {
        folder = "Pgm";
        extension = "pgm";
    }
    else {
        folder = "Ppm";
        extension = "ppm";
    }

    char cNomImgEcrite[250];
    
    strcpy(cNomImgEcrite, std::string(std::string("Images/") + folder + std::string("/Noise/Speckle/") + cNomImgLue + std::string("_Speckle_") + std::to_string(intensite).c_str() + std::string(".") + extension).c_str());

    if (estImgGrise) {
        ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    }
    else {
        ecrire_image_ppm(cNomImgEcrite, ImgOut,  nH, nW);
    }


    free(ImgIn); free(ImgOut);
}

int main(int argc, char* argv[])
{
  char cNomImgLue[250];
  
  if (argc != 2) 
     {
       printf("Usage: ImageIn\n"); 
       exit (1) ;
     }
   
    sscanf (argv[1],"%s",cNomImgLue);

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    if (strcmp(cDernieresLettres, "pgm") == 0) {
        char cNomImgLueLocation[250];
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Pgm/") + cNomImgLue).c_str());

        cNomImgLue[longueur - 4] = '\0';

        bruitPoivreEtSel(cNomImgLue, cNomImgLueLocation, 0.05, true);
        bruitPoivreEtSel(cNomImgLue, cNomImgLueLocation, 0.1, true);
        bruitPoivreEtSel(cNomImgLue, cNomImgLueLocation, 0.25, true);
        
        bruitGaussien(cNomImgLue, cNomImgLueLocation, 0, 25, "Gaussian", true);
        bruitGaussien(cNomImgLue, cNomImgLueLocation, 0, 10, "Gaussian", true);
        bruitGaussien(cNomImgLue, cNomImgLueLocation, 5, 10, "Gaussian", true);

        bruitPoisson(cNomImgLue, cNomImgLueLocation, 10, "Poisson", true);
        bruitPoisson(cNomImgLue, cNomImgLueLocation, 20, "Poisson", true);

        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 10, true);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 25, true);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 50, true);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 75, true);

        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 25, true);
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 50, true);
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 100, true);
    }
    else {
        char cNomImgLueLocation[250];
        strcpy(cNomImgLueLocation, std::string(std::string("Images/Ppm/") + cNomImgLue).c_str());

        cNomImgLue[longueur - 4] = '\0';

        bruitPoivreEtSel(cNomImgLue, cNomImgLueLocation, 0.05, false);
        bruitPoivreEtSel(cNomImgLue, cNomImgLueLocation, 0.1, false);
        bruitPoivreEtSel(cNomImgLue, cNomImgLueLocation, 0.25, false);

        bruitGaussien(cNomImgLue, cNomImgLueLocation, 0, 25, "Gaussian", false);
        bruitGaussien(cNomImgLue, cNomImgLueLocation, 0, 10, "Gaussian", false);
        bruitGaussien(cNomImgLue, cNomImgLueLocation, 5, 10, "Gaussian", false);

        bruitPoisson(cNomImgLue, cNomImgLueLocation, 10, "Poisson", false);
        bruitPoisson(cNomImgLue, cNomImgLueLocation, 20, "Poisson", false);

        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 10, false);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 25, false);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 50, false);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 75, false);

        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 25, false);
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 50, false);
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 100, false);
    }

   return 1;
}
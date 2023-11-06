#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include "image_ppm.h"
#include <random>

void bruitPoivreEtSel_G(char *cNomImgLue, char *cNomImgLueLocation, float proportion)
{
  int nH, nW, nTaille;

  OCTET *ImgIn, *ImgOut;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

  allocation_tableau(ImgOut, OCTET, nTaille);

    for (int i=0; i < nH; i++) 
    {
        for (int j=0; j < nW; j++) 
        {
            float randNb = (float)(rand())/(float)(RAND_MAX);

            if (randNb < proportion/2.0) {
                ImgOut[i*nW+j] = 0;
            }
            else if (randNb > 1 - proportion/2.0) {
                ImgOut[i*nW+j] = 255;
            }
            else {
                ImgOut[i*nW+j] = ImgIn[i*nW+j];
            }
        }
    }

  char cNomImgEcrite[250];
  strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Noise/Salt_and_Pepper/") + cNomImgLue + std::string("_PoivreEtSel_") + std::to_string(proportion).c_str() + std::string(".pgm") ).c_str());

  ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
  free(ImgIn); free(ImgOut);

}

float genererNombreGaussien(int moyenne, int ecartType, int p) {

    float nb1 = (float)(rand())/(float)(RAND_MAX);
    float nb2 = (float)(rand())/(float)(RAND_MAX);

    float z = sqrt(-2 * log(nb1)) * cos(2 * M_PI * nb2);

    return (float)(z * ecartType + moyenne);
}

void bruitGaussien_G(char *cNomImgLue, char *cNomImgLueLocation, int moyenne, int ecartType) {

    int nH, nW, nTaille;

  OCTET *ImgIn, *ImgOut;
   
  lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);
  nTaille = nH * nW;
  
  allocation_tableau(ImgIn, OCTET, nTaille);
  lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

  allocation_tableau(ImgOut, OCTET, nTaille);

  double mu = 0.0;  // Moyenne
    double sigma = 1.0;  // Écart-type

    // Initialiser un générateur de nombres aléatoires gaussiens
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> distribution(0.0, 0.5);

    float coefficient = 10.0;

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

    char cNomImgEcrite[250];
    
    strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Noise/Gaussian/") + cNomImgLue + std::string("_Gaussien__") + std::to_string(moyenne).c_str() + std::string("_") + std::to_string(ecartType).c_str() + std::string(".pgm") ).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
    free(ImgIn); free(ImgOut);
    
}

/*float genererNombrePoisson(float facteur) {
    float L = exp(-facteur);
    float k = 0;
    float p = 1;
    float u;

    do {
        k += 1;
        u = (float)(rand())/(float)(RAND_MAX);
        p *= u;
    } while (p > L);

    float N = k - 1;

    return N;
}*/

void bruitPoisson_G(char *cNomImgLue, char *cNomImgLueLocation, float facteur) 
{
    bruitGaussien_G(cNomImgLue, cNomImgLueLocation, facteur, facteur );
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

    char cNomImgLueLocation[250];
    strcpy(cNomImgLueLocation, std::string(std::string("Images/Pgm/") + cNomImgLue).c_str());

    char cDernieresLettres[4];
    int longueur = strlen(cNomImgLue);

    strcpy(cDernieresLettres, cNomImgLue + longueur - 3);

    cNomImgLue[longueur - 4] = '\0';

    if (strcmp(cDernieresLettres, "pgm") == 0) {
        bruitPoivreEtSel_G(cNomImgLue, cNomImgLueLocation, 0.05);
        bruitPoivreEtSel_G(cNomImgLue, cNomImgLueLocation, 0.1);
        bruitPoivreEtSel_G(cNomImgLue, cNomImgLueLocation, 0.25);
        
        bruitGaussien_G(cNomImgLue, cNomImgLueLocation, 0, 25);
        bruitGaussien_G(cNomImgLue, cNomImgLueLocation, 0, 10);
        bruitGaussien_G(cNomImgLue, cNomImgLueLocation, 5, 10);

        bruitPoisson_G(cNomImgLue, cNomImgLueLocation, 10);
        bruitPoisson_G(cNomImgLue, cNomImgLueLocation, 20);
    }
    else {
        std::cout << "Le fichier n'est pas de type pgm" << std::endl;
    }

   return 1;
}
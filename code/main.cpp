#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include "image_ppm.h"
#include <random>
#include "PSNR.cpp"
#include <fstream>
#include <ctime>

void calculDistributionGaussienne(const std::vector<float>& distribution, int moyenne, int ecartType, int nTaille) {

    std::ofstream fichier(std::string(std::string("Images/Statistiques/donnees_gaussiennes_") + std::to_string(moyenne).c_str() + std::string("_") + std::to_string(ecartType).c_str() + std::string(".dat")).c_str());
    for (int i = 0; i < nTaille; ++i) {
        fichier << distribution[i] << std::endl;
    }
    fichier.close();

    std::ofstream gnuplotScript("Images/Statistiques/plot_histo.gnu");
    gnuplotScript << "set terminal png\n";
    gnuplotScript << "set output '" << "Images/Statistiques/donnees_gaussiennes_" << moyenne << "_" << ecartType << ".png'\n";
    gnuplotScript << "set title 'Distribution Gaussienne (Moyenne: " << moyenne << ", Écart-type: " << ecartType << ")'\n";
    gnuplotScript << "set xlabel 'Valeurs'\n";
    gnuplotScript << "set ylabel 'Fréquence'\n";
    gnuplotScript << "binwidth = 1\n";
    gnuplotScript << "bin(x, width) = width*floor(x/width)\n";
    gnuplotScript << "plot '" << "Images/Statistiques/donnees_gaussiennes_" << moyenne << "_" << ecartType << ".dat' using (bin($1, binwidth)):(1.0) smooth freq with lines\n";
    gnuplotScript.close();

    system("gnuplot -p Images/Statistiques/plot_histo.gnu");
}

void calculDistributionPoisson(const std::vector<float>& distribution, int lambda, int nTaille) {

    std::ofstream fichier(std::string(std::string("Images/Statistiques/donnees_poisson_") + std::to_string(lambda).c_str() + std::string(".dat")).c_str());
    for (int i = 0; i < nTaille; ++i) {
        fichier << distribution[i] << std::endl;
    }
    fichier.close();

    std::ofstream gnuplotScript("Images/Statistiques/plot_histo.gnu");
    gnuplotScript << "set terminal png\n";
    gnuplotScript << "set output '" << "Images/Statistiques/donnees_poisson_" << lambda << ".png'\n";
    gnuplotScript << "set title 'Distribution de Poisson (Lambda: " << lambda << ")'\n";
    gnuplotScript << "set xlabel 'Valeurs'\n";
    gnuplotScript << "set ylabel 'Fréquence'\n";
    gnuplotScript << "binwidth = 1\n";
    gnuplotScript << "bin(x, width) = width*floor(x/width)\n";
    gnuplotScript << "plot '" << "Images/Statistiques/donnees_poisson_" << lambda << ".dat' using (bin($1, binwidth)):(1.0) smooth freq with lines\n";
    gnuplotScript.close();

    system("gnuplot -p Images/Statistiques/plot_histo.gnu");
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

    std::cout << "PSNR image poivre et sel : " << calculPSNR(ImgOut, ImgIn, nTaille) << std::endl;

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

float genererNombreGaussien(int moyenne, int ecartType) {

    float nb1 = (float)(rand())/(float)(RAND_MAX);
    float nb2 = (float)(rand())/(float)(RAND_MAX);

    float z = sqrt(-2 * log(nb1)) * cos(2 * M_PI * nb2);

    return (float)(z * ecartType + moyenne);
}

int genererBruitPoisson(float moyenne) {
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

void calculBruitGaussien(OCTET *ImgIn, OCTET *ImgOut, int nW, int nH, float moyenne, float ecartType, std::vector<float> &distribution, std::string name) {

    distribution.clear();
    std::srand(std::time(0));

    for (int i=0; i < nH; i++) 
    {
        for (int j=0; j < nW; j++) 
        {

            float gaussien;
            float poisson;
            float val;

            gaussien = genererNombreGaussien(moyenne, ecartType);

            if (name == "Gaussian") {
                val = ImgIn[i*nW+j] + gaussien;
                distribution.push_back(gaussien);
            }
            else if (name == "Poisson") {
                poisson = genererBruitPoisson(moyenne);

                val = ImgIn[i*nW+j] + poisson;

                distribution.push_back(poisson);
            }

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

void bruitGaussien(char *cNomImgLue, char *cNomImgLueLocation, float moyenne, float ecartType, std::string name, bool estImgGrise) {

    int nH, nW, nTaille, nTaille3;
    std::string folder, extension;

    OCTET *ImgIn, *ImgOut;

    if (estImgGrise) {
        lire_nb_lignes_colonnes_image_pgm(cNomImgLueLocation, &nH, &nW);

        nTaille = nH * nW;

        allocation_tableau(ImgIn, OCTET, nTaille);
        lire_image_pgm(cNomImgLueLocation, ImgIn, nH * nW);

        allocation_tableau(ImgOut, OCTET, nTaille);

        std::vector<float> distribution;
        calculBruitGaussien(ImgIn, ImgOut, nW, nH, moyenne, ecartType, distribution, name);

        if (name == "Gaussian") {
            calculDistributionGaussienne(distribution, moyenne, ecartType, nTaille);
        }
        else if (name == "Poisson") {
            calculDistributionPoisson(distribution, moyenne, nTaille);
        }

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
    
        std::vector<float> distributionR;
        std::vector<float> distributionG;
        std::vector<float> distributionB;
        calculBruitGaussien(tabIdR, traitementR, nW, nH, moyenne, ecartType, distributionR, name);
        calculBruitGaussien(tabIdG, traitementG, nW, nH, moyenne, ecartType, distributionG, name);
        calculBruitGaussien(tabIdB, traitementB, nW, nH, moyenne, ecartType, distributionB, name);

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

    std::cout << "PSNR gaussien : " << calculPSNR(ImgIn, ImgOut, nTaille) << std::endl;

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


double calculerMesureSimilarite(OCTET* patch1, OCTET* patch2, int tailleFenetre, double sigma) {
    double sum = 0;

    for(int i = 0; i < tailleFenetre * tailleFenetre; i++) {
        sum += exp(-(patch1[i] - patch2[i]) * (patch1[i] - patch2[i]) / (2 * sigma * sigma));
    }

    return sum;
}


double calculerPoids(double mesureSimilarite, double sigma, double h) {
    double denom = sigma * sigma * h * h;
    return (denom != 0.0) ? exp(-mesureSimilarite / denom) : 0.0;
}



void nlMeansDenoising(char *cNomImgLue, char *cNomImgLueLocation, double sigma, double h, int tailleFenetre) {

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
            double sommePoids = 0.0;
            double sommePoidsValeurs = 0.0;

            extrairePatch(ImgIn, patchActuel, y, x, tailleFenetre, nW, nH);

            for (int j = -m; j <= m; ++j) {
                for (int i = -m; i <= m; ++i) {
                    extrairePatch(ImgIn, patchVoisin, y + j, x + i, tailleFenetre, nW, nH);

                    double mesureSimilarite = calculerMesureSimilarite(patchActuel, patchVoisin, tailleFenetre, sigma);
                    double poids = calculerPoids(mesureSimilarite, sigma, h);

                    sommePoids += poids;
                    sommePoidsValeurs += poids * ImgIn[(y + j) * nW + (x + i)];
                }
            }

            ImgOut[y * nW + x] = sommePoidsValeurs / sommePoids;

        }
    }

    char cNomImgEcrite[250];

    std::cout << "PSNR non local means : " << calculPSNR(ImgIn, ImgOut, nTaille) << std::endl;
    
    strcpy(cNomImgEcrite, std::string(std::string("Images/Pgm/Denoise/NonLocalMeans/") + cNomImgLue + std::string("_NonLocalMeans_") + std::to_string(sigma).c_str() + std::string("_") + std::to_string(h).c_str() + std::string("_") + std::to_string(tailleFenetre).c_str() + std::string(".pgm")).c_str());

    ecrire_image_pgm(cNomImgEcrite, ImgOut, nH, nW);

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

        bruitPoisson(cNomImgLue, cNomImgLueLocation, 1, "Poisson", true);
        bruitPoisson(cNomImgLue, cNomImgLueLocation, 10, "Poisson", true);
        bruitPoisson(cNomImgLue, cNomImgLueLocation, 20, "Poisson", true);

        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 10, true);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 25, true);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 50, true);
        bruitImpulsif(cNomImgLue, cNomImgLueLocation, 75, true);

        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 25, true);
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 50, true);
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 100, true);
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 200, true);

        nlMeansDenoising("citrouilles_PoivreEtSel_0.100000", "Images/Pgm/Noise/Salt_and_Pepper/citrouilles_PoivreEtSel_0.100000.pgm", 0.05, 10, 2);
        nlMeansDenoising("citrouilles_PoivreEtSel_0.100000", "Images/Pgm/Noise/Salt_and_Pepper/citrouilles_PoivreEtSel_0.100000.pgm", 0.05, 30, 2);
        nlMeansDenoising("citrouilles_PoivreEtSel_0.100000", "Images/Pgm/Noise/Salt_and_Pepper/citrouilles_PoivreEtSel_0.100000.pgm", 0.05, 30, 4);
        nlMeansDenoising("citrouilles_PoivreEtSel_0.100000", "Images/Pgm/Noise/Salt_and_Pepper/citrouilles_PoivreEtSel_0.100000.pgm", 0.1, 36, 2);

        nlMeansDenoising("citrouilles_Gaussian_0_10", "Images/Pgm/Noise/Gaussian/citrouilles_Gaussian_0_10.pgm", 0.05, 10, 2);
        nlMeansDenoising("citrouilles_Gaussian_0_10", "Images/Pgm/Noise/Gaussian/citrouilles_Gaussian_0_10.pgm", 0.05, 30, 2);
        nlMeansDenoising("citrouilles_Gaussian_0_10", "Images/Pgm/Noise/Gaussian/citrouilles_Gaussian_0_10.pgm", 0.05, 30, 4);
        nlMeansDenoising("citrouilles_Gaussian_0_10", "Images/Pgm/Noise/Gaussian/citrouilles_Gaussian_0_10.pgm", 0.1, 10, 2);

        nlMeansDenoising("citrouilles_Impulsif_25", "Images/Pgm/Noise/Impulsif/citrouilles_Impulsif_25.pgm", 0.05, 10, 2);
        nlMeansDenoising("citrouilles_Impulsif_25", "Images/Pgm/Noise/Impulsif/citrouilles_Impulsif_25.pgm", 0.05, 30, 2);
        nlMeansDenoising("citrouilles_Impulsif_25", "Images/Pgm/Noise/Impulsif/citrouilles_Impulsif_25.pgm", 0.05, 30, 4);
        nlMeansDenoising("citrouilles_Impulsif_25", "Images/Pgm/Noise/Impulsif/citrouilles_Impulsif_25.pgm", 0.1, 10, 2);

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
        bruitSpeckle(cNomImgLue, cNomImgLueLocation, 200, false);
    }

   return 1;
}
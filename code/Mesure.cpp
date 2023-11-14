#include <stdio.h>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>


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

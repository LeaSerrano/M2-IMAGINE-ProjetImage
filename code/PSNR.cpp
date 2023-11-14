#include <stdio.h>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>


float calculPSNR(OCTET *ImgIn, OCTET *ImgIn2, int nTaille) {
    float EQM = 0, PSNR;

    for (int i=0; i < nTaille; i++) {
        EQM += pow((ImgIn[i]-ImgIn2[i]), 2);
    }

    EQM/= nTaille;

    PSNR = 10*log10(pow(255, 2)/EQM);

    return PSNR;
}

//float calculSSIM()

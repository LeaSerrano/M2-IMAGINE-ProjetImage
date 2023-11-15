#include <stdio.h>
#include <tgmath.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
    if (argc != 3) 
     {
       printf("Usage: ImageIn1.ppm ImageIn2.ppm\n"); 
       exit (1) ;
     }

    char cNomImgLue1[250];
    int nH1, nW1;
  
    sscanf(argv[1],"%s",cNomImgLue1) ;
   
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue1, &nH1, &nW1);

    char cNomImgLue2[250];
    int nH2, nW2;
  
    sscanf(argv[2],"%s",cNomImgLue2) ;
   
    lire_nb_lignes_colonnes_image_pgm(cNomImgLue2, &nH2, &nW2);

    if( nH1 != nH2 || nW1 != nW2 )
        return 1; 
  
   int nTaille3 = nH1 * nW1;
   OCTET *ImgIn1, *ImgIn2;
   allocation_tableau(ImgIn1, OCTET, nTaille3);
   lire_image_pgm(cNomImgLue1, ImgIn1, nH1 * nW1);
   allocation_tableau(ImgIn2, OCTET, nTaille3);
   lire_image_pgm(cNomImgLue2, ImgIn2, nH2 * nW2);

   double EQM = 0.;
	
    for( int i = 0; i < nTaille3; i++)
        EQM += pow(ImgIn1[i]- ImgIn2[i], 2.);

    EQM /= (double) nTaille3;

    double PSNR = 10.*log10(pow(255., 2.)/EQM);

    printf("PSNR between %s and %s : %f\n", cNomImgLue1, cNomImgLue2, PSNR );

   free(ImgIn1);
   free(ImgIn2);
   return 1;
}

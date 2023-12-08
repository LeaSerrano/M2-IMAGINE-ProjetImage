#include <stdio.h>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include "image_ppm.h"

void get_Mean_Var( std::vector< float > Arr, float& Mean, float& Var )
{
    float mean = 0.;

    for( uint i = 0; i < Arr.size(); i++ )
        mean += Arr[i];

    mean = mean / (float) Arr.size();

    float var = 0.;

    for( uint i = 0; i < Arr.size(); i++ )
        var += pow( Arr[i] - mean, 2. );

    var = var / (float) Arr.size();

    Mean = mean;
    Var = var;
}

int main(int argc, char* argv[])
{
    if( argc < 3 ) 
    {
        printf("Usage: TxtFileIn TxtFileOut nb_Args_Supp\n"); 
        exit(1);
    }

    FILE* fIn = fopen(argv[1], "r" );
    if( !fIn )
    {
        printf("Le fichier %s n'a pas pu être ouvert\n", argv[1] );
        exit(1);
    }

    FILE* fOut = fopen(argv[2], "a" );
    if( !fOut )
    {
        printf("Le fichier %s n'a pas pu être ouvert\n", argv[2] );
        exit(1);
    }

    float mid = 0.;
    std::vector< float > vals;
    while( fscanf( fIn, "%f", &mid ) != EOF )
        vals.push_back( mid );

    float Mean, Var;
    get_Mean_Var( vals, Mean, Var );

    printf("%f %f\n", Mean, Var );

    return 0;

    fprintf( fOut, "%f %f", Mean, Var );

    int nb_args = atoi( argv[3] );
    for( uint i = 0; i < nb_args; i++ )
        fprintf(fOut, " %.2f", atof(argv[i+4]));

    fprintf(fOut, "\n");

    fclose(fIn);
    fclose(fOut);
    

    return 0;
}
/*
for i in GAU IMP PES POI SPE; do for j in 2.5 5 7.5 10 15 20 25 35; ./Mesure ./NN_Imgs/Img_N/$i\_$j\_lean.pgm ./NN_Imgs/TradImg_D/$i\_$j\_lean.pgm PSNR; done done
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <list>
#include <algorithm> 
#include <cmath>

int main(int argc, char* argv[])
{
    if( argc < 3 ) 
    {
        printf("Usage: DB TxtFileIn {NomArg {fixed_val=float|variable=V}}^nb_Args\n"); 
        exit(1);
    }

    std::string modes[4] = { std::string("PSNR"),  std::string("SNR"), std::string("RMSE") };
    std::string colors[4] = { "#FF0000", "#0066CC", "#990099" };
    std::string DB( argv[1] );
    std::string TxtFileData( argv[2] );

    // plot 'cities.dat' using 5:4:1 with labels font "Times,8"
    uint nb_Args = (argc-1)/2 - 1;
    std::vector< std::string > Nom_Args;
    Nom_Args.resize( nb_Args );
    float* Val_Args = (float*) malloc( sizeof( float ) * nb_Args );
    int ArgV = -1;

    std::string TmpDatFile("TmpDatFile_" + DB + "_" + TxtFileData);
    std::string TmpGnuScript("TmpGnuScript_" + DB + "_" + TxtFileData);

    for( uint a = 0; a < nb_Args; a++ )
    {
        Nom_Args[a] = std::string( argv[a*2+3] );
        if( '0' <= argv[a*2+4][0] && argv[a*2+4][0] <= '9' )
            Val_Args[a] = atof(argv[a*2+4]);
        else 
        {
            Val_Args[a] = INFINITY;
            ArgV = a;
        }

        TmpDatFile += "_" + Nom_Args[a] + "_" + argv[a*2+4];
        TmpGnuScript += "_" + Nom_Args[a] + "_" + argv[a*2+4];
    }
    TmpDatFile += ".dat";
    TmpGnuScript += ".gnu";

    if( ArgV == -1 )
    {
        printf("There is no variable\n");
        exit(1);
    }

    for( uint i = 0; i < 3; i++ )
    {
        std::string mode( modes[i] );
        std::string color( colors[i] );

        std::string TxtFileIn( "DB/" + DB + "_M/" + mode + "/{" + mode + "}_" + TxtFileData + ".txt" );

        std::string PngFileOut( "DB/" + DB + "_M/PLOT/" + TxtFileData + "_{ " + mode + " }___" );
        for( uint a = 0; a < nb_Args; a++ )
            if( a == ArgV )
                PngFileOut = PngFileOut + "_#" + Nom_Args[a] + "#";
            else 
                PngFileOut = PngFileOut + "_( " + Nom_Args[a] + ", " + argv[a*2+4] + " )";
        PngFileOut = PngFileOut + ".png";

        FILE* fIn = fopen(TxtFileIn.c_str(), "r" );
        if( !fIn )
        {
            printf("Le fichier %s n'a pas pu être ouvert\n", TxtFileIn.c_str() );
            exit(1);
        }

        FILE* fOut = fopen(TmpDatFile.c_str(), "w" );
        if( !fOut )
        {
            printf("Le fichier %s n'a pas pu être ouvert\n", TmpDatFile.c_str() );
            exit(1);
        }

        bool EOF_reached = false;
        while( !EOF_reached )
        {
            float Mean, Var, Val, Trash;
            EOF_reached |= ( fscanf(fIn, "%f", &Mean) == EOF );
            EOF_reached |= ( fscanf(fIn, "%f", &Var) == EOF );

            bool to_insert = true;
            
            for( uint a = 0; a < nb_Args; a++ )
                if( a == ArgV  )
                    EOF_reached |= ( fscanf(fIn, "%f", &Val) == EOF );
                else 
                {
                    EOF_reached |= ( fscanf(fIn, "%f", &Trash) == EOF );

                    float epsilon = 1e-3;
                    if( !( Val_Args[a] - epsilon <= Trash && Trash <= Val_Args[a] + epsilon ) )
                        to_insert = false;
                }
            
            if( to_insert )
                fprintf( fOut, "%f %.2f %f\n", (i==1)?(-Mean):(Mean), Var, Val );
        }

        fclose(fIn);
        fclose(fOut);

        std::ofstream gnuplotScript( TmpGnuScript.c_str() );

        std::string PlotTitle;
        PlotTitle += TxtFileData[0]; PlotTitle += TxtFileData[1]; PlotTitle += TxtFileData[2];
        PlotTitle += " ";
        PlotTitle += TxtFileData[4]; PlotTitle += TxtFileData[5]; PlotTitle += TxtFileData[6];
        PlotTitle += " - " + mode + " - ( ";
        for( uint a = 0; a < nb_Args; a++ )
        {
            PlotTitle += Nom_Args[a];
            
            if( a != ArgV )
                PlotTitle += " = " + std::string( argv[a*2+4] );

            PlotTitle += (a==nb_Args-1)?(" )"):(", ");
        }

        gnuplotScript << "ALPHA=0.05\n";
        gnuplotScript << "stats '" << TmpDatFile <<"' using 1 nooutput\n";
        gnuplotScript << "MAX_Y=STATS_max\n";
        gnuplotScript << "MIN_Y=STATS_min\n";
        gnuplotScript << "stats '" << TmpDatFile <<"' using 3 nooutput\n";
        gnuplotScript << "MAX_X=STATS_max\n";
        gnuplotScript << "MIN_X=STATS_min\n";
        gnuplotScript << "set yrange [MIN_Y-(MAX_Y-MIN_Y)*ALPHA:MAX_Y+(MAX_Y-MIN_Y)*ALPHA]\n";
        gnuplotScript << "set xrange [MIN_X-(MAX_X-MIN_X)*ALPHA:MAX_X+(MAX_X-MIN_X)*ALPHA]\n";
        gnuplotScript << "set terminal png size 800,450\n";
        gnuplotScript << "set output '" << PngFileOut << "'\n";
        gnuplotScript << "set title '" << PlotTitle << "'\n";
        gnuplotScript << "set xlabel '" << Nom_Args[ArgV] <<"'\n";
        gnuplotScript << "set ylabel '" << mode << "'\n";
        gnuplotScript << "plot '" << TmpDatFile <<"'  using 3:1:2 with labels font \",9\" notitle, '" << TmpDatFile <<"' using 3:1 smooth unique with lines lt rgb '"<< color << "' notitle\n";
        gnuplotScript.close();

        system( std::string("gnuplot -p " + TmpGnuScript ).c_str() );
        system( std::string("rm " + TmpDatFile ).c_str() );
        system( std::string("rm " + TmpGnuScript ).c_str() );
    }

    free(Val_Args);

    return 0;
}
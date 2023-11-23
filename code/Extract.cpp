#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <list>
#include <algorithm> 
#include <cmath>

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
    gnuplotScript << "plot '" << "Images/Statistiques/donnees_gaussiennes_" << moyenne << "_" << ecartType << ".dat' using (bin($1, binwidth)):(1.0) smooth freq with boxes\n";
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
    gnuplotScript << "plot '" << "Images/Statistiques/donnees_poisson_" << lambda << ".dat' using (bin($1, binwidth)):(1.0) smooth freq with boxes\n";
    gnuplotScript.close();

    system("gnuplot -p Images/Statistiques/plot_histo.gnu");
}

int main(int argc, char* argv[])
{
    if( argc < 3 ) 
    {
        printf("Usage: DB TxtFileIn {NomArg {fixed_val=float|variable=V}}^nb_Args\n"); 
        exit(1);
    }

    std::string modes[4] = { std::string("PSNR"),  std::string("SNR"), std::string("SSIM"), std::string("RMSE") };
    std::string DB( argv[1] );
    std::string TxtFileData( argv[2] );

    // plot 'cities.dat' using 5:4:1 with labels font "Times,8"
    uint nb_Args = (argc-1)/2 - 1;
    std::string* Nom_Args = new std::string[nb_Args];
    float* Val_Args = new float[nb_Args];
    int ArgV = -1;

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
    }

    if( ArgV == -1 )
    {
        printf("There is no variable\n");
        exit(1);
    }

    std::string TmpDatFile("TmpDatFile.dat");
    std::string TmpGnuScript("TmpGnuScript.gnu");
    for( std::string mode : modes )
    {
        std::string TxtFileIn( "DB/" + DB + "_M/" + mode + "/{" + mode + "}_" + TxtFileData + ".txt" );

        std::string PngFileOut( "DB/" + DB + "_M/PLOT/{" + mode + "}_" + TxtFileData );
        for( uint a = 0; a < nb_Args; a++ )
            if( Val_Args[a] == INFINITY )
                PngFileOut = PngFileOut + "_(((" + Nom_Args[a] + ")))";
            else 
                PngFileOut = PngFileOut + "_(" + Nom_Args[a] + "," + argv[a*2+4] + ")";
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
                if( Val_Args[a]==INFINITY )
                    EOF_reached |= ( fscanf(fIn, "%f", &Val) == EOF );
                else 
                {
                    EOF_reached |= ( fscanf(fIn, "%f", &Trash) == EOF );

                    float epsilon = 1e-3;
                    if( !( Val_Args[a] - epsilon <= Trash && Trash <= Val_Args[a] + epsilon ) )
                        to_insert = false;
                }
            
            if( to_insert )
                fprintf( fOut, "%f %.2f %f\n", Mean, Var, Val );
        }

        fclose(fIn);
        fclose(fOut);

        std::ofstream gnuplotScript( TmpGnuScript.c_str() );
        gnuplotScript << "set terminal png\n";
        gnuplotScript << "set output '" << PngFileOut << "'\n";
        gnuplotScript << "set title '" << TxtFileData << "( " << mode << " , " << Nom_Args[ArgV] << " )'\n";
        gnuplotScript << "set xlabel '" << Nom_Args[ArgV] <<"'\n";
        gnuplotScript << "set ylabel '" << mode << "'\n";
        gnuplotScript << "plot '" << TmpDatFile <<"' using 3:1:2 with labels, '" << TmpDatFile <<"' using 3:1 with lines\n";
        gnuplotScript.close();

        std::string command("gnuplot -p " + TmpGnuScript );
        system( command.c_str() );
    }

    delete Nom_Args;
    delete Val_Args;

    return 0;
}
#include <iostream>
#include <fstream>
#include <vector>

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
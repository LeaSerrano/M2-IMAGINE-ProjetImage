set terminal png
set output 'Images/Statistiques/donnees_gaussiennes_0_10.png'
set title 'Distribution Gaussienne (Moyenne: 0, Écart-type: 10)'
set xlabel 'Valeurs'
set ylabel 'Fréquence'
binwidth = 1
bin(x, width) = width*floor(x/width)
plot 'Images/Statistiques/donnees_gaussiennes_0_10.dat' using (bin($1, binwidth)):(1.0) smooth freq with boxes

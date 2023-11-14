set terminal png
set output 'Images/Statistiques/donnees_poisson_20.png'
set title 'Distribution de Poisson (Lambda: 20)'
set xlabel 'Valeurs'
set ylabel 'Fréquence'
binwidth = 1
bin(x, width) = width*floor(x/width)
plot 'Images/Statistiques/donnees_poisson_20.dat' using (bin($1, binwidth)):(1.0) smooth freq with lines

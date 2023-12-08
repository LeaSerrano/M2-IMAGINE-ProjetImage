
DB=$1

voisins='1 2 3 4 5'
voisinsNLM='1 2 3 4'

mean_GAU='0'
var_GAU='2.5 5 7.5 10 15 20 25 35'

puissance='0.5 0.7 1 1.3 1.5 1.7 2'

ponderation='0.25 0.5 0.75'
tailleRecherche='10 20 30'

Gaussien()
{
    sh GenImg.sh $DB N GAU $1 $2
    DirN=GAU\_$1\_$2

    ImgsIn=`ls DB/$DB/`
    N="_N"
    DImg="DB/$DB$N/$DirN/"

    for eachfile in $ImgsIn
    do
            txtfile="DB/{PSNR}_$DirN.txt"

            ./Mesure DB/$DB/$eachfile $DImg$eachfile PSNR $txtfile
    done
    
    sh GenImg.sh DB/$DB\_N/$DirN R 
}

RunGaussien()
{
    Range_Mean='0 1 2'
    Range_STD='2.5 5 7.5 10 15 20 25 35'

    for m in $Range_Mean
    do
        Gaussien $m 1
    done
    
    for std in $Range_STD
    do
        Gaussien 0 $std
    done

    exit

       # Filtre Moyenneur
    for m in $Range_Mean
    do
        ./Plot $DB GAU_MOY voisins V moyenne $m ecart-type 1
    done
    
    for std in $Range_STD
    do
        ./Plot $DB GAU_MOY voisins V moyenne 0 ecart-type $std
    done

    for v in $voisins 
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_MOY voisins $v moyenne M ecart-type $std
        done
        ./Plot $DB GAU_MOY voisins $v moyenne 0 ecart-type E
    done
    

       # Filtre Médian
    for m in $Range_Mean
    do
        ./Plot $DB GAU_MED voisins V moyenne $m ecart-type 1
    done
    
    for std in $Range_STD
    do
        ./Plot $DB GAU_MED voisins V moyenne 0 ecart-type $std
    done

    for v in $voisins 
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_MED voisins $v moyenne M ecart-type $std
        done
        ./Plot $DB GAU_MED voisins $v moyenne 0 ecart-type E
    done
    

     # Filtre Gaussien
    for mean in $mean_GAU
    do
        for var in $var_GAU
        do
            for m in $Range_Mean
            do
                ./Plot $DB GAU_GAU voisins V moyenne_debruit $mean ecart-type_debruit $var moyenne_bruit $m ecart-type_bruit 1
            done
            
            for std in $Range_STD
            do
                ./Plot $DB GAU_GAU voisins V moyenne_debruit $mean ecart-type_debruit $var moyenne_bruit 0 ecart-type_bruit $std
            done
        done
    done
    for v in $voisins
    do
        for var in $var_GAU
        do
            for m in $Range_Mean
            do
                ./Plot $DB GAU_GAU voisins $v moyenne_debruit 0 ecart-type_debruit E moyenne_bruit $m ecart-type_bruit 1
            done
            
            for std in $Range_STD
            do
                ./Plot $DB GAU_GAU voisins $v moyenne_debruit 0 ecart-type_debruit E moyenne_bruit 0 ecart-type_bruit $std
            done
        done
    done
    

     # Filtre Gradient 
    for c in 0 1 
    do
        for m in $Range_Mean
        do
            ./Plot $DB GAU_GRA voisins V type $c moyenne $m ecart-type 1
        done
        
        for std in $Range_STD
        do
            ./Plot $DB GAU_GRA voisins V type $c moyenne 0 ecart-type $std
        done
        for v in $voisins 
        do
            for m in $Range_Mean
            do
                ./Plot $DB GAU_GRA voisins $v type $c moyenne $m ecart-type E
            done
            
            for std in $Range_STD
            do
                ./Plot $DB GAU_GRA voisins $v type $c moyenne M ecart-type $std
            done
        done
    done
    for v in $voisins 
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_GRA voisins $v type T moyenne 0 ecart-type $std
        done
    done
    

     # Filtre Pondéré
    for v in $voisins 
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_PON voisins $v puissance P moyenne 0 ecart-type $std
        done
    done
    for p in $puissance
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_PON voisins V puissance $p moyenne 0 ecart-type $std
        done
    done
    for v in $voisins 
    do
        for p in $puissance
        do
            ./Plot $DB GAU_PON voisins $v puissance $p moyenne 0 ecart-type E
        done
    done
    

     # Non Local Mean
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for v in $voisinsNLM 
            do
                ./Plot $DB GAU_NLM voisins $v tailleRecherche $t ponderation $p moyenne 0 ecart-type E
            done
        done
    done
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for std in $Range_STD
            do
                ./Plot $DB GAU_NLM voisins V tailleRecherche $t ponderation $p moyenne 0 ecart-type $std
            done
        done
    done
    for v in $voisinsNLM 
    do
        for t in $tailleRecherche
        do
            for std in $Range_STD
            do
                ./Plot $DB GAU_NLM voisins $v tailleRecherche $t ponderation P moyenne 0 ecart-type $std
            done
        done
    done
    for p in $ponderation
    do
        for v in $voisinsNLM 
        do
            for std in $Range_STD
            do
                ./Plot $DB GAU_NLM voisins $v tailleRecherche T ponderation $p moyenne 0 ecart-type $std
            done
        done
    done
    
}

Other()
{
    sh GenImg.sh $DB N $1 $2
    DirN=$1\_$2

    ImgsIn=`ls DB/$DB/`
    N="_N"
    DImg="DB/$DB$N/$DirN/"

    for eachfile in $ImgsIn
    do
            txtfile="DB/{PSNR}_$DirN.txt"

            ./Mesure DB/$DB/$eachfile $DImg$eachfile PSNR $txtfile
    done
    
    sh GenImg.sh DB/$DB\_N/$DirN R 
}

RunPoivreEtSel()
{
    Other PES 0.025
    Other PES 0.05
    Other PES 0.075
    Other PES 0.1
    Other PES 0.15
    Other PES 0.20
    Other PES 0.25
    Other PES 0.35

    # PlotOther PES proportion $Range_prop
}
    

RunPoisson()
{
    Other POI 2.5
    Other POI 5
    Other POI 7.5
    Other POI 10
    Other POI 15
    Other POI 20
    Other POI 25
    Other POI 35

    # PlotOther POI moyenne_poisson $Range_moy
}

RunImpulsif()
{
    Other IMP 2.5
    Other IMP 5
    Other IMP 7.5
    Other IMP 10
    Other IMP 15
    Other IMP 20
    Other IMP 25
    Other IMP 35

    # PlotOther IMP facteur $Range_fact
}

RunSpeckle()
{
    Other SPE 2.5
    Other SPE 5
    Other SPE 7.5
    Other SPE 10
    Other SPE 15
    Other SPE 20
    Other SPE 25
    Other SPE 35

    # PlotOther SPE intensite $Range_intens
}

RunImpulsif 
RunPoivreEtSel 
RunPoisson 
RunSpeckle

if [ "$2" = "GAU" ]
then
    RunGaussien 
fi
if [ "$2" = "IMP_PES" ]
then
    RunImpulsif 
    RunPoivreEtSel 
fi
if [ "$2" = "POI_SPE" ]
then
    RunPoisson 
    RunSpeckle 
fi


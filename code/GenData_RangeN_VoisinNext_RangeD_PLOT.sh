
DB=$1

voisins='1 2 3 4 5'
voisinsNLM='1 2 3 4'

mean_GAU='0'
var_GAU='2.5 5 7.5 10 15 20 25 35'

puissance='0.5 0.7 1 1.3 1.5 1.7 2'

ponderation='0.25 0.5 0.75'
tailleRecherche='10 20 30'

RunGaussien()
{
    Range_Mean='0 1 2'
    Range_STD='2.5 5 7.5 10 15 20 25 35'

       # Filtre meanur
    for m in $Range_Mean
    do
        ./Plot $DB GAU_MOY voisins V mean $m std 1
    done
    
    for std in $Range_STD
    do
        ./Plot $DB GAU_MOY voisins V mean 0 std $std
    done

    for v in $voisins 
    do
        ./Plot $DB GAU_MOY voisins $v mean 0 std E
    done
    

       # Filtre Médian
    for m in $Range_Mean
    do
        ./Plot $DB GAU_MED voisins V mean $m std 1
    done
    
    for std in $Range_STD
    do
        ./Plot $DB GAU_MED voisins V mean 0 std $std
    done

    for v in $voisins 
    do
        ./Plot $DB GAU_MED voisins $v mean 0 std E
    done
    

     # Filtre Gaussien
    for mean in $mean_GAU
    do
        for var in $var_GAU
        do
            for m in $Range_Mean
            do
                ./Plot $DB GAU_GAU voisins V meanD $mean stdD $var meanN $m stdN 1
            done
            
            for std in $Range_STD
            do
                ./Plot $DB GAU_GAU voisins V meanD $mean stdD $var meanN 0 stdN $std
            done
        done
    done
    for v in $voisins
    do
        for var in $var_GAU
        do
            for m in $Range_Mean
            do
                ./Plot $DB GAU_GAU voisins $v meanD 0 stdD E meanN $m stdN 1
            done
            
            for std in $Range_STD
            do
                ./Plot $DB GAU_GAU voisins $v meanD 0 stdD E meanN 0 stdN $std
            done
        done
    done
    

     # Filtre Gradient 
    for c in 0 1 
    do  
        for std in $Range_STD
        do
            ./Plot $DB GAU_GRA voisins V type $c mean 0 std $std
        done
        for v in $voisins 
        do
            ./Plot $DB GAU_GRA voisins $v type $c mean 0 std E
        done
    done
    for v in $voisins 
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_GRA voisins $v type T mean 0 std $std
        done
    done
    

     # Filtre Pondéré
    for v in $voisins 
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_PON voisins $v pow P mean 0 std $std
        done
    done
    for p in $puissance
    do
        for std in $Range_STD
        do
            ./Plot $DB GAU_PON voisins V pow $p mean 0 std $std
        done
    done
    for v in $voisins 
    do
        for p in $puissance
        do
            ./Plot $DB GAU_PON voisins $v pow $p mean 0 std E
        done
    done
    

     # Non Local Mean
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for v in $voisinsNLM 
            do
                ./Plot $DB GAU_NLM pond° $p ResSize $t voisins $v mean 0 std E
            done
        done
    done
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for std in $Range_STD
            do
                ./Plot $DB GAU_NLM pond° $p ResSize $t voisins V mean 0 std $std 
            done
        done
    done
    for v in $voisinsNLM 
    do
        for t in $tailleRecherche
        do
            for std in $Range_STD
            do
                ./Plot $DB GAU_NLM pond° P ResSize $t voisins $v mean 0 std $std
            done
        done
    done
    for p in $ponderation
    do
        for v in $voisinsNLM 
        do
            for std in $Range_STD
            do
                ./Plot $DB GAU_NLM pond° $p ResSize T voisins $v mean 0 std $std
            done
        done
    done
    
}

PlotOther()
{
    N=$1
    NameArg=$2
    Range=$3

       # Filtre mean
    for r in $Range
    do
        ./Plot $DB $N\_MOY voisins V $NameArg $r
    done

    for v in $voisins 
    do
        ./Plot $DB $N\_MOY voisins $v $NameArg R
    done
    

       # Filtre Médian
    for r in $Range
    do
        ./Plot $DB $N\_MED voisins V $NameArg $r
    done

    for v in $voisins 
    do
        ./Plot $DB $N\_MED voisins $v $NameArg R
    done
    

    # Filtre Gaussien
    for mean in $mean_GAU
    do
        for var in $var_GAU
        do
            for r in $Range
            do
                ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg $r
            done

            for v in $voisins 
            do
                ./Plot $DB $N\_GAU voisins $v mean $mean std $var $NameArg R
            done
        done
    done
    for v in $voisins
    do
        for var in $var_GAU
        do
            for r in $Range
            do
                ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg $r
            done
        done
    done
    

    # Filtre Gradient 
    for c in 0 1 
    do
        for mean in $mean_GAU
        do
            for var in $var_GAU
            do
                for r in $Range
                do
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg $r
                done

                for v in $voisins 
                do
                    ./Plot $DB $N\_GRA voisins $v type $c $NameArg R
                done
            done
        done
        for v in $voisins
        do
            for r in $Range
            do
                ./Plot $DB $N\_GRA voisins $v type T $NameArg $r
            done
        done
    done
    

    # Filtre Pondéré
    for v in $voisins 
    do
        for r in $Range
        do
            ./Plot $DB $N\_PON voisins $v pow P $NameArg $r
        done
    done
    for p in $puissance
    do
        for r in $Range
        do
            ./Plot $DB $N\_PON voisins V pow $p $NameArg $r
        done
    done
    for v in $voisins 
    do
        for p in $puissance
        do
            ./Plot $DB $N\_PON voisins $v pow $p $NameArg R
        done
    done

    # Non Local Mean
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for v in $voisinsNLM 
            do
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins $v $NameArg R
            done
        done
    done
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for r in $Range
            do
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg $r
            done
        done
    done
    for v in $voisinsNLM 
    do
        for t in $tailleRecherche
        do
            for r in $Range
            do
                ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg $r
            done
        done
    done
    for p in $ponderation
    do
        for v in $voisinsNLM 
        do
            for r in $Range
            do
                ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg $r
            done
        done
    done
}

RunPoivreEtSel()
{
    Range_prop='0.03 0.05 0.07 0.1 0.15 0.2 0.25 0.35'
    PlotOther PES proport° $Range_prop
}
    

RunPoisson()
{
    Range_moy='2.5 5 7.5 10 15 20 25 35'
    PlotOther POI meanPOI $Range_moy
}

RunImpulsif()
{
    Range_fact='2.5 5 7.5 10 15 20 25 35'
    PlotOther IMP fact $Range_fact
}

RunSpeckle()
{
    Range_intens='2.5 5 7.5 10 15 20 25 35'
    PlotOther SPE intens $Range_intens
}

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
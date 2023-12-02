
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
    
     # Filtre Gaussien
    for v in $voisins
    do
                for std in $Range_STD
                do
                    ./Plot $DB GAU_GAU voisins $v meanD 0 stdD $std meanN 0 stdN s
                done
    done
    

}

PlotOther()
{
    N=$1
    NameArg=$2
    Range=$3

    for mean in $mean_GAU
    do
        for var in $var_GAU
        do
            ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg 5
            ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg 7.5
            ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg 10
            ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg 15
            ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg 20
            ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg 25
            ./Plot $DB $N\_GAU voisins V mean $mean std $var $NameArg 35
        done
    done
    for v in $voisins
    do
        for var in $var_GAU
        do
            ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg 5
            ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg 7.5
            ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg 10
            ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg 15
            ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg 20
            ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg 25
            ./Plot $DB $N\_GAU voisins $v mean 0 std E $NameArg 35
        done
    done

    for v in $voisins 
    do
        ./Plot $DB $N\_PON voisins $v pow P $NameArg 5
        ./Plot $DB $N\_PON voisins $v pow P $NameArg 7.5
        ./Plot $DB $N\_PON voisins $v pow P $NameArg 10
        ./Plot $DB $N\_PON voisins $v pow P $NameArg 15
        ./Plot $DB $N\_PON voisins $v pow P $NameArg 20
        ./Plot $DB $N\_PON voisins $v pow P $NameArg 25
        ./Plot $DB $N\_PON voisins $v pow P $NameArg 35
    done
    for p in $puissance
    do
        ./Plot $DB $N\_PON voisins V pow $p  $NameArg 5
        ./Plot $DB $N\_PON voisins V pow $p  $NameArg 7.5
        ./Plot $DB $N\_PON voisins V pow $p  $NameArg 10
        ./Plot $DB $N\_PON voisins V pow $p  $NameArg 15
        ./Plot $DB $N\_PON voisins V pow $p  $NameArg 20
        ./Plot $DB $N\_PON voisins V pow $p  $NameArg 25
        ./Plot $DB $N\_PON voisins V pow $p  $NameArg 35
    done


    for c in 0 1 
    do
        for mean in $mean_GAU
        do
            for var in $var_GAU
            do
                for r in $Range
                do
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg 5
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg 7.5
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg 10
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg 15
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg 20
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg 25
                    ./Plot $DB $N\_GRA voisins V type $c $NameArg 35
                done
            done
        done
        for v in $voisins
        do
            
                ./Plot $DB $N\_GRA voisins $v type T $NameArg 5
                ./Plot $DB $N\_GRA voisins $v type T $NameArg 7.5
                ./Plot $DB $N\_GRA voisins $v type T $NameArg 10
                ./Plot $DB $N\_GRA voisins $v type T $NameArg 15
                ./Plot $DB $N\_GRA voisins $v type T $NameArg 20
                ./Plot $DB $N\_GRA voisins $v type T $NameArg 25
                ./Plot $DB $N\_GRA voisins $v type T $NameArg 35
            
        done
    done

    ./Plot $DB $N\_MED voisins V $NameArg 5
    ./Plot $DB $N\_MED voisins V $NameArg 7.5
    ./Plot $DB $N\_MED voisins V $NameArg 10
    ./Plot $DB $N\_MED voisins V $NameArg 15
    ./Plot $DB $N\_MED voisins V $NameArg 20
    ./Plot $DB $N\_MED voisins V $NameArg 25
    ./Plot $DB $N\_MED voisins V $NameArg 35

    ./Plot $DB $N\_MOY voisins V $NameArg 5
    ./Plot $DB $N\_MOY voisins V $NameArg 7.5
    ./Plot $DB $N\_MOY voisins V $NameArg 10
    ./Plot $DB $N\_MOY voisins V $NameArg 15
    ./Plot $DB $N\_MOY voisins V $NameArg 20
    ./Plot $DB $N\_MOY voisins V $NameArg 25
    ./Plot $DB $N\_MOY voisins V $NameArg 35

    for p in $ponderation
    do
        for t in $tailleRecherche
        do
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg 5
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg 7.5
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg 10
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg 15
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg 20
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg 25
                ./Plot $DB $N\_NLM pond° $p ResSize $t voisins V $NameArg 35
        done
    done
    for v in $voisinsNLM 
    do
        for t in $tailleRecherche
        do
            ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg 5
            ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg 7.5
            ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg 10
            ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg 15
            ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg 20
            ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg 25
            ./Plot $DB $N\_NLM pond° P ResSize $t voisins $v $NameArg 35
        done
    done
    for p in $ponderation
    do
        for v in $voisinsNLM 
        do
            ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg 5
            ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg 7.5
            ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg 10
            ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg 15
            ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg 20
            ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg 25
            ./Plot $DB $N\_NLM pond° $p ResSize T voisins $v $NameArg 35
        done
    done

    exit

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
    Range_prop='0.03 5 7.5 10 105 20 205 0.35'
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

PlotOther SPE intens $Range_intens

if [ "$2" = "GAU" ]
then
    RunGaussien 
fi
if [ "$2" = "IMP" ]
then
    RunImpulsif 
    RunPoisson 
    RunSpeckle 
fi
if [ "$2" = "PES" ]
then
    RunPoivreEtSel 
fi


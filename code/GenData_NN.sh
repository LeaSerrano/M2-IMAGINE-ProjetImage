
DB=$1

voisins='1 2 3 4 5 6'
voisinsNLM='1 2 3 4'

mean_GAU='0'
var_GAU='1 5 10 15 20 25 30 35'

puissance='0.5 0.7 0.9 1 1.1 1.3 1.5'

ponderation='0.25 0.5 0.75'
tailleRecherche='10 20 30'

Gaussien()
{
    sh GenImg.sh $DB N GAU $1 $2
    DirN=GAU\_$1\_$2
    ImgNLoc=DB/$DB\_N/$DirN
    ImgDLoc=DB/$DB\_D/$DirN
    ImgsN='ls $ImgNLoc'
    FileF=GAU_NN
    extension=""

    for eachfile in $ImgsN
    do 
        convert $ImgNLoc/$eachfile $ImgNLoc/${eachfile%.*}.png
        rm $ImgNLoc/$eachfile
        extension="${eachfile##*.}"
    done

    mkdir $ImgDLoc

    python3 preEntraine_test_Restormer.py $ImgNLoc $ImgDLoc

    ImgsD='ls $ImgDLoc'

    for eachfile in $ImgsD
    do 
        convert $ImgDLoc/$eachfile $ImgDLoc/${eachfile%.*}.$extension
        rm $ImgDLoc/$eachfile
    done

    sh GenImg.sh $DB M $DirN
    sh GenImg.sh $DB F $DirN $FileF 2 $1 $2

    sh GenImg.sh $ImgNLoc R 
    sh GenImg.sh $ImgDLoc R 
}

RunGaussien()
{
    Range_Mean='0 1 2'
    Range_STD='5 10 15 20 25 30 35'

    for m in $Range_Mean
    do
        Gaussien $m 1
    done
    
    for std in $Range_STD
    do
        Gaussien 0 $std
    done

    {   # Filtre Moyenneur
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
    }&
    T1=${!}

    {   # Filtre Médian
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
    }&
    T2=${!}

    { # Filtre Gaussien
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
    }&
    T3=${!}

    { # Filtre Gradient 
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
    }&
    T4=${!}

    { # Filtre Pondéré
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
    }&
    T5=${!}

    { # Non Local Mean
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
    }&
    T6=${!}

    wait ${T1}
    wait ${T2}
    wait ${T3}
    wait ${T4}
    wait ${T5}
    wait ${T6}
}

Other()
{
    sh GenImg.sh $DB N $1 $2
    DirN=$1\_$2
    ImgNLoc=DB/$DB\_N/$DirN
    ImgDLoc=DB/$DB\_D/$DirN
    ImgsN='ls $ImgNLoc'
    FileF=$1\_NN
    extension=""

    for eachfile in $ImgsN
    do 
        convert $ImgNLoc/$eachfile $ImgNLoc/${eachfile%.*}.png
        rm $ImgNLoc/$eachfile
        extension="${eachfile##*.}"
    done

    mkdir $ImgDLoc

    python3 preEntraine_test_Restormer.py $ImgNLoc $ImgDLoc

    ImgsD='ls $ImgDLoc'

    for eachfile in $ImgsD
    do 
        convert $ImgDLoc/$eachfile $ImgDLoc/${eachfile%.*}.$extension
        rm $ImgDLoc/$eachfile
    done

    sh GenImg.sh $DB M $DirN
    sh GenImg.sh $DB F $DirN $FileF 2 $1 $2

    sh GenImg.sh $ImgNLoc R 
    sh GenImg.sh $ImgDLoc R 
}

PlotOther()
{
    N=$1
    NameArg=$2
    Range=$3

    echo $Range

    for r in $Range
    do
        echo $r
    done

    {   # Filtre Moyenneur
    for r in $Range
    do
        ./Plot $DB $N\_MOY voisins V $NameArg $r
    done

    for v in $voisins 
    do
        ./Plot $DB $N\_MOY voisins $v $NameArg R
    done
    }&
    T1=${!}

    {   # Filtre Médian
    for r in $Range
    do
        ./Plot $DB $N\_MED voisins V $NameArg $r
    done

    for v in $voisins 
    do
        ./Plot $DB $N\_MED voisins $v $NameArg R
    done
    }&
    T2=${!}

    { # Filtre Gaussien
    for mean in $mean_GAU
    do
        for var in $var_GAU
        do
            for r in $Range
            do
                ./Plot $DB $N\_GAU voisins V moyenne_debruit $mean ecart-type_debruit $var $NameArg $r
            done

            for v in $voisins 
            do
                ./Plot $DB $N\_GAU voisins $v moyenne_debruit $mean ecart-type_debruit $var $NameArg R
            done
        done
    done
    for v in $voisins
    do
        for var in $var_GAU
        do
            for r in $Range
            do
                ./Plot $DB $N\_GAU voisins $v moyenne_debruit 0 ecart-type_debruit E $NameArg $r
            done
        done
    done
    }&
    T3=${!}

    { # Filtre Gradient 
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
                ./Plot $DB $N\_GRA voisins $v type 0 $NameArg $r
                ./Plot $DB $N\_GRA voisins $v type 1 $NameArg $r
            done
        done
    done
    }&
    T4=${!}

    { # Filtre Pondéré
    for v in $voisins 
    do
        for r in $Range
        do
            ./Plot $DB GAU_PON voisins $v puissance P $NameArg $r
        done
    done
    for p in $puissance
    do
        for r in $Range
        do
            ./Plot $DB GAU_PON voisins V puissance $p $NameArg $r
        done
    done
    for v in $voisins 
    do
        for p in $puissance
        do
            ./Plot $DB GAU_PON voisins $v puissance $p $NameArg R
        done
    done
    }&
    T5=${!}

    { # Non Local Mean
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for v in $voisinsNLM 
            do
                ./Plot $DB GAU_NLM voisins $v tailleRecherche $t ponderation $p $NameArg R
            done
        done
    done
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for r in $Range
            do
                ./Plot $DB GAU_NLM voisins V tailleRecherche $t ponderation $p $NameArg $r
            done
        done
    done
    for v in $voisinsNLM 
    do
        for t in $tailleRecherche
        do
            for r in $Range
            do
                ./Plot $DB GAU_NLM voisins $v tailleRecherche $t ponderation P $NameArg $r
            done
        done
    done
    for p in $ponderation
    do
        for v in $voisinsNLM 
        do
            for r in $Range
            do
                ./Plot $DB GAU_NLM voisins $v tailleRecherche T ponderation $p $NameArg $r
            done
        done
    done
    }&
    T6=${!}

    wait ${T1}
    wait ${T2}
    wait ${T3}
    wait ${T4}
    wait ${T5}
    wait ${T6}
}

RunPoivreEtSel()
{
    Range_prop='0.1'
    # 0.05 0.10 0.15 0.20 0.25 0.30 0.35'

    for prop in $Range_prop
    do
        Other PES $prop
    done

    PlotOther PES proportion $Range_prop
}
    

RunPoisson()
{
    Range_moy='1 5 10 15 20 25 30 35'

    for moy in $Range_moy
    do
        Other POI $moy
    done

    PlotOther POI moyenne_poisson $Range_moy
}

RunImpulsif()
{
    Range_fact='1 5 10 15 20 25 30 35'

    for fact in $Range_fact
    do
        Other IMP $fact
    done

    PlotOther IMP facteur $Range_fact
}

RunSpeckle()
{
    Range_intens='1 5 10 15 20 25 30 35'

    for intens in $Range_intens
    do
        Other SPE $intens
    done

    PlotOther SPE intensite $Range_intens
}

{
    RunGaussien 
}&
Th1=${!}
{ 
    RunPoivreEtSel 
}&
Th2=${!}
{
    RunPoisson
}&
Th3=${!}
{
    RunImpulsif
}&
Th4=${!}
{
    RunSpeckle
}&
Th5=${!}

wait ${Th1}
wait ${Th2}
wait ${Th3}
wait ${Th4}
wait ${Th5}
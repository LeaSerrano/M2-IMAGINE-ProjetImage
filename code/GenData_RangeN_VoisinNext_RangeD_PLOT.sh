
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

      # Filtre Moyenneur
    for v in $voisins 
    do
        DirD=$DirN\_MOY\_$v
        FileF=GAU_MOY

        sh GenImg.sh $DB D $DirN MOY $v
        sh GenImg.sh $DB M $DirD
        sh GenImg.sh $DB F $DirD $FileF 3 $v $1 $2
        sh GenImg.sh DB/$DB\_D/$DirD R 

        for mode in PSNR SNR SSIM RMSE
        do
            rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
        done
    done
    

   # Filtre Médian
    for v in $voisins 
    do
        DirD=$DirN\_MED\_$v
        FileF=GAU_MED

        sh GenImg.sh $DB D $DirN MED $v
        sh GenImg.sh $DB M $DirD
        sh GenImg.sh $DB F $DirD $FileF 3 $v $1 $2
        sh GenImg.sh DB/$DB\_D/$DirD R 

        for mode in PSNR SNR SSIM RMSE
        do
            rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
        done
    done
    

     # Filtre Gaussien
    for v in $voisins 
    do
        for mean in $mean_GAU
        do
            for var in $var_GAU
            do
                DirD=$DirN\_GAU\_$v\_$mean\_$var
                FileF=GAU_GAU

                sh GenImg.sh $DB D $DirN GAU $v $mean $var
                sh GenImg.sh $DB M $DirD
                sh GenImg.sh $DB F $DirD $FileF 5 $v $mean $var $1 $2
                sh GenImg.sh DB/$DB\_D/$DirD R 

                for mode in PSNR SNR SSIM RMSE
                do
                    rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
                done
            done
        done
    done
    

     # Filtre Gradient 
    for v in $voisins 
    do
        for c in 0 1 
        do
            DirD=$DirN\_GRA\_$v\_$c
            FileF=GAU_GRA

            sh GenImg.sh $DB D $DirN GRA $v $c
            sh GenImg.sh $DB M $DirD
            sh GenImg.sh $DB F $DirD $FileF 4 $v $c $1 $2
            sh GenImg.sh DB/$DB\_D/$DirD R 

            for mode in PSNR SNR SSIM RMSE
            do
                rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
            done
        done
    done
    

     # Filtre Pondéré
    for v in $voisins 
    do
        for p in $puissance
        do
            DirD=$DirN\_PON\_$v\_$p
            FileF=GAU_PON

            sh GenImg.sh $DB D $DirN PON $v $p
            sh GenImg.sh $DB M $DirD
            sh GenImg.sh $DB F $DirD $FileF 4 $v $p $1 $2
            sh GenImg.sh DB/$DB\_D/$DirD R 

            for mode in PSNR SNR SSIM RMSE
            do
                rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
            done
        done
    done
    

     # Non Local Mean
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for v in $voisinsNLM 
            do
                DirD=$DirN\_NLM\_$p\_$t\_$v
                FileF=GAU_NLM

                sh GenImg.sh $DB D $DirN NLM $p $t $v
                sh GenImg.sh $DB M $DirD
                sh GenImg.sh $DB F $DirD $FileF 5 $p $t $v $1 $2
                sh GenImg.sh DB/$DB\_D/$DirD R 

                for mode in PSNR SNR SSIM RMSE
                do
                    rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
                done
            done
        done
    done
    

    sh GenImg.sh DB/$DB\_N/$DirN R 
}

RunGaussien()
{
    Range_Mean='0 1 2'
    Range_STD='2.5 5 7.5 10 15 20 25 35'

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

    
      # Filtre Moyenneur
    for v in $voisins 
    do
        DirD=$DirN\_MOY\_$v
        FileF=$1\_MOY

        sh GenImg.sh $DB D $DirN MOY $v
        sh GenImg.sh $DB M $DirD
        sh GenImg.sh $DB F $DirD $FileF 2 $v $2
        sh GenImg.sh DB/$DB\_D/$DirD R 

        for mode in PSNR SNR SSIM RMSE
        do
            rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
        done
    done
    

     # Filtre Médian
    for v in $voisins 
    do
        DirD=$DirN\_MED\_$v
        FileF=$1\_MED

        sh GenImg.sh $DB D $DirN MED $v
        sh GenImg.sh $DB M $DirD
        sh GenImg.sh $DB F $DirD $FileF 2 $v $2
        sh GenImg.sh DB/$DB\_D/$DirD R 

        for mode in PSNR SNR SSIM RMSE
        do
            rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
        done
    done
    

     # Filtre Gaussien
    for v in $voisins 
    do
        for mean in $mean_GAU
        do
            for var in $var_GAU
            do
                DirD=$DirN\_GAU\_$v\_$mean\_$var
                FileF=$1\_GAU

                sh GenImg.sh $DB D $DirN GAU $v $mean $var
                sh GenImg.sh $DB M $DirD
                sh GenImg.sh $DB F $DirD $FileF 4 $v $mean $var $2
                sh GenImg.sh DB/$DB\_D/$DirD R 

                for mode in PSNR SNR SSIM RMSE
                do
                    rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
                done
            done
        done
    done
    

    # Filtre Gradient 
    for v in $voisins 
    do
        for c in 0 1 
        do
            DirD=$DirN\_GRA\_$v\_$c
            FileF=$1\_GRA

            sh GenImg.sh $DB D $DirN GRA $v $c
            sh GenImg.sh $DB M $DirD
            sh GenImg.sh $DB F $DirD $FileF 3 $v $c $2
            sh GenImg.sh DB/$DB\_D/$DirD R 

            for mode in PSNR SNR SSIM RMSE
            do
                rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
            done
        done
    done
    

     # Filtre Pondéré
    for v in $voisins 
    do
        for p in $puissance
        do
            DirD=$DirN\_PON\_$v\_$p
            FileF=$1\_PON

            sh GenImg.sh $DB D $DirN PON $v $p
            sh GenImg.sh $DB M $DirD
            sh GenImg.sh $DB F $DirD $FileF 3 $v $p $2
            sh GenImg.sh DB/$DB\_D/$DirD R 

            for mode in PSNR SNR SSIM RMSE
            do
                rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
            done
        done
    done
    

     # Non Local Mean
    for p in $ponderation
    do
        for t in $tailleRecherche
        do
            for v in $voisinsNLM 
            do
                DirD=$DirN\_NLM\_$p\_$t\_$v
                FileF=$1\_NLM

                sh GenImg.sh $DB D $DirN NLM $p $t $v
                sh GenImg.sh $DB M $DirD
                sh GenImg.sh $DB F $DirD $FileF 4 $p $t $v $2
                sh GenImg.sh DB/$DB\_D/$DirD R 

                for mode in PSNR SNR SSIM RMSE
                do
                    rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
                done
            done
        done
    done
    

    sh GenImg.sh DB/$DB\_N/$DirN R 
}

PlotOther()
{
    N=$1
    NameArg=$2
    Range=$3

       # Filtre Moyenneur
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
                ./Plot $DB $N\_GRA voisins $v type 0 $NameArg $r
                ./Plot $DB $N\_GRA voisins $v type 1 $NameArg $r
            done
        done
    done
    

    # Filtre Pondéré
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

    # Non Local Mean
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
}

RunPoivreEtSel()
{
    Range_prop='0.025 0.05 0.075 0.1 0.15 0.2 0.25 0.35'
    PlotOther PES proportion $Range_prop
    exit

    for prop in $Range_prop
    do
        Other PES $prop
    done

    # PlotOther PES proportion $Range_prop
}
    

RunPoisson()
{
    Range_moy='2.5 5 7.5 10 15 20 25 35'
    PlotOther POI moyenne_poisson $Range_moy
    exit

    for moy in $Range_moy
    do
        Other POI $moy
    done

    # PlotOther POI moyenne_poisson $Range_moy
}

RunImpulsif()
{
    Range_fact='2.5 5 7.5 10 15 20 25 35'
    PlotOther IMP facteur $Range_fact
    exit

    for fact in $Range_fact
    do
        Other IMP $fact
    done

    # PlotOther IMP facteur $Range_fact
}

RunSpeckle()
{
    Range_intens='2.5 5 7.5 10 15 20 25 35'
    PlotOther SPE intensite $Range_intens
    exit

    for intens in $Range_intens
    do
        Other SPE $intens
    done

    # PlotOther SPE intensite $Range_intens
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


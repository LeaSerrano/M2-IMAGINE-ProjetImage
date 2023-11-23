
# prepare executable
g++ Denoise_Data.cpp -o Denoise -O3
g++ Noise_Data.cpp -o Noise -O3
g++ Mesure_Data.cpp -o Mesure -O3
g++ GetMeanVar.cpp -o GetMeanVar -O3

# Use example

DB=$1

voisins='1 2 3 4 5 6'
voisinsNLM='1 2 3 4'

mean_GAU='0'
var_GAU='0.5 1 1.5 2 2.5'

puissance='0.5 0.7 0.9 1 1.1 1.3 1.5'

ponderation='0.25 0.5 0.75'
tailleRecherche='10 20 30'

Gaussien()
{
    sh GenImg.sh $DB N GAU $1 $2
    DirN=GAU\_$1\_$2

    
    {   # Filtre Moyenneur
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
    }&
    T1=${!}

    {   # Filtre Médian
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
    }&
    T2=${!}

    { # Filtre Gaussien
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
                sh GenImg.sh $DB F $DirD $FileF 3 $v $mean $var $1 $2
                sh GenImg.sh DB/$DB\_D/$DirD R 

                for mode in PSNR SNR SSIM RMSE
                do
                    rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
                done
            done
        done
    done
    }&
    T3=${!}

    { # Filtre Gradient 
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
    }&
    T4=${!}

    { # Filtre Pondéré
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
    }&
    T5=${!}

    { # Non Local Mean
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
    }&
    T6=${!}

    wait ${T1}
    wait ${T2}
    wait ${T3}
    wait ${T4}
    wait ${T5}
    wait ${T6}

    sh GenImg.sh DB/$DB\_N/$DirN R 
}

Other()
{
    sh GenImg.sh $DB N $1 $2
    DirN=$1\_$2

    
    {   # Filtre Moyenneur
    for v in $voisins 
    do
        DirD=$DirN\_MOY\_$v
        FileF=$1\_MOY

        sh GenImg.sh $DB D $DirN MOY $v
        sh GenImg.sh $DB M $DirD
        sh GenImg.sh $DB F $DirD $FileF 3 $v $2
        sh GenImg.sh DB/$DB\_D/$DirD R 

        for mode in PSNR SNR SSIM RMSE
        do
            rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
        done
    done
    }&
    T1=${!}

    { exit   # Filtre Médian
    for v in $voisins 
    do
        DirD=$DirN\_MED\_$v
        FileF=$1\_MED

        sh GenImg.sh $DB D $DirN MED $v
        sh GenImg.sh $DB M $DirD
        sh GenImg.sh $DB F $DirD $FileF 3 $v $2
        sh GenImg.sh DB/$DB\_D/$DirD R 

        for mode in PSNR SNR SSIM RMSE
        do
            rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
        done
    done
    }&
    T2=${!}

    { exit # Filtre Gaussien
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
                sh GenImg.sh $DB F $DirD $FileF 3 $v $mean $var $2
                sh GenImg.sh DB/$DB\_D/$DirD R 

                for mode in PSNR SNR SSIM RMSE
                do
                    rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
                done
            done
        done
    done
    }&
    T3=${!}

    { exit # Filtre Gradient 
    for v in $voisins 
    do
        for c in 0 1 
        do
            DirD=$DirN\_GRA\_$v\_$c
            FileF=$1\_GRA

            sh GenImg.sh $DB D $DirN GRA $v $c
            sh GenImg.sh $DB M $DirD
            sh GenImg.sh $DB F $DirD $FileF 4 $v $c $2
            sh GenImg.sh DB/$DB\_D/$DirD R 

            for mode in PSNR SNR SSIM RMSE
            do
                rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
            done
        done
    done
    }&
    T4=${!}

    { exit # Filtre Pondéré
    for v in $voisins 
    do
        for p in $puissance
        do
            DirD=$DirN\_PON\_$v\_$p
            FileF=$1\_PON

            sh GenImg.sh $DB D $DirN PON $v $p
            sh GenImg.sh $DB M $DirD
            sh GenImg.sh $DB F $DirD $FileF 4 $v $p $2
            sh GenImg.sh DB/$DB\_D/$DirD R 

            for mode in PSNR SNR SSIM RMSE
            do
                rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
            done
        done
    done
    }&
    T5=${!}

    { exit # Non Local Mean
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
                sh GenImg.sh $DB F $DirD $FileF 5 $p $t $v $2
                sh GenImg.sh DB/$DB\_D/$DirD R 

                for mode in PSNR SNR SSIM RMSE
                do
                    rm DB/$DB\_M/$mode/{$mode}\_$DirD.txt
                done
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

    sh GenImg.sh DB/$DB\_N/$DirN R 
}

RunGaussien()
{
    Gaussien 0 1
    Gaussien 1 1
    Gaussien 2 1

    Gaussien 0 5
    Gaussien 0 10
    Gaussien 0 15
    Gaussien 0 20
    Gaussien 0 25
    Gaussien 0 30
    Gaussien 0 35
}

RunPoivreEtSel()
{
    Other PES 1 
    exit
    Other PES 5
    Other PES 10 
    Other PES 15
    Other PES 20
    Other PES 25
    Other PES 30
    Other PES 35
}

RunPoisson()
{
    Other POI 1 
    Other POI 5
    Other POI 10 
    Other POI 15
    Other POI 20
    Other POI 25
    Other POI 30
    Other POI 35
}

RunImpulsif()
{
    Other IMP 1 
    Other IMP 5
    Other IMP 10 
    Other IMP 15
    Other IMP 20
    Other IMP 25
    Other IMP 30
    Other IMP 35
}

RunSpeckle()
{
    Other SPE 1 
    Other SPE 5
    Other SPE 10 
    Other SPE 15
    Other SPE 20
    Other SPE 25
    Other SPE 30
    Other SPE 35
}

RunPoivreEtSel 

rm Denoise
rm Noise
rm Mesure 
rm GetMeanVar
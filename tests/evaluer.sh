#!/bin/bash
##########################################################################
# UQAM - Département d'informatique
# INF3105 - Structures de données et algorithmes
# TP3
# http://ericbeaudry.ca/INF3105/tp3/
#
# Script d'évaluation du TP3
#
# Instructions:
# 1. Déposer ce script avec les fichiers problèmes dans un répertoire
#    distinct (ex: tests).
# 2. Se placer dans le répertoire contenant votre programme ou contenant
#    la liste des soumissions Oto (*.tp_oto).
# 3. Lancer ce script (ex: ../tests/evaluer.sh).
##########################################################################

cartes="uqam stejulie montsthilaire victoria montreal"

# Obtenir le chemin du répertoire contenant le présent script et les fichiers tests
pushd `dirname $0`
repertoire_tests=`pwd`
# Lister les fichier *-carte.txt disponibles
#cartes=`ls *-carte.txt`
popd

# Programme à évaluer
programme="tp3"

echo "UQAM | Département d'informatique"
echo "INF3105 | Structures de données et algorithmes"
echo "Évaluation du programme ${programme} ..."
echo

if [ `pwd` -ef $repertoire_tests ];
then
    echo "Ce script doit être dans un répertoire différent de celui contenant votre ${programme}."
    echo "Ce script a été arrêté afin de ne pas écraser les fichiers test_[CDG]*.resultat!"
    exit -2;
fi

# Exécutable du programme de validation
OS=`uname`
valideur="${repertoire_tests}/valideur"

if [ -x ${valideur}-${OS} ]
then
    valideur=${valideur}-${OS}
fi

if [ -x ${valideur} ]
then
    entetevalidation="Valid	Optimal	DistOK	RouteOK	/Total	Réussi?"
else
    entetevalidation="diff"
fi

# Détection si l'utilitaire time sous Linux est disponible (peut afficher 0.00)
echo "Détection de /usr/bin/time..."
/usr/bin/time -f %U echo 2>&1 > /dev/null
souslinux=$?

# Fixer les limites : 60 s, 2048Mo mémoire et 128Mo fichier
ulimit -t 60 -v 2097152 -f 131072
echo "ulimit (limites courantes des ressources) :"
ulimit -t -v -f
echo "-----"

date=`date +%Y%m%d_%H%M%S`
date2=`date "+%Y-%m-%d %H:%M:%S"`

# Détection du CPU
if [ -e /proc/cpuinfo ] ; then
    cpuinfo=`grep "model name" /proc/cpuinfo | sort -u | cut -d ":" -f 2`
else
    cpuinfo="?"
fi


##### Fonction d'évaluation d'un TP ######
function EvaluerTP
{
    ## Forcer la recompilation.
    #rm $programme *.o
    #make clean

    logfile="log-${date}.txt"
	echo "Les résultats seront déposés dans $logfile..."

	#echo "Machine : " `hostname`
	echo "#Machine : " `hostname`  > $logfile
	#echo "CPU :$cpuinfo" 
	echo "#CPU :$cpuinfo"  >> $logfile
	#echo "Date début : $date"
	echo "#Date début : $date"  >> $logfile
	#echo "Limite de `ulimit -t` secondes par test"
	echo "#Limite de `ulimit -t` secondes par test"  >> $logfile

    # Pour statistiques : nombre de lignes de code...
    echo "Taille des fichiers source :" >> ${logfile}
    wc *.{c*,h*}  >> ${logfile}

    taille=`wc *.{c*,h*} | grep total`
    sommaire="$taille"

    # Ne pas recompiler automatiquement
    #make $programme

    # Vérification de l'existance du programme.
    if [ ! -e ${programme} ]
	    then
	    echo "  Erreur : le fichier ${programme} est inexistant!"
	    return
    fi
    if [ ! -x ${programme} ]
	    then
	    echo "  Erreur : le fichier ${programme} n'est pas exécutable!"
	    return
    fi


	echo 
	echo "#" >> $logfile

	echo -e "Test            \tMiss\tCPU\tMém.(k)\t${entetevalidation}"
	echo -e "Test            \tMiss\tCPU\tMém.(k)\t${entetevalidation}" >> $logfile

	for carte in $cartes;
	do
            carte=${carte%-carte.txt}
	    # Lister les fichiers tests pour le réseau actuel
        tests="`cd $repertoire_tests && ls ${carte}-miss*-*.txt`"
        
        for test in $tests;
        do
            fichiercarte="${repertoire_tests}/${carte}-carte.txt"
            fichiertest="${repertoire_tests}/${test}"
            fichierresultat="${test%.txt}.resultat"
            fichiervalidation="${test%.txt}.validation"
    	    fichiersolution="$repertoire_tests/${test%.txt}.solution"
    	    nbmissions=`cat $fichiertest | wc -l`
    	    
    	    #echo "./$programme $fichiercarte $fichiertest > $fichierresultat"
            if [ $souslinux -eq 0 ]; then
                tcpu="`(/usr/bin/time -f "%U\t%Mk" ./$programme $fichiercarte $fichiertest > $fichierresultat) 2>&1 | tail -n 1`"
        	else
            	tcpu=`(time -p ./$programme $fichiercarte $fichiertest > $fichierresultat) 2>&1 | egrep user | cut -f 2 -d " "`
            fi
        
        	verif="?"
        	if ( [ -x ${valideur} ] && [ -e $fichierresultat ] )
	        then
	            $valideur $fichiercarte $fichiertest $fichierresultat $fichiersolution > $fichiervalidation
            	verif=`tail -n 1 $fichiervalidation`
        	else
            	if( [ -e $fichiersolution ] )
            	then
                	diff -tibw $fichierresultat $fichiersolution 2>&1 > /dev/null
            		if [ $? -eq 0 ];
            		then
            	    	verif="OK"
             		else
             	    	verif="Different"
            		fi
            	fi
        	fi

        	echo -e "$test\t$nbmissions\t$tcpu\t$verif"
        	echo -e "$test\t$nbmissions\t$tcpu\t$verif" >> $logfile
        	sommaire="${sommaire}\t\t${tcpu}\t${verif}"

        done
    
	done
}

# Lister les soumissions Oto (répertoires terminant par .tp_oto)
tps=`ls -1 | grep .tp_oto`
# Si aucun répertoire .tp_oto n'existe, essayer le répertoire courant (auto-évaluation)
if [ ! -n "$tps" ]; then
    tps="."
fi

# Génération de l'entête du rapport
echo "#Rapport de correction INF3105 / $programme" > "rapport-${date}.txt"
echo -e "#Date:\t${date2}" >> "rapport-${date}.txt"
echo -e "#Machine :\t" `hostname` >> "rapport-${date}.txt"
echo -e "#CPU :\t$cpuinfo" >> "rapport-${date}.txt"
echo >> "rapport-${date}.txt"

# Génération des titres des colonnes
echo -e -n "#\t\t\t" >> "rapport-${date}.txt"
for carte in ${cartes}; 
do
    carte=${carte%-carte.txt}
    tests="`cd $repertoire_tests && ls ${carte}-miss*-*.txt`"
    for test in $tests;
    do
        echo -e -n "$test\t\t\t\t\t\t\t\t\t" >> "rapport-${date}.txt"
    done        
done
echo >> "rapport-${date}.txt"

echo -e -n "#Soumission\tTaille sources" >> "rapport-${date}.txt"
for carte in ${cartes}; 
do
    carte=${carte%-carte.txt}
    tests="`cd $repertoire_tests && ls ${carte}-miss*-*.txt`"
    for test in $tests;
    do
        echo -e -n "\t\tCPU\tMem\t${entetevalidation}" >> "rapport-${date}.txt"
    done        
done
echo >> "rapport-${date}.txt"

# Itération sur chaque TP
for tp in $tps; do
    sommaire=""
    echo "## CORRECTION : $tp"
    pushd $tp
    	EvaluerTP
    popd
    #echo -e ">> ${sommaire}"
    echo -e "${tp}\t${sommaire}" >> "rapport-${date}.txt"
done





# Makefile pour TP3
# à adapter au besoin

OPTIONS = -Wall

#Option -g pour avoir les infos de débogage
#OPTIONS = -g -O0 -Wall

#Option -O2 pour le code optimisé
OPTIONS = -O2 -Wall

# Syntaxe : cible : dépendance1 dépendance2 ...
# Ensuite, la ou les ligne(s) débutant par une tabulation (\t) donne les commandes pour construire une cible

all : tp3

tp3: tp3.o carte.o coordonnee.o
	g++ $(OPTIONS) -o tp3 tp3.o carte.o coordonnee.o

tp3.o: tp3.cpp carte.h coordonnee.h
	g++ $(OPTIONS) -c -o tp3.o tp3.cpp

carte.o: carte.h carte.cpp coordonnee.h
	g++ $(OPTIONS) -c -o carte.o carte.cpp

coordonnee.o: coordonnee.h coordonnee.cpp
	g++ $(OPTIONS) -c -o coordonnee.o coordonnee.cpp

clean :
	rm -f *.o
	rm -f tp3
	rm -f *~


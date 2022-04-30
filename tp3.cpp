/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <math.h>
#include "carte.h"

using namespace std;

void tp3(Carte& carte, istream& ismissions)
{
    while(ismissions) {

        string nomlieuaffaire;
        char deuxpoints = 0;
        ismissions >> nomlieuaffaire >> deuxpoints;
        if(!ismissions) break;
        assert(deuxpoints==':');
        list<string> destinations;
        while(ismissions){
            string destination;
            ismissions >> destination; // ne pas ajouter « >> std::std » ici.
            if(destination==";" || !ismissions) break;
            assert(destination.find(";")==string::npos);
            destinations.push_back(destination);
        }
        
        list<string> chemin_noeuds, chemin_routes;
        double distance = carte.calculer_trajet(nomlieuaffaire, destinations, chemin_noeuds, chemin_routes);

        for(auto iter = chemin_noeuds.begin() ; iter != chemin_noeuds.end() ; ++iter )
            cout << *iter << " ";
        cout << endl;
        for(auto iter = chemin_routes.begin() ; iter != chemin_routes.end() ; ++iter )
            cout << *iter << " ";
        cout << endl;
        cout << round(distance) << " m" << endl;
    }
    
}

int main(int argc, const char** argv)
{
    if(argc<=1 || argc>3){
        cout << "Syntaxe: ./tp3 carte.txt [missions.txt]" << endl;
        return 1;
    }
    // Lecture de la carte.
    Carte carte;
    {
        ifstream fichiercarte(argv[1]);
        if(fichiercarte.fail()){
            cout << "Erreur ouverture du fichier : " << argv[1] << endl;    
            return 2;
        }
        fichiercarte >> carte;
        //carte.afficher_noeuds();
    }
    if(argc>2){
        // Lecture des missions depuis un fichier.
        ifstream ismissions(argv[2]);
        if(ismissions.fail()){
            cout << "Erreur d'ouverture du fichier : " << argv[2] << endl;
            return 3;
        }else
            tp3(carte, ismissions);
    }else{
        // Lecture depuis stdin (console).
        tp3(carte, cin);
    }
    return 0;
}


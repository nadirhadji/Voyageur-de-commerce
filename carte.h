/**
 * @file carte.h
 * @author Nadir Hadji
 * @brief 
 * @version 0.1
 * @date 2022-04-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef CARTE_HEADER
#define CARTE_HEADER
#include <stdlib.h>
#include <assert.h>
#include <limits>
#include <istream>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <math.h>
#include "coordonnee.h"
using namespace std;

struct Arc
{
  string id_voisin;
  double distance;
  string id_rue;

  Arc() : id_voisin("-"), distance(numeric_limits<int>::infinity()) , id_rue("") {}
  Arc(string v, double d) : id_voisin(v) , distance(d) {}
  Arc(string p, double d, string r) : id_voisin(p) , distance(d) , id_rue(r) {}

  Arc& operator = (const Arc& a)
  {
    id_voisin = a.id_voisin;
    distance = a.distance;
    id_rue = a.id_rue;
    return *this;
  }

  bool operator < (const Arc& autre) const { return distance < autre.distance; }
  bool operator > (const Arc& autre) const { return distance > autre.distance; }
  bool operator <= (const Arc& autre) const { return distance <= autre.distance; }
  bool operator >= (const Arc& autre) const { return distance >= autre.distance; }
  bool operator == (const Arc& autre) const { return distance == autre.distance; }
  bool operator != (const Arc& autre) const { return distance != autre.distance; }
};

struct Noeud // Un noeud est un sommet dans le graphe
{             
  Coordonnee coord;
  set<Arc> voisins; // ensemble des noeuds liés par un segment (tronçon) de route (arête).
  Noeud& operator = (const Noeud& n) 
  {
    coord = n.coord;
    voisins = n.voisins;
    return *this;
  }
};

struct Voisin
{
  string id_voisin;
  double distance;
  Voisin(string s, double d) : id_voisin(s) , distance(d) {}
};

struct compare_voisin
{
  bool operator()(const Voisin& gauche, const Voisin& droite)
  {
      return gauche.distance < droite.distance;
  }
};

class Carte
{
  private:

    pair<bool,int> trouver_indice_route(string& nom );

    int     obtenir_indice_nom_route( string& nom);

    void    djikstra( string& nom_origine, map<string, pair<double,string>>& table_distances);

    double  calculer_prochaine_destination ( string& origine, list<string>& destinations, 
                            list<string>& chemin_noeuds, list<string>& chemin_routes );

    double  lire_table_djikstra( string& origine, map<string,pair<double,string>>& table_distances, 
                            list<string>& chemin_noeuds, list<string>& chemin_routes);

    map<string,Noeud> noeuds;
    vector<string> routes;

  public:

    void    ajouter_lieu(string& nom, Coordonnee& c);

    void    ajouter_route(string& nom, list<string>& noms);

    double  calculer_trajet(string& origine, list<string>& destination, 
                          std::list<string>& out_cheminnoeuds, std::list<string>& out_cheminroutes);

    double  calculer_chemin( string& origine,  string& destination,
                          std::list<string>& out_cheminnoeuds, std::list<string>& out_cheminroutes);

    void    afficher_noeuds() const;

  friend istream& operator >> (istream& is, Carte& carte);
};

#endif



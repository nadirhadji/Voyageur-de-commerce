/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  Automne 2014 / TP3
  
  Squelette de départ. Vous pouvez modifier ce fichier à votre guise.
  Vous n'êtes pas oubligés de l'utiliser.

  En principe, vous ne devriez pas avoir à modifier la classe Coordonnee.
*/


#include <assert.h>
#include <math.h>
#include "coordonnee.h"

#define PI 3.14159265359
#define RAYONTERRE 6371000  // en mètres

Coordonnee::Coordonnee(const Coordonnee& coor)
  : latitude(coor.latitude), longitude(coor.longitude)
{
}

Coordonnee::Coordonnee(double latitude_, double longitude_) 
  : latitude(latitude_), longitude(longitude_)
{
}

double Coordonnee::distance(const Coordonnee& coor) const {
  double s1 = sin((coor.latitude-latitude)/2);
  double s2 = sin((coor.longitude-longitude)/2);
  
  return 2*RAYONTERRE * asin(sqrt(s1*s1 + cos(latitude)*cos(coor.latitude)*s2*s2));
}

std::ostream& operator << (std::ostream& os, const Coordonnee& coor) {
  os << "(" 
     << (coor.latitude * 180.0 / PI)
     << "," 
     << (coor.longitude * 180.0 / PI)
     << ")";
  return os;
}

std::istream& operator >> (std::istream& is, Coordonnee& coor) {
  char po, vir, pf;
  is >> po;
  if(is){
    is >> coor.latitude >> vir >> coor.longitude >> pf;
    assert(po=='(');
    assert(vir==',');
    assert(pf==')');
    coor.latitude  *= PI / 180.0;
    coor.longitude *= PI / 180.0;
  }
  return is;
}




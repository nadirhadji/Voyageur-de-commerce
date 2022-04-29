/*
  INF3105 - Structures de données et algorithmes
  UQAM / Département d'informatique
  Automne 2014 / TP3
  
  Squelette de départ. Vous pouvez modifier ce fichier à votre guise.
  Vous n'êtes pas oubligés de l'utiliser.

  En principe, vous ne devriez pas avoir à modifier la classe Coordonnee.
*/

#if !defined(__COORDONNEE_H__)
#define __COORDONNEE_H__

#include <iostream>

class Coordonnee {

  public:
    Coordonnee(){}
    Coordonnee(double latitude_, double longitude_);
    Coordonnee(const Coordonnee&);

    double distance(const Coordonnee&) const;

  private:
    double latitude;
    double longitude;

  friend std::ostream& operator << (std::ostream&, const Coordonnee&);
  friend std::istream& operator >> (std::istream&, Coordonnee&);
};

#endif


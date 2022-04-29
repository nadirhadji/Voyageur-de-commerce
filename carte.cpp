/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#include "carte.h"
#include <limits>

//Rechercher la position d'un nom de rue dans le vecteur routes.
pair<bool,int> Carte::trouverIndiceRoute( string& nom )
{
    std::pair<bool, int > resultat;

    // Trouver l'element dans le vecteur
    auto it = std::find(routes.begin(), routes.end(), nom);
    if (it != routes.end())
    {
        resultat.second = distance(routes.begin(), it);
        resultat.first = true;
    }
    else
    {
        resultat.first = false;
        resultat.second = -1;
    }
    return resultat;
}

//Obtenir l'indice du nom de route. Insertion si il n'existe pas.
int Carte::obtenirIndiceNomRoute( string& nom)
{
    pair<bool,int> indice = trouverIndiceRoute(nom);
    int positionNom = indice.second;
    if ( indice.first == false ) 
    {
        int taille = routes.size();
        routes.push_back(nom);
        positionNom = taille;
    }
    return positionNom;
}

void Carte::ajouterLieu( string& nomlieu, Coordonnee& c)
{
    Noeud noeud;
    noeuds[nomlieu] = noeud;
    noeuds[nomlieu].coord = c;
}

void Carte::ajouterRoute( string& nomroute,  list<string>& route) 
{   
    int positionNomRoute = obtenirIndiceNomRoute(nomroute);
    list<string>::const_iterator  it = route.begin();
    string id_parent = (*it);
    ++it;
    while ( it != route.end() )
    {
        string id_voisin = (*it);
        int distance = noeuds[id_parent].coord.distance(noeuds[id_voisin].coord);
        Arc arc(id_voisin,distance,positionNomRoute);
        noeuds[id_parent].voisins.insert(arc);
        id_parent = id_voisin;
        ++it;
    }
}

void Carte::afficher_noeuds() const
{
    for (const auto& item : noeuds)
    {
        std::cout << item.first << ": ";

        std::set<Arc>::iterator iter = item.second.voisins.begin();

        while( iter != item.second.voisins.end() )
        {
            std::cout << (*iter).id_voisin << " ";
            std::cout << (*iter).distance << " ";
            std::cout << "rue : " << routes[(*iter).id_rue] << "; ";
            iter++;
        }
        std::cout << std::endl;
    }
}

/* Lire une carte. */
istream& operator >> (istream& is, Carte& carte)
{
    // Lire les lieux
    while(is){
        string nomlieu;
        is >> nomlieu;
        if(nomlieu == "---") break;
        Coordonnee coor;
        is >> coor;       
        carte.ajouterLieu(nomlieu, coor);
    }

    // Lire les routes
    while(is){
        string nomroute;
        is >> nomroute;
        if(nomroute == "---" || nomroute=="" || !is) break;
        
        char deuxpoints;
        is >> deuxpoints;
        assert(deuxpoints == ':');
        
        std::list<std::string> listenomslieux;

        string nomlieu;        
        while(is){
            is>>nomlieu;
            if(nomlieu==";") break;
            assert(nomlieu!=":");
            assert(nomlieu.find(";")==string::npos);
            listenomslieux.push_back(nomlieu);
        }
        
        assert(nomlieu==";");
        carte.ajouterRoute(nomroute, listenomslieux);
    }
    
    return is;
}

//Implementation de l'algorithme de dijskstra pour le calcul des chemins
//en plus court chemin
//registre = map < sommet , paire < distance , nom_sommet_parent > >
void Carte::djikstra(string& nom_origine, map<string,pair<double,string>>& registre )
{
    //Initialisation de la table distance/parent pour chaque noeud O(n)
    for ( const auto& item : noeuds )
    {
        pair<double,string> paire(numeric_limits<double>::infinity(),"-");
        registre[item.first] = paire;
    }

    registre[nom_origine].first = 0;

    //Init de Q avec tout les voisin du sommet d'origine.
    priority_queue<Voisin, vector<Voisin>, compare_voisin> queue;
    queue.push(Voisin(nom_origine,0));

    while ( ! queue.empty() )
    {
        string nom_premier = queue.top().id_voisin;
        queue.pop();
        if (registre[nom_premier].first == numeric_limits<double>::infinity()) break;

        for ( auto element : noeuds[nom_premier].voisins )
        {
            int d = registre[nom_premier].first + element.distance;

            if ( d < registre[element.id_voisin].first && d != 0 )
            {
                registre[element.id_voisin].first = d;
                registre[element.id_voisin].second = nom_premier;
                queue.push( Voisin(element.id_voisin,d) );
            }
        }
    }

    // std::cout << "\n Afficher Registre Djikstra" << std::endl;

    // for ( auto elem : registre)
    // {
    //     std::cout << elem.first << " : " << elem.second.first << " / " << elem.second.second << std::endl;
    // }
}

double Carte::LireRegistreDjikstra( string& nom_destination, map<string,pair<double,string>>& registre, 
                        list<string>& chemin)
{
    double total = registre[nom_destination].first;
    string str_courrant = nom_destination;
    chemin.push_front(str_courrant);

    while (registre[str_courrant].first != 0 )
    {
        chemin.push_front(registre[str_courrant].second);
        str_courrant = registre[str_courrant].second;
    }
    return total;
}

//TODO : trouver un moyen de retrouner le chemin trouvé
void Carte::calculerProchaineDestination(string& nom_origine, 
                                    list<string>& noms_destinations, 
                                    map<string,pair<double,string>>& registre,
                                    pair<double,list<string>>& elue )
{
    double distance_min = numeric_limits<double>::infinity();
    list<string> chemin_min;
    string destination_elue;
    //djikstra(nom_origine,registre);

    for ( list<string>::iterator iter = noms_destinations.begin() ; iter != noms_destinations.end() ; iter++ ) 
    {
        list<string> chemin_courrant;
        double distance_courrante = LireRegistreDjikstra((*iter),registre,chemin_courrant);
        if (distance_courrante < distance_min )
        {
            distance_min = distance_courrante;
            chemin_min = chemin_courrant;
            destination_elue = (*iter);
        }
    }
    elue.first = distance_min;
    elue.second = chemin_min;
}

double Carte::calculerTrajet(string& nom_origine, list<string>& noms_destinations, 
                             std::list<string>& out_chemin_noeuds, std::list<string>& out_chemin_routes) 
{
    string point_initial = nom_origine;
    string position = nom_origine;
    double total = 0;

    map< string , pair<double,string> > registre;
    djikstra(nom_origine,registre);

    //list<string>::const_reverse_iterator
    while(noms_destinations.size() != 0 )
    {
        //map<string,pair<double,string>> registre;
        pair<double,list<string>> destination_elue;
        calculerProchaineDestination(position,noms_destinations,registre,destination_elue); 
        position = destination_elue.second.back();
        noms_destinations.remove(destination_elue.second.back());

        for (std::list<string>::iterator it = destination_elue.second.begin(); it != destination_elue.second.end(); ++it){
            std::cout << (*it) << " -> ";
        }
        std::cout << " = " << destination_elue.first << " \n";

        total += destination_elue.first;
    }

    noms_destinations.push_back ( point_initial );
    map<string,pair<double,string>> registre_final;
    pair<double,list<string>> prochain_final;
    calculerProchaineDestination( position , noms_destinations , registre_final , prochain_final ); 
    position = prochain_final.second.back();
    noms_destinations.remove( prochain_final.second.back() );

    for (std::list<string>::iterator it = prochain_final.second.begin(); it != prochain_final.second.end(); ++it){
            std::cout << (*it) << " -> ";
    }
    std::cout << " = " << prochain_final.first;

    total += prochain_final.first;

    std::cout << " \n total = " << total << "\n\n" << endl;

    return total;
}

double Carte::calculerChemin( string& nom_origine,  string& nom_destination,
                             std::list<string>& out_chemin_noeuds, std::list<string>& out_chemin_routes) 
{
    // À compléter.
    return numeric_limits<double>::infinity();
}
                    
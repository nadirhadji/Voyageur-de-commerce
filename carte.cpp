/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2014
 *  Squelette pour le TP3
*/

#include "carte.h"

//Rechercher la position d'un nom de rue dans le vecteur routes.
pair<bool,int> Carte::trouver_indice_route( string& nom )
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
int Carte::obtenir_indice_nom_route( string& nom)
{
    pair<bool,int> indice = trouver_indice_route(nom);
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
    int position_nom_route = obtenir_indice_nom_route(nomroute);
    list<string>::const_iterator  it = route.begin();
    string id_parent = (*it);
    ++it;
    while ( it != route.end() )
    {
        string id_voisin = (*it);
        double distance = noeuds[id_parent].coord.distance(noeuds[id_voisin].coord);
        Arc arc(id_voisin,distance,position_nom_route);
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

//Algorithme de dijkstra pour le calcul des chemins en plus court chemin
//registre = map < sommet , paire < distance , nom_sommet_parent > >
void Carte::djikstra( string& origine, map<string,pair<double,string>>& registre )
{
    //Initialisation de la table distance/parent pour chaque noeud O(n)
    for ( const auto& item : noeuds )
    {
        pair<double,string> paire(numeric_limits<double>::infinity(),"-");
        registre[item.first] = paire;
    }

    registre[origine].first = 0;

    //Init de Q avec tout les voisin du sommet d'origine.
    priority_queue<Voisin, vector<Voisin>, compare_voisin> queue;
    queue.push(Voisin(origine,0));

    while ( ! queue.empty() )
    {
        string nom_premier = queue.top().id_voisin;
        queue.pop();
        if (registre[nom_premier].first == numeric_limits<double>::infinity()) break;

        for ( auto element : noeuds[nom_premier].voisins )
        {
            double d = registre[nom_premier].first + element.distance;

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

double Carte::lire_table_djikstra( string& nom_destination, map<string,pair<double,string>>& registre, 
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
    chemin.pop_front();
    return total;
}

//TODO : trouver un moyen de retrouner le chemin trouvé
double Carte::calculer_prochaine_destination ( string& origine, list<string>& destinations, 
                            list<string>& chemin_noeuds )
{
    double distance_minimale = numeric_limits<double>::infinity();
    list<string> chemin_minimale;
    map<string,pair<double,string>> table_chemins_minimal;
    djikstra(origine,table_chemins_minimal);

    for ( auto iter = destinations.begin() ; iter != destinations.end() ; iter++ ) 
    {
        list<string> chemin;
        double distance = lire_table_djikstra((*iter),table_chemins_minimal,chemin);
        if (distance < distance_minimale )
        {
            distance_minimale = distance;
            chemin_minimale = chemin;
        }
    }
    chemin_noeuds.splice(chemin_noeuds.end(),chemin_minimale);
    return distance_minimale;
}

double Carte::calculerTrajet (string& origine, list<string>& a_visiter, 
                        list<string>& chemin_noeuds, list<string>& chemin_routes) 
{
    chemin_noeuds.push_front(origine);
    string point_initial = origine;
    string position = origine;
    double total = 0;

    while(a_visiter.size() != 0 )
    {
        double distance = calculer_prochaine_destination(position,a_visiter,chemin_noeuds); 
        position = chemin_noeuds.back();
        a_visiter.remove(chemin_noeuds.back());
        total += distance;
    }

    a_visiter.push_back(point_initial);
    double distance_retour = calculer_prochaine_destination( position,a_visiter,chemin_noeuds); 
    total += distance_retour;

    for (auto it = chemin_noeuds.begin(); it != chemin_noeuds.end(); ++it) {
            std::cout << (*it) << " ";
    }
    std::cout << " \n" << round(total) << " m\n";

    return total;
}

double Carte::calculerChemin( string& nom_origine,  string& nom_destination,
                             std::list<string>& out_chemin_noeuds, std::list<string>& out_chemin_routes) 
{
    // À compléter.
    return numeric_limits<double>::infinity();
}
                    
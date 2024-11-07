/**
 * \file NombrePremier.cpp
 * \brief Programme de tests du multithread pour trouver des nombres premiers.
 * \author Simon.D, Gabin.D, Cabrel, Jaures
 * \version 0.1
 * \date 7 Novembre 2024
 *
 * Programme de tests du multithread sur des nombres premiers.
 * Utilise des threads pour calculer les nombres premiers dans un intervalle donné.
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <mutex>

/// Liste globale pour stocker les nombres premiers trouvés.
std::vector<int> listePremier;

/**
 * \brief Fonction qui vérifie si un nombre est premier.
 * 
 * Cette fonction détermine si un nombre entier est premier ou non.
 * Elle fonctionne en vérifiant si le nombre est divisible par 2, 3 ou tout autre nombre premier inférieur à sa racine carrée.
 *
 * \param valeur Entier à tester pour savoir s'il est premier.
 * \return true si le nombre est premier, false sinon.
 */
bool estPremier(int valeur)
{
    if (valeur == 2 || valeur == 3 || valeur == 5 || valeur == 7)
    {
        return true;
    }
    else if (valeur == 1 || valeur % 2 == 0)
    {
        return false;
    }
    for (int i = 3; i <= std::round(std::sqrt(valeur)); i += 2)
    {
        if (valeur % i == 0)
        {
            return false;
        }
    }
    return true;
}

/**
 * \brief Fonction qui calcule les nombres premiers dans un intervalle en utilisant un thread.
 * 
 * Cette fonction permet à un thread de rechercher les nombres premiers dans un intervalle défini par les paramètres `id`, `fin` et `threadsUtilise`.
 * Chaque thread parcourt les nombres dans l'intervalle en fonction de son ID et de l'utilisation du multithreading.
 *
 * \param id L'index du thread (utilisé pour répartir le travail entre les threads).
 * \param fin La valeur maximale de l'intervalle dans lequel chercher des nombres premiers.
 * \param threadsUtilise Le nombre total de threads pour la division du travail.
 */
void trouverNombresPremiers(int id, int fin, int threadsUtilise)
{
    std::vector<int> resultatsLocaux;
    for (int i = id; i <= fin; i += threadsUtilise)
    {
        if (estPremier(i))
        {
            resultatsLocaux.push_back(i);
        }
    }

    // Insertion des résultats locaux dans la liste principale des nombres premiers
    listePremier.insert(listePremier.end(), resultatsLocaux.begin(), resultatsLocaux.end());
}

/**
 * \brief Fonction qui gère le calcul des nombres premiers en utilisant des threads.
 * 
 * Cette fonction initialise les threads et les répartit pour calculer les nombres premiers dans l'intervalle de 1 à `fin`.
 * Les résultats sont ensuite enregistrés dans un fichier.
 *
 * \param threadsUtilise Le nombre de threads à utiliser pour le calcul.
 * \param fin La valeur maximale pour l'intervalle dans lequel chercher des nombres premiers.
 */
void calculerNombresPremiers(int threadsUtilise, int fin)
{
    std::vector<std::thread> listeAsync;
    auto begin = std::chrono::high_resolution_clock::now();

    // Création des threads pour rechercher les nombres premiers dans l'intervalle
    for (int i = 0; i < threadsUtilise; i++)
    {
        listeAsync.push_back(std::thread(trouverNombresPremiers, i, fin, threadsUtilise));
    }

    // Attente de la fin de l'exécution de tous les threads
    for (auto& t : listeAsync)
    {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << elapsed.count() << " microsecondes  " << " \t";

    // Enregistrement des résultats dans un fichier
    std::ofstream fichier("nombres_premiers.txt");
    fichier.clear();
    if (fichier.is_open())
    {
        for (int premier : listePremier)
        {
            fichier << premier << std::endl;
        }
        fichier.close();
        std::cout << "Les nombres premiers ont ete enregistres dans 'nombres_premiers.txt'." << std::endl;
    }
    else
    {
        std::cerr << "Erreur : impossible d'ouvrir le fichier." << std::endl;
    }
}

/**
 * \brief Fonction qui calcule la rentabilité du multithreading pour le calcul des nombres premiers.
 * 
 * Cette fonction calcule le pourcentage de rentabilité du multithreading en comparant le temps d'exécution avec un seul thread et avec plusieurs threads.
 *
 * \param threadsUtilise Le nombre de threads à utiliser pour le calcul.
 * \param fin La valeur maximale pour l'intervalle dans lequel chercher des nombres premiers.
 * \param maxThreads Le nombre maximal de threads disponibles.
 * \return Le pourcentage de rentabilité.
 */
double calculerRentabilite(int threadsUtilise, int fin, int maxThreads)
{
    // Temps d'exécution avec un seul thread
    auto beginSingle = std::chrono::high_resolution_clock::now();
    calculerNombresPremiers(1, fin);
    auto endSingle = std::chrono::high_resolution_clock::now();
    auto elapsedSingle = std::chrono::duration_cast<std::chrono::microseconds>(endSingle - beginSingle);

    // Temps d'exécution avec le nombre de threads spécifié
    auto beginMulti = std::chrono::high_resolution_clock::now();
    calculerNombresPremiers(threadsUtilise, fin);
    auto endMulti = std::chrono::high_resolution_clock::now();
    auto elapsedMulti = std::chrono::duration_cast<std::chrono::microseconds>(endMulti - beginMulti);

    // Calcul du pourcentage de rentabilité
    double rentabilite = ((elapsedSingle.count() - elapsedMulti.count()) / static_cast<double>(elapsedSingle.count())) * 100;

    return rentabilite;
}

/**
 * \brief Fonction principale qui gère l'exécution du programme.
 * 
 * Cette fonction initialise le nombre de threads à utiliser et appelle la fonction `calculerNombresPremiers` pour différentes plages de nombres.
 * Elle appelle également la fonction `calculerRentabilite` pour afficher la rentabilité du multithreading.
 *
 * \return Code de retour (0 si le programme s'est bien exécuté).
 */
int main()
{
    int threadsUtilise = std::thread::hardware_concurrency();
    int maxThreads = std::thread::hardware_concurrency();

    /*calculerNombresPremiers(1, 10000);
    calculerNombresPremiers(threadsUtilise, 10000);
    calculerNombresPremiers(1, 100000);
    calculerNombresPremiers(threadsUtilise, 100000);
    calculerNombresPremiers(1, 1000000);
    calculerNombresPremiers(threadsUtilise, 1000000);*/

    // Calcul de la rentabilité pour différents 
    double rentabilite = calculerRentabilite(threadsUtilise, 1000000, maxThreads);
    std::cout << "Rentabilite du multithreading pour 1000000 : " << rentabilite << "%" << std::endl;
    double rentabilite2 = calculerRentabilite(threadsUtilise, 10000000, maxThreads);
    std::cout << "Rentabilite du multithreading pour 10000000 : " << rentabilite2 << "%" << std::endl;
    return 0;
}

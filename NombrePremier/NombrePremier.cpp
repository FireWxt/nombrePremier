// Simon / Gabin / Jaures / Cabrel

#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <mutex>


std::vector<int> listePremier;

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

    listePremier.insert(listePremier.end(), resultatsLocaux.begin(), resultatsLocaux.end());

}


void calculerNombresPremiers(int threadsUtilise, int fin)
{


    std::vector<std::thread> listeAsync;
    auto begin = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < threadsUtilise; i++)
    {
        listeAsync.push_back(std::thread(trouverNombresPremiers, i, fin, threadsUtilise));
    }

    for (auto& t : listeAsync)
    {
        t.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << elapsed.count() << " microsecondes  " << " \t";


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

int main()
{

    int threadsUtilise = std::thread::hardware_concurrency();

    calculerNombresPremiers(1, 10000);;
    calculerNombresPremiers(threadsUtilise, 10000);
    calculerNombresPremiers(1, 100000);
    calculerNombresPremiers(threadsUtilise, 100000);
    calculerNombresPremiers(1, 1000000);
    calculerNombresPremiers(threadsUtilise, 1000000);

    return 0;
}
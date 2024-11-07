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




int main()
{

}
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "sorts.h"
#include "randfiller.h"

int main()
{
    randfiller rf(54321);
    std::vector<int> sizes(100);
    rf.fill(sizes, 1, 200);
    for (int n : sizes) {
        std::vector<int> vec(n);
        rf.fill(vec, -1000, 1000);
        quicksort(vec);
        if (!is_sorted(vec)) {
            std::cerr << "Errore: quicksort non ha ordinato correttamente un vettore di int.\n";
            return EXIT_FAILURE;
        }
    }
    std::vector<std::string> words = {
        "zebra", "cane", "gatto", "anatra", "orso",
        "lupo", "volpe", "tigre", "leone", "ape"
    };
    quicksort(words);
    if (!is_sorted(words)) {
        std::cerr << "Errore: quicksort non ha ordinato correttamente il vettore di stringhe.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include "sorts.h"
#include "randfiller.h"
#include "timecounter.h"

template<typename SortFunction>
double average_time_on_many_vectors(SortFunction sort_function,
                                    const std::vector<std::vector<int>>& original_vectors)
{
    std::vector<std::vector<int>> vectors = original_vectors;
    timecounter tc;
    tc.tic();
    for (std::size_t i = 0; i < vectors.size(); ++i) {
        sort_function(vectors[i]);
    }
    double total_time = tc.toc();
    return total_time / static_cast<double>(vectors.size());
}
std::vector<std::vector<int>> make_random_vectors(randfiller& rf, int n, int how_many)
{
    std::vector<std::vector<int>> vectors(how_many, std::vector<int>(n));
    for (int i = 0; i < how_many; ++i) {
        rf.fill(vectors[i], -100000, 100000);
    }
    return vectors;
}
int main()
{
    randfiller rf(123456);
    std::cout << "N	bubble	selection	insertion	quick	merge	quick_hybrid	std_sort\n";
    for (int n = 2; n <= 100; n += 2) {
        std::vector<std::vector<int>> base_vectors = make_random_vectors(rf, n, 100);
        double t_bubble = average_time_on_many_vectors(
            [](std::vector<int>& v) { bubblesort(v); },
            base_vectors
        );
        double t_selection = average_time_on_many_vectors(
            [](std::vector<int>& v) { selectionsort(v); },
            base_vectors
        );
        double t_insertion = average_time_on_many_vectors(
            [](std::vector<int>& v) { insertionsort(v); },
            base_vectors
        );
        double t_quick = average_time_on_many_vectors(
            [](std::vector<int>& v) { quicksort(v); },
            base_vectors
        );
        double t_merge = average_time_on_many_vectors(
            [](std::vector<int>& v) { mergesort(v); },
            base_vectors
        );
        double t_quick_hybrid = average_time_on_many_vectors(
            [](std::vector<int>& v) { quicksort_hybrid(v, 16); },
            base_vectors
        );
        double t_std = average_time_on_many_vectors(
            [](std::vector<int>& v) { std::sort(v.begin(), v.end()); },
            base_vectors
        );
        std::cout << n << "	"
                  << t_bubble << "	"
                  << t_selection << "	"
                  << t_insertion << "	"
                  << t_quick << "	"
                  << t_merge << "	"
                  << t_quick_hybrid << "	"
                  << t_std << "\n";
    }
    return EXIT_SUCCESS;
}
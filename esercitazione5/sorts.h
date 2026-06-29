#pragma once
#include <vector>
#include <utility>
#include <cstddef>
#include <string>

template<typename T>
bool is_sorted(const std::vector<T>& vec)
{
    if (vec.size() < 2) {
        return true;
    }
    for (std::size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) {
            return false;
        }
    }
    return true;
}
template<typename T>
void bubblesort(std::vector<T>& vec)
{
    if (vec.size() < 2) {
        return;
    }
    for (std::size_t i = 0; i < vec.size() - 1; ++i) {
        for (std::size_t j = 0; j < vec.size() - 1 - i; ++j) {
            if (vec[j + 1] < vec[j]) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}
template<typename T>
void insertionsort(std::vector<T>& vec)
{
    for (std::size_t i = 1; i < vec.size(); ++i) {
        T key = vec[i];
        std::size_t j = i;
        while (j > 0 && key < vec[j - 1]) {
            vec[j] = vec[j - 1];
            --j;
        }
        vec[j] = key;
    }
}
template<typename T>
void selectionsort(std::vector<T>& vec)
{
    for (std::size_t i = 0; i < vec.size(); ++i) {
        std::size_t min_index = i;
        for (std::size_t j = i + 1; j < vec.size(); ++j) {
            if (vec[j] < vec[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            std::swap(vec[i], vec[min_index]);
        }
    }
}
template<typename T>
void merge_vectors(std::vector<T>& vec, int p, int q, int r)
{
    int n1 = q - p + 1;
    int n2 = r - q;
    std::vector<T> left(n1);
    std::vector<T> right(n2);
    for (int i = 0; i < n1; ++i) {
        left[i] = vec[p + i];
    }
    for (int j = 0; j < n2; ++j) {
        right[j] = vec[q + 1 + j];
    }
    int i = 0;
    int j = 0;
    int k = p;
    while (i < n1 && j < n2) {
        if (!(right[j] < left[i])) {
            vec[k] = left[i];
            ++i;
        } else {
            vec[k] = right[j];
            ++j;
        }
        ++k;
    }
    while (i < n1) {
        vec[k] = left[i];
        ++i;
        ++k;
    }
    while (j < n2) {
        vec[k] = right[j];
        ++j;
        ++k;
    }
}
template<typename T>
void mergesort_recursive(std::vector<T>& vec, int p, int r)
{
    if (p < r) {
        int q = (p + r) / 2;
        mergesort_recursive(vec, p, q);
        mergesort_recursive(vec, q + 1, r);
        merge_vectors(vec, p, q, r);
    }
}
template<typename T>
void mergesort(std::vector<T>& vec)
{
    if (vec.size() < 2) {
        return;
    }
    mergesort_recursive(vec, 0, static_cast<int>(vec.size()) - 1);
}
template<typename T>
int partition_vector(std::vector<T>& vec, int p, int r)
{
    T pivot = vec[r];
    int i = p - 1;
    for (int j = p; j <= r - 1; ++j) {
        if (!(pivot < vec[j])) {
            ++i;
            std::swap(vec[i], vec[j]);
        }
    }
    std::swap(vec[i + 1], vec[r]);
    return i + 1;
}
template<typename T>
void quicksort_recursive(std::vector<T>& vec, int p, int r)
{
    if (p < r) {
        int q = partition_vector(vec, p, r);
        quicksort_recursive(vec, p, q - 1);
        quicksort_recursive(vec, q + 1, r);
    }
}
template<typename T>
void quicksort(std::vector<T>& vec)
{
    if (vec.size() < 2) {
        return;
    }
    quicksort_recursive(vec, 0, static_cast<int>(vec.size()) - 1);
}
template<typename T>
void insertion_sort_range(std::vector<T>& vec, int p, int r)
{
    for (int i = p + 1; i <= r; ++i) {
        T key = vec[i];
        int j = i - 1;
        while (j >= p && key < vec[j]) {
            vec[j + 1] = vec[j];
            --j;
        }
        vec[j + 1] = key;
    }
}
template<typename T>
void quicksort_hybrid_recursive(std::vector<T>& vec, int p, int r, int threshold)
{
    if (p < r) {
        if (r - p + 1 <= threshold) {
            insertion_sort_range(vec, p, r);
            return;
        }
        int q = partition_vector(vec, p, r);
        quicksort_hybrid_recursive(vec, p, q - 1, threshold);
        quicksort_hybrid_recursive(vec, q + 1, r, threshold);
    }
}
template<typename T>
void quicksort_hybrid(std::vector<T>& vec, int threshold = 16)
{
    if (vec.size() < 2) {
        return;
    }
    quicksort_hybrid_recursive(vec, 0, static_cast<int>(vec.size()) - 1, threshold);
}
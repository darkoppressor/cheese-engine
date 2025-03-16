/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef sorting_h
#define sorting_h

#include "rng/rng.h"

#include <vector>
#include <cstdint>
#include <utility>

class Sorting {
    public:
        template<typename T>
        static void shuffle (RNG& rng, std::vector<T>& objects) {
            for (std::uint32_t i = objects.size() - 1; i >= 1; i--) {
                std::uint32_t j = rng.random_range(0, i);

                std::swap(objects[i], objects[j]);
            }
        }

        template<typename T>
        static void quick_sort (std::vector<T>& objects) {
            if (objects.size() <= 1) {
                return;
            }

            std::uint32_t pivot_index = objects.size() / 2;
            std::vector<T> less_than;
            std::vector<T> greater_than;
            T pivot = objects[pivot_index];

            for (std::uint32_t i = 0; i < objects.size(); i++) {
                if (i != pivot_index) {
                    if (objects[i] <= pivot) {
                        less_than.push_back(objects[i]);
                    } else {
                        greater_than.push_back(objects[i]);
                    }
                }
            }

            quick_sort(less_than);
            quick_sort(greater_than);

            objects = less_than;
            objects.push_back(pivot);
            objects.insert(objects.end(), greater_than.begin(), greater_than.end());
        }
};

#endif

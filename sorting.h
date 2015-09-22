/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef sorting_h
#define sorting_h

#include <vector>

class Sorting{
public:

    template<typename Object>
    static void quick_sort(std::vector<Object>& objects){
        if(objects.size()<=1){
            return;
        }

        int pivot_index=(int)floor((double)(objects.size()-1)/2.0);

        std::vector<Object> less_than;
        std::vector<Object> greater_than;
        Object pivot=objects[pivot_index];

        for(int i=0;i<objects.size();i++){
            if(i!=pivot_index){
                if(objects[i]<=pivot){
                    less_than.push_back(objects[i]);
                }
                else{
                    greater_than.push_back(objects[i]);
                }
            }
        }

        quick_sort(less_than);
        quick_sort(greater_than);

        objects=less_than;
        objects.push_back(pivot);
        objects.insert(objects.end(),greater_than.begin(),greater_than.end());
    }
};

#endif

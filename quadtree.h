/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef quadtree_h
#define quadtree_h

#include "collision.h"

#include <vector>
#include <cstdint>

template<typename T,typename Object_ID>
class Quadtree_Object{
public:

    Collision_Rect<T> box;

    Object_ID id;

    Quadtree_Object(const Collision_Rect<T>& get_box,Object_ID get_id){
        box=get_box;

        id=get_id;
    }
};

template<typename T,typename Object_ID>
class Quadtree{
private:

    std::uint32_t max_objects;
    std::uint32_t max_levels;

    std::uint32_t level;

    std::vector<Quadtree_Object<T,Object_ID>> objects;

    Collision_Rect<T> bounds;

    std::vector<Quadtree<T,Object_ID>> nodes;

public:

    Quadtree(){
        max_objects=0;
        max_levels=0;

        level=0;
    }

    Quadtree(std::uint32_t get_max_objects,std::uint32_t get_max_levels,std::uint32_t get_level,const Collision_Rect<T>& get_bounds){
        setup(get_max_objects,get_max_levels,get_level,get_bounds);
    }

    void setup(std::uint32_t get_max_objects,std::uint32_t get_max_levels,std::uint32_t get_level,const Collision_Rect<T>& get_bounds){
        max_objects=get_max_objects;
        max_levels=get_max_levels;

        level=get_level;

        bounds=get_bounds;
    }

    void clear_tree(){
        objects.clear();

        for(size_t i=0;i<nodes.size();i++){
            nodes[i].clear_tree();
        }

        nodes.clear();
    }

    void split_tree(){
        T x=bounds.x;
        T y=bounds.y;
        T width=bounds.w/2;
        T height=bounds.h/2;

        nodes.emplace_back(max_objects,max_levels,level+1,Collision_Rect<T>(x+width,y,width,height));
        nodes.emplace_back(max_objects,max_levels,level+1,Collision_Rect<T>(x,y,width,height));
        nodes.emplace_back(max_objects,max_levels,level+1,Collision_Rect<T>(x,y+height,width,height));
        nodes.emplace_back(max_objects,max_levels,level+1,Collision_Rect<T>(x+width,y+height,width,height));
    }

    std::vector<std::uint32_t> get_node_indices(const Collision_Rect<T>& box) const{
        std::vector<std::uint32_t> node_indices;

        T center_x=bounds.center_x();
        T center_y=bounds.center_y();

        bool left=false;
        bool right=false;
        bool top=false;
        bool bottom=false;

        if(box.x<=center_x){
            left=true;
        }
        if(box.x+box.w>=center_x){
            right=true;
        }

        if(box.y<=center_y){
            top=true;
        }
        if(box.y+box.h>=center_y){
            bottom=true;
        }

        if(top && right){
            node_indices.push_back(0);
        }
        if(top && left){
            node_indices.push_back(1);
        }
        if(bottom && left){
            node_indices.push_back(2);
        }
        if(bottom && right){
            node_indices.push_back(3);
        }

        return node_indices;
    }

    void insert_object(const Collision_Rect<T>& box,Object_ID object_id){
        if(nodes.size()>0){
            std::vector<std::uint32_t> node_indices=get_node_indices(box);

            for(size_t i=0;i<node_indices.size();i++){
                nodes[node_indices[i]].insert_object(box,object_id);
            }
        }
        else{
            objects.push_back(Quadtree_Object<T,Object_ID>(box,object_id));

            if(objects.size()>max_objects && level<max_levels-1){
                if(nodes.size()==0){
                    split_tree();
                }

                for(size_t i=0;i<objects.size();i++){
                    std::vector<std::uint32_t> node_indices=get_node_indices(objects[i].box);

                    for(size_t n=0;n<node_indices.size();n++){
                        nodes[node_indices[n]].insert_object(objects[i].box,objects[i].id);
                    }
                }

                objects.clear();
            }
        }
    }

    void insert_object(const Collision_Circ<T>& circle,Object_ID object_id){
        Collision_Rect<T> box;
        box.x=circle.x-circle.r;
        box.y=circle.y-circle.r;
        box.w=circle.r*(T)2;
        box.h=circle.r*(T)2;

        insert_object(box,object_id);
    }

    void get_objects(std::vector<Object_ID>& return_objects,const Collision_Rect<T>& box) const{
        if(nodes.size()>0){
            std::vector<std::uint32_t> node_indices=get_node_indices(box);

            for(size_t i=0;i<node_indices.size();i++){
                nodes[node_indices[i]].get_objects(return_objects,box);
            }
        }
        else{
            for(size_t i=0;i<objects.size();i++){
                return_objects.push_back(objects[i].id);
            }
        }
    }

    void get_objects(std::vector<Object_ID>& return_objects,const Collision_Circ<T>& circle) const{
        Collision_Rect<T> box;
        box.x=circle.x-circle.r;
        box.y=circle.y-circle.r;
        box.w=circle.r*(T)2;
        box.h=circle.r*(T)2;

        get_objects(return_objects,box);
    }
};

#endif

/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef quadtree_h
#define quadtree_h

#include "collision.h"

#include <vector>
#include <cstdint>

class Quadtree_Rect{
public:

    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t w;
    std::uint32_t h;

    Quadtree_Rect();
    Quadtree_Rect(std::uint32_t get_x,std::uint32_t get_y,std::uint32_t get_w,std::uint32_t get_h);
};

template<typename T,typename Object_ID>
class Quadtree_Object{
public:

    Collision_Rect<T> box;

    Object_ID id;

    Quadtree_Object(const Collision_Rect<T>& get_box, Object_ID get_id){
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

    Quadtree_Rect bounds;

    std::vector<Quadtree<T,Object_ID>> nodes;

public:

    Quadtree(){
        max_objects=0;
        max_levels=0;

        level=0;
    }

    Quadtree(std::uint32_t get_max_objects,std::uint32_t get_max_levels,std::uint32_t get_level,const Quadtree_Rect& get_bounds){
        setup(get_max_objects,get_max_levels,get_level,get_bounds);
    }

    void setup(std::uint32_t get_max_objects,std::uint32_t get_max_levels,std::uint32_t get_level,const Quadtree_Rect& get_bounds){
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
        std::uint32_t x=bounds.x;
        std::uint32_t y=bounds.y;
        std::uint32_t width=bounds.w/2;
        std::uint32_t height=bounds.h/2;

        nodes.emplace_back(max_objects,max_levels,level+1,Quadtree_Rect(x+width,y,width,height));
        nodes.emplace_back(max_objects,max_levels,level+1,Quadtree_Rect(x,y,width,height));
        nodes.emplace_back(max_objects,max_levels,level+1,Quadtree_Rect(x,y+height,width,height));
        nodes.emplace_back(max_objects,max_levels,level+1,Quadtree_Rect(x+width,y+height,width,height));
    }

    std::uint32_t get_node_index(const Collision_Rect<T>& box){
        std::uint32_t node_index=4;

        std::uint32_t mid_x=bounds.x+(bounds.w/2);
        std::uint32_t mid_y=bounds.y+(bounds.h/2);

        bool top_half=box.y<mid_y && box.y+box.h<mid_y;
        bool bottom_half=box.y>mid_y;

        if(box.x<mid_x && box.x+box.w<mid_x){
            if(top_half){
                node_index=1;
            }
            else if(bottom_half){
                node_index=2;
            }
        }
        else if(box.x>mid_x){
            if(top_half){
                node_index=0;
            }
            else if(bottom_half){
                node_index=3;
            }
        }

        return node_index;
    }

    void insert_object(const Collision_Rect<T>& box,Object_ID object_id){
        if(nodes.size()>0){
            std::uint32_t node_index=get_node_index(box);

            if(node_index<4){
                nodes[node_index].insert_object(box,object_id);

                return;
            }
        }

        objects.push_back(Quadtree_Object<T,Object_ID>(box,object_id));

        if(objects.size()>max_objects && level<max_levels-1){
            if(nodes.size()==0){
                split_tree();
            }

            for(size_t i=0;i<objects.size();){
                std::uint32_t node_index=get_node_index(objects[i].box);

                if(node_index<4){
                    nodes[node_index].insert_object(objects[i].box,objects[i].id);
                    objects.erase(objects.begin()+i);
                }
                else{
                    i++;
                }
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

    void get_objects(std::vector<Object_ID>& return_objects,const Collision_Rect<T>& box,Object_ID object_id){
        if(nodes.size()>0){
            std::uint32_t node_index=get_node_index(box);

            if(node_index<4){
                nodes[node_index].get_objects(return_objects,box,object_id);
            }

            for(size_t i=0;i<objects.size();i++){
                return_objects.push_back(objects[i].id);
            }
        }
    }

    void get_objects(std::vector<Object_ID>& return_objects,const Collision_Circ<T>& circle,Object_ID object_id){
        Collision_Rect<T> box;
        box.x=circle.x-circle.r;
        box.y=circle.y-circle.r;
        box.w=circle.r*(T)2;
        box.h=circle.r*(T)2;

        get_objects(return_objects,box,object_id);
    }
};

#endif

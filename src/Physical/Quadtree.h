#pragma once

#include<vector>
#include<memory>
#include"AABB.h"

// 维护一个vector 存储所有Item 包含了一个泛型数据 和 这个数据的AABB盒 这个盒不同于碰撞箱 是用于区分对象在哪个区域用的
// 每个四叉树都有四个孩子结点 通过unique_ptr来存储 
// 每个四叉树都有用于表示自己是否是叶子节点的标识
// 如果自己塞满了就创造叶子结点
// 记录自己的深度 当深度大于等于最大深度的时候 不能够创建新新子树() 只能硬塞到当前结点
// 每个叶子结点都要查看自己是否可以包含当前AABB盒 所有可以包含的都存一份
// 实现以下接口
// 1. 初始化一个最初的四叉树
// 2. void insert(const T& , const AABB&);
// 3. std::vector<T> query(const AABB& box); //这里考虑到NRVO优化后要比外部直接注入vector时间开销要小一些 更灵活 
// 重新考虑后决定使用 void query(Vector<T>& result,AABB& box); 因为频繁构造vector会产生内存碎片 直接写好一个 每次清理复用可以增加内存重用度
// 4. void clear() 用于清空当前树 每一帧都需要清空并且重新构造四叉树
struct Item;

template <class T>
class Quadtree{
public:
    Quadtree(glm::vec2 pos,glm::vec2 half_wh,int deepth){
        this->bounds.half_wh = half_wh;
        this->bounds.pos = pos; 
        this->deepth = deepth;
        items.reserve(MAX_ITEMS);
        this->is_leaf = true;
    }

    void insert(const T& data,const AABB& box){
        if(!bounds.intersects(box)) return;

        //当前结点还能装得下 且是叶子结点
        if((this->items.size() < MAX_ITEMS && this->is_leaf)){
            this->items.push_back(Item{box,data});
            return;
        }

        // 如果当前结点装不下 或者刚执行完加入后正好填满 但是还没有到最大深度
        if(this->items.size() >=MAX_ITEMS && this->deepth < MAX_DEEPTH && is_leaf){
            this->split();
            for(auto& item : items){
                for(int i = 0;i<4;i++){
                    children[i]->insert(item.data,item.bounds);
                }
            }
            items.clear();
        }
        //如果不是叶子结点 就直接发给叶子节点
        if(!is_leaf){
            for(int j = 0;j < 4;j++){
                children[j]->insert(data,box);
            }
        }else{ // 是叶子结点 并且不能再分 最大深度已经够了 并且最大存储量也够了 
            this->items.push_back(Item{box,data});
        }
    }

    void query(std::vector<T>& result,const AABB& box){
        if(!this->bounds.intersects(box)) return;
        for(const auto& item : items){
            if(box.intersects(item.bounds)) result.push_back(item.data);
        }
        if(is_leaf) return;
        for(int i = 0; i < 4;i++){
            children[i]->query(result,box);
        }
    }

    void clear(){
        this->items.clear();
        if(!this->is_leaf){
            for(int i = 0; i < 4;i++){
                children[i]->clear();
            }
        }
        this->is_leaf = true;
    }


private:
    struct Item
    {
        AABB bounds;
        T data;
    };
    
    static const int MAX_ITEMS = 8;
    static const int MAX_DEEPTH = 6;

    AABB bounds;
    int deepth;
    std::vector<Item> items;
    std::unique_ptr<Quadtree> children[4];   

    bool is_leaf;

    void split(){
        if(this->deepth == MAX_DEEPTH) return;
        this->is_leaf = false;
        if(this->children[0]) return;
        float h_w = this->bounds.half_wh.x/2;
        float h_h = this->bounds.half_wh.y/2;
        this->children[0] = std::make_unique<Quadtree>(glm::vec2{this->bounds.pos.x - h_w,this->bounds.pos.y - h_h},glm::vec2{h_w,h_h},deepth + 1); //左上
        this->children[1] = std::make_unique<Quadtree>(glm::vec2{this->bounds.pos.x - h_w,this->bounds.pos.y + h_h},glm::vec2{h_w,h_h},deepth + 1); //左下
        this->children[2] = std::make_unique<Quadtree>(glm::vec2{this->bounds.pos.x + h_w,this->bounds.pos.y - h_h},glm::vec2{h_w,h_h},deepth + 1); //右上
        this->children[3] = std::make_unique<Quadtree>(glm::vec2{this->bounds.pos.x + h_w,this->bounds.pos.y + h_h},glm::vec2{h_w,h_h},deepth + 1); //右下

        this->is_leaf = false;
    }
};
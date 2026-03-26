#pragma once

#include<vector>
#include<memory.h>

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
// 重新考虑后决定使用 void query(Vector<T>& result,int float); 因为频繁构造vector会产生内存碎片 直接写好一个 每次清理复用可以增加内存重用度
// 4. void clear() 用于清空当前树 每一帧都需要清空并且重新构造四叉树


template <class T>
class Quadtree{
public:



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
    std::Vector<Item> items;
    std::unique_ptr<Quadtree> children[4];

    bool is_leaf;


};
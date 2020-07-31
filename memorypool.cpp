#include "memorypool.h"
#include <mutex>
#include <iostream>
#include <cstring>

MemoryPool::MemoryPool()
{

}


MemoryPool::~MemoryPool(){

}

int MemoryPool::initBlockList(uint block_size){

    free_block_map[block_size].clear();
    used_mem_count[block_size] = 0;

    return extendBlockList(block_size);
}

int MemoryPool::extendBlockList(uint block_size){

    //预计分配block_count块内存
    const uint block_count = chunk_size / block_size;

    std::list<void*> &free_list = free_block_map[block_size];

    for(uint i=0; i < block_count; ++i)
    {
        void *mem = new char[block_size];
        if (mem == NULL)
            return -1;

        // 清零初始化后，再分配出去
        std::memset(mem, 0, block_size);

        free_list.push_back(mem);
    }

    return 0;
}

MemoryPool& MemoryPool::getInstance(){

    static MemoryPool instance;
    return instance;

}

size_t MemoryPool::getFitSize(size_t size){
    if(size == 0 || size > max_size){
        std::cout << "can't allocate !" << std::endl;
        return 0;
    }
    if(size <= 64)
        return 64;
    else if(size <= 128)
        return 128;
    else if(size <= 512)
        return 512;
    else
        return 1024;
}

void* MemoryPool::alloc(size_t size){

    size = getFitSize(size);
    if(size == 0)
        return nullptr;

    // 获取free_block_map所在空闲链表
    auto it_block = free_block_map.find(size);
    if (it_block == free_block_map.end())
    {
        // 当前Size并没有分配过内存，需要进行空闲链表初始化
        const int ret = this->initBlockList(size);
        if (ret != 0)
            return NULL;
    }

    std::list<void*> &free_list= free_block_map[size];

    // 检测当前空闲链表是否有空闲块
    if (free_list.empty())
    {
        const int ret = this->extendBlockList(size);
        if (ret != 0)
            return NULL;
    }

    // 从空闲链表摘取一块，分配出去
    void *mem = free_list.front();
    free_list.pop_front();
    used_mem_count[size] ++;
    return mem;

}

int MemoryPool::dealloc(void* p, uint block_size){
    block_size = getFitSize(block_size);
    auto it_block = free_block_map.find(block_size);
    if(it_block == free_block_map.end())
        return -1;

    free_block_map[block_size].push_back(p);
    used_mem_count[block_size] --;
    return 0;
}

void MemoryPool::getInfo(){

    uint total_used_block = 0;
    uint total_used_memory = 0;
    auto it = used_mem_count.begin();
    while(it != used_mem_count.end()){
        total_used_block += it->second;
        total_used_memory += it -> first * it -> second;
        ++ it;
    }
    std::cout << "total_used_block:" << total_used_block << std::endl;
    std::cout << "total_used_memory:" << total_used_memory << std::endl;

    auto free_it = free_block_map.begin();
    uint total_free_memory = 0;
    while(free_it != free_block_map.end()){
        total_free_memory += free_it->second.size() * free_it->first;
        ++ free_it;
    }
    std::cout << "idle rate:" << (double)total_free_memory / (double)(total_free_memory + total_used_memory) << std::endl;

}

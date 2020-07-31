#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#define BlOCKSIZE 1024

#include <list>
#include <array>
#include <unordered_map>
#include <map>
#include <set>

typedef unsigned int uint;
static uint chunk_size = 1 << 12;
const static uint max_size = 1024;

//class BaseBlock{
//public:
//    BaseBlock* next;
//};

//class Chunk{
//public:
//    BaseBlock data[1024];
//};

//class Block64:public BaseBlock{
//public:
//    char data[64];
//};

//class Block128:public BaseBlock{
//public:
//    char data[128];
//};

//class Block512:public BaseBlock{
//public:
//    char data[512];
//};

//class Block1024:public BaseBlock{
//public:
//    char data[1024];
//};

class MemoryPool
{
public:
    static MemoryPool& getInstance();
    void* alloc(size_t size);
    int dealloc(void* p, const uint block_size);
    void getInfo();

private:

    MemoryPool();
    ~MemoryPool();
    int initBlockList(uint block_size);
    int extendBlockList(uint block_size);
    size_t getFitSize(size_t size);

    std::map<uint, std::list<void*> > free_block_map;
    std::map<uint, uint > used_mem_count;


};

#endif // MEMORYPOOL_H

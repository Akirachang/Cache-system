#pragma once

const long long ADDR_SIZE = 64;
const char OP_WRITE = 'w';
const char OP_READ = 'r';
const char OP_LOAD = 'l';
const unsigned char FLAG_VALID = 0x01;
const unsigned char FLAG_DIRTY = 0x02;
const unsigned char FLAG_MASK = 0xff;

typedef unsigned long long double_long;

enum swap_style{
    LRU,
    RAND,
    TREE
};

enum write_style: char{
    writeThrough,
    writeNotThrough,
    writeBack,
    writeNotBack
};

struct Cache_element{
    double_long tag;
    char flag;
    short LRU;
    char BIT;
};


class CacheSimulator{
    public:
    double_long size, 
                cacheElementSize, 
                cacheElementNum,
                mapWays,
                setNum,

                cacheSetSize,
                cacheSetShifts,
                cacheLineShifts,

                readCount,
                writeCount,
                writeMemCount,
                readMemCount,
                hitCount,
                missCount,
                readHitCount,
                writeHitCount,
                writeMissCount,
                readMissCount;

    Cache_element* caches;

    int swapStyle;
    write_style writeStyle;

    int write_alloc;
    int write_through;

    CacheSimulator();
    void initialize(double_long size, double_long lineSize, double_long mapWays, swap_style ss);
    void setM(int m);
    void updateBT(double_long point, char state);
    int checkHit(double_long set, double_long tag);
    int getEmptyElement(double_long set);
    int selectVictim(double_long set, int swapStyle, int hitIndex);
    void cache_hit(double_long setBae, double_long index, int ss);
    void insert_cache(double_long setBase, double_long index, int ss);
    int op_cache(double_long addr, char op_style);
    
    void readTrace(char* file);
    float missRate();
    float hitRate();

    int getSetFlag(double_long setBase);
    // void createMap(unordered_map<string, char> *um);

};


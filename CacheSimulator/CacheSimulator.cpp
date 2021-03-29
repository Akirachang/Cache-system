#include "CacheSimulator.h"
#include <iostream>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

using namespace std;
char mask[8] = {0b1101000,0b1101000,0b1100100,0b1100100,0b1010010,0b1010010,0b1010001, 0b1010001};
char val[8] = {0b0000000, 0b0001000, 0b0100000,0b0100100,0b1000000,0b1000010,0b1010000,
0b1010001};
char next_val[8] = {0b1101000,0b1100000,0b1000100,0b1000000,0b0010010,0b0010000,0b0000001,0b0000000};
int CacheSimulator::checkHit(double_long setBase, double_long addr){
    double_long i;
    for (i = 0; i < mapWays; ++i) {
        if ((caches[setBase + i].flag & FLAG_VALID) && (caches[setBase + i].tag == ((addr >> (cacheSetShifts + cacheLineShifts))))) {
            return i; 
        }
    }
    return -1;
}

void CacheSimulator::cache_hit(double_long setBase, double_long index, int swapStyle){
    switch (swapStyle)
    {
    case LRU:
        for(int i =0;i<mapWays;i++){
            if((caches[setBase+i].LRU < caches[setBase+index].LRU) && (caches[setBase+i].flag & FLAG_VALID)){
                caches[setBase+i].LRU++;
            }
        }
        break;
    case TREE:
            char curr = caches[setBase].BIT;
            for(int i=0;i<(1<<mapWays);i++){
                int temp;
                if(curr & mask[i] == val[i]){
                    curr ^= mask[i];
                    break;
                }
            }
            updateBT(setBase, curr);
            break;
    }
}
int CacheSimulator::selectVictim(double_long setBase, int swapStyle, int hitIndex){
    int freeIndex = -1;
    double_long maxLRU;
    double_long maxLRUnew;

    switch(swapStyle){
        case LRU:
            maxLRU = 0;
                for (double_long j = 0; j < mapWays; j++) {
                    if (caches[setBase + j].LRU > maxLRU) {
                        maxLRU = caches[setBase + j].LRU; 
                        freeIndex = j;
                    }
                }
            break;

        case RAND:
            freeIndex = rand() % mapWays;
            break;

        case TREE:
            char curr = caches[setBase].BIT;
            for(int i=0;i<(1<<mapWays);i++){
                int temp;
                if((curr & mask[i]) == val[i]){          
                    freeIndex = i;
                    curr ^= mask[i];
                    break;
                }
            }
            updateBT(setBase, curr);
            break;

    }

    if (freeIndex >= 0) {
        if (caches[setBase + freeIndex].flag & FLAG_DIRTY) {
            caches[setBase + freeIndex].flag &= ~FLAG_DIRTY;
            writeMemCount++;
        }
    } 
    return freeIndex;

}

void CacheSimulator::insert_cache(double_long setBase, double_long index, int swapStyle){
    double rand;
    switch(swapStyle){
        case LRU:
                    for (double_long j = 0; j < mapWays; j++) {
                        if ((caches[setBase + j].LRU < caches[setBase + index].LRU) && (caches[setBase + j].flag & FLAG_VALID)) {
                            caches[setBase + j].LRU ++;
                        }
                    }
                    caches[setBase + index].LRU = 0;
                    break;
    }
}

void CacheSimulator::updateBT(double_long setBase, char state){
    for(int i=0;i<(1 << mapWays); i++){
        caches[setBase + i].BIT= state;
    }
}

int CacheSimulator::op_cache(double_long addr, char op_style){
    double_long set;
    double_long setBase;
    int hitIndex;
    int freeIndex;
    int rtn;
    if(op_style == OP_WRITE)
        readCount++;
    else if(op_style == OP_READ)
        writeCount++;

    set = (addr>>cacheLineShifts)%cacheSetSize;
    setBase = set*mapWays;
    hitIndex = checkHit(setBase, addr);

    if(op_style == OP_READ){
        if(hitIndex >= 0){
            readHitCount++;
            hitCount++;
            cache_hit(setBase, hitIndex, swapStyle);
            rtn = 1; // cache hit
        }
        else {
            readMissCount++;
            readMemCount++;
            missCount++;
            freeIndex = selectVictim(setBase, swapStyle, hitIndex);

            caches[setBase + freeIndex].tag = cacheSetShifts+cacheLineShifts;
            caches[setBase + freeIndex].flag = (char)~FLAG_MASK;
            caches[setBase + freeIndex].flag |= FLAG_VALID;

            insert_cache(setBase, freeIndex, swapStyle);
            rtn = -1;

        }
    }
    else{
        if(hitIndex>=0){
            writeHitCount++;
            hitCount++;
            if(writeThrough){
                writeMemCount++;
            }else{
                caches[setBase+hitIndex].flag|=FLAG_DIRTY;
            }
            cache_hit(setBase, hitIndex,swapStyle);
        }
        else{
            writeMissCount++;
            missCount++;
            if(write_alloc){
                readMemCount++;
                freeIndex = selectVictim(setBase, swapStyle, hitIndex);

                caches[setBase+freeIndex].tag = addr>>(cacheSetShifts+cacheLineShifts);
                caches[setBase+freeIndex].flag = (char)~FLAG_MASK;
                caches[setBase+freeIndex].flag|=FLAG_VALID;
                if(write_through){
                    writeMemCount++;
                }else{
                    caches[setBase+freeIndex].flag|=FLAG_DIRTY;
                }

                insert_cache(setBase, freeIndex, swapStyle);

            }
        }
    }
    return rtn;
}

CacheSimulator::CacheSimulator(){}

void CacheSimulator::initialize(double_long size, double_long lineSize, double_long mapWays, swap_style ss){
    this -> swapStyle = ss;
    this -> mapWays = mapWays;
    cacheElementSize = lineSize;
    this -> size= size;
    
    cacheElementNum = (double_long) size/lineSize;
    cacheLineShifts = (double_long) log2(lineSize);

    cacheSetSize = cacheElementNum / mapWays;

    cacheSetShifts = (double_long) log2(cacheSetSize);

    readCount=0;
    writeCount=0;
    writeMemCount=0;
    readMemCount=0;
    hitCount=0;
    missCount=0;
    readHitCount=0;
    writeHitCount=0;
    writeMissCount=0;
    readMissCount=0;

    caches = (Cache_element*) malloc(sizeof(Cache_element)*cacheElementNum);
    memset(caches, 0, sizeof(Cache_element) * cacheElementNum);

    for(int i=0;i<cacheSetSize;i++){
        for(int j=0;j<mapWays;j++){
            caches[i*mapWays+j].LRU=j;
        }
    }
    for(int i=0;i<cacheElementNum;i++){
        caches[i].BIT=0b0000000;
    }

    write_through = 0;
    write_alloc = 1;

}


void CacheSimulator::readTrace(char* file){
    int index=0;
    ofstream  myfile;
    myfile.open("output/4_8_8_TREE.txt");

    FILE* fileName = fopen(file,"r");
    while(!feof(fileName))
    {
       char op;
       char op_style;
       char addr[128];
        fscanf(fileName, "%s %c\n", &addr, &op);
        if(op == 'r'||op == 'l'){
            op_style = 'r';
        }
        else{
            op_style = 'w';
        }

        string sBinary = string(addr);
    
    long long num=0;
    for(int i=2;i<sBinary.length();i++){
        int curr = sBinary[i] - '0';
        num+=curr;
        num=num<<1;
    }
       
        int rtn=op_cache(num, op_style);
    // cout<<rtn;
    if(rtn == -1)
        myfile<<"Miss"<<endl;
    else
        myfile<<"Hit"<<endl;
    // myfile<<index<<",";
    // myfile << 100.0 * hitCount / (hitCount + missCount)<<",";
    // myfile<<100.0 * missCount / (hitCount + missCount)<<endl;

    // printf("index: %d, ", index);
    // printf("hit rate:%f, miss rate: %f\n",100.0 * hitCount / (hitCount + missCount),100.0 * missCount / (hitCount + missCount));
    index++;

}

}
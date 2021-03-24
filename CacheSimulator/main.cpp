#include <iostream>
#include "CacheSimulator.h"

using namespace std;

int main(int argc, char* argv[]){
    double_long cacheSize = 0x400000;
    double_long lineSize=64;
    double_long ways=4;
    swap_style swapStyle=LRU;
    CacheSimulator cacheSim;
    char* file="test_trace/2.trace";
    cacheSim.initialize(cacheSize,lineSize,ways,swapStyle);
    cacheSim.readTrace(file);
    return 0;
} 
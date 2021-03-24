#include <iostream>
#include "CacheSimulator.h"

using namespace std;

int main(int argc, char* argv[]){
    double_long cacheSize = 0x400000;
    double_long lineSize=8;
    double_long ways=8;
    swap_style swapStyle=TREE;
    CacheSimulator cacheSim;
    char* file="test_trace/4.trace";
    cacheSim.initialize(cacheSize,lineSize,ways,swapStyle);
    cacheSim.readTrace(file);
    return 0;
} 
#pragma once

#ifndef _BPLUSTREE_INDEX_H_
#define _BPLUSTREE_INDEX_H_

#include "rc.h"
#include "parse_defs.h"
#include "meta.h"
#include "disk_buffer_pool.h"

#define MAX_CHILD_BLOCK 15


struct Block {
    //number of nodes
    int tNodes;
    //for parent Block and index
    Block* parentBlock;
    //values
    int value[MAX_CHILD_BLOCK];
    //child Blocks
    Block* childBlock[MAX_CHILD_BLOCK];
    Block() { //constructor to initialize a block
        tNodes = 0;
        parentBlock = NULL;
        for (int i = 0; i < MAX_CHILD_BLOCK; i++) {
            value[i] = INT_MAX;
            childBlock[i] = NULL;
        }
    }
};

//只支持Int索引
class BplusTreeIndex {
public:
    RC insertNode(Block* curBlock, AttrType type, int length, char* value);
    RC splitLeaf(Block* curBlock);
    RC splitNonLeaf(Block* curBlock);

    RC deleteNode(Block* curBlock, AttrType type, int length, char* value, int curBlockPosition);
    RC mergeBlock(Block* leftBlock, Block* rightBlock, bool isLeaf, int posOfRightBlock);
    RC redistributeBlock(Block* leftBlock, Block* rightBlock, bool isLeaf, int posOfLeftBlock, int whichOneisCurBlock);

    RC print(vector < Block* > Blocks);

    Block* rootBlock = new Block();
    int numberOfPointers = 5;   //=size/record_size
    bool dataFound = false;

};

#endif
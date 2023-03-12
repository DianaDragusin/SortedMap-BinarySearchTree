#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

SMIterator::SMIterator(const SortedMap &m) : map(m) {
    stack = new BSTNode[map.nrElem];

    //save elements from array in stack
    int stack_index = 0;
    for (int index = 0; index < map.cap; index++) {
        if (map.elements[index].info.first != NULL_TKEY) {
            stack[stack_index] = map.elements[index];
            stack_index++;
        }
    }

    //sort elements
    for (int index = 0; index < map.nrElem; index++) {
        int minim = index;
        for (int i = index + 1; i < map.nrElem; i++) {
            if (map.rel(stack[i].info.first, stack[minim].info.first)) {
                minim = i;
            }
        }
        BSTNode aux = stack[index];
        stack[index] = stack[minim];
        stack[minim] = aux;
    }

    currentkey = 0;
}

void SMIterator::first() {
    currentkey = 0;
}

void SMIterator::next() {
    if(valid()){
        currentkey++;
    }
    else{
        throw exception();
    }
}

bool SMIterator::valid() const {
    if(currentkey < map.nrElem)
        return true;
    return false;
}

TElem SMIterator::getCurrent() const {
    if(valid()){
        return stack[currentkey].info;
    }
    else{
        throw exception();
    }
}



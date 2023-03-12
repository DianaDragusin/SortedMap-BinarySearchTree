#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

SortedMap::SortedMap(Relation r) {
    cap = 5;
    nrElem = 0;
    elements = new BSTNode[cap];
    root = -1;

    for (int index = 0; index < cap - 1; index++) {
        elements[index].info.first = NULL_TKEY;
        elements[index].left = index + 1;
        elements[index].right = -1;
        elements[index].parent = -1;
    }

    elements[cap - 1].info.first = NULL_TKEY;
    elements[cap - 1].left = -1;
    elements[cap - 1].right = -1;

    firstEmpty = 0;
    rel = r;
}

TValue SortedMap::add(TKey k, TValue v) {

    //resize
    if (nrElem == cap) {
        cap = cap * 2;
        BSTNode *newvec = new BSTNode[cap];

        for (int index = 0; index < nrElem; index++) {
            newvec[index] = elements[index];
        }

        firstEmpty = nrElem;
        for(int index = nrElem;index<cap-1;index++){
            newvec[index].info.first = NULL_TKEY;
            newvec[index].left = index+1;
        }
        newvec[cap-1].info.first = NULL_TKEY;
        newvec[cap-1].left = -1;
        newvec[cap-1].right = -1;

        delete[] elements;
        elements = newvec;
    }

    //tree is empty
    if (root == -1) {
        elements[firstEmpty].info.first = k;
        elements[firstEmpty].info.second = v;

        root = firstEmpty;
        firstEmpty = elements[firstEmpty].left;

        elements[root].left = -1;
        elements[root].right = -1;
        elements[root].parent = -1;
        nrElem++;
        return NULL_TVALUE;
    }

    int pos = root;
    int prev_pos;
    while (pos != -1) {
        //key is already in the tree
        if (elements[pos].info.first == k) {
            TValue old = elements[pos].info.second;
            elements[pos].info.second = v;
            return old;
        }

        if (rel(elements[pos].info.first, k)) {
            prev_pos = pos;
            pos = elements[pos].right;
        } else {
            prev_pos = pos;
            pos = elements[pos].left;
        }
    }

    //we add new key to the tree
    elements[firstEmpty].info.first = k;
    elements[firstEmpty].info.second = v;
    elements[firstEmpty].parent = prev_pos;

    int pos_to_add_to = firstEmpty;
    firstEmpty = elements[firstEmpty].left;

    elements[pos_to_add_to].left = -1;
    elements[pos_to_add_to].right = -1;

    if (rel(elements[prev_pos].info.first, k)) {
        elements[prev_pos].right = pos_to_add_to;
    } else {
        elements[prev_pos].left = pos_to_add_to;
    }
    nrElem++;

    return NULL_TVALUE;
}

TValue SortedMap::search(TKey k) const {
    int pos = root;

    while (pos != -1) {
        if (elements[pos].info.first == k)
            return elements[pos].info.second;

        if (rel(elements[pos].info.first, k))
            pos = elements[pos].right;
        else {
            pos = elements[pos].left;
        }
    }

    return NULL_TVALUE;
}

TValue SortedMap::remove(TKey k) {

    //tree has only one element left
    if(nrElem == 1 && elements[root].info.first == k){
        TValue oldvalue = elements[root].info.second;
        elements[root].info.first = NULL_TKEY;
        elements[root].info.second = NULL_TVALUE;

        //position is first Empty
        if (root < firstEmpty) {
            int save_pos = firstEmpty;
            firstEmpty = root;
            elements[firstEmpty].left = save_pos;
        }
        //removed elemnents will be linked
        else {
            int index_empty = firstEmpty;
            int prev_index_empty;
            while (index_empty < root && index_empty != -1) {
                prev_index_empty = index_empty;
                index_empty = elements[index_empty].left;
            }
            elements[prev_index_empty].left = root;
            elements[root].left = index_empty;
        }

        nrElem--;
        return oldvalue;
    }

    int pos = root;
    int prev_pos;
    while (pos != -1) {
        //we search for the key
        if (elements[pos].info.first == k) {

            //key to be removed has no child
            if(elements[pos].left == -1 && elements[pos].right == -1){
                TValue oldvalue = elements[pos].info.second;
                elements[pos].info.first = NULL_TKEY;
                elements[pos].info.second = NULL_TVALUE;
                elements[pos].parent = -1;

                if (rel(elements[prev_pos].info.first, k)) {
                    elements[prev_pos].right = -1;
                } else {
                    elements[prev_pos].left = -1;
                }

                //position is first Empty
                if (pos < firstEmpty) {
                    int save_pos = firstEmpty;
                    firstEmpty = pos;
                    elements[firstEmpty].left = save_pos;
                }
                //removed elements will be linked
                else {
                    int index_empty = firstEmpty;
                    int prev_index_empty;
                    while (index_empty < pos && index_empty != -1) {
                        prev_index_empty = index_empty;
                        index_empty = elements[index_empty].left;
                    }
                    elements[prev_index_empty].left = pos;
                    elements[pos].left = index_empty;
                }

                nrElem--;
                return oldvalue;
            }

            //key to be removed has a child
            if(elements[pos].left == -1 || elements[pos].right == -1){
                TValue oldvalue = elements[pos].info.second;
                elements[pos].info = NULL_TPAIR;

                int next_pos;
                if (elements[pos].left == -1) {
                    next_pos = elements[pos].right;
                } else {
                    next_pos = elements[pos].left;
                }


                if(pos == root){
                    elements[pos].left = -1;
                    elements[pos].right = -1;
                    root = next_pos;
                }
                else {

                    elements[pos].left = -1;
                    elements[pos].right = -1;
                    elements[pos].parent = -1;

                    elements[next_pos].parent = prev_pos;


                    if (rel(elements[prev_pos].info.first, k)) {
                        elements[prev_pos].right = next_pos;
                    } else {
                        elements[prev_pos].left = next_pos;
                    }
                }

                //position is first Empty
                if (pos < firstEmpty) {
                    int save_pos = firstEmpty;
                    firstEmpty = pos;
                    elements[firstEmpty].left = save_pos;
                }
                //removed elements will be linked
                else {
                    int index_empty = firstEmpty;
                    int prev_index_empty;
                    while (index_empty < pos && index_empty != -1) {
                        prev_index_empty = index_empty;
                        index_empty = elements[index_empty].left;
                    }
                    elements[prev_index_empty].left = pos;
                    elements[pos].left = index_empty;
                }

                nrElem--;
                return oldvalue;
            }
        }

        if (rel(elements[pos].info.first, k)) {
            prev_pos = pos;
            pos = elements[pos].right;
        } else {
            prev_pos = pos;
            pos = elements[pos].left;
        }
    }

    return NULL_TVALUE;
}

int SortedMap::size() const {
    return nrElem;
}

bool SortedMap::isEmpty() const {
    return nrElem == 0;
}

SMIterator SortedMap::iterator() const {
    return SMIterator(*this);
}

SortedMap::~SortedMap() {
    delete[] elements;
}

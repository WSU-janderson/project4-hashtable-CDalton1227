/**
 * HashTable.cpp
 */

#include "HashTable.h"

HashTable::HashTable(size_t initCapacity) {
    trueSize = 0; // nothing in it yet
    capacity = initCapacity; // set to the size input in constructor

    buckets.resize(capacity); // resize (vector) to initCapacity number of buckets
}
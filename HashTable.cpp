/**
 * HashTable.cpp
 */

#include "HashTable.h"
#include <functional>
#include <numeric>
#include <random>
#include <algorithm>

HashTable::HashTable(size_t initCapacity) {
    trueSize = 0; // nothing in it yet
    capacity = initCapacity; // set to the size input in constructor

    buckets.resize(capacity); // resize (vector) to initCapacity number of buckets

    offsets.resize(capacity - 1);

    std::iota(offsets.begin(), offsets.end(), 1); // make the list of offsets starting with 1
    std::random_device rd; // seed for rng
    std::mt19937 gen(rd()); // create rng
    std::shuffle(offsets.begin(), offsets.end(), gen); // shuffle list
}

size_t HashTable::hash(const std::string& key) const {
    size_t hashVal = std::hash<std::string>()(key);
    return hashVal % capacity; // keep index within bounds
}
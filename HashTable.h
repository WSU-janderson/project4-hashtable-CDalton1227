/**
 * HashTable.h
 */

#include <iostream>
#include <string>
#include <vector>

// Create an enum for the bucket type
// NORMAL - not empty,
// ESS - empty since start,
// EAR - empty after removal
enum class BucketType {
    NORMAL, ESS, EAR
};


// create the hash table buckets
class HashTableBucket {
public:
    HashTableBucket () {
        type = BucketType::ESS; // default type
    }
private:
    std::string key;
    size_t value;
    BucketType type;
};

// create the hash table class
class HashTable {
    public:
    private:
        std::vector<HashTableBucket> buckets;
};


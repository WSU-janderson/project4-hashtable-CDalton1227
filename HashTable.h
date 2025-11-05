/**
 * HashTable.h
 */

#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <ostream>

// Create an enum for the bucket type
// NORMAL - not empty,
// ESS - empty since start,
// EAR - empty after removal
enum class BucketType {
    NORMAL, ESS, EAR
};

class HashTable;

// create the hash table buckets
class HashTableBucket {
    friend class HashTable;
    friend std::ostream& operator<<(std::ostream& os, const HashTable& t);
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
        // constructor that sets size; default 8
        HashTable(size_t initCapacity = 8);

        friend std::ostream& operator<<(std::ostream& os, const HashTable& ht);

        bool insert(const std::string& key, const size_t& value);

        size_t size() const;
        double alpha() const;

        bool contains(const std::string& key) const;

        std::optional<size_t> get(const std::string& key) const;
        bool remove(const std::string& key);

        size_t& operator[](const std::string& key);

        std::vector<std::string> keys() const;

        size_t capacity() const;



    private:
        std::vector<HashTableBucket> buckets;
        size_t trueSize; // number of things in it
        size_t currentCapacity; // number of things it could have
        std::vector<size_t> offsets; // probing offsets

        // hash function to prevent excessive repetition in every other method
        size_t hash(const std::string& key) const;
        // resizer - double when load factor >= .5
        void resize();
};
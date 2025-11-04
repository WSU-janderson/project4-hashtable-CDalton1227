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


// create the hash table buckets
class HashTableBucket {
    friend class HashTable;
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

        bool HashTable::insert(const std::string& key, const size_t& value){
          	// check load factor and resize if needed
        	if (alpha() <= .5) {
            	resize();
            }

            size_t home = hash(key); // get index

            std::optionl<size_t> bucket;

            if (buckets[home].type != BucketType::NORMAL) {
            	// this bucket is empty use it
                bucket = home;
            } else if (buckets[home].key == key) {
              	// repeated item
            	return false;
			}

            // do p.r.probing if collision happened
            if (buckets[home].type == BucketType::NORMAL) {
            	for (size_t i = 0; i < offsets.size(); ++i) {
                	size_t probe = (home + offsets[i]) % capacity;

                    // bucket to probe
                    if (probe.type == BucketType::ESS) {
                    	// this is empty since start so nothing has been here
                        if (!bucket.has_value()) {
                        	bucket = probe;
                        }
                        break;
                    }
            	}
            }


        }

        // get num items in table
        size_t HashTable::size() const{
            return trueSize;
        }

        // load factor -> size / capacity, casted to doubles just to be sure
        double HashTable::alpha() const{
            return static_cast<double>trueSize / static_cast<double>capacity;
        }

        // resize (double for simplicity) when load factor reaches .5
        void HashTable::resize() {
        }


    private:
        std::vector<HashTableBucket> buckets;
        size_t trueSize; // number of things in it
        size_t capacity; // number of things it could have
        std::vector<size_t> offsets; // probing offsets

        // hash function to prevent excessive repetition in every other method
        size_t hash(const std::string& key) const;
        // resizer
        void resize();
};


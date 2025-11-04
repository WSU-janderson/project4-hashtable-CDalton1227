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

        bool insert(const std::string& key, const size_t& value){
          	// check load factor and resize if needed
        	if (alpha() >= .5) {
            	resize();
            }

            size_t home = hash(key); // get index

            std::optional<size_t> bucket;

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
                  	// use offsets vector to get new index
                    // make sure to check for dupes
                	size_t probe = (home + offsets[i]) % capacity;

                    // bucket to probe
                    if (buckets[probe].type == BucketType::ESS) {
                    	// this is empty since start so nothing has been here
                        if (!bucket.has_value()) {
                        	bucket = probe;
                        }
                        break;
                    }

                    if (buckets[probe].type == BucketType::EAR) {
                    	if (!bucket.has_value()) {
                        	bucket = probe;
                    	}
                    }

                    if (buckets[probe].type == BucketType::NORMAL) {
                    	if (probe.key == key) {
                        	// dupe
                            return false;
                    	}
                    }
            	}
            }

            // actually make the insert
            if (bucket.has_value()) {
            	buckets[bucket.value()].key = key; // set key
                buckets[bucket.value()].value = value; // set val
                buckets[bucket.value()].type = BucketType::NORMAL; // occupied set to NORMAL
                trueSize++; // inserted so increment true size count
                return true;
            }
            return false; // THIS SHOULD NOT BE NEEDED
        }

        // get num items in table
        size_t size() const{
            return trueSize;
        }

        // load factor -> size / capacity, casted to doubles just to be sure
        double alpha() const{
            return static_cast<double>(trueSize) / static_cast<double>(capacity);
        }

    private:
        std::vector<HashTableBucket> buckets;
        size_t trueSize; // number of things in it
        size_t capacity; // number of things it could have
        std::vector<size_t> offsets; // probing offsets

        // hash function to prevent excessive repetition in every other method
        size_t hash(const std::string& key) const;
        // resizer - double when load factor >= .5
        void resize();
};


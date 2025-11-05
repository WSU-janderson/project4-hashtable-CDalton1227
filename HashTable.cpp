/**
 * HashTable.cpp
 */

#include "HashTable.h"
#include <functional>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <ostream>
#include <utility>

HashTable::HashTable(size_t initCapacity) {
    trueSize = 0; // nothing in it yet
    currentCapacity = initCapacity; // set to the size input in constructor

    buckets.resize(currentCapacity); // resize (vector) to initCapacity number of buckets

    offsets.resize(currentCapacity - 1);

    std::iota(offsets.begin(), offsets.end(), 1); // make the list of offsets starting with 1
    std::random_device rd; // seed for rng
    std::mt19937 gen(rd()); // create rng
    std::shuffle(offsets.begin(), offsets.end(), gen); // shuffle list
}

size_t HashTable::hash(const std::string& key) const {
    size_t hashVal = std::hash<std::string>()(key);
    return hashVal % currentCapacity; // keep index within bounds
}

bool HashTable::insert(const std::string& key, const size_t& value){
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
            size_t probe = (home + offsets[i]) % currentCapacity;

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
                if (buckets[probe].key == key) {
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
    return false; // should not be needed
}

// get num items in table
size_t HashTable::size() const {
    return trueSize;
}

// load factor -> size / capacity, casted to doubles just to be sure
double HashTable::alpha() const {
    return static_cast<double>(trueSize) / static_cast<double>(currentCapacity);
}

// resizer - double when load factor >= .5
void HashTable::resize() {
    // create a save of the current buckets
    std::vector<HashTableBucket> temp = std::move(buckets);

    // double cap
    currentCapacity = currentCapacity * 2;

    buckets.resize(currentCapacity);

    // new offsets for new capa, basically all the same as before in HashTable up top
    offsets.resize(currentCapacity - 1);
    std::iota(offsets.begin(), offsets.end(), 1);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(offsets.begin(), offsets.end(), gen);

    trueSize = 0;

    for (const auto& bucket : temp) {
        if (bucket.type == BucketType::NORMAL) {
            insert(bucket.key, bucket.value);
        }
    }
}

bool HashTable::contains(const std::string& key) const {
    // index
    size_t home = hash(key);
    const HashTableBucket& bucket = buckets[home];

    if (bucket.type == BucketType::NORMAL && bucket.key == key) {
        // this is it
        return true;
    }

    if (bucket.type == BucketType::ESS) {
        // empty
        return false;
    }

    for (size_t i = 0; i < offsets.size(); ++i) {
        size_t index = (home + offsets[i]) % currentCapacity;
        const HashTableBucket& probe = buckets[index];

        if (probe.type == BucketType::NORMAL) {
            if (probe.key == key) {
                return true; // Normal and same key = true
            }
        }

        if (probe.type == BucketType::ESS) {
            // cant' be here - ess
            return false;
        }
    }
    return false; // if program reaches here, no key, no item
}

std::optional<size_t> HashTable::get(const std::string& key) const {
	// get home index
    size_t home = hash(key);
    const HashTableBucket& bucket = buckets[home];

    if (bucket.type == BucketType::ESS) {
		return std::nullopt; // because this bucket has never been used
    }

    if (bucket.type == BucketType::NORMAL && bucket.key == key) {
    	return bucket.value; // this is it
    }

    for (size_t i = 0; i < offsets.size(); ++i) {
    	size_t index = (home + offsets[i]) % currentCapacity;
        const HashTableBucket& probe = buckets[index];

        if (probe.type == BucketType::ESS) {
        	return std::nullopt; // empty
        }

        if (probe.type == BucketType::NORMAL) {
        	if (probe.key == key) {
            	return probe.value; // key found
        	}
        }
    }
    return std::nullopt; // key not found
}

bool HashTable::remove(const std::string& key) {
  	// home index
	size_t home = hash(key);
    HashTableBucket& bucket = buckets[home];

    // check home
    if (bucket.type == BucketType::ESS) {
    	return false; // key shouldn't be here unless type assignments aren't working
    }

    if (bucket.type == BucketType::NORMAL && bucket.key == key) {
    	bucket.type = BucketType::EAR; // mark as removed from
        trueSize--; // shrinks after loss
        return true; // done
    }

    for (size_t i = 0; i < offsets.size(); ++i) {
    	size_t index = (home + offsets[i]) % currentCapacity;
        HashTableBucket& probe = buckets[index];

        if (probe.type == BucketType::ESS) {
        	return false; // empty
        }

        if (probe.type == BucketType::NORMAL) {
        	if (probe.key == key) {
                  probe.type = BucketType::EAR; // removal
                  trueSize--; // shrinks
                  return true; // done
        	}
        }
    }
    return false; // not found
}

size_t& HashTable::operator[](const std::string& key) {
  	// home index again
	size_t home = hash(key);
    HashTableBucket& bucket = buckets[home];

    // check home again
    if (bucket.type == BucketType::NORMAL && bucket.key == key) {
    	return bucket.value;
    }

    for (size_t i = 0; i < offsets.size(); ++i) {
    	size_t index = (home + offsets[i]) % currentCapacity;
        HashTableBucket& probe = buckets[index];

        if (probe.type == BucketType::NORMAL && probe.key == key) {
        	return probe.value;
        }
    }
    throw std::runtime_error("Key not found");
}

std::vector<std::string> HashTable::keys() const {
	std::vector<std::string> keys; // new vector for keys
    // loop through the buckets and add all keys to new vector if type is normal (has a key)
    for (const auto& bucket : buckets) {
    	if (bucket.type == BucketType::NORMAL) {
        	keys.push_back(bucket.key);
    	}
    }
    return keys; // return vector of keys
}

size_t HashTable::capacity() const {
	return currentCapacity; // simple enough, returns the capacity member var
}

std::ostream& operator<<(std::ostream& os, const HashTable& t) {
  	// loop through the buckets
	for (size_t i = 0; i < t.capacity(); ++i) {
    	const HashTableBucket& bucket = t.buckets[i];

        // check bucket type = normal?
        if (bucket.type == BucketType::NORMAL) {
        	os << "Bucket " << i << ": <" << bucket.key << ", " << bucket.value << ">" << std::endl;
        }
	}
   	return os;
}
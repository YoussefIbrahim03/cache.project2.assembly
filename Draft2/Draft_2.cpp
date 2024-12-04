#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <bitset>

using namespace std;

// Global counters for hit and miss ratios
float hits = 0;
float misses = 0;
float total = 0;

// Struct for cache blocks
struct CacheBlock {
    bool valid = false;
    int tag = 0;
    int data = 0;
};

// Function to print cache contents
void printCache(const vector<CacheBlock>& cache) {
    for (size_t i = 0; i < cache.size(); i++) {
        cout << "Index: " << i
             << ", Tag: " << cache[i].tag
             << ", Valid: " << cache[i].valid
             << ", Data: " << cache[i].data << endl;
    }
}

// Function to calculate tag, index, and displacement
void check(int address, vector<CacheBlock>& cache, int block_size,
           int line_size, int index_bits, int mem_access_time,
           int cache_access_time, int& total_cycles, int offset_bits) {
            
    // Displacement (offset)
    int offset = address & ((1 << offset_bits) - 1);

    // Index
    int index = (address >> offset_bits) & ((1 << index_bits) - 1);

    // Tag
    int tag = address >> (offset_bits + index_bits);

    cout << "Address: " << bitset<32>(address) << endl;
    cout << "Tag: " << tag << ", Index: " << index << ", Offset: " << offset << endl;

    // Cache operation
    if (!cache[index].valid || cache[index].tag != tag) {
        // Cache miss
        misses++;
        total++;
        cache[index].valid = true;
        cache[index].tag = tag;
        cache[index].data = address; // Simulate storing data
        total_cycles += mem_access_time + cache_access_time;
    } else {
        // Cache hit
        hits++;
        total++;
        total_cycles += cache_access_time;
    }
}

int main() {
    int cache_size, line_size, mem_access_time, cache_access_time, mem_size;
    int total_cycles = 0;

    // Input configuration
    cout << "Enter memory size in bits (16 to 40): ";
    cin >> mem_size;

    cout << "Enter memory access time in clock cycles (50 to 200): ";
    cin >> mem_access_time;

    cout << "Enter cache size in bytes: ";
    cin >> cache_size;

    cout << "Enter line size in bytes: ";
    cin >> line_size;

    cout << "Enter cache access time in clock cycles (1 to 10): ";
    cin >> cache_access_time;

    // Cache configuration
    int block_count = cache_size / line_size; // Number of blocks in cache
    int index_bits = log2(block_count);      // Number of bits for index
    int offset_bits = log2(line_size);       // Number of bits for displacement
    int tag_bits = mem_size - index_bits - offset_bits;

    vector<CacheBlock> cache(block_count);

    // Simulated memory accesses
    vector<int> memory_access = {12345, 54321, 12345, 99999, 12345, 54321};

    // Simulate each memory access
    for (size_t i = 0; i < memory_access.size(); i++) {
        check(memory_access[i], cache, block_count, line_size, index_bits,
              mem_access_time, cache_access_time, total_cycles, offset_bits);
    }

    // Print cache and statistics
    printCache(cache);
    cout << "Hit Ratio: " << (hits / total) << endl;
    cout << "Miss Ratio: " << (misses / total) << endl;
    cout << "Total Clock Cycles: " << total_cycles << endl;

    return 0;
}

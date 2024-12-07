#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <bitset>
#include <fstream>
#include <sstream>
using namespace std;


float hits = 0;
float misses = 0;
float total = 0;


struct CacheBlock {
    bool valid = false;
    int tag = 0;
    int data = 0;
};

vector<int> readMemoryAccessSequence(const string& filename) {
    vector<int> memory_access;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return memory_access;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string address;
        while (getline(ss, address, ',')) {
            try {
                memory_access.push_back(hexToDecimal(address));
            } catch (const exception& e) {
                cerr << "Invalid address in file: " << address << endl;
            }
        }
    }
      file.close();
    return memory_access;
}

int hexToDecimal(const string& hex) {
    int decimalValue = 0;
    int base = 1; 

    for (int i = hex.length() - 1; i >= 0; i--) {
        char digit = hex[i];

        if (digit >= '0' && digit <= '9') {
            decimalValue += (digit - '0') * base;
        } else if (digit >= 'A' && digit <= 'F') {
            decimalValue += (digit - 'A' + 10) * base;
        } else if (digit >= 'a' && digit <= 'f') {
            decimalValue += (digit - 'a' + 10) * base;
        } else {
            std::cerr << "Invalid hexadecimal character: " << digit << std::endl;
            return -1; 
        }

        base *= 16; 
    }

    return decimalValue;
}


void printCache(const vector<CacheBlock>& cache) {
    for (size_t i = 0; i < cache.size(); i++) {
        cout << "Index: " << i
             << ", Tag: " << cache[i].tag
             << ", Valid: " << cache[i].valid
             << ", Data: " << cache[i].data << endl;
    }
}


void check(int address, vector<CacheBlock>& cache, int block_size,
           int line_size, int index_bits, int mem_access_time,
           int cache_access_time, int& total_cycles, int offset_bits) {
            
    int offset = address & ((1 << offset_bits) - 1);

    int index = (address >> offset_bits) & ((1 << index_bits) - 1);

    int tag = address >> (offset_bits + index_bits);

    cout << "Address: " << bitset<32>(address) << endl;
    cout << "Tag: " << tag << ", Index: " << index << ", Offset: " << offset << endl;

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
    string cache_size, line_size;
    int mem_access_time, cache_access_time, mem_size;
    int total_cycles = 0;

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

    int block_count = hexToDecimal(cache_size) / hexToDecimal(line_size); // Number of blocks in cache
    int index_bits = log2(block_count);      // Number of bits for index
    int offset_bits = log2(hexToDecimal(line_size));       // Number of bits for displacement
    int tag_bits = mem_size - index_bits - offset_bits;

    vector<CacheBlock> cache(block_count);

    string filename;
    cout << "Enter the file name containing memory access sequence: ";
    cin >> filename;

   vector<int> memory_access = readMemoryAccessSequence(filename);

    if (memory_access.empty()) {
        cerr << "No valid memory access sequence found. Exiting." << endl;
        return -1;
    }

    for (size_t i = 0; i < memory_access.size(); i++) {
        check(memory_access[i], cache, block_count, hexToDecimal(line_size), index_bits,
              mem_access_time, cache_access_time, total_cycles, offset_bits);
        printCache(cache);
        cout << "Hit Ratio: " << (hits / total) << endl;
        cout << "Miss Ratio: " << (misses / total) << endl;
        cout << "Total Clock Cycles: " << total_cycles << endl;
    }

    return 0;
}

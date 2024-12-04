#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

float hits=0;
float misses=0;
float total=0;

vector<pair<int, int>> memory_access;

struct cache_block
{
    bool valid=false;
    int tag=0;
    int data=0;
};


void print_cache(vector<cache_block> &cache,int cache_size)
{
    for(int i=0;i<cache_size;i++)
    {
        cout << "index: " << i << " tag: " << cache[i].tag << " valid: " << cache[i].valid << " data: " << cache[i].data << endl;
    }
}

void check(int mem_index, vector<cache_block> &cache,int cache_size,
           int cache_line_size,
           int index_size,
           int mem_access_t,
           int cache_access_t,
           int cc,
           int bit_size)
{
    
    bitset<40> binary(memory_access[mem_index].first);
    int index_1=0;
    for(int i=0;i<index_size;i++)
    {
        if(binary[i]==1)
        {
        index_1=index_1+(1 << i);
        }
    }
    
    int tag_1=0;
    for(int i=bit_size-1;i>=index_size;i--)
    {
        if(binary[i]==1)
        {
        tag_1=tag_1+(1 << i);
        }
    }
    tag_1=(tag_1>>index_size);
    
    if(!cache[index_1].valid)
    {
        misses++;
        total++;
        cache[index_1].valid=true;
        cache[index_1].tag=tag_1;
        cache[index_1].data=memory_access[mem_index].second;
        cc=mem_access_t+cache_access_t+cc;
    }
    else
    {
        hits++;
        total++;
        cc=cache_access_t+cc;
    }
    
}

int main()
{
    
    int cache_size=0;
    int cache_line_size=1;
    int mem_access_t=130;
    int cache_access_t=10;
    int cc=0;
    int bit_size=32;
    
    
    
    do
    {
        cout << "enter number of bits needed to access the memory" << endl;
        cin >> bit_size;
    }while(bit_size<16 || bit_size>40);
    do
    {
        cout << "enter memory access time in clock cycles" << endl;
        cin >> mem_access_t;
    }while(mem_access_t<50 || mem_access_t>200);
    
    
    
    cout << "enter cache size" << endl;
    cin >> cache_size;
    cout << "enter line size" << endl;
    cin >> cache_line_size;
    do
    {
        cout << "enter cache access time in clock cycles" << endl;
        cin >> cache_access_t;
    }while(cache_access_t<1 || cache_access_t>10);
    
    int index_size=log(cache_size)/log(2);
    
    
    vector<cache_block> cache(cache_size);
    
    
    for (int i = 0; i < 30; i++)
    {
        int k = rand() % 4294967296;
        memory_access.emplace_back(i, k);
    }
    
    check(0, cache,cache_size, cache_line_size,index_size,mem_access_t,cache_access_t,cc,bit_size);
    //check(19);
    print_cache(cache, cache_size);
    cout << "hit ratio: " << hits/total << endl;
    cout << "miss ratio: " << misses/total << endl;
    cout << "clock cycles: " << cc << endl;
    
    return 0;
}

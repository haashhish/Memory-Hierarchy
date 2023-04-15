#include<iostream>
#include<vector>
#include<map>
#include<fstream>
#include<sstream>
#include<string>
#include<bitset>
using namespace std;

struct cachee
{
    int tag;
    bool valid_bit = 0;
};

string decimalToBinary(int number)
{
    string binary = bitset<16>(number).to_string();
    return binary;
}

void output(int lineNumber, int i, int tag1, int valid1, int tag2, int valid2, int hitcount, int misscount, int numberOfAccesses, int accessTime, int hit_miss)
{
 /*
 the program should output the following:
 -Valid bits and tags of all entries
 -Total number of accesses
 -The hit and miss ratios
 -The Average Memory Access Time (AMAT) of the memory hierarchy (in cycles)
 */
    
    cout<<"Word Address: "<< i <<endl;
    cout<<"Binary Address: "<< decimalToBinary(i) <<endl;
    if(hit_miss)
    {
    cout<<"Hit/Miss: Hit"<<endl;
    }
    else
    {
        cout<<"Hit/Miss: Miss"<<endl;
    }
    cout<<"Cache block: "<< lineNumber<<endl;
    cout<<"Before accessing:"<<endl;
    cout<<"------------------"<<endl;
    cout<<"Valid bit = "<< valid1<<endl;
    cout<<"Tag = "<<tag1<<"\n\n";
    cout<<"After accessing:"<<endl;
    cout<<"------------------"<<endl;
    cout<<"Valid bit = "<<1<<"\n"<<"Tag = "<<tag2<<endl;
    cout<<"____________________________________"<<endl;
}

void CachingSystem(int cacheSize, int lineSize, int cycles, vector<int> &memoryAddresses)
{
    int hit_counter = 0;
    int miss_counter = 0;
    int hit_miss=0;
    int numberOfAccesses;
    int block_number, line_number;
    int valid1, tag1;
    int valid2, tag2;

    numberOfAccesses = memoryAddresses.size();
    int cacheLines = cacheSize / lineSize;

    vector<cachee> cache(cacheLines);
   
    for(auto i : memoryAddresses)
    {
      
        block_number = i / lineSize;   //block_number = memory_address / line size....[block_number = tag_bits + line_number]
        line_number = block_number % cacheLines;   //line_number = block_number % cache_lines
        tag2 = block_number / cacheLines;   //tag_bits = block_number  / cache_lines

     //storing the values of valid bit and tag before accessing memory addresses
       valid1 = cache.at(line_number).valid_bit;
       tag1 = cache.at(line_number).tag;

       if((cache.at(line_number).valid_bit==1) && (tag2 == cache.at(line_number).tag)) //if the valid bit = 1 and the tag of the address = the original tag -> hit
       {
         hit_counter++;
         hit_miss = 1;
       }
       else
       {
         miss_counter++;
         hit_miss = 0;
         cache.at(line_number).valid_bit = 1;
         cache.at(line_number).tag = tag2;
       }

        output(line_number, i, tag1, valid1, tag2, valid2, hit_counter, miss_counter, numberOfAccesses, cycles, hit_miss);

    }
    cout<<"number of hits: "<<hit_counter<<endl;
    cout<<"number of misses: "<<miss_counter<<endl;
    cout<<"The total number of accesses: "<<numberOfAccesses<<endl;
    //Hit Rate = total hits/(hits+misses)
    cout<<"The hit ratio: "<<(float(hit_counter)/numberOfAccesses)*100<<" %"<<endl;
    //Hit Rate = total misses/(hits+misses)
    cout<<"The miss ratio: "<<(float(miss_counter)/numberOfAccesses)*100<<" %"<<endl;
    //AMAT = hit time + (miss rate * miss penalty)
    cout<<"The Average Memory Access Time (AMAT) of the memory hierarchy (in cycles): "<< cycles + ((float(miss_counter)/numberOfAccesses)*100)<<endl;
}

string parser(string line)
{
    char space = ' ';
    for(int i=0; i<line.size(); i++) //loop to remove any commas from the instruction
    {
        if(line[i]==',')
        {
            line[i] = space;
        }
    }

    return line;
}

void readinput()
{
    int S, L, clk;
    string line;
    string temp;
//reading input values from the text file.
    ifstream input;
    input.open("input.txt");
    while(getline(input, line))
    {
        parser(line); //parsing the values of the text files.
    }
    input.close();

    stringstream split(line);
//setting input values for total cache size (S), cache line size (L), and the number of cycles needed to access the cache(clk).
    split >> line;
    S = stoi(line);
    split >> temp;
    L = stoi(temp);
    split >> temp;
    clk = stoi(temp);

    cout<<"Total cache size (S): "<<S<<"\n"<<"Cache line size (L): "<<L<<"\n"<<"Number of cycles to access cache(clk): "<<clk<<endl;

    vector<int> memoryAddresses;
    int totalAccesses;
    ifstream sequence;
    sequence.open("MemoryAddresses.txt");
    while(!sequence.eof() && sequence >> totalAccesses) 
    {
 
        memoryAddresses.push_back(totalAccesses);
    }
    sequence.close();
    
    CachingSystem(S, L, clk, memoryAddresses);
}


main()
{
    readinput();
}
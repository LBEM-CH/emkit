
#include <iostream>
#include <fstream>
#include <iosfwd>

#include "elements.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if(argc < 2) {
        std::cerr << "Usage:\n\t" << argv[0] << " <table_file> \n\n";
        exit(1);
    }
    
    Table table = Table::read_table(argv[1], 6);
    cout << table;
    
    cout << "Amplitudes column:\n";
    for(auto v : table.get_column<double>(4)) std::cout << v << "\n";
    
    cout << "Setting phases to zero:\n";
    std::vector<double> zeros(table.rows(), 0.0);
    table.set_column(4, zeros);
    cout << table;
    
    cout << "Sorting:\n";
    table.sort<int>(2);
    table.sort<int>(1);
    table.sort<int>(0);
    cout << table;
    
    std::string out_file = std::string(argv[1]) + ".out";
    cout << "Writing to file:\n\t" << out_file << "\n";
    table.write_table(out_file);
    
    return 0;
}


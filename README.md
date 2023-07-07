# db_converter
Converting tool from BerkeleyDB to various databases formats

requires: libdb-devel, gdbm-devel, lmdb-devel, g++

## Usage
* Edit `convert.cc`
1. set libdb file as parameter of method `connect_database`
2. set output filename as parameter of `create_database` method
* Build the binary `make`
* Run the binary `./convert`
* Your new file with converted database should be created

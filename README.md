# db\_converter
Converting tool from BerkeleyDB to various databases formats

requires: libdb-devel, gdbm-devel, lmdb-devel, g++, make

## Build
* Clone the repo
* Build the binary `make`
## Usage
db\_converter --src SOURCE --dst DEST [ DBTYPE ]
* SOURCE - path to BerkeleyDB database
* DEST - file path to be created in the new database format
* DBTYPE - optional argument `--lmdb` for producing LMDB database or `--gdbm` for GDBM databse
GDBM is the default one

## Example
`./db\_convert --src access.db --dst access.gdbm`
`./db\_convert --src access.db --dst access.lmdb --lmdb`
* Your new file with converted database will be created

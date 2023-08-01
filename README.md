# db\_converter
Converting tool from BerkeleyDB to various databases formats

requires: libdb-devel, gdbm-devel, lmdb-devel, g++, make

## Build
* Clone the repo
* Build the binary `make`
## Usage
db\_converter --src SOURCE --dest DEST [ DBTYPE ]
* SOURCE - path to BerkeleyDB database
* DEST - file path to be created in the new database format
* DBTYPE - optional argument `--lmdb` for producing LMDB database or `--gdbm` for GDBM databse
GDBM is the default one

## Example
`./db_convert --src access.db --dest access.gdbm`

`./db_convert --src access.db --dest access.lmdb --lmdb`
* Your new file with converted database will be created

#include "convert_db.h"

int main(int argc, char* argv[]){
	Libdb x =Libdb();
	x.connect_database("access.db");
	LMDB_ y = LMDB_();
	y.create_database("./");
	y.fill_database(x);
	y.close_db();
	GDBM_ b = GDBM_();
	b.create_database("test.gdbm");
	//b.fill_database(x);
	return 0;
}

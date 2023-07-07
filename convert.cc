#include "convert_db.h"

int main(int argc, char* argv[]){
	Libdb x =Libdb();
	x.connect_database("access.db");
	GDBM_ b = GDBM_();
	b.create_database("access.gdbm");
	if (!b.fill_database(x))
		return 1;
    	b.close_db();
	return 0;
}

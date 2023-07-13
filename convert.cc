#include "convert_db.h"

int main(int argc, char* argv[]){
	std::string src, dst;
	if (argc == 3) {
		src = argv[1];
		dst = argv[2];
	} else {
		src = "access.db";
		dst = "access.gdbm";
	}

	Libdb x =Libdb();
	x.connect_database(src);
	GDBM_ b = GDBM_();
	b.create_database(dst);
	if (!b.fill_database(x))
		return 1;
    	b.close_db();
	return 0;
}

#include "convert_db.h"
#include <getopt.h>

void help(int e){
	std::cout<<"Usage: db_converter --src SRC --dest DEST [DBTYPE]"<<std::endl;
	std::cout<<"Convert database from BerkeleyDB format to GDBM/LMDB"<<std::endl;
	std::cout<<std::endl;
	std::cout<<" --src\t\tsource database file in BerkeleyDB format"<<std::endl;
	std::cout<<" --dst\t\tfile to be created in GDBM/LMDB format"<<std::endl;
	std::cout<<" --lmdb\t\tDBTYPE - specifies output database type to LMDB. LMDB database consists of multiple files. In case of using LMDB output directory must be created and provided as DEST"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"If DBTYPE is not set, GDBM will be used as default"<<std::endl;
	std::cout<<std::endl;
	std::cout<<"Report bugs to: <https://github.com/fila43/db_converter>"<<std::endl;
	exit(e);
}

int main(int argc, char* argv[]){
	std::string src, dst;
	src = dst ="";
	bool lmdb  = false;

	option long_options[] = {
        	{"dest", required_argument, NULL, 'd'},
        	{"src", required_argument, NULL, 's'},
        	{"lmdb", no_argument, NULL, 'l'},
        	{"help", no_argument, NULL, 'h'},
		{0}
	};
	int option_index = 0;
	while (1) {
		const int opt = getopt_long(argc, argv, "d:s:h", long_options, &option_index);
		if (opt == - 1)
			break;

		switch (opt) {
			case 'd':
				dst = optarg;
				break;
			case 's':
				src = optarg;
				break;
			case 'l':
				lmdb = true;
				break;
			case 'h':
				help(0);
			default:
				break;
		}
	}
	if (src == "" || dst == "")
		help(1);


	DB_ * x = new Libdb();
	if (!x->connect_database(src)) {
	// error is printed in the connect_database function
	delete x;
	return 1;
	}
	DB_ *b;
	if (lmdb)
		b = new LMDB_();
	else
		b = new GDBM_();
	if (!b->create_database(dst)) {
		std::cerr<<"Failed to create destination database\n";
		delete x;
		delete b;
		return 1;
	}
	if (!b->fill_database(x)){
		std::cerr<<"database filling failed\n";
		delete x;
		delete b;
		return 1;
	}
	b->close_db();
	delete x;
	delete b;
	return 0;
}


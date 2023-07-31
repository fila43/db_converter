#include "convert_db.h"
#include <getopt.h>

void help(int e){
	std::cout<<"Help TBD"<<std::endl;
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
	x->connect_database(src);
	DB_ *b;
	if (lmdb)
		b = new LMDB_();
	else
		b = new GDBM_();
	b->create_database(dst);
	if (!b->fill_database(x)){
		std::cerr<<"database filling failed\n";
		return 1;
	}
    	b->close_db();
	delete x;
	delete b;
	return 0;
}


CFLAGS=-Wall -Wextra
all:db.cc convert_db.h convert.cc
	g++ $(CFLAGS) convert.cc db.cc -llmdb -ldb -lgdbm -lgdbm_compat -o db_converter
static:db.cc convert_db.h convert.cc
	g++ $(CFLAGS) convert.cc db.cc -llmdb -Wl,-Bstatic $(LDFLAGS) -ldb-5.3 -Wl,-Bdynamic -lgdbm -lgdbm_compat -o db_converter

clean:
	rm -rf db_converter

all:db.cc convert_db.h convert.cc
	g++ -Wall -Wextra convert.cc db.cc -llmdb -ldb -lgdbm -lgdbm_compat -o convert

clean:
	rm -rf convert

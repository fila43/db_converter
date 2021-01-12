/*
 * Author: Filip Januš
 * DBConverter
 * Main purpose of project is convert BerkeleyDB database to other
 * supported format
 *
 */
#include <iostream>
#include <string>
#include <db.h>
#include <gdbm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
extern "C"{
#include <gdbm/ndbm.h>
}
#include <lmdb.h>
enum DB_type {
	LIBDB,
	LMDB,
	SQLITE,
	GDBM
};

class DB_ {
	protected:
		DB_type database_type;
	public:
		DB_();
		std::string path;
		void * get_item();
		bool put_item();
		virtual bool connect_database(std::string path);
		bool create_db();
};

class Libdb: public DB_ {
	private:
		DBC * cursorp;
		DB * db;
	public:
		Libdb();
		bool connect_database(std::string path);
		DBC * get_database();
};

class GDBM_: public DB_ {
	GDBM_FILE f;
	public:
		GDBM_();
		bool create_database(std::string name);
		bool fill_database(Libdb old_database);
		void close_db();
};
//https://github.com/LMDB/lmdb/blob/mdb.master/libraries/liblmdb/mtest2.c
//further inspiration
class LMDB_: public DB_ {
	private:
		MDB_env * lmdb_database;
		MDB_txn * lmdb_transaction;
		MDB_dbi database_handle;
	public:
		LMDB_();
		bool create_database(std::string name);
		bool fill_database(Libdb old_database);
		void close_db();
};


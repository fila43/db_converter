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
/*
 * "Semi" Abstract class for database all database types
 */
class DB_ {
	protected:
		DB_type database_type;
	public:
		DB_();
		std::string path;
		void * get_item();
		bool put_item();
		virtual bool connect_database(std::string path);
		virtual bool create_database(std::string name);
		bool create_db();
		virtual bool fill_database(DB_ * old_database);
		virtual void close_db() = 0;
		virtual DBC * get_database();
		virtual ~DB_();
};
/*
 * Libdb class needs only open and read data from libdb database
 * connect_database - open database file and stores pointer to database to atributte
 * get_database - return pointer to database structure
 */
class Libdb: public DB_ {
	private:
		DBC * cursorp;
		DB * db;
	public:
		Libdb();
		bool connect_database(std::string path);
		DBC * get_database();
		void close_db();
		~Libdb();
};
/*
 * GDBM class provides API for GDBM, allowes to open and create and fill gdbm database
 * create_database (name) - create database file identified by name variable
 * fill_database (old_database) - append data from provided libdb database to newly creqated GDBM database
 * 	it's necessary to call create_database before fill_database
 *	database cursor of old database is not seeked to the beginning of database after reading
 * close_db - close database file
 */
class GDBM_: public DB_ {
	GDBM_FILE f;
	public:
		GDBM_();
		bool create_database(std::string name);
		bool fill_database(DB_ * old_database);
		void close_db();
		~GDBM_();
};
//https://github.com/LMDB/lmdb/blob/mdb.master/libraries/liblmdb/mtest2.c
//further inspiration
/*
 * LMDB API for creating and filling database file
 * The API is the same asi in case of GDBM
 */
class LMDB_: public DB_ {
	private:
		MDB_env * lmdb_database;
		MDB_txn * lmdb_transaction;
		MDB_dbi database_handle;
	public:
		LMDB_();
		bool create_database(std::string name);
		bool fill_database(DB_ * old_database);
		void close_db();
		~LMDB_();
};


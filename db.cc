#include "convert_db.h"



Libdb::Libdb(){
	database_type = DB_type::LIBDB;
};
bool DB_::connect_database(std::string){
	return false;
}
DB_::DB_(){
	;
};
bool DB_::fill_database(DB_ *) {
	return false;
}
bool DB_::create_database(std::string) {
	return false;
}
DBC * DB_::get_database() {
	return NULL;
}
void DB_::close_db(){}

DB_::~DB_() {
}

bool Libdb::connect_database(std::string path){
	DB * db;
	int status;

	if (db_create(&db, NULL, 0) != 0)
	//throw exception
		return false;

	status = db->open(db,NULL,path.data(),NULL,DB_UNKNOWN,0,0);
	if (status != 0){
		fprintf(stderr,"%s%d\n","Can't open database - ",status);
		return false;
	}

	if (db->cursor(db,NULL,&(this->cursorp),0) != 0){
		fprintf(stderr,"can't open cursor");
		return false;
	}
	this->db = db;

	return true;
};

DBC * Libdb::get_database(){
	return this->cursorp;
}

void Libdb::close_db() {
	cursorp->close(cursorp);
	db->close(db, 0);
}

GDBM_::GDBM_(){
	database_type = DB_type::GDBM;
}

bool GDBM_::create_database(std::string name){
	f = gdbm_open(name.data(),512,GDBM_NEWDB,O_RDWR|S_IRUSR|S_IWUSR,NULL);
	if (f == NULL)
		return false;
	this->f = f;
	return true;
}

bool GDBM_::fill_database(DB_ * old_database){
	DBT * data_db;
	DBT * key_db;
	datum key, value;

	DBC * cursorp = old_database->get_database();

	//Alocate libdb record
	data_db = (DBT*)calloc(1, sizeof(DBT));
	key_db = (DBT*)calloc(1, sizeof(DBT));

	while(cursorp->c_get(cursorp, key_db, data_db, DB_NEXT) == 0){
		key.dptr = (char*)key_db->data;
		key.dsize = key_db->size;
		value.dptr = (char*)data_db->data;
		value.dsize = data_db->size;

		int status = gdbm_store(this->f, key, value, GDBM_REPLACE);
		if (status != 0)
			return false;
	}
	return true;
}

void GDBM_::close_db(){
	gdbm_close(this->f);
}

LMDB_::LMDB_(){
	database_type = DB_type::LMDB;

}
bool LMDB_::create_database(std::string name){
	if (mdb_env_create(&lmdb_database) != 0){
                std::cerr<<"Can't create LMDB database handler\n";
                return false;
        }
        /* default size 10485760 from documentation */
        if (mdb_env_set_mapsize(lmdb_database,10485760) != 0){
	 	return false;
	}
	int status;
        status = mdb_env_open(lmdb_database, name.data(),0,0666);
       	if (status!= 0){
		std::cerr<<"error in mdb_env_open - status: "<<status<<std::endl;
                return false;
	}

        if (mdb_txn_begin(lmdb_database, NULL, 0, &lmdb_transaction) != 0){
                return false;
	}

        if (mdb_dbi_open(lmdb_transaction, NULL, MDB_DUPSORT, &database_handle) != 0){
                return false;
	}

	return true;
}

bool LMDB_::fill_database(DB_ * old_database){
	DBT * data_db;
	DBT * key_db;
	MDB_val * key;
	MDB_val * data;
	DBC * cursorp = old_database->get_database();

	data_db = (DBT*)calloc(1,sizeof(DBT));
        key_db = (DBT*)calloc(1,sizeof(DBT));

	key = (MDB_val*)calloc(1,sizeof(DBT));
        data = (MDB_val*)calloc(1,sizeof(DBT));

	while (cursorp->c_get(cursorp,key_db,data_db,DB_NEXT) == 0){
                key->mv_size = key_db->size;
                key->mv_data = key_db->data;

                data->mv_size = data_db->size;
                data->mv_data = data_db->data;

                int status = mdb_put(this->lmdb_transaction,this->database_handle, key, data, 0);
		if (status == MDB_KEYEXIST )
			continue;

		if (status != 0)
			return false;
        }

	free(data_db);
	free(key_db);
	free(key);
	free(data);

	return true;
}

void LMDB_::close_db(){
	mdb_txn_commit(this->lmdb_transaction);
	mdb_dbi_close(this->lmdb_database, this->database_handle);
	mdb_env_close(this->lmdb_database);
}



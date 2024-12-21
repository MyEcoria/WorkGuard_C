#ifndef DB_H
    #define DB_H
    #define DATA_PATH dataf
    #include <stdio.h>
    #include <stdlib.h>
    #include "../include/lmdb.h"
    #include <sys/stat.h>
    #include <sys/types.h>
    typedef struct {
        MDB_env *env;
        MDB_dbi dbi;
        MDB_val key;
        MDB_val value;
        MDB_txn *txn;
        int rc;
    } db_t;
    void create_dir_if_not_exists(const char *path);
    int open_lmdb(db_t *database, const char *db_path);
    int begin_transaction(db_t *database);
    void create_database(db_t *database);
    int write_in_db(db_t *database, const char *key, const char *value);
    char *get_value(db_t *database, const char *key);
    void close_db(db_t *database);
#endif
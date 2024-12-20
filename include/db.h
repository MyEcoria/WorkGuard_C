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
#endif
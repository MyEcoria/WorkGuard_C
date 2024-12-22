/*
** EPITECH PROJECT, 2024
** my_putchar
** File description:
** print a character
*/
#include <stdio.h>
#include <stdlib.h>
#include "../../../include/lmdb.h"
#include "../../../include/db.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int open_lmdb(db_t *database, const char *db_path)
{
    database->rc = mdb_env_create(&database->env);
    if (database->rc != 0) {
        fprintf(stderr, "Erreur création de l'environnement: %s\n",
        mdb_strerror(database->rc));
        return -1;
    }
    database->rc = mdb_env_set_maxdbs(database->env, 1);
    if (database->rc != 0) {
        fprintf(stderr,
        "Erreur configuration de l'environnement: %s\n",
        mdb_strerror(database->rc));
        return -1;
    }
    database->rc = mdb_env_open(database->env, db_path, 0, 0664);
    if (database->rc != 0) {
        fprintf(stderr, "Erreur ouverture de l'environnement: %s\n",
        mdb_strerror(database->rc));
        return -1;
    }
    return 0;
}

int begin_transaction(db_t *database)
{
    database->rc = mdb_txn_begin(database->env, NULL, 0, &database->txn);
    if (database->rc != 0) {
        fprintf(stderr,
        "Erreur création de la transaction: %s\n",
        mdb_strerror(database->rc));
        return -1;
    }
    return 0;
}

void create_database(db_t *database)
{
    database->rc = mdb_dbi_open(database->txn, NULL, 0, &database->dbi);
    if (database->rc != 0) {
        fprintf(stderr,
        "Erreur ouverture de la base de données: %s\n",
        mdb_strerror(database->rc));
        exit(1);
    }
}

int write_in_db(db_t *database, const char *key, const char *value)
{
    database->key.mv_size = strlen(key) + 1;
    database->key.mv_data = (void *)key;
    database->value.mv_size = strlen(value) + 1;
    database->value.mv_data = (void *)value;
    database->rc = mdb_put(database->txn, database->dbi,
    &database->key, &database->value, 0);
    if (database->rc != 0) {
        fprintf(stderr,
        "Erreur écriture dans la base de données: %s\n",
        mdb_strerror(database->rc));
        return -1;
    }
    database->rc = mdb_txn_commit(database->txn);
    if (database->rc != 0) {
        fprintf(stderr,
        "Erreur validation de la transaction: %s\n",
        mdb_strerror(database->rc));
        return -1;
    }
    return 0;
}

char *get_value(db_t *database, const char *key)
{
    MDB_val key_data;
    MDB_val value2;
    char *result = NULL;

    key_data.mv_size = strlen(key) + 1;
    key_data.mv_data = (void *)key;
    database->rc = mdb_txn_begin(database->env, NULL,
    MDB_RDONLY, &database->txn);
    if (database->rc != 0) {
        return NULL;
    }
    database->rc = mdb_get(database->txn, database->dbi, &key_data, &value2);
    if (database->rc != 0) {
        fprintf(stderr, "Lecture: %s\n", mdb_strerror(database->rc));
        mdb_txn_abort(database->txn);
        return NULL;
    }
    result = strdup((char *)value2.mv_data);
    mdb_txn_commit(database->txn);
    return result;
}

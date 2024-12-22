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

void create_dir_if_not_exists(const char *path)
{
    struct stat st;

    if (stat(path, &st) == -1) {
        if (mkdir(path, 0755) != 0) {
            perror("Erreur lors de la création du répertoire");
            exit(1);
        }
    }
}

void close_db(db_t *database)
{
    mdb_dbi_close(database->env, database->dbi);
    mdb_env_close(database->env);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <kv.h>





size_t hash(char *val, int capacity){

    size_t hash = 0x34524019dba13af;

    while(*val){
        hash ^= *val;
        hash = hash << 8;
        hash += *val;

        val++;
    }

    return hash % capacity;
}


// fn kv_put
// params:
// - db: a pointer to the db
// -key: a pointer to the key value
// -value: a pointer to the value itself
// retunrs: the indexx of the key, otherwise on
// error, returns -1, on not found returns -2

int kv_put(kv_t *db, char *key, char *value){
    if(!db || !key || !value) return -1;

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity -1; i++){

        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        // the key is alredy set, updating
        if(entry->key &&
        entry->key != (void*)TOMBSTONE &&
        !strcmp(entry->key,key)){
            char *newval = strdup(value);
            if(!newval) return -1;
            free(entry->value);
            entry->value = newval;
            return 0;
        }
        //land in a slot that is "empty"
        //null or tombstone
        if(!entry->key || 
        entry->key == (void*)TOMBSTONE){
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if(!newval || !newkey) {
                free(newval);
                free(newkey);
                return -1;
            }
            entry->value = newval;
            entry->key = newkey;
            db->count++;
            return 0;
        }
    }   

    // db is occupied
    return -2;
}


kv_t *kv_init(size_t capacity){
    if(capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if(table == NULL) return NULL;

    table->capacity = capacity;
    table->count    = 0;

    table->entries = calloc(sizeof(kv_entry_t),capacity);
    if (table->entries == NULL) return NULL;

    return table;
}


// fn kv_put
// params:
// - db: a pointer to the db
// -key: a pointer to the key value
// -value: a pointer to the value itself
// retunrs: the pointer to the key
// NULL If not found

char *kv_get(kv_t *db, char *key){
    if(!db || !key) return NULL;

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity -1; i++){
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        // is no key, therefore return nothing
        if(entry->key == NULL){
            return NULL;
        }

        //find an entry and the keys match
        if(entry->key && 
        entry->key != (void*)TOMBSTONE &&
        !strcmp(entry->key,key)){
            return entry->value;
        }
    }

    return NULL;
}


// fn kv_delete
// params:
// - db: a pointer to the db
// -key: a pointer to the key value
// -value: a pointer to the value itself
// retunrs: index of the deletion
// -1 if not found db or key

int kv_delete(kv_t *db, char *key){
    if(!db || !key) return -1;

    size_t idx = hash(key, db->capacity);

    for(int i = 0; i < db->capacity -1; i++){
        size_t real_idx = (idx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_idx];

        // is no key, therefore return nothing
        if(entry->key == NULL){
            return -1;
        }

        //find an entry and the keys match
        if(entry->key && 
        entry->key != TOMBSTONE &&
        !strcmp(entry->key,key)) {

            free(entry->key);
            free(entry->value);
            db->count--;
            entry->key = TOMBSTONE;
            entry->value = NULL;

            return real_idx;
        }
    }
    return -1;
}



// fn kv_free
// params:
//  - db    : a pointer to the db
//  returns : 0 on success, -1 on failure


void kv_free(kv_t *db){
    if(!db) return -1;

    for(int i = 0; i < db->capacity-1; i++){
        kv_entry_t *e = &db->entries[i];

        if(e->key && e->key != TOMBSTONE){
            free(e->key);
            free(e->value);
            e->key = NULL;
            e->value = NULL;
            db->count--;
        } 
    }

    free(db->entries);
    free(db);

    return 0;
}

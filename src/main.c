#include <stdio.h>
#include <kv.h>

int main(){
    kv_t *table = kv_init(1024);

    printf("%p\n", table);
    printf("%llu\n", table->capacity);

    printf("%d\n", kv_put(table, "hehe", "haha"));
    printf("%d\n", kv_put(table, "hehe", "hoho"));
    printf("%d\n", kv_put(table, "lala", "baba"));

    for(int i = 0; i < table->capacity; i++){
        if(table->entries[i].key){
            printf("[%d] %s: %s\n",
                i,
                table->entries[i].key,
                table->entries[i].value);
        }
    }

    char *val = kv_get(table,"hehe");
    char *val2 = kv_get(table,"lala");
    char *val3 = kv_get(table,"this_doesnt_exist");

    printf("%s %s %s\n",val, val2, val3);

    printf("Deleted index: %d \n",kv_delete(table,"hehe"));

    val = NULL;
    val = kv_get(table,"hehe");

    printf("%s %s %s\n",val, val2, val3);

    kv_free(table);
    table = NULL;

    val = kv_get(table,"hehe");
    printf("%p\n",val);

    return 0;
}
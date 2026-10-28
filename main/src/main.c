#include <stdio.h>
#include <kv.h>

int main(){
    kv_t *table = kv_init(3);

    printf("%p\n", table);
    printf("%llu\n", table->capacity);

    free(table->entries);
    free(table);

    return 0;
}
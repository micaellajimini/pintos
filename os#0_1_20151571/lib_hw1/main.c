#include "main.h"

int main(void){
    char input[256];
    char *token, *type, *ds_name, **argument, *ds;
    char *arg[8];
    int argnum, command_num, len, i;
    struct all_in_one all;
    while(1){
        argnum = 0;
        fgets(input,sizeof(input), stdin);
        len = strlen(input); input[len - 1] = '\0';
        token = strtok(input, " ");
        while( token != NULL ){
            arg[argnum++] = token;
            token = strtok(NULL, " ");
        }
        arg[argnum] = NULL;
        
        type = arg[0];
        command_num = 4;
        for (i = 0; i < 4; ++i ){
            if ( strcmp(type, command[i]) == 0 ){
                command_num = i;
                break;
            }
        }
        switch(command_num){
            case 0: // create
                ds = arg[1]; ds_name = arg[2];
                if ( arg[3] != NULL )
                    command_create(&all, ds, ds_name, atoi(arg[3]));
                else
                    command_create(&all, ds, ds_name, 0);
                break;
            case 1: //delete
                ds_name = arg[1];
                command_delete(&all, ds_name);
                break;
            case 2: //dumpdata
                ds_name = arg[1];
                command_dump(&all, ds_name);
                break;
            case 3: // quit
                exit(EXIT_SUCCESS);
                break;
            case 4:
                argument = &arg[1];
                command_argument(&all, type, argument); 
                break;
            default: // command
                break;
        }
    } 
    return 0;
}

void
print_list(struct list *l){
    struct list_elem *e;
    struct list_item *item;
    for (e = list_begin(l); e != list_end(l); e = list_next(e)){
        item = list_entry(e, struct list_item, elem);
        printf("%d ", item->data);
    }
    if ( !list_empty(l) ) puts("");
}

void
print_hash(struct hash *h){
    struct hash_iterator i;
    struct hash_item *hash_item;
    hash_first(&i, h);
    while (hash_next (&i)){
        hash_item = hash_entry(hash_cur(&i), struct hash_item, elem);
        printf("%d ", hash_item->data);
    }
    if ( !hash_empty(h) ) printf("\n");
}

void
print_bitmap(struct bitmap *b){
   size_t size = bitmap_size(b), i;
   for ( i = 0; i < size; ++i ){
       printf("%d", bitmap_test(b, i));
   }
   puts("");
}

void
command_create(struct all_in_one *all, char *ds, char *ds_name, int bit_cnt){
    int num;
    if ( strcmp(ds, "list") == 0 ){
        num = ds_name[4] - '0';
        list_command_create(&(all->lists[num]));
    }

    if ( strcmp(ds, "hashtable") == 0 ){
        num = ds_name[4] - '0';
        hash_command_create(&(all->hash_table[num]));
    }

    if ( strcmp(ds, "bitmap") == 0 ){
        num = ds_name[2] - '0';
        bitmap_command_create(&(all->bitmaps[num]), bit_cnt);
    }
}

void
command_delete(struct all_in_one *all, char *ds_name){
    int len = strlen(ds_name);
    int num = ds_name[len - 1] - '0';
    ds_name[len - 1] = '\0';
    if ( strcmp(ds_name, "list") == 0 ){
        delete_list(&(all->lists[num]));
    }

    if ( strcmp(ds_name, "hashtable") == 0 ){
        delete_hash(&(all->hash_table[num]));
    }

    if ( strcmp(ds_name, "bm") == 0 ){
        delete_bitmap(&(all->bitmaps[num]));
    }
}

void delete_list(struct list *list){
    struct list_elem *e;
    for ( e = list_begin(list); e != list_end(list); e = list_remove(e) );
    list_init(list);
}
void delete_hash(struct hash *h){
    hash_clear(h, hash_destructor);
}

void delete_bitmap(struct bitmap **b){
    bitmap_destroy(*b);
    *b = NULL;
}

void
command_argument(struct all_in_one *all, char *command, char **argument){
    if ( strstr(command, "list") != NULL ){
        list_commands(all->lists, command, argument); 
    }

    if ( strstr(command, "hash") != NULL ){
        hash_commands(all->hash_table, command, argument);
    }

    if ( strstr(command, "bitmap") != NULL ){
        bitmap_commands(all->bitmaps, command, argument);
    }
}

void command_dump(struct all_in_one *all, char *ds_name){
    int len = strlen(ds_name);
    int num = ds_name[len - 1] - '0';
    if ( strstr(ds_name, "list") != NULL ){
        print_list(&(all->lists[num]));
    }

    if ( strstr(ds_name, "hash") != NULL ){
        print_hash(&(all->hash_table[num]));
    }


    if ( strstr(ds_name, "bm") != NULL ){
        print_bitmap(all->bitmaps[num]);
    }
}

void
list_commands (struct list lists[10], char *command, char **argument){
    size_t command_size = sizeof (list_command) / sizeof (char *);
    size_t i;
    int command_num;
    for ( i = 0; i <  command_size; ++i){
        if ( strcmp(list_command[i], command ) == 0 ){
            command_num = i;
            break;
        }
    }
    switch(command_num){
        case 0:
            list_command_insert(lists, argument);
            break;
        case 1:
            list_command_splice(lists, argument);
            break;
        case 2:
            list_command_push_front(lists, argument);
            break;
        case 3:
            list_command_push_back(lists, argument);
            break;
        case 4:
            list_command_remove(lists, argument);
            break;
        case 5:
            list_command_pop_front(lists, argument);
            break;
        case 6:
            list_command_pop_back(lists, argument);
            break;
        case 7:
            list_command_front(lists, argument);
            break;
        case 8:
            list_command_back(lists, argument);
            break;
        case 9:
            list_command_size(lists, argument);
            break;
        case 10:
            list_command_empty(lists, argument);
            break;
        case 11:
            list_command_reverse(lists, argument);
            break;
        case 12:
            list_command_sort(lists, argument);
            break;
        case 13:
            list_command_insert_ordered(lists, argument);
            break;
        case 14:
            list_command_unique(lists, argument);
            break;
        case 15:
            list_command_max(lists, argument);
            break;
        case 16:
            list_command_min(lists, argument);
            break;
        case 17:
            list_command_swap(lists, argument);
            break;
        case 18:
            list_command_shuffle(lists, argument);
            break;
        default:
            break;
    }
}

bool
list_less(struct list_elem *list_elem1, struct list_elem *list_elem2){
    struct list_item *list_item1, *list_item2;
    list_item1 = list_entry(list_elem1, struct list_item, elem);
    list_item2 = list_entry(list_elem2, struct list_item, elem);
    return (list_item1->data) < (list_item2->data);
}

void
list_command_create(struct list *list){
    list_init(list);
}

void
list_command_insert (struct list lists[10], char **argument){
    char *name = argument[0];
    int pos = atoi(argument[1]), i;
    int data = atoi(argument[2]);
    int list_num = name[4] - '0';
    struct list *nlist = &lists[list_num];
    struct list_elem *e;
    struct list_item *new_list = malloc(sizeof(struct list_item));
    new_list->data = data;
    for ( i = 0, e = list_begin(nlist); i < pos && e != list_end(nlist); ++i, e = list_next(e) );
    list_insert(e, &(new_list->elem));
}

void
list_command_splice (struct list lists[10], char **argument) {
    char *name1, *name2;
    int pos, from, to, i; 
    int list1_num, list2_num;
    struct list *list1, *list2;
    struct list_elem *before, *first, *last;

    name1 = argument[0]; name2 = argument[2];
    pos = atoi(argument[1]);
    from = atoi(argument[3]); to = atoi(argument[4]);
    list1_num = name1[4] -'0'; list2_num = name2[4] - '0';
    list1 = &lists[list1_num]; list2 = &lists[list2_num];

    for ( i = 0, before = list_begin(list1);  i < pos && before != list_end(list1); ++i, before = list_next(before) );
    for ( i = 0, first = list_begin(list2);  i < from && first != list_end(list2); ++i, first = list_next(first) );
    for ( i = 0, last = list_begin(list2);  i < to && last != list_end(list2); ++i, last = list_next(last) );

    list_splice(before, first, last);
}

void
list_command_push_front (struct list lists[10], char **argument){
    char *name;
    int data, list_num;
    struct list *nlist;
    struct list_item *new_list = malloc( sizeof(struct list_item) );

    name = argument[0];
    data = atoi(argument[1]);
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    new_list->data = data;

    list_push_front(nlist, &(new_list->elem));
}

void
list_command_push_back (struct list lists[10], char **argument){
    char *name;
    int data, list_num;
    struct list *nlist;
    struct list_item *new_list = malloc( sizeof(struct list_item) );

    name = argument[0];
    data = atoi(argument[1]);
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    new_list->data = data;

    list_push_back(nlist, &(new_list->elem));
}
void
list_command_remove (struct list lists[10], char **argument){
    char *name;
    int pos, list_num, i;
    struct list *nlist;
    struct list_elem *e;
    name = argument[0];
    pos = atoi(argument[1]);
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    for ( i = 0, e = list_begin(nlist); i < pos && e != list_end(nlist); ++i, e = list_next(e));
    list_remove(e);
}

void
list_command_pop_front (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    list_pop_front(nlist);
}

void
list_command_pop_back (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    list_pop_back(nlist);
}

void
list_command_front (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';
    nlist = &lists[list_num];
    e = list_front(nlist);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_back (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    e = list_back(nlist);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_size (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    size_t size;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    size = list_size(nlist);
    printf("%u\n", size);
}

void
list_command_empty (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    bool is_empty;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    is_empty = list_empty(nlist);
    if ( is_empty ) puts("true");
    else puts("false");
}

void
list_command_reverse (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    list_reverse(nlist);
}

void
list_command_sort (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    name = argument[0];
    list_num = name[4] - '0';

    nlist = &lists[list_num];
    list_sort(nlist, list_less, NULL);

}

void
list_command_insert_ordered(struct list lists[10], char **argument){
    char *name;
    int list_num, data;
    struct list *nlist;
    struct list_item *new_list = malloc( sizeof (struct list_item) );

    name = argument[0];
    list_num = name[4] - '0';

    data = atoi(argument[1]);
    new_list->data = data;

    nlist = &lists[list_num];

    list_insert_ordered(nlist, &(new_list->elem), list_less, NULL);
}

void
list_command_unique (struct list lists[10], char **argument){
    char *name1, *name2;
    int list_num1, list_num2;
    struct list *list1, *list2 = NULL;
    name1 = argument[0]; name2 = argument[1];
    list_num1 = name1[4] - '0';
    list1 = &lists[list_num1];
    if ( name2 != NULL ) {
        list_num2 = name2[4] - '0';
        list2 = &lists[list_num2];
    }
    list_unique(list1, list2, list_less, NULL);
    
}

void
list_command_max (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';
    nlist = &lists[list_num];
    e = list_max(nlist, list_less, NULL);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_min (struct list lists[10], char **argument){
    char *name;
    int list_num;
    struct list *nlist;
    struct list_elem *e;
    struct list_item *item;
    name = argument[0];
    list_num = name[4] - '0';
    nlist = &lists[list_num];
    e = list_min(nlist, list_less, NULL);
    item = list_entry(e, struct list_item, elem);
    printf("%d\n", item->data);
}

void
list_command_swap(struct list lists[10], char **argument){
    char *list_name;
    int list_num, first, second, i;
    struct list *nlist;
    struct list_elem *a, *b;
    list_name = argument[0];
    list_num = list_name[4] - '0';
    first = atoi(argument[1]); second = atoi(argument[2]);
    nlist = &lists[list_num];
    for ( i = 0, a = list_begin(nlist); i < first && a != list_end(nlist);
            ++i, a = list_next(a) );
    for ( i = 0, b = list_begin(nlist); i < second && b != list_end(nlist);
            ++i, b = list_next(b) );
    list_swap(a, b);
}

void
list_command_shuffle(struct list lists[10], char **argument){
   char *list_name;
   int list_num;
   struct list *l;
   list_name = argument[0];
   list_num = list_name[4] - '0';
   l = &lists[list_num] ;
   list_shuffle(l);
}

/* hash functions */
void
hash_commands (struct hash hash_table[10], char *command, char **argument){
    size_t command_size = sizeof (hash_command) / sizeof (char *);
    size_t i;
    int command_num;
    for ( i = 0; i < command_size; ++i){
        if ( strcmp(hash_command[i], command)  == 0){
            command_num = i;
            break;
        }
    }
    switch(command_num){
        case 0:
            hash_command_insert(hash_table, argument);
            break;
        case 1:
            hash_command_replace(hash_table, argument);
            break;
        case 2:
            hash_command_find(hash_table, argument);
            break;
        case 3:
            hash_command_delete(hash_table, argument);
            break;
        case 4:
            hash_command_clear(hash_table, argument);
            break;
        case 5:
            hash_command_size(hash_table, argument);
            break;
        case 6:
            hash_command_empty(hash_table, argument);
            break;
        case 7:
            hash_command_apply(hash_table, argument);
            break;
        default:
            break;
    }
}

unsigned
hash_func(struct hash_elem *e, void *aux){
    return hash_int((hash_entry(e, struct hash_item, elem))->data);
}

bool
hash_less(struct hash_elem *A, struct hash_elem *B, void *aux){
    struct hash_item *a, *b;
    a = hash_entry(A, struct hash_item, elem);
    b = hash_entry(B, struct hash_item, elem);
    return (a->data) < (b->data);
}

void
hash_destructor(struct hash_elem *e, void *aux){
    struct hash_item *hash_item;
    hash_item = hash_entry(e, struct hash_item, elem );
    free(hash_item);
}

void
hash_square(struct hash_elem *e, void *aux){
    struct hash_item *hash_item;
    int data;
    hash_item = hash_entry(e, struct hash_item, elem);
    data = hash_item->data;
    hash_item->data = data * data;
}


void
hash_triple(struct hash_elem *e, void *aux){
    struct hash_item *hash_item;
    int data;
    hash_item = hash_entry(e, struct hash_item, elem);
    data = hash_item->data;
    hash_item->data = data * data * data;
}

void
hash_command_create (struct hash *h){
    hash_init(h, hash_func, hash_less, NULL);
}

void
hash_command_insert (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int data = atoi(argument[1]);
    int hash_num = hash_name[4] - '0';
    struct hash_item *hash_item = malloc (sizeof (struct hash_item) );
    hash_item->data = data;
    hash_insert(&hash_table[hash_num], &(hash_item->elem)); 
}

void
hash_command_replace (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int item = atoi(argument[1]);
    int hash_num = hash_name[4] - '0';
    struct hash_item *hash_item = malloc (sizeof (struct hash_item));
    hash_item->data = item;
    hash_replace(&hash_table[hash_num], &(hash_item->elem));
}

void
hash_command_find (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int data = atoi(argument[1]);
    int hash_num = hash_name[4] - '0';
    struct hash_item hash_item;
    struct hash_elem *e;
    hash_item.data = data;
    e = hash_find(&hash_table[hash_num], &(hash_item.elem));
    if ( e != NULL ) {
        printf("%d\n", (hash_entry(e, struct hash_item, elem))->data);
    }
}

void
hash_command_delete (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int data = atoi(argument[1]) ;
    int hash_num = hash_name[4] - '0';
    struct hash_item hash_item;
    struct hash_elem *e;
    hash_item.data = data;
    e = hash_find(&hash_table[hash_num], &(hash_item.elem));
    if ( e != NULL )
        hash_delete(&hash_table[hash_num], e);
}

void
hash_command_clear (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    hash_clear(&hash_table[hash_num], hash_destructor); 
}


void
hash_command_size (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    size_t size;
    size = hash_size(&hash_table[hash_num]);
    printf("%u\n", size);
}

void
hash_command_empty (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    bool is_empty;
    is_empty = hash_empty(&hash_table[hash_num]);
    if ( is_empty ) puts("true");
    else puts("false");
}

void
hash_command_apply (struct hash hash_table[10], char **argument){
    char *hash_name = argument[0];
    int hash_num = hash_name[4] - '0';
    char *hash_func = argument[1];
    if ( strcmp(hash_func, "square") == 0){
        hash_apply(&hash_table[hash_num], hash_square);
    }
    if ( strcmp(hash_func, "triple") == 0){
        hash_apply(&hash_table[hash_num], hash_triple);
    }
}

/* bitmap functions */
void
bitmap_commands (struct bitmap *bitmaps[10], char *command, char **argument){
    size_t command_size = sizeof (bitmap_command) / sizeof (char *);
    size_t i;
    int command_num;
    for ( i = 0; i < command_size; ++i){
        if ( strcmp(bitmap_command[i], command)  == 0){
            command_num = i;
            break;
        }
    }
    switch(command_num){
        case 0:
            bitmap_command_size(bitmaps, argument);
            break;
        case 1:
            bitmap_command_set(bitmaps, argument);
            break;
        case 2:
            bitmap_command_mark(bitmaps, argument);
            break;
        case 3:
            bitmap_command_reset(bitmaps, argument);
            break;
        case 4:
            bitmap_command_flip(bitmaps, argument);
            break;
        case 5:
            bitmap_command_test(bitmaps, argument);
            break;
        case 6:
            bitmap_command_set_all(bitmaps, argument);
            break;
        case 7:
            bitmap_command_set_multiple(bitmaps, argument);
            break;
        case 8:
            bitmap_command_count (bitmaps, argument);
            break;
        case 9:
            bitmap_command_contains (bitmaps, argument);
            break;
        case 10:
            bitmap_command_any (bitmaps, argument);
            break;
        case 11:
            bitmap_command_none (bitmaps, argument);
            break;
        case 12:
            bitmap_command_all (bitmaps, argument);
            break;
        case 13:
            bitmap_command_scan (bitmaps, argument);
            break;
        case 14:
            bitmap_command_scan_and_flip (bitmaps, argument);
            break;
        case 15:
            bitmap_command_dump (bitmaps, argument);
            break;
        case 16:
            bitmap_command_expand (bitmaps, argument);
        default:
            break;
    }
}

void
bitmap_command_create (struct bitmap **b, int bit_cnt){
    *b = bitmap_create(bit_cnt);
}

void
bitmap_command_size (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    size_t size;
    size = bitmap_size(bitmaps[bitmap_num]);
    printf("%u\n", size);
}

void 
bitmap_command_set (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    int bit_idx = atoi(argument[1]);
    char *bool_value = argument[2];
    bool value;
    value = ( strcmp(bool_value, "true") == 0 ) ? true : false;
    bitmap_set(bitmaps[bitmap_num], bit_idx, value);
}

void 
bitmap_command_mark (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bitmap_mark(b, bit_idx);
}

void 
bitmap_command_reset (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bitmap_reset(b, bit_idx); 
}

void 
bitmap_command_flip (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bitmap_flip(b, bit_idx);
}

void
bitmap_command_test (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int bit_idx = atoi(argument[1]);
    bool result;
    result = bitmap_test(b, bit_idx);
    if ( result ) printf("true\n");
    else printf("false\n");
}

void 
bitmap_command_set_all (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    bool value = ( strcmp(argument[1], "true") == 0 ) ? true : false;
    bitmap_set_all(b, value);
}


void 
bitmap_command_set_multiple (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    bitmap_set_multiple(b, start, cnt, value);
}

void
bitmap_command_count (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    size_t value_cnt;
    value_cnt = bitmap_count(b, start, cnt, value);
    printf("%u\n", value_cnt);
}

void
bitmap_command_contains (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    bool is_contain;
    is_contain = bitmap_contains(b, start, cnt, value);
    if ( is_contain ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_any (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool is_any_contain;
    is_any_contain = bitmap_any(b, start, cnt);
    if ( is_any_contain ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_none (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool is_none;
    is_none = bitmap_none(b, start, cnt);
    if ( is_none ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_all (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool is_all;
    is_all = bitmap_all(b, start, cnt);
    if ( is_all ) printf("true\n");
    else printf("false\n");
}

void
bitmap_command_scan (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    size_t bit_idx;
    bit_idx = bitmap_scan(b, start, cnt, value);
    printf("%u\n", bit_idx);
}

void
bitmap_command_scan_and_flip (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    size_t start = atoi(argument[1]), cnt = atoi(argument[2]);
    bool value = ( strcmp(argument[3], "true") == 0 ) ? true : false;
    size_t bit_idx;
    bit_idx = bitmap_scan_and_flip(b, start, cnt, value);
    printf("%u\n", bit_idx);
}

void
bitmap_command_dump (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    bitmap_dump(b); 
}

void
bitmap_command_expand (struct bitmap *bitmaps[10], char **argument){
    char *bitmap_name = argument[0];
    int bitmap_num = bitmap_name[2] - '0';
    struct bitmap *b = bitmaps[bitmap_num];
    int size = atoi(argument[1]);
    b = bitmap_expand(b, size);
    bitmaps[bitmap_num] = b;
}

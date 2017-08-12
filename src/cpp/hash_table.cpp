#include "hash_table.hpp"
#include <iostream>
#include <stdlib.h>
#include<bits/stdc++.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/* LIST functions

	* INSERT
	* DELETE

*/

/* Insert an element into a given list
	* If there is no head, insert element at head position
		* New node with the (key, value) pair pointing to nothing
		* Set head and tail of list to new node
		* Set next element to self

	* else, insert element at tail position
		* Set next of tail to new node with value
		* set tail to new node

	PARAM: l --> Pointer to the list
	        val --> Integer value of the node to be inserted
*/

node *cur, *it, *itx;
hash_table *n_table, *t;

void list_insert(list* l, int key_n, int value) {
    cur = new node; cur->key = key_n; cur->val = value; cur->next = NULL;
    it = l->head;
    if ( it == NULL ) {
        l->head = cur;
    } else {
        while ( it->next != NULL ) {
            it = it->next;
        }
        it->next = cur;
    }
}


/* Delete first occurence of an element with the given key from a given list
	* Find a node s.t the NEXT node has the desired value to delete
	* Set the "next" of this node to the next of the NEXT node
*/

void list_delete(list* l, int k) {
    it = l->head;
    if ( it->next == NULL && it->key == k ) {
        l->head = NULL;
    } else {
        while ( it->next != NULL && it->next->key != k ) {
            it = it->next;
        }
        if ( it->next->key == k ) {
            it->next = it->next->next;
        }
    }
}


node *list_contains(list *l, int k) {
    it = l->head;
    if ( it == NULL ) {
        return NULL;
    }
    while ( it->next != NULL && it->key != k ) {
        it = it->next;
    }
    if ( it->key == k ) {
        return it;
    } else {
        return NULL;
    }
}

/********************************************************************
 ********************************************************************
 ********************************************************************
 ********************************************************************/

/* HASH TABLE Functions

	* INSERT
	* DELETE
	* GET
	* CONTAINS
	* HASH

*/

/* Grow Table function
	* Creates a new hash table of twice the current size
	* Re-hashes all keys into table
	* Returns pointer to new table
*/
void grow_table(hash_table *td);
void shrink_table(hash_table *td);

/* Hash function
	* Function of KEY and SIZE that must return a valid index into the hash table
	* PARAMETERS: key --> Key for which to return the hash
				  s   --> Size of table
*/
int hash(int key, int s) {
    return key%s;
}

/* Whether or not the Hash table contains the given key
   PARAMETERS: t   --> Pointer to the hash table
               key --> Key to look for
*/
node *hash_contains(hash_table *t, int key) {
    int index = hash(key , t->current_size);
    return list_contains(&t->table[index] , key);
}

/* Deletes the first occurence of the given key from the hash table if it is present
	* PARAMETERS: t --> Pointer to the hash table
				  key --> Key of (key, value) pair to delete
*/
void hash_delete(hash_table *t, int key) {
    if ( t->nods+1 > t->current_size ) {
        grow_table(t);
    } else if ( t->nods <= 1/4*t->current_size && t->nods>10 ) {
        shrink_table(t);
    }
    int index = hash(key , t->current_size);
    t->nods--;
    return list_delete(&t->table[index] , key);
}

/* Inserts the given key value pair into the hash table
	* Overwrites previous value if present
	* PARAMETERS: t --> Pointer to the hash table
				  key --> Key of the (key, value) pair
				  value --> Value fo the (key, value) pair
*/


void hash_insert(hash_table* t, int key, int value) {
    if ( t->nods+1 > t->current_size ) {
        grow_table(t);
    } else if ( t->nods <= 1/4*t->current_size && t->nods>10 ) {
        shrink_table(t);
    }
    int index = hash(key , t->current_size);
    t->nods++;
    return list_insert(&t->table[index] , key , value);
}


/* Gets the value assosciated with the key in the hash table
	* Returns NULL if key not present
*/


int hash_get(hash_table* t, int key) {
    node *g = hash_contains(t , key);
    if ( g != NULL ) {
        return g->val;
    } else {
        return NULL;
    }
}


void shrink_table(hash_table *t) {
    n_table = new hash_table(t->current_size/2);
    for ( int i = 0; i < t->current_size; i++ ) {
        itx = t->table[i].head;
        if ( itx != NULL ) {
            while ( itx->next != NULL ) {
                hash_insert(n_table , itx->key , itx->val);
                itx = itx->next;
            }
            hash_insert(n_table , itx->key , itx->val);
        }
    }
    *t = *n_table;
}

void grow_table(hash_table *t) {
    n_table = new hash_table(t->current_size*2);
    for ( int i = 0; i < t->current_size; i++ ) {
        itx = t->table[i].head;
        if ( itx != NULL ) {
            while ( itx->next != NULL ) {
                hash_insert(n_table, itx->key, itx->val);
                itx = itx->next;
            }
            hash_insert(n_table , itx->key , itx->val);
        }
    }
    *t = *n_table;
}

void printResult(bool pass) {
    if ( pass )
        printf("%s %s %s %c", ANSI_COLOR_GREEN, "OK", ANSI_COLOR_RESET, '\n');
    else
        printf("%s %s %s %c",
        ANSI_COLOR_RED , "FAIL" , ANSI_COLOR_RESET , '\n');
}

void printline() {
    for ( int i = 0; i < 15; i++ ) {
        printf("-");
    }
    printf("\n");
}

void pprint(hash_table *t) {
    printf("\n\n");
    printline();
    printf("| %*s | %*s |\n", 4, "key", 4, "val");
    printline();

    for ( int j = 0; j < t->current_size; j++ ) {
        node *n = t->table[j].head;

       while ( n != NULL ) {
            printf("| %*d | %*d |  %d\n", 4, n->key, 4, n->val, j);
            printline();
            n = n->next;
       }
    }
}

void test(hash_table **table) {
    int passed = 0, failed = 0;
    t = *table;
    printf("TEST 1: INSERT\n");
    printf("RESULT: ");
    hash_insert(t, 5, 32);

    if ( hash_get(t, 5) == 32 )
        printResult(true), passed++;
    else
        printResult(false), failed++;
    printf("\n\n");


    printf("TEST 2: DELETE\n");
    printf("RESULT: ");
    hash_delete(t, 5);
    if ( hash_get(t , 5) == NULL )
        printResult(true), passed++;
    else
        printResult(false), failed++;
    printf("\n\n");


    printf("TEST 3: CONTAINS\n");
    printf("RESULT: ");
    hash_insert(t, 5, 32);
    if ( hash_contains(t, 5) != NULL )
        printResult(true), passed++;
    else
        printResult(false), failed++;
    printf("\n\n");


    printf("TEST 4: COLISSION\n");
    printf("RESULT: ");
    hash_insert(t, 15, 65);
    if ( hash_get(t, 15) == 65 )
        printResult(true), passed++;
    else
        printResult(false), failed++;
    printf("\n\n");


    printf("TEST 5: GROW TABLE\n");
    printf("RESULT: ");

    for ( int j = 0; j < 12; j++ ) {
        hash_insert(t, rand() % 50, rand() % 50);
    }

    if ( t->current_size == 20 )
        printResult(true), passed++;
    else
        printResult(false), failed++;
    printf("\n\n");

    if ( failed > 0 )
        printf("%s %s %s", ANSI_COLOR_RED,  "FAIL",  ANSI_COLOR_RESET);
    else
        printf("%s %s %s", ANSI_COLOR_GREEN , "PASS" , ANSI_COLOR_RESET);
}


int main() {
    t = new hash_table(10);
    test(&t);
    pprint(t);
    return 0;
}


/*
** Author: Stephen Sheldon 3/13/2019
**
** Hash Table implemented in C
**
** Time Complexity
** Access: O(1) - Assuming we don't have to search chains
** Access: O(n) - Worst case. Ex: We have lots of elements hashed on the same key (long chain)
** Insert: O(1) - This assume we insert at beginning of a potential chain
** Insert: O(n) - Worst case. Ex: We have a key with a long chain and due to
**				  chains be lexicographically sorted we somehow end up having
**                to insert the element at the end of the chain.
**
** Hash Table stores people
** People have first and last names, sometimes the same.
** People have a 5 digit unique number associated with them.
** Implement a hash table using chaining for collision avoidance.
** Chains should be sorted lexicographically (only by first name for right now).
**
** First line in file is # of people in DB.
** Next line is first name.
** Next Line is last name.
** Next line is unique ID.
** Repeat.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max length of a person's first or last name.
#define MAX_LEN 100
// Initial size of table will be 7, let's
// keep it small to test our collision resolution
// policy of chaining using a singly linked list.
#define INITIAL_LEN 7

struct person {
	char first_name[MAX_LEN];
	char last_name[MAX_LEN];
	int id;
	struct person* next;
};

struct hashtable
{
    struct person** store;
    int length; // Length of the array
    int num_elements;
};


struct hashtable* new_hashtable(); 
void insert(struct hashtable* h, struct person* p);
int hash_function(char first_name[], char last_name[]); 
void delete_hashtable(struct hashtable* h); 
void lookup(struct hashtable* h, char first_name[], char last_name[]);
char * to_lowercase(char s[]);

//void expand(struct hashtable* h);
//int remove(struct hashtable* h, struct person* p);

int main(void)
{
	int i, choice, num_entries;
	FILE *fp;
	char* token;
	char first_name[MAX_LEN];
	char last_name[MAX_LEN];
	
	// Create a new hashtable
	struct hashtable* my_hashtable = new_hashtable();
	
	// Open database file for reading
	fp = fopen("HashPeople.txt", "r");
	// How many people are we scanning in?
	fscanf(fp, "%d", &num_entries);
	for (i = 0; i < num_entries; i++)
	{
		struct person* new_person = malloc(sizeof(struct person));
		fscanf(fp, "%s", new_person->first_name);
		fscanf(fp, "%s", new_person->last_name);
		fscanf(fp, "%d", &new_person->id);
		new_person->next = NULL;
		insert(my_hashtable, new_person);
	}
	
	
	if (fp == NULL)
	{
		printf("Sorry, we could not open the database file!\n");
		return 0;
	}
	
	// Close file after reading
	fclose(fp);
	
	
	printf("Welcome to the HashPeople Database!\n");
	printf("We have all sorts of people hashed in this here database\n");
	printf("Maybe you want us to look and see if we have someone you know?\n\n\n");
	
	do {
		printf("1. Look up by first and last name\n");
		printf("0. Exit program\n");
		scanf("%d", &choice);
		
		if (choice == 1)
		{
			printf("Please enter the first and last name, separated by a space\n");
			scanf("%s %s", first_name, last_name);
			
			// Convert to all lowercase
			strcpy(first_name, to_lowercase(first_name));
			strcpy(last_name, to_lowercase(last_name));
			
			lookup(my_hashtable, first_name, last_name);
		}
		
	} while (choice != 0);
	
	delete_hashtable(my_hashtable);
	system("PAUSE");
	exit(0);
}

// Create a new hash table of size INITIAL_LEN
struct hashtable* new_hashtable() 
{
	struct hashtable* h;
	h = malloc(sizeof(struct hashtable));
	h->length = INITIAL_LEN;
	h->num_elements = 0;
	// Use calloc to initialize to zeros
	h->store = calloc(INITIAL_LEN, sizeof(struct person*));
	return h;
}

// Insert a new person into the hashtable
void insert(struct hashtable* h, struct person* p)
{
	int hash_index;
	struct person* current_node;
	
	h->num_elements++;
	
	// We need to grab the hash table index using
	// our hashing function.
	hash_index = hash_function(p->first_name, p->last_name);
	
	// Now we need to insert the person into that index.
	// First check if index is NULL, meaning nothing is there.
	// If there is nothing there then let's put the person there.
	if (h->store[hash_index] == NULL ||
		strcmp(h->store[hash_index]->first_name, p->first_name) > 0)
	{
		p->next = h->store[hash_index];
		h->store[hash_index] = p;
		return;
	}
	
	current_node = h->store[hash_index];
	
	// If next node is not null and the name we're inserting doesn't
	// come before the next node's name, then go to the next node	
	while (current_node->next != NULL && strcmp(current_node->next->first_name, p->first_name) < 0)
		current_node = current_node->next;
	
	p->next = current_node->next;
	current_node->next = p;	
}

// Hash function for generating index, utilizes ASCII sums of first + last names
int hash_function(char first_name[], char last_name[]) 
{
	char concat_name[MAX_LEN*2];
	int string_sum, hash_value;
	int i, j;
	
	// For now we will concat name to use for hash function
	strcpy(concat_name, first_name);
	strcat(concat_name, last_name);
	// Now let's convert the strings to lowercase
	strcpy(concat_name, to_lowercase(concat_name));

	// Now because this is a string we first need to process the text
	// to convert it to numbers. So let's parse each individual char,
	// summing all ASCII integers of the string.
	j = 0;
	string_sum = 0;
	while (concat_name[j] != '\0')
	{
		string_sum += (concat_name[j] - '0');
		j++;
	}
	
	// Mod the string sum by the length of our hash table
	hash_value = string_sum % INITIAL_LEN;
	
	return hash_value;
}

// Prints all people in the database matching the first and last name
void lookup(struct hashtable* h, char first_name[], char last_name[])
{
	int hash_index;
	int seen;
	struct person* current_node;
	char temp_first[MAX_LEN];
	char temp_last[MAX_LEN];
	
	// Grab hash index	
	hash_index = hash_function(first_name, last_name);

	current_node = h->store[hash_index];
	seen = 0;
	
	while (current_node != NULL) 
	{	
		if (strcmp(strcpy(temp_first, to_lowercase(current_node->first_name)), first_name) == 0 &&
			strcmp(strcpy(temp_last, to_lowercase(current_node->last_name)), last_name) == 0)
		{
			seen = 1;
			printf("\nFound name: %s %s  ", current_node->first_name, current_node->last_name);
			printf("Personal ID: %d\n", current_node->id);
		}
		current_node = current_node->next;
	}	
	
	if (seen == 0)
		printf("\nI'm sorry, I could not find %s %s in the database\n", first_name, last_name);
		
	printf("\n");
}

// Free all memory allocated to hash table and it's elements
void delete_hashtable(struct hashtable* h) 
{
	int i;
	struct person* temp;
	struct person* current_node;
	
	for (i = 0; i < h->length; i++)
	{
		current_node = h->store[i];	
		while (current_node != NULL) 
		{
			// Free the people!
			temp = current_node;
			current_node = current_node->next;
			free(temp);
		}
	}
	
	// Free the array of people
	free(h->store);
	// Free the hash table
	free(h);
 } 
 
 // Convert a string to all lowercase
 char * to_lowercase(char s[])
 {
 	int i;
 	// Convert to all lowercase
	for (i = 0; s[i]; i++)
		s[i] = tolower((unsigned char)s[i]);
	
	return s; 
 }
//void expand(struct hashtable* h); 
//int remove(struct hashtable* h, struct person* p);

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
** Delete: O(1) - Assuming we don't have huge chains to search
** Delete: O(n) - Worst case, we have to search a chain at the hash index.
**
** Notes: At the moment this hash table is going to hit worst case scenarios the majority of the time
**        due to the size of the hash table along with the number of inputs. The main purpose of
**        this hash table is to demonstrate chaining versus optimizing the time complexity. 
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
#include <ctype.h>

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
int lookup(struct hashtable* h, char first_name[], char last_name[]);
int remove_person(struct hashtable* h, char first_name[], char last_name[], int pid);
void print_table(struct hashtable* h);
char * to_lowercase(char s[]);

int main(void)
{
	int i, choice, num_entries, pid, seen;
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
		seen = 0;
		
		printf("1. Look up by first and last name\n");
		printf("2. Remove a person by first and last name\n");
		printf("3. Print hash table\n");
		printf("0. Exit program\n");
		scanf("%d", &choice);
		
		if (choice == 1 || choice == 2)
		{
			printf("Please enter the first and last name, separated by a space\n");
			scanf("%s %s", first_name, last_name);
			
			// Convert to all lowercase
			strcpy(first_name, to_lowercase(first_name));
			strcpy(last_name, to_lowercase(last_name));
			
			seen = lookup(my_hashtable, first_name, last_name);
		}
		if (choice == 2 && seen != 0)
		{
			printf("Now that we found the person you wish to remove, please enter their ID to delete them\n");
			scanf("%d", &pid);
			
			if (remove_person(my_hashtable, first_name, last_name, pid))
				printf("Successfully removed %s %s ID: %d\n", first_name, last_name, pid);
			else
				printf("There was an error removing %s %s, please make sure you entered the correct ID\n", first_name, last_name);
		}
		else if (choice == 3)
			print_table(my_hashtable);
		
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
int lookup(struct hashtable* h, char first_name[], char last_name[])
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
	printf("\n");
	while (current_node != NULL) 
	{	
		if (strcmp(strcpy(temp_first, to_lowercase(current_node->first_name)), first_name) == 0 &&
			strcmp(strcpy(temp_last, to_lowercase(current_node->last_name)), last_name) == 0)
		{
			seen = 1;
			printf("Found name: %s %s  ", current_node->first_name, current_node->last_name);
			printf("Personal ID: %d\n", current_node->id);
		}
		current_node = current_node->next;
	}	
	
	if (seen == 0)
	{
		printf("I'm sorry, I could not find %s %s in the database\n\n", first_name, last_name);
		return 0;
	}
	printf("\n");
	return 1;
}

int remove_person(struct hashtable* h, char first_name[], char last_name[], int pid)
{
	// if successfully removed return 1
	int hash_index;
	struct person* current_node;
	struct person* temp;
	char temp_first[MAX_LEN];
	char temp_last[MAX_LEN];
	
	// Grab hash index
	hash_index = hash_function(first_name, last_name);

	// Case: Head is null 
	if (h->store[hash_index] == NULL)
		return 0;
	
	// Case: Head of chain is the person we are looking for, normally we could just check
	//       if the ID is the one we're looking for, but for the sake of debugging to make
	//       sure our input file doesn't have duplicate ID's, let's check the first and last name.
	if (strcmp(strcpy(temp_first, to_lowercase(h->store[hash_index]->first_name)), first_name) == 0 &&
		strcmp(strcpy(temp_last, to_lowercase(h->store[hash_index]->last_name)), last_name) == 0 &&
		pid == h->store[hash_index]->id)
	{
		temp = h->store[hash_index];
		if (h->store[hash_index]->next == NULL)
		
			h->store[hash_index] = NULL;
		else 
		{ 	
			h->store[hash_index] = h->store[hash_index]->next;
		}
		
		free(temp);
			
		return 1;
	}
	
	// Set current node to point at beginning of potential chain at hash index
	current_node = h->store[hash_index];
	
	// Because the node is sorted lexicographically by the first name, we can stop if
	// the first name of our current node comes before (lexicographically) than the name we're
	// looking for.
	while (current_node->next != NULL &&
		   strcmp(strcpy(temp_first, to_lowercase(current_node->next->first_name)), first_name) < 0)
	{
		current_node = current_node->next;
	}
	
	// Case: Either the next node is NULL OR the current node COULD be the person
	//       we are looking for. At this point we really only need to check the id 
	//       because that's what we're basing our deletes off of.
	if (current_node->next == NULL || current_node->next->id != pid)
		return 0;
		
	// Case: We've found the matching person!
	temp = current_node->next;
	current_node->next = current_node->next->next;
	free(temp);
	return 1;	
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
 
 void print_table(struct hashtable* h)
 {
	int i;
	struct person* current_node;
	
	for (i = 0; i < h->length; i++)
	{
		printf("Hash index %d ", i);
		current_node = h->store[i];	
		
		// Case: Head at index is null, continue to next hash index
		if (current_node == NULL)
		{
			printf("there is no one here!\n\n");
			continue;
		}
		
		printf("we have:\n");
		while (current_node != NULL) 
		{
			printf("%s %s ID: %d\n", current_node->first_name, current_node->last_name, current_node->id);
			current_node = current_node->next;
		}
		printf("\n");
	}
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



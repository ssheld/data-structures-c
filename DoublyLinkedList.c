/*
** Author: Stephen Sheldon 3/10/2019
** A simple doubly linked list that stores names in SORTED lexicographic order
**
** Time Complexity
** Insert: O(n) (Because array is sorted we need to traverse list to find the correct position)
** Delete: O(n) (This is assuming we are deleting a name from the list, we don't know where it
**				 is thus we need to traverse the list till we find it
** Search: O(n) (See above, because search is O(n) is why insert is O(n) for insert and delete)
**
** Benefit over a Singly Linked List is that given a pointer N, we can follow links in either 
** direction (for example my print functions). We can also insert or delete a node either before
** only given a pointer to node N.
**
** In theory if we had a pointer to the exact node we needed to delete this we make the time
** complexity for deletion O(1) thus making this more ideal than an array. 
** Also, if the list was unsorted our time complexity would speed up to O(1) because
** we would just insert the new node at the beginning of the list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

struct list {
	struct node* head;
};
struct node {
	char name[MAX_LENGTH];
	struct node* next;
	struct node* prev;
};

struct list* create_list(void);
void insert(struct list* linked_list, char name[]);
void print_list(struct list* linked_list);
void print_list_reverse(struct list* linked_list); // FOR DEBUGGING 
int delete_node(struct list* linked_list, char name[]);
void delete_list(struct list* linked_list);

// Create the new linked list.
// Allocate the memory for it.
// Initialize the head to NULL.
struct list* create_list(void)
{
	struct list* new_list;	
	new_list = malloc(sizeof(struct list));
	new_list->head = NULL;
	return new_list;
}

int delete_node(struct list* linked_list, char name[])
{
	struct node* current_node;
	struct node* temp;
	
	// Case: List is empty
	if (linked_list->head == NULL)
			return 0;
	
	// Case: First node in the list is equal to the name
	//       that we wish to delete.
	if (strcmp(linked_list->head->name, name) == 0)
	{
		temp = linked_list->head;
		
		if (linked_list->head->next != NULL)
			linked_list->head->next->prev = NULL;
		
		linked_list->head = linked_list->head->next;
		free(temp);
		return 1;
	}

	current_node = linked_list->head;

	while (current_node != NULL)
	{
		if (strcmp(current_node->name, name) == 0)
		{
			temp = current_node;
			
			if (current_node->next != NULL)
			{
				current_node->next->prev = current_node->prev;
				current_node->prev->next = current_node->next;
			}
			else
				current_node->prev->next = NULL;
			
			free(temp);
			return 1;
		}
		current_node= current_node->next;
	}
	return 0;
}

// Insert node into linked list in lexicographic order
void insert(struct list* linked_list, char name[])
{
	struct node* new_node;
	new_node = malloc(sizeof(struct node));
	strcpy(new_node->name, name);
	new_node->next = NULL;
	new_node->prev = NULL;

	// Base Case: List is empty
	if (linked_list->head == NULL)
	{
		linked_list->head = new_node;
		return;
	}
	
	struct node* current_node;
	current_node = linked_list->head;
	
	// Case: Name comes before first name in list
	if (strcmp(linked_list->head->name, name) > 0)
	{
		new_node->next = current_node;
		linked_list->head = new_node;
		current_node->prev = new_node;
		return; 
	}
	
	// Case: Name we wish to insert comes before the current node OR
	//       if the current node's next node is null then we've reached
	//       the end of the list
	while (current_node != NULL)
	{	
		// Case: Name comes before current node's name
		if (strcmp(current_node->name, name) > 0)
		{
			new_node->next = current_node;
			new_node->prev = current_node->prev;
			current_node->prev->next = new_node;
			current_node->prev = new_node;
			return;
		}
		// Case: We've reached the end of the list
		else if (current_node->next == NULL)
		{
			current_node->next = new_node;
			new_node->prev = current_node;
			return;
		}
		current_node = current_node->next;
	}
}

// Delete the entire list
void delete_list(struct list* linked_list)
{
	struct node* temp;
	
	// Cycle through and free all nodes until head is null
	while (linked_list->head != NULL)
	{
		temp = linked_list->head;
		linked_list->head = linked_list->head->next;
		free(temp); 
	}
	
	// Finally free the linked_list
	free(linked_list);
}

void print_list(struct list* linked_list)
{
	struct node* current_node;
	
	printf("\n");
	
	// Base Case: List is empty
	if (linked_list->head == NULL)
		printf("There is nothing currently in the list!\n");
		
	current_node = linked_list->head;
	while (current_node != NULL)
	
	{
		printf("%s ", current_node->name);
		current_node = current_node->next;
	}
	printf("\n\n");
}

// Use this for debugging struct prev pointers after inserts and deletes
void print_list_reverse(struct list* linked_list)
{
	struct node* current_node;
	
	printf("\n");
	
	// Base Case: List is empty
	if (linked_list->head == NULL)
		printf("There is nothing currently in the list!\n");

	current_node = linked_list->head;
		
	// Let's move to the end of the list
	while (current_node->next != NULL)
		current_node = current_node->next;
	
	// Then let's backtrack and print
	while (current_node != NULL)
	{
		printf("%s ", current_node->name);
		current_node = current_node->prev;
	}
	printf("\n\n");
}

int main() 
{
	int choice;
	char name[MAX_LENGTH];
	
	struct list* linked_list;
	linked_list = create_list();
	
	printf("Welcome to the names database!\n");
	printf("We're happy to store all the names you like.\n");
	
	do {
		printf("What would you like to do?\n");
		printf("1. Add a name to the list.\n");
		printf("2. Print the list\n");
		printf("3. Delete name from the list\n");
		printf("4. DEBUG: Print list in reverse order\n");
		printf("0. Exit the program\n");
		scanf("%d", &choice);
		
		if (choice == 1) 
		{
			printf("Please enter the name you wish to add to the list\n");
			scanf("%s", name);
			insert(linked_list, name);
		}	
		else if (choice == 2)
			print_list(linked_list);
		else if (choice == 3)
		{
			printf("Please enter the name you wish to delete\n");
			scanf("%s", name);
			if (delete_node(linked_list, name))
				printf("Succesfully deleted %s from the list!\n", name);
			else
				printf("Sorry I could not find %s in the list\n", name);
		}
		else if (choice == 4)
			print_list_reverse(linked_list);
	} while (choice != 0);
	
	delete_list(linked_list);
	
	system("PAUSE");
	
	exit(0);
}

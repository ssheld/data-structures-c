#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
** Author: Stephen Sheldon 3/7/2019
** A simple singly linked list that stores names in SORTED lexicographic order
**
** Time Complexity
** Insert: O(n) (Because array is sorted we need to traverse list to find the correct position)
** Delete: O(n) (This is assuming we are deleting a name from the list, we don't know where it
**				 is thus we need to traverse the list till we find it
** Search: O(n) (See above, because search is O(n) is why insert is O(n) for insert and delete)
*/
#define MAX_LENGTH 100

struct list {
	struct node* head;
};
struct node {
	char name[MAX_LENGTH];
	struct node* next;
};

struct list* create_list(void);
void insert(struct list* linked_list, char name[]);
void print_list(struct list* linked_list);
int delete_node(struct list* linked_list, char name[]);
void delete_list(struct list* linked_list);


// Create a new empty List
struct list* create_list(void)
{
	struct list* new_list;
	// Allocate memory for head pointer
	new_list = malloc(sizeof(struct list));
	// Set the head to null as no items are currently in the list
	new_list->head = NULL;
	
	return new_list;
}

void delete_list(struct list* linked_list)
{
	struct node* current_node;
	
	while (linked_list->head != NULL)
	{
		current_node = linked_list->head;
		// change the head node's pointer to node
		// after the first node
		linked_list->head = linked_list->head->next;
		// free the current node;
		free(current_node);
	}
	// Finally free up the memory allocated to the list	
	free(linked_list);
}

int delete_node(struct list* linked_list, char name[])
{
	struct node* current_node;
	struct node* temp;
	
	current_node = linked_list->head;
	if (linked_list->head == NULL)
		return 0;
		
	if (strcmp(linked_list->head->name, name) == 0)
	{
		temp = linked_list->head;
		linked_list->head = linked_list->head->next;
		free(temp);
		return 1;
	}
		
	current_node = linked_list->head;
	
	while (current_node->next != NULL && strcmp(current_node->next->name, name) < 0)
		current_node = current_node->next;
	
	if (current_node->next == NULL || strcmp(current_node->next->name, name) != 0)
		return 0;
	
	// We've found the matching node
	temp = current_node->next;
	current_node->next = current_node->next->next;
	free(temp);
	return 1;
}


// Insert a name into the List
void insert(struct list* linked_list, char name[])
{
	struct node* new_node;
	struct node* current_node;
	
	// Initialize memory for new node
	new_node = malloc(sizeof(struct node));
	
	// Copy the name into the node
	strcpy(new_node->name, name);

	// Check if head is null or if the new node's name 
	// comes before the first node's name
	if (linked_list->head == NULL ||
			strcmp(linked_list->head->name, name) > 0)
	{
		new_node->next = linked_list->head;
		linked_list->head = new_node;
		return;
	}

	current_node = linked_list->head;
	
	// If next node is not null and the name we're inserting doesn't
	// come before the next node's name, then go to the next node	
	while (current_node->next != NULL && strcmp(current_node->next->name, name) < 0)
		current_node = current_node->next;
	
	new_node->next = current_node->next;
	current_node->next = new_node;
}

void print_list(struct list* linked_list)
{
	struct node* current_node;
	current_node = linked_list->head;
			
	printf("\n");
	
	// Print nodes till we hit null
	while(current_node != NULL)
	{
		printf("%s ", current_node->name);
		current_node = current_node->next;
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
	} while (choice != 0);
	
	delete_list(linked_list);
	
	system("PAUSE");
	
	exit(0);
}

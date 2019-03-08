/*
** Author: Stephen Sheldon 3/7/2019
**
** A simple stack
**
** Time Complexity
** Push:   O(1) (Worst case)
** Pop:    O(1) (Worst case)
** Search: O(n) (Because we have to potentially pop the entire stack of n values during search)
**
** Note: This stack is meant to only store numbers greater than or to 0
*/

#include <stdio.h>

struct node {
	int value;
	struct node* next;
};

struct stack {
	struct node* top;
};

struct stack* create_stack(void);
void delete_stack(struct stack* s);
void push(struct stack* s, int value);
int pop(struct stack* s);
int is_empty(struct stack* s);
void print_stack(struct stack* s); 

// For debugging purposes
void print_stack(struct stack* s)
{
	struct node* current_node;
	
	current_node = s->top;
	printf("\n");
	while (current_node != NULL)
	{
		printf("%d\n", current_node->value);
		current_node = current_node->next;
	}
	printf("\n");
}

// Delete the stack
void delete_stack(struct stack* s)
{
	struct node* temp;
	
	temp = s->top;
	
	while (s->top != NULL)
	{
		temp = s->top;
		s->top = s->top->next;
		free(temp);
	}
	
	free(s);
}

// Create initial stack
struct stack* create_stack() 
{
	struct stack* new_stack;
	new_stack = malloc(sizeof(struct node));
	new_stack->top = NULL;
	return new_stack;
}

// Return 1 if stack is empty or 0 if stack is not empty
int is_empty(struct stack* s)
{
	if (s->top == NULL)
		return 1;
	else
		return 0;
}

// Push new value on top of stack
void push(struct stack* s, int value)
{
	struct node* new_node;
	
	new_node = malloc(sizeof(struct node));
	new_node->value = value;
	new_node->next = s->top;
	s->top = new_node;
}

// Pop a value from the top of the stack
int pop(struct stack* s)
{
	int value;
	struct node* temp;
	
	if (s->top == NULL)
		return -1;
		
	value = s->top->value;
	temp = s->top;
	s->top = s->top->next;
	free(temp);
	
	return value;
}

int main()
{
	int choice, value;
	struct stack* my_stack = create_stack();
	
	printf("Hello there! Welcome to stackify!\n");
	printf("We pride ourselves in stacking names, so tell us, what would you like to do...\n\n");
	
	do {
		
		printf("1. Push a value onto the stack.\n");
		printf("2. Pop the top value from the stack.\n");
		printf("3. Print the current stack\n");
		printf("4. Check if stack is empty\n");
		printf("0. Exit the program\n");
		scanf("%d", &choice);
		
		if (choice == 1)
		{
			printf("What value would you like to put on the stack?\n");
			scanf("%d", &value);
			
			push(my_stack, value);
		}
		else if (choice == 2)
		{
			value = pop(my_stack);
			
			printf("Just popped %d from the stack\n", value);
		}
		else if (choice == 3)
			print_stack(my_stack);
		else if (choice == 4)
		{
			if (is_empty(my_stack) < 0)
				printf("The stack is empty!\n");
			else
				printf("There exist an item(s) on the stack\n");
		}
	} while(choice != 0);
	
	printf("Goodbyte!\n");
	// Free up stack memory
	free(my_stack);
	exit(0);
}

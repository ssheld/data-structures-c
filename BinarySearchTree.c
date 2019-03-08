#include <stdio.h>
#include <stdlib.h>
/*
** Author: Stephen Sheldon 3/8/2019
**
** A binary search tree
**
** Time Complexity
** Insert    O(h) in general where h is height of tree or O(n) (Worst case) 
** Delete    O(h) in gneneral or O(n) (Worst case)
** Traverse: O(n) Depth first traversal
** Search:   O(h) in general or O(n) (Worst case)
**
*/

struct node {
	int data;
	struct node* left;
	struct node* right;
};

int delete_node(struct node** root, int value);
struct node* remove_largest_node(struct node** root);
void insert(struct node** root, int value);
int lookup(struct node* root, int value);
void free_tree(struct node* root);
void in_order(struct node* root);
void pre_order(struct node* root);
void post_order(struct node* root);

// Insert a value into the tree
void insert(struct node** root, int value)
{
	struct node* new_node;
	
	// Base case: we've hit the bottom of the tree so insert node
	if (*root == NULL)
	{
		new_node = malloc(sizeof(struct node));
		new_node->data = value;
		new_node->left = new_node->right = NULL;
		*root = new_node;
		return;
	}
	
	// Go left if the root is greater or equal to value to insert
	if (value < (*root)->data)
		insert(&((*root)->left), value);
	else
		insert(&((*root)->right), value);
}

// Delete a specific node in the tree by value
int delete_node(struct node** root, int value)
{
	struct node* temp;
	
	// Base case: Node could not be found
	if (*root == NULL)
		return 0;
		
	// Base case: Found the node
	if ((*root)->data == value)
	{
		temp = *root;
		
		// Case: Node has no children
		if ((*root)->left == NULL && (*root)->right == NULL)
			*root = NULL;
		// Case: Node with only right child
		else if ((*root)->left == NULL && (*root)->right != NULL)
			*root = (*root)->right;
		// Case: Node with only left child
		else if ((*root)->left != NULL && (*root)->right == NULL)
			*root = (*root)->left;
		// Case: Node with two children
		else
		{
			*root = remove_largest_node(&(*root)->left);
			(*root)->left = temp->left;
			(*root)->right = temp->right;
		}
		
		free(temp);
		return 1;
	}
	
	// If we haven't found the value then we need
	// to keep recursively cycling through tree
	if ((*root)->data > value)
		delete_node(&((*root)->left), value);
	else
		delete_node(&((*root)->right), value);
}

// Remove the largest node in the tree
struct node* remove_largest_node(struct node** root)
{
	struct node* temp;
	
	// Base case: Found the largest node
	if ((*root)->right == NULL)
	{
		temp = *root;
		*root = (*root)->left;
		return temp;
	}
	
	// Otherwise recursive case
	return remove_largest_node(&(*root)->right);
}

// Look up a specific value in the tree
int lookup(struct node* root, int value)
{
	if (root == NULL)
		return 0;
		
	if (root->data < value)
		lookup(root->left, value);
	else if (root->data > value)
		lookup(root->right, value);
	else if (root->data == value)
		return 1;
}

// Post-order traversal to free memory
void free_tree(struct node* root)
{	
	if (root == NULL)
		return;
	
	free_tree(root->right);
	free(root);
	free_tree(root->left);	
}

// In-order traversal
void in_order(struct node* root)
{
	if (root == NULL)
		return;
	
	in_order(root->left);
	printf("%d ", root->data);
	in_order(root->right);
}

// Pre-order traversal
void pre_order(struct node* root)
{
	if (root == NULL)
		return;
	
	printf("%d ", root->data);
	pre_order(root->left);
	pre_order(root->right);
}

// Post-order traversal
void post_order(struct node* root)
{
	if (root == NULL)
		return;
		
	post_order(root->right);
	printf("%d ", root->data);
	post_order(root->left);
}


int	main(void)
{
    struct node* root = NULL;
    int choice, value;
    
    do
    {
        printf("Make a choice:\n");
        printf("1. Insert\n");
        printf("2. Lookup\n");
        printf("3. Delete\n");
        printf("4. Print all elements\n");
        printf("0. Quit\n");
        scanf("%d", &choice);        
        if(choice == 1)
        {
            printf("What value do you want to insert?\n");
            scanf("%d", &value);
            insert(&root, value);
        }
        else if(choice == 2)
        {
            printf("What value do you want to lookup?\n");
            scanf("%d", &value);
            if(lookup(root, value))
            {
                printf("Found it\n");   
            }   
            else
            {
                printf("Didn't find it\n");   
            }
        }
        else if(choice == 3)
        {
            printf("What value do you want to delete?\n");
            scanf("%d", &value);
            if(delete_node(&root, value))
            {
                printf("DELETED\n");
            } 
            else
            {
                printf("That value doesn't exist!\n");   
            }
        }
        else if(choice == 4)
        {
            printf("In-order: ");
            in_order(root);
            printf("\n");
            
            printf("Pre-order: ");
            pre_order(root);
            printf("\n");
            
            printf("Post-order: ");
            post_order(root);
            printf("\n");
        }
    }while(choice != 0);    
    
    free_tree(root);
	
	system("PAUSE");
	exit(0);
}

/*
* To call wrappers, define CS550_RENAME with
*    -DCS550_RENAME
*
* To run this use:
*
*     ./a.out check iters n_blocks max_size
*
* The arguments are:
*
*     check
*          Either 0 or 1. 0 means don't check for overwriting. Use 0 when testing performance.
*
*      iters
*          Number of iterations to do.
*
*      n_blocks
*          Maximum possible number of blocks to be allocated at any one time.
*
*      max_size
*          Maximum size of a block.
*
*

//#ifdef DCS550_RENAME
//#define malloc cs550_malloc
//#define free cs550_free
//#define cal--loc cs550_calloc
//#define realloc cs550_realloc
//#define sbrk cs550_sbrk
//#endif*/
//#include "utility.h"
//#include <stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>

// TODO: align


struct data
{
	int size;
	int *adrs;
};
typedef struct data data_val;
struct node
{
	struct data value;
	struct node *parent;
	struct node *left_child;
	struct node *right_child;
};
typedef struct node* node_ptr;
node_ptr sroot, aroot;
int max = 0;//least;
void insertNode_size(node_ptr curr, data_val inputvalue);
void insert_size_bst(int *ptr, int val);
void insertNode_adrs(node_ptr curr, data_val inputvalue);
void insert_adrs_bst(int *ptr, int val);
//void *memset(void *str, int c, size_t n);
//void *memcpy(void *str1, const void *str2, size_t n)
node_ptr create_node()
{
	node_ptr curr = sbrk(sizeof(struct node));//(struct node*)malloc(sizeof(struct node));
	curr->left_child = NULL;
	curr->right_child = NULL;
	curr->parent = NULL;
	return curr;
}
node_ptr search_lmrst(node_ptr temp)
{
	if (temp->left_child == NULL)
	{
		return temp;
	}
	else
	{
		temp = search_lmrst(temp->left_child);
		return temp;
	}
}
void* delete_node(node_ptr curr)
{
	node_ptr temp, temp2;
	if (curr->left_child == NULL && curr->right_child == NULL && curr->parent == NULL)
	{
				sroot=NULL;
				aroot=NULL;
		return curr;
	}
	else if (curr->left_child == NULL && curr->right_child == NULL && curr->parent != NULL)
	{
		temp = curr->parent;
		if (temp->left_child == curr)
		{
			curr->parent = NULL;
			temp->left_child = NULL;
		}
		else if (temp->right_child == curr)
		{
			curr->parent = NULL;
			temp->right_child = NULL;

		}
		return curr;
	}
	else if (curr->left_child != NULL && curr->right_child == NULL && curr->parent != NULL)
	{
		temp = curr->parent;
		if (temp->left_child == curr)
		{
			curr->parent = NULL;
			temp2 = curr->left_child;
			curr->left_child = NULL;
			temp->left_child = temp2;
			temp2->parent = temp;

		}
		else if (temp->right_child == curr)
		{
			curr->parent = NULL;
			temp2 = curr->left_child;
			curr->left_child = NULL;
			temp->right_child = temp2;
			temp2->parent = temp;

		}
		return curr;
	}
	else if (curr->left_child == NULL && curr->right_child != NULL && curr->parent != NULL)
	{
		temp = curr->parent;
		if (temp->left_child == curr)
		{
			curr->parent = NULL;
			temp2 = curr->right_child;
			curr->right_child = NULL;
			temp->left_child = temp2;
			temp2->parent = temp;

		}
		else if (temp->right_child == curr)
		{
			curr->parent = NULL;
			temp2 = curr->right_child;
			curr->right_child = NULL;
			temp->right_child = temp2;
			temp2->parent = temp;

		}
		return curr;
	}
	else
	{
		//search left most of right sub tree
		temp = curr->right_child;
		if (temp->left_child != NULL)
		{
			temp = search_lmrst(temp);
		}
		temp2 = curr->parent;
		if (temp2->left_child == curr)
		{
			curr->parent = NULL;
			temp2->left_child = temp;
		}
		else if (temp2->right_child == curr)
		{
			curr->parent = NULL;
			temp2->right_child = temp;

		}
		temp->left_child = curr->left_child;
		temp->right_child = curr->right_child;
		curr =(node_ptr) delete_node(temp);
		return curr;
	}
}
node_ptr search_big(node_ptr curr, data_val inputvalue)
{
	node_ptr temp;
	int new_size;
	int* meta_size, temp_adrs;
	node_ptr new_adrs;

	if (curr->value.size>inputvalue.size)
	{
		//split and delete_node.
		temp = (node_ptr)delete_node(curr);

		if (curr->value.size - inputvalue.size>16)
		{
			meta_size = (int*)temp - 2;
			meta_size[0] = inputvalue.size;
			meta_size[1] = 0;
			//temp_adrs = curr;
			new_adrs = (node_ptr)((int)curr + inputvalue.size + 8);
			new_size = curr->value.size - inputvalue.size - 8;
			insert_size_bst((int*)new_adrs, new_size);
		}
		return curr;

	}
	else
	{
		if (curr->parent)
		{
			return search_big(curr->parent, inputvalue);
		}
		else
		{
			return search_big(curr->right_child, inputvalue);
		}
	}
}
node_ptr searchNode(node_ptr curr, data_val inputvalue)
{
	if (curr == NULL)
	{
		//("\nNode not found\n");
		return NULL;
	}
	else
	{
		if (curr->value.size == inputvalue.size)
		{
			return curr;
		}
		else if (inputvalue.size > curr->value.size)
		{
			if (curr->right_child == NULL)
			{
				return curr;
			}
			else
			{
				curr = curr->right_child;
				curr = searchNode(curr, inputvalue);
				return curr;
			}
		}
		else if (inputvalue.size<curr->value.size)
		{
			if (curr->left_child == NULL)
			{
				//\nNode not Found\n");
				//return curr;
			}
			else
			{
				curr = curr->left_child;
				curr = searchNode(curr, inputvalue);
				//return curr;
			}
			return curr;
		}
		else
		{
			return NULL;
		}
	}
}
node_ptr search(node_ptr root, data_val inputvalue)
{
	node_ptr curr, temp;
	int new_size;
	int* meta_size;
	node_ptr new_adrs;

	if (sroot == NULL)
	{
		// Request space from OS

		return NULL;
	}
	else
	{
		curr = sroot;

		curr = searchNode(curr, inputvalue);
	}
	if (curr->value.size == inputvalue.size)
	{
		//found node
		//delete_node node
		curr = (node_ptr)delete_node(curr);
		return (node_ptr)curr->value.adrs;
	}
	else if (curr->value.size < inputvalue.size)
	{
		if (inputvalue.size>max)
		{
			//if not found request space from os.
			return NULL;
		}
		else
		{
			//search the next bigger node
			curr = search_big(curr->parent, inputvalue);
			return (node_ptr)curr->value.adrs;

		}
	}
	else if (curr->value.size > inputvalue.size)
	{
		//split and delete_node
		temp = (node_ptr)delete_node(curr);
		if (curr->value.size - inputvalue.size>16)
		{
			meta_size = (int*)temp - 2;
			meta_size[0] = inputvalue.size;
			meta_size[1] = 0;
			curr = temp;
			new_adrs = curr + inputvalue.size + 8;
			new_size = curr->value.size - inputvalue.size - 8;
			insert_size_bst((int*)new_adrs, new_size);
		}
		return (node_ptr)curr->value.adrs;
	}
	else
	{
		// Request space from OS
		return NULL;
	}
}
void insertNode_size(node_ptr curr, data_val inputvalue)
{
	node_ptr temp;
	int* meta_size;
	if ((inputvalue.size + (int)inputvalue.adrs + 8) == (int)curr)
	{
		curr->value.adrs = inputvalue.adrs;
		curr->value.size = curr->value.size + inputvalue.size + 8;
		meta_size = (int*)curr - 2;
		meta_size[0] = curr->value.size;
		meta_size[1] = 0;
		temp = curr + 2;
		return;
	}
	else if ((curr->value.size + curr->value.adrs + 8) == inputvalue.adrs)
	{
		curr->value.size = curr->value.size + inputvalue.size + 8;
		meta_size = (int*)curr - 2;
		meta_size[0] = curr->value.size;
		meta_size[1] = 0;
		temp = curr;
		return;
	}
	else
	{
		if (curr->left_child == NULL&&curr->right_child == NULL)
		{
			if (inputvalue.size >= curr->value.size)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->right_child = temp;
				return;
			}
			else if (inputvalue.size<curr->value.size)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->left_child = temp;
				return;
			}
		}
		else if (curr->left_child == NULL&&curr->right_child != NULL)
		{
			if (inputvalue.size<curr->value.size)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->left_child = temp;
				return;
			}
			else if (inputvalue.size >= curr->value.size)
			{
				insertNode_size(curr->right_child, inputvalue);
				return;
			}
		}
		else if (curr->left_child != NULL&&curr->right_child == NULL)
		{
			if (inputvalue.size<curr->value.size)
			{
				insertNode_size(curr->left_child, inputvalue);
				return;
			}
			else if (inputvalue.size >= curr->value.size)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->right_child = temp;
				return;
			}
		}
		else
		{
			if (inputvalue.size<curr->value.size)
			{
				insertNode_size(curr->left_child, inputvalue);
				return;
			}
			else if (inputvalue.size >= curr->value.size)
			{
				insertNode_size(curr->right_child, inputvalue);
				return;
			}


			return;
		}
	}
}

void insert_size_bst(int *ptr, int val)
{
	node_ptr curr;
	if (sroot == NULL)
	{
		sroot = create_node();
		sroot->value.size = val;
		sroot->value.adrs = ptr;
	}
	else
	{
		curr = sroot;
		data_val inputvalue;
		inputvalue.size = val;
		inputvalue.adrs = ptr;
		insertNode_size(curr, inputvalue);
	}
}
void insertNode_adrs(node_ptr curr, data_val inputvalue)
{
	node_ptr temp;
	int* meta_size;
	if ((inputvalue.size + (int)inputvalue.adrs + 8) == (int)curr)
	{
		curr->value.adrs = inputvalue.adrs;
		curr->value.size = curr->value.size + inputvalue.size + 8;
		meta_size = (int*)curr - 2;
		meta_size[0] = curr->value.size;
		meta_size[1] = 0;
		temp = curr;
		return;
	}
	else if ((curr->value.size + curr->value.adrs + 8) == inputvalue.adrs)
	{
		curr->value.size = curr->value.size + inputvalue.size + 8;
		meta_size = (int*)curr - 2;
		meta_size[0] = curr->value.size;
		meta_size[1] = 0;
		temp = curr;
		return;
	}
	else
	{
		if (curr->left_child == NULL&&curr->right_child == NULL)
		{
			if (inputvalue.adrs >= curr->value.adrs)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->right_child = temp;
				return;
			}
			else if (inputvalue.adrs<curr->value.adrs)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->left_child = temp;
				return;
			}
		}
		else if (curr->left_child == NULL&&curr->right_child != NULL)
		{
			if (inputvalue.adrs<curr->value.adrs)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->left_child = temp;
				return;
			}
			else if (inputvalue.adrs >= curr->value.adrs)
			{
				insertNode_adrs(curr->right_child, inputvalue);
				return;
			}
		}
		else if (curr->left_child != NULL&&curr->right_child == NULL)
		{
			if (inputvalue.adrs<curr->value.adrs)
			{
				insertNode_adrs(curr->left_child, inputvalue);
				return;
			}
			else if (inputvalue.adrs >= curr->value.adrs)
			{
				temp = create_node();
				temp->value.size = inputvalue.size;
				temp->value.adrs = inputvalue.adrs;
				temp->parent = curr;
				curr->right_child = temp;
				return;
			}
		}
		else
		{
			if (inputvalue.adrs<curr->value.adrs)
			{
				insertNode_adrs(curr->left_child, inputvalue);
				return;
			}
			else if (inputvalue.adrs >= curr->value.adrs)
			{
				insertNode_adrs(curr->right_child, inputvalue);
				return;
			}


			return;
		}
	}
}

void insert_adrs_bst(int *ptr, int val)
{
	node_ptr curr;
	if (aroot == NULL)
	{
		aroot = create_node();
		aroot->value.size = val;
		aroot->value.adrs = ptr;
	}
	else
	{
		curr = aroot;
		data_val inputvalue;
		inputvalue.size = val;
		inputvalue.adrs = ptr;
		insertNode_adrs(curr, inputvalue);
	}
}
// sbrk some extra space every time we need it.
// This does no bookkeeping and therefore has no ability to free, realloc, etc.
/*void *nofree_malloc(size_t size)
{
	void *p = sbrk(0);
	void *request = sbrk(size);
	if (request == (void*)-1) {
		return NULL; // sbrk failed
	}
	else {
		assert(p == request); // Not thread safe.
		return p;
	}
}*/



#define META_SIZE sizeof(struct node)

//void *root = NULL;

// Iterate through blocks until we find one that's large enough.
// TODO: split block up if it's larger than necessary
/*struct node *find_free_block(struct node **last, size_t size)
{
struct node *current = root;
while (current && !(current->free && current->size >= size)) {
*last = current;
current = current->right_child;
}
return current;
}*/

struct node *request_space(struct node* last, size_t size)
{
	struct node *block;
	//block = sbrk(0);
	int size_cal = size % 8;
	if (size_cal != 0)
	{
		size_cal = 8 - size_cal;
	}
	size_cal = size + size_cal + 8;
	void *request = sbrk(size_cal);//malloc(size_cal);//
	//assert((void*)block == request); // Not thread safe.
	if (request == (void*)-1)
	{
		return NULL; // sbrk failed.
	}
	int *meta_size = (int*)request;
	meta_size[0] = size_cal;

	meta_size[1] = 0;
	meta_size = (int*)request + 2;

	block = (node_ptr)meta_size;
	return block;
}

// If it's the first ever call, i.e., root == NULL, request_space and set root.
// Otherwise, if we can find a free block, use it.
// If not, request_space.
void* CS550_malloc(size_t size)
{

	struct node *block;
	// TODO: align size?

	if (size <= 0)
	{
		return NULL;
	}

	if (!sroot)
	{ // First call.
		block = request_space(NULL, size);
		if (!block)
		{
			return NULL;
		}
	}
	else
	{
		struct node *last = sroot;
		data_val inputvalue;
		int size_cal = size % 8;
		if (size_cal != 0)
		{
			size_cal = 8 - size_cal;
		}
		size_cal = size + size_cal + 8;
		//new_size = inputvalue.size % 8;
		//new_size = 8 - new_size;
		//new_size = inputvalue.size + size;
		inputvalue.size = size_cal;
		block = search(last, inputvalue);
		if (!block)
		{ // Failed to find free block.
			block = request_space(last, size);
			if (!block)
			{
				return NULL;
			}
		}

	}
	int* meta = (int*)block - 2;
	
	return block;
}
void* CS550_calloc(size_t size, int data_size)
{
	struct node *block;
	int new_size;
	// TODO: align size?

	if (size <= 0)
	{
		return NULL;
	}

	if (!sroot)
	{ // First call.
		block = request_space(NULL, size);
		if (!block)
		{
			return NULL;
		}
	}
	else
	{
		struct node *last = sroot;
		data_val inputvalue;
		inputvalue.size = size;
		new_size = inputvalue.size % 8;
		new_size = 8 - new_size;
		new_size = inputvalue.size + size;
		inputvalue.size = new_size;
		block = search(last, inputvalue);
		if (!block)
		{ // Failed to find free block.
			block = request_space(last, size);
			if (!block)
			{
				return NULL;
			}
		}

	}
	memset(block, 0, size*data_size);
	return block;
}

void* CS550_realloc(void* cur, size_t size)
{
	node_ptr curr = (node_ptr)cur;
	struct node *block;
	int new_size;
	// TODO: align size?

	if (size <= 0)
	{
		return NULL;
	}

	if (!sroot)
	{ // First call.
		block = request_space(NULL, size);
		if (!block)
		{
			return NULL;
		}
	}
	else
	{
		struct node *last = sroot;
		data_val inputvalue;
		inputvalue.size = size;
		new_size = inputvalue.size % 8;
		new_size = 8 - new_size;
		new_size = inputvalue.size + size;
		inputvalue.size = new_size;
		block = search(last, inputvalue);
		if (!block)
		{ // Failed to find free block.
			block = request_space(last, size);
			if (!block)
			{
				return NULL;
			}
		}

	}
	memcpy(curr, block, curr->value.size);


	return block;
}
int get_size(void *ptr)
{
	int *meta = ((int*)ptr) - 2;
	int size = meta[0];

	return size;
}
void CS550_free(void *ptr)
{
	if (!ptr)
	{
		return;
	}
	int size = 0;
	size = get_size(ptr);
	if (size>max)
	{
		max = size;
	}
	insert_size_bst((int*)ptr, size);
	insert_adrs_bst((int*)ptr, size);
	/*	int size_cal=size%8;
	if(size_cal!=0)
	{
	size_cal=8-size_cal;
	}
	size_cal=size+size_cal+8;*/
	return;
}



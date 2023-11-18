#include<stdio.h> 
#include<stdlib.h> 
#include <time.h>
#define arvore_quant 1000

struct Node 
{ 
	int key; 
	struct Node *left; 
	struct Node *right; 
    struct Node *dad;
	int height; 
}; 
 
int height(struct Node *N) 
{ 
	if (N == NULL) 
		return 0; 
	return N->height; 
} 

int max(int a, int b) 
{ 
	return (a > b)? a : b; 
} 

/* creates a node */
struct Node* newNode(int key) 
{ 
	struct Node* node = (struct Node*)malloc(sizeof(struct Node)); 
	node->key = key;
	node->dad = NULL; 
	node->left = NULL; 
	node->right = NULL; 
	node->height = 1; 
	return(node); 
} 


struct Node *rightRotate(struct Node *y) 
{ 
	struct Node *x = y->left; 
	struct Node *T2 = x->right; 

	x->right = y; 
	y->left = T2; 

	// Update heights 
	y->height = max(height(y->left),height(y->right)) + 1; 
	x->height = max(height(x->left),height(x->right)) + 1; 
 
	return x; 
} 

struct Node *leftRotate(struct Node *x) 
{ 
	struct Node *y = x->right; 
	struct Node *T2 = y->left; 
 
	y->left = x; 
	x->right = T2; 

	// Update heights 
	x->height = max(height(x->left),height(x->right)) + 1; 
	y->height = max(height(y->left), height(y->right)) + 1; 

	return y; 
} 

// Get Balance factor of node No 
int getBalance(struct Node *No)
{ 
	if (No == NULL) 
		return 0; 
	return height(No->left) - height(No->right); 
} 

// Recursive function to insert a key in the subtree rooted 
// with node and returns the new root of the subtree. 
struct Node* insert(struct Node* node, int key, struct Node* dad){ 

	if (node == NULL){
		struct Node* new = newNode(key);
		new->dad = dad;
		return new;
		//return(newNode(key)); 
	}
	if (key < node->key) 
		node->left = insert(node->left, key, node); 
	else if (key > node->key) 
		node->right = insert(node->right, key, node); 
	else // Equal keys
		return node; 

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left), 
						height(node->right)); 

	/* 3. Get the balance factor of this ancestor 
		node to check whether this node became 
		unbalanced */
	int balance = getBalance(node); 

	// If this node becomes unbalanced, then 
	// there are 4 cases 

	// Left Left Case 
	if (balance > 1 && key < node->left->key) 
		return rightRotate(node); 

	// Right Right Case 
	if (balance < -1 && key > node->right->key) 
		return leftRotate(node); 

	// Left Right Case 
	if (balance > 1 && key > node->left->key) 
	{ 
		node->left = leftRotate(node->left); 
		return rightRotate(node); 
	} 

	// Right Left Case 
	if (balance < -1 && key < node->right->key) 
	{ 
		node->right = rightRotate(node->right); 
		return leftRotate(node); 
	} 

	/* return the (unchanged) node pointer */
	return node; 
} 

// A utility function to print preorder traversal 
// of the tree. 
// The function also prints height of every node 
void preOrder(struct Node *root) 
{ 
	if(root != NULL) 
	{ 	
		printf("valor %d, altura = %d\n", root->key, root->height); 
		preOrder(root->left); 
		preOrder(root->right); 
	} 
} 

int contar_elementos_no(struct Node* no) {
    if (no == NULL) {
        return 0;
    }
    return 1 + contar_elementos_no(no->left) + contar_elementos_no(no->right);
}

struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}
 
// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Node* deleteNode(struct Node* root, int key)
{
    // STEP 1: PERFORM STANDARD BST DELETE
 
    if (root == NULL)
        return root;
 
    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteNode(root->left, key);
 
    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteNode(root->right, key);
 
    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct Node *temp = root->left ? root->left :
                                             root->right;
 
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            struct Node* temp = minValueNode(root->right);
 
            // Copy the inorder successor's data to this node
            root->key = temp->key;
 
            // Delete the inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }
 
    // If the tree had only one node then return
    if (root == NULL)
      return root;
 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
 
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  leftRotate(root->left);
        return rightRotate(root);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
 
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
 
    return root;
}


int main() { 
	struct Node *root = NULL; 

	srand( (unsigned)time(NULL) );
	for(int i = 0; contar_elementos_no(root)<arvore_quant ;i++){
		int valor = rand()%arvore_quant;
		root = insert(root,valor,NULL);
	}


	printf("the root is %d\n",root->key);
	printf("there are %d levels\n",root->height);
	printf("There are %d elements\n",contar_elementos_no(root));
	for(int i = 500; i<1000;i++){
		root = deleteNode(root,i);
	}

	return 0; 
} 

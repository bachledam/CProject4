#include <stdio.h>
#include <stdlib.h>

struct tree_name_node_struct
{
  char treeName[32];
  struct tree_name_node_struct *parent, *left, *right;
  struct item_node_struct *theTree;
};

typedef struct tree_name_node_struct tree_name_node;

struct item_node_struct
{
  char name[32];
  int count;
  struct item_node_struct *iparent, *left, *right;
  struct tree_name_node_struct *tparent;
};

typedef struct item_node_struct item_node;

//constructor for tree_name_node
tree_name_node *new_tree_name_node;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Basic binary tree with integer payload. */
struct bintree_node_struct {
    int payload;
    struct bintree_node_struct *parent;
    struct bintree_node_struct *left;
    struct bintree_node_struct *right;
};
typedef struct bintree_node_struct bintree_node;
/* Constructor for bintree_node. */
bintree_node *new_bintree_node(int payload) {
    bintree_node *b = malloc(sizeof(bintree_node));
    b->payload = payload;
    b->parent = NULL;
    b->left = NULL;
    b->right = NULL;
    return b;
}
/* Shallow destructor for bintree_node. */
void dispose_bintree_node(bintree_node *b) {
    free(b);
}
/* Recursive destructor for bintrees. */
void dispose_bintree(bintree_node *t) {
    if(t->left) {
        dispose_bintree(t->left);
    }
    if(t->right) {
        dispose_bintree(t->right);
    }
    dispose_bintree_node(t);
}
/* Sets the left child.  parent and child must be non-null and void.  Don't call
   this on a parent that already has a left child, you'll lose the pointer. */
void set_left_child(bintree_node *parent, bintree_node *child) {
    parent->left = child;
    child->parent = parent;
}
/* Sets the right child.  Similar to set_left_child(). */
void set_right_child(bintree_node *parent, bintree_node *child) {
    parent->right = child;
    child->parent = parent;
}
/* Remove and return the left child.  Returns null if there is no left child.
   Returned child is still live, has a null parent, and is the root of its own
   binary tree, including descendants if it has any. */
bintree_node *remove_left_child(bintree_node *parent) {
    /* We need a copy of the pointer so we can return it after we nullify it. */
    bintree_node *child = parent->left;
    if(child) {  // same as if(child != NULL)
        parent->left = NULL;
        child->parent = NULL;
    }
    return child;
}
/* Remove and return the right child.  Similar to remove_left_child(). */
bintree_node *remove_right_child(bintree_node *parent) {
    bintree_node *child = parent->right;
    if(child) {  // same as if(child != NULL)
        parent->right = NULL;
        child->parent = NULL;
    }
    return child;
}
/* Okay, we've got our basic adders and removers.  Let's get to work. */
/* Insert a node into a binary tree, treating it as a binary search tree.
   Maintains the property that all descendants leftward of parent have a value
   < parent, and descendants are rightward of parent otherwise.  Returns the
   child as it is finally added.
   parent and new_node must be allocated and valid. */
bintree_node *bst_insert(bintree_node *parent, bintree_node *new_node) {
    if(new_node->payload < parent->payload) { // value <; go LEFT
        if(parent->left) { // already a left child; RECURSIVE case.
            return bst_insert(parent->left, new_node);
        } else { // no left child; BASIS case.
            set_left_child(parent, new_node);
            return new_node;
        }
    } else { // value >=, go RIGHT
        if(parent->right) { // already a right child; RECURSIVE case.
            return bst_insert(parent->right, new_node);
        } else { // no right child; BASIS case.
            set_right_child(parent, new_node);
            return new_node;
        }
    }
}
/* Finds and returns a node with a payload value of target in the binary search
   tree rooted at parent.  Returns NULL if value is not a payload in this tree. */
bintree_node *bst_find(bintree_node *parent, int target) {
    if(parent == NULL) { // FAILURE case
        return NULL;
    } else if(target == parent->payload) { // BASIS case
        return parent;
    } else if(target < parent->payload) { // LEFT recursive case
        return bst_find(parent->left, target);
    } else { // target >= parent -> payload - RIGHT recursive case
        return bst_find(parent->right, target);
    }
}
void print_indents(int indent_level)
{
    for(int i = 0; i < indent_level; i++) {
        printf("  ");
    }
}
void print_bintree_node(bintree_node *b) {
    printf("Node w/payload %d", b->payload);
    if(b->parent) printf(", parent %d", b->parent->payload);
    if(b->left) printf(", left %d", b->left->payload);
    if(b->right) printf(", right %d", b->right->payload);
    printf("\n");
}
/* There are three ways to traverse a binary tree.  This is PREORDER, in which
   we visit the parent, then the left child, then the right child; it's the
   "natural" tree traversal order we've been using up to now. */
void print_bintree_preorder(bintree_node *t, int level) {
    print_indents(level);
    print_bintree_node(t);
    if(t->left) print_bintree_preorder(t->left, level + 1);
    if(t->right) print_bintree_preorder(t->right, level + 1);
}
/* INORDER traversal visits the left child, then the parent, then the right
   child.  It has the property that it will traverse a binary search tree in
   sorted order! */
void print_bintree_inorder(bintree_node *t, int level) {
    if(t->left) print_bintree_inorder(t->left, level + 1);
    print_indents(level);
    print_bintree_node(t);
    if(t->right) print_bintree_inorder(t->right, level + 1);
}
/* POSTORDER traversal visits the left child, then the right child, then the
   parent.  It'll be important a bit later. */
void print_bintree_postorder(bintree_node *t, int level) {
    if(t->left) print_bintree_postorder(t->left, level + 1);
    if(t->right) print_bintree_postorder(t->right, level + 1);
    print_indents(level);
    print_bintree_node(t);
}
/* To get the sum of the payloads of a binary tree, we grab the sum of each side,
   then add the parent.
   Preorder versus postorder versus in-order doesn't matter here - try all
   three and you'll see that they're equal. */
int sum_bintree(bintree_node *t) {
    int val = t->payload;
    /* The basis case is implicit - if we're a leaf, we will simply return our
       payload. */
    if(t->left) val += sum_bintree(t->left);
    if(t->right) val += sum_bintree(t->right);
    return val;
}
/* EXERCISE: Write a recursive function to find the height of a binary tree.  The
   height is the longest path from the root to a leaf. */
int height_bintree(bintree_node *t) {
    /* Basis: The height of a leaf is zero. */
    int left = 0;
    int right = 0;
    /* Recursion: The height of a nonleaf is one more than the height of its
       tallest child.  Find the height of both children... */
    if(t->left) left = height_bintree(t->left) + 1;
    if(t->right) right = height_bintree(t->right) + 1;
    /* ...and return the tallest. */
    if(left > right) return left;
    return right;
}
int main()
{
    bintree_node *tree = new_bintree_node(50);
    srand(time(NULL));
    for(int i = 0; i < 10; i++) {
        bst_insert(tree, new_bintree_node(rand() % 100));
    }
    printf("In-order:\n");
    print_bintree_inorder(tree, 0);
    printf("Pre-order:\n");
    print_bintree_preorder(tree, 0);
    printf("Post-order:\n");
    print_bintree_postorder(tree, 0);
    printf("Sum of nodes: %d\n", sum_bintree(tree));
    printf("Height of tree: %d\n", height_bintree(tree));
    dispose_bintree(tree);
    return 0;
}

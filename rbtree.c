#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct rb_node rb_node;

static inline void msg(const char* s) {
        printf("%s\n", s);
}

// sentinel node: all leaf nodes and the parent of the root, color is black
struct rb_node __sentinel_nil = {NIL, RB_BLACK, NULL, NULL};

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

struct rb_node* get_node() {
        struct rb_node* node = malloc(sizeof(struct rb_node));
        assert(node);
        node->rb_parent = node->rb_left = node->rb_right = NIL;
        node->rb_color = RB_BLACK;
        node->key = -1;
        node->s = NULL;
        return node;
}

// check [here](http://www.cse.yorku.ca/~oz/hash.html)
static unsigned long hash(unsigned char* str, int *len) {
        unsigned long hash = 5381;
        int c;
        *len = 0;
        while ((c = *str++)) {
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
                (*len)++;
        }
        return hash;
}

struct rb_node* get_node_str(unsigned char* ps) {
        struct rb_node *node = get_node();
        if (ps) {
                int len = 0;
                unsigned long k = hash(ps, &len);
                node->key = k;
                node->s = malloc((len + 1) * sizeof(unsigned char));
                node->s[len] = '\0';
                while (len--) node->s[len] = ps[len];  // copy the string
                //printf("node_str: %s\n", node->s);
        }
        return node;
}

/*
   |                                          |
   y        <-----left_rotate(T, x)----       x
   / \                                        / \
   x   c      ------right_rotate(T, y)--->    a   y
   / \                                            / \
   a   b                                          b   c
   */

// root and x both are not NULL
static rb_node* left_rotate(rb_node* root, rb_node* x) {
        rb_node* y = x->rb_right;
        x->rb_right = y->rb_left;
        if (y->rb_left != NIL) {  // b is not NIL
                y->rb_left->rb_parent = x;
        }
        y->rb_parent = x->rb_parent;
        if (x->rb_parent == NIL) {  // x is root
                root = y;
        } else if (x == x->rb_parent->rb_left) {  // x is its parent's left child
                x->rb_parent->rb_left = y;
        } else {  // x is its patent's right child
                x->rb_parent->rb_right = y;
        }
        y->rb_left = x;
        x->rb_parent = y;
        return root;
}

// root and y both are not NULL
static rb_node* right_rotate(rb_node* root, rb_node* y) {
        rb_node *x = y->rb_left;
        y->rb_left = x->rb_right;
        if (x->rb_right != NIL) {  // b is not NIL
                x->rb_right->rb_parent = y;
        }
        x->rb_parent = y->rb_parent;
        if (y->rb_parent == NIL) {  // y is root
                root = x;
        } else if (y == y->rb_parent->rb_left) {
                y->rb_parent->rb_left = x;
        } else {
                y->rb_parent->rb_right = x;
        }
        x->rb_right = y;
        y->rb_parent = x;
        return root;
}

static struct rb_node* rb_insert_fixup(rb_node* root, rb_node* z) {
        while (z->rb_parent->rb_color == RB_RED) {  // violation: both 'z' and 'z->rb_parent' are red
                if (z->rb_parent == z->rb_parent->rb_parent->rb_left) {  // z's parent is a left child
                        rb_node* y = z->rb_parent->rb_parent->rb_right;  // let 'y' be z's uncle 
                        if (y->rb_color == RB_RED) {  // uncle is red
                                z->rb_parent->rb_color = RB_BLACK;  // set parent BLACK
                                y->rb_color = RB_BLACK;  // set uncle BLACK
                                z->rb_parent->rb_parent->rb_color = RB_RED;  // set grandparent RED
                                z = z->rb_parent->rb_parent;  // let 'z' be its grandparent, now 'z' is still RED
                        } else {
                                if (z == z->rb_parent->rb_right) {  // 'z' is a right child
                                        z = z->rb_parent; root = left_rotate(root, z);  // z's height is kept, now uncle is black
                                }
                                // we are in case 3
                                z->rb_parent->rb_color = RB_BLACK;
                                z->rb_parent->rb_parent->rb_color = RB_RED;
                                root = right_rotate(root, z->rb_parent->rb_parent);
                        }
                } else {  // z's parent is a right child
                        rb_node* y = z->rb_parent->rb_parent->rb_left;  // let 'y' be z's uncle 
                        if (y->rb_color == RB_RED) {  // uncle is red
                                z->rb_parent->rb_color = RB_BLACK;  // set parent BLACK
                                y->rb_color = RB_BLACK;  // set uncle BLACK
                                z->rb_parent->rb_parent->rb_color = RB_RED;  // set grandparent RED
                                z = z->rb_parent->rb_parent;  // let 'z' be its grandparent, now 'z' is still RED
                        } else {
                                if (z == z->rb_parent->rb_left) {  // 'z' is a right child
                                        z = z->rb_parent; root = right_rotate(root, z);  // z's height is kept, now uncle is black
                                }
                                // we are in case 3
                                z->rb_parent->rb_color = RB_BLACK;
                                z->rb_parent->rb_parent->rb_color = RB_RED;
                                root = left_rotate(root, z->rb_parent->rb_parent);
                        }
                }
        }
        root->rb_color = RB_BLACK;
        return root;
}

// 'z' can not be NULL, however 'root' can
struct rb_node* rb_insert(rb_node *root, rb_node *z) {
        struct rb_node *y = NIL, *x = root;  // y is the parent of x
        while (x != NIL) {
                y = x;  // keep y to be the parent of x
                if (z->key < x->key) {
                        x = x->rb_left;  // go left
                } else {
                        x = x->rb_right;  // go right
                }
        }
        z->rb_parent = y;  // x is NIL now, and y is the parent of x

        if (y == NIL) {
                root = z; // empty tree, we make 'z' root node
        } else if (z->key < y->key) {
                y->rb_left = z;
        } else {
                y->rb_right = z;
        }
        z->rb_left = z->rb_right = NIL;  // if we get 'z' by 'get_node', there's no need to assign 'left' and 'right'
        z->rb_color = RB_RED;  // colorize it
        root = rb_insert_fixup(root, z);
        return root;
}

static rb_node* rb_transplant(rb_node *root, rb_node *u, rb_node *v) {
        if (u->rb_parent == NIL) {  // u is root
                root = v;
        } else if (u == u->rb_parent->rb_left) {  // u is a left child
                u->rb_parent->rb_left = v;
        } else {  // u is a right child
                u->rb_parent->rb_right = v;
        }
        v->rb_parent = u->rb_parent;
        return root;
}

struct rb_node* rb_minimum(rb_node *root) {
        while (root->rb_left != NIL)
                root = root->rb_left;
        return root;
}

struct rb_node *rb_delete_fixup(rb_node* root, rb_node* x) {
        while (x != root && x->rb_color == RB_BLACK) {
                if (x == x->rb_parent->rb_left) {
                        // x's brother: w, and w can never be NIL
                        rb_node *w = x->rb_parent->rb_right;
                        if (w->rb_color == RB_RED) {
                                w->rb_color = RB_BLACK;
                                x->rb_parent->rb_color = RB_RED;
                                root = left_rotate(root, x->rb_parent);
                                w = x->rb_parent->rb_right;  // w is still x's brother, with black color
                        }
                        if (w->rb_left->rb_color == RB_BLACK && w->rb_right->rb_color == RB_BLACK) {
                                w->rb_color = RB_RED;  // remove the extra black color of x and w
                                x = x->rb_parent;  // add the extra black color to x->rb_parent, keep looping
                        } else {
                                if (w->rb_right->rb_color == RB_BLACK) {
                                        w->rb_left->rb_color = RB_BLACK;
                                        w->rb_color = RB_RED;
                                        root = right_rotate(root, w);
                                        w = x->rb_parent->rb_right;
                                }
                                w->rb_color = x->rb_parent->rb_color;
                                x->rb_parent->rb_color = RB_BLACK;
                                w->rb_right->rb_color = RB_BLACK;
                                root = left_rotate(root, x->rb_parent);
                                x = root;
                        }
                } else {
                        // x's brother: w, and w can never be NIL
                        rb_node *w = x->rb_parent->rb_left;
                        if (w->rb_color == RB_RED) {
                                w->rb_color = RB_BLACK;
                                x->rb_parent->rb_color = RB_RED;
                                root = right_rotate(root, x->rb_parent);
                                w = x->rb_parent->rb_left;  // w is still x's brother, with black color
                        }
                        if (w->rb_right->rb_color == RB_BLACK && w->rb_left->rb_color == RB_BLACK) {
                                w->rb_color = RB_RED;  // remove the extra black color of x and w
                                x = x->rb_parent;  // add the extra black color to x->rb_parent, keep looping
                        } else {
                                if (w->rb_left->rb_color == RB_BLACK) {
                                        w->rb_right->rb_color = RB_BLACK;
                                        w->rb_color = RB_RED;
                                        root = left_rotate(root, w);
                                        w = x->rb_parent->rb_left;
                                }
                                w->rb_color = x->rb_parent->rb_color;
                                x->rb_parent->rb_color = RB_BLACK;
                                w->rb_left->rb_color = RB_BLACK;
                                root = right_rotate(root, x->rb_parent);
                                x = root;
                        }
                }
        }

        /*
         * if x is root, we just remove the extra black
         * if x is RB_RED, we just set it black
         */
        x->rb_color = RB_BLACK;
        return root;
}

struct rb_node* rb_delete(rb_node *root, rb_node *z) {
        rb_node *y = z;
        rb_node *x = NULL;
        int y_original_color = y->rb_color;
        if (z->rb_left == NIL) {  // z only has right child
                x = z->rb_right;
                root = rb_transplant(root, z, x);  // replace z with x
        } else if (z->rb_right == NIL) {  // z only has left child
                x = z->rb_left;
                root = rb_transplant(root, z, x);
        } else {  // z has both left and right children
                // now we use z's minimum to replace z
                y = rb_minimum(z->rb_right);  // we know y has no left child
                y_original_color = y->rb_color;
                x = y->rb_right;  // x may be NIL
                if (y != z->rb_right) {
                        root = rb_transplant(root, y, x);  // we kick 'y' out, let 'x' replace it, x's parent is y's parent
                        y->rb_right = z->rb_right;
                        y->rb_right->rb_parent = y;
                } else {
                        x->rb_parent = y;  // even x is NIL
                }
                root = rb_transplant(root, z, y);  // we kick 'z' out, let 'y' replace it
                y->rb_left = z->rb_left;
                y->rb_left->rb_parent = y;
                y->rb_color = z->rb_color;
        }
        if (y_original_color == RB_BLACK) root = rb_delete_fixup(root, x);
        return root;
}

inline void free_node(rb_node* p) {
        if (p && p != NIL) free(p);
}

struct rb_node* rb_search(rb_node* root, long long key) {
        while (root != NIL) {
                if (root->key == key) break;
                if (root->key > key) root = root->rb_left;
                if (root->key < key) root = root->rb_right;
        }
        return root;
}

void tranversal_inorder(struct rb_node *root) {
        if (root == NIL) return;
        tranversal_inorder(root->rb_left);
        int flag = root->s ? 1 : 0;
        printf("%s%lld %s%s", root->rb_color == RB_RED ? RED : RESET, root->key,
                        flag ? root->s : " ",
                        flag ? "\n" : " ");
        tranversal_inorder(root->rb_right);
}


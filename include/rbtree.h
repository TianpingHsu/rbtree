#ifndef __RB_TREE_H__
#define __RB_TREE_H__

struct rb_node {
        struct rb_node* rb_parent;
        int rb_color;
#define RB_RED 0
#define RB_BLACK 1
        struct rb_node* rb_left;
        struct rb_node* rb_right;
        int key;
};

extern struct rb_node __sentinel_nil;
#define NIL (&__sentinel_nil)

extern struct rb_node* get_node();
extern void free_node(struct rb_node*);
extern struct rb_node* rb_search(struct rb_node* root, int key);
extern struct rb_node* rb_insert(struct rb_node *root, struct rb_node *node);  // return root
extern struct rb_node* rb_delete(struct rb_node *root, struct rb_node *node);  // return root
extern struct rb_node* rb_maximum(struct rb_node *root);
extern struct rb_node* rb_minimum(struct rb_node *root);

extern void level_tranversal(struct rb_node *root);
extern void tranversal_inorder(struct rb_node *root);

#endif

#include "rbtree.h"
#include <stdio.h>
#include <assert.h>

int calc_black_height(struct rb_node* root) {
        int lh = root->rb_color == RB_BLACK ? 1 : 0;
        int rh = lh;
        if (root && root != NIL) {
                lh += calc_black_height(root->rb_left);
                rh += calc_black_height(root->rb_right);
        }
        assert(lh == rh);
        return lh;
}

int main() {
        struct rb_node *root = NIL;

        const int arr[] = {
                //2, 9, 7, 11, 5, 8, 6
                //9, 5, 7
                8, 9, 31, 6, 35, 39, 43, 7, 34, 20, 44, 1, 33, 30, 12, 38, 16, 24, 21, 15, 18, 4, 14, 41, 48, 23, 11, 25, 46, 27, 42, 19, 10, 17, 2, 32, 47, 45, 13, 26, 28, 3, 36, 49, 40, 22, 37, 29, 5

                //10, 7, 12, 11
        };
        int count = 0;
        struct rb_node *p = NULL;
        do {
                p = get_node();
                p->key = arr[count++];
                root = rb_insert(root, p);
                //tranversal_inorder(root);
                //printf("\n");
                calc_black_height(root);
        } while (count < sizeof(arr)/sizeof(const int));
        tranversal_inorder(root);
        printf("\n");

        //p = rb_search(root, 12);
        //p->rb_color = (p->rb_color == RB_RED ? RB_BLACK : RB_RED);
        //calc_black_height(root);

        for (int i = sizeof(arr)/sizeof(const int) - 1; i >= 0; i--) {
                struct rb_node *p = rb_search(root, arr[i]);
                if (p != NIL) {
                        root = rb_delete(root, p);
                        free_node(p);
                }
                //tranversal_inorder(root);
                //printf("\n");
                calc_black_height(root);
        }
        calc_black_height(root);
        //tranversal_inorder(root);
        //printf("\n");
}

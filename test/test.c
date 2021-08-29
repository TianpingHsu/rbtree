#include "rbtree.h"
#include <stdio.h>
#include <assert.h>

void test1();
void test2();

int main() {
        test1();
        test2();
}

void test2()
{
        struct rb_node *root = get_node_str((unsigned char*)"");
        struct rb_node *p = NULL;
#define insert_it(root, s) do { \
        p = get_node_str((unsigned char*)s); \
        root = rb_insert(root, p); \
} while(0);
        const char* s[] = {
                "hello",
                "d",
                "b",
                "c",
                "a",
                "world",
        };
        for (unsigned int i = 0; i < sizeof(s)/sizeof(const unsigned char*); i++) {
                insert_it(root, s[i]);
        }
        tranversal_inorder(root);
}

// Returns returns tree if the Binary tree is balanced like a Red-Black
// tree. This function also sets value in maxh and minh (passed by
// reference). maxh and minh are set as maximum and minimum heights of root.
int isBalancedUtil(struct rb_node *root, int *maxh, int *minh)
{
	// Base case
	if (root == NULL)
	{
		maxh = minh = 0;
		return 1;
	}

	int lmxh, lmnh; // To store max and min heights of left subtree
	int rmxh, rmnh; // To store max and min heights of right subtree

	// Check if left subtree is balanced, also set lmxh and lmnh
	if (isBalancedUtil(root->rb_left, &lmxh, &lmnh) == 0)
		return 0;

	// Check if right subtree is balanced, also set rmxh and rmnh
	if (isBalancedUtil(root->rb_right, &rmxh, &rmnh) == 0)
		return 0;

	// Set the max and min heights of this node for the parent call
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
	*maxh = max(lmxh, rmxh) + 1;
	*minh = min(lmnh, rmnh) + 1;

	// See if this node is balanced
	if (*maxh <= 2*(*minh))
		return 1;

	return 0;
}

// A wrapper over isBalancedUtil()
int isBalanced(struct rb_node *root)
{
	int maxh, minh;
	return isBalancedUtil(root, &maxh, &minh);
}

int calc_black_height(struct rb_node* currNode) {
            // For an empty subtree the answer is obvious
    if (currNode == NULL)
        return 0;
    // Computes the height for the left and right child recursively
    int leftHeight = calc_black_height(currNode->rb_left);
    int rightHeight = calc_black_height(currNode->rb_right);
    int add = currNode->rb_color == RB_BLACK ? 1 : 0;
    // The current subtree is not a red black tree if and only if
    // one or more of current node's children is a root of an invalid tree
    // or they contain different number of black nodes on a path to a null node.
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight)
        return -1;
    else
        return leftHeight + add;
}

int is_rb_tree_balanced(struct rb_node* root)
{
    return calc_black_height(root) != -1;
}


void test1()
{
        struct rb_node *root = NIL;

        const int arr[] = {
                //2, 9, 7, 11, 5, 8, 6
                //9, 5, 7
                //8, 9, 31, 6, 35, 39, 43, 7, 34, 20, 44, 1, 33, 30, 12, 38, 16, 24, 21, 15, 18, 4, 14, 41, 48, 23, 11, 25, 46, 27, 42, 19, 10, 17, 2, 32, 47, 45, 13, 26, 28, 3, 36, 49, 40, 22, 37, 29, 5
141, 119, 58, 126, 7, 5, 93, 29, 13, 147, 50, 3, 66, 61, 89, 25, 18, 87, 129, 46, 69, 97, 138, 8, 9, 124, 14, 143, 117, 44, 56, 15, 116, 99, 149, 127, 76, 65, 120, 38, 70, 88, 82, 144, 57, 81, 17, 72, 84, 6, 2, 60, 34, 27, 21, 114, 55, 26, 134, 31, 35, 52, 108, 90, 30, 107, 111, 32, 91, 142, 47, 33, 67, 136, 145, 122, 130, 137, 78, 128, 19, 48, 123, 80, 131, 22, 71, 85, 110, 28, 148, 135, 146, 133, 140, 12, 113, 68, 103, 51, 106, 118, 92, 39, 16, 45, 96, 104, 4, 77, 125, 49, 64, 100, 37, 11, 42, 105, 101, 75, 74, 109, 112, 83, 98, 23, 10, 95, 20, 94, 62, 1, 40, 79, 139, 121, 54, 86, 132, 59, 24, 115, 63, 53, 36, 73, 102, 41, 43

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
        if (is_rb_tree_balanced(root)) {
                printf("Balanced!\n");
        } else {
                printf("Unbalanced!\n");
        }

        //p = rb_search(root, 12);
        //p->rb_color = (p->rb_color == RB_RED ? RB_BLACK : RB_RED);
        //calc_black_height(root);

        for (int i = sizeof(arr)/sizeof(const int) - 1; i >= 0; i--) {
                struct rb_node *p = rb_search(root, arr[i]);
                if (p != NIL) {
                        root = rb_delete(root, p);
                        free_node(p);
                }
                if (is_rb_tree_balanced(root)) {
                        printf("Balanced!\n");
                } else {
                        printf("Unbalanced!\n");
                }
                //tranversal_inorder(root);
                //printf("\n");
                //printf("%d\n", calc_black_height(root));
        }
        calc_black_height(root);
        //tranversal_inorder(root);
        //printf("\n");
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char name[50];
    int rank;
    struct Node *left, *right;
    int height; // Height of the node for AVL balancing
} Node;

Node* createNode(char *name, int rank) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->rank = rank;
    newNode->left = newNode->right = NULL;
    newNode->height = 1; // New node is initially added at leaf
    return newNode;
}

int height(Node *N) {
    if (N == NULL) return 0;
    return N->height;
}

int getBalance(Node *N) {
    if (N == NULL) return 0;
    return height(N->left) - height(N->right);
}

Node* rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));

    // Return new root
    return x;
}

Node* leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = 1 + (height(x->left) > height(x->right) ? height(x->left) : height(x->right));
    y->height = 1 + (height(y->left) > height(y->right) ? height(y->left) : height(y->right));

    // Return new root
    return y;
}

Node* insert(Node* root, char *name, int rank) {
    if (root == NULL) return createNode(name, rank);

    if (rank < root->rank) {
        root->left = insert(root->left, name, rank);
    } else if (rank > root->rank) {
        root->right = insert(root->right, name, rank);
    } else {
        // Duplicate rank not allowed
        printf("Duplicate rank %d for %s ignored.\n", rank, name);
        return root;
    }

    // Update height of this ancestor node
    root->height = 1 + (height(root->left) > height(root->right) ? height(root->left) : height(root->right));

    // Get the balance factor
    int balance = getBalance(root);

    // If the node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && rank < root->left->rank) {
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && rank > root->right->rank) {
        return leftRotate(root);
    }

    // Left Right Case
    if (balance > 1 && rank > root->left->rank) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Left Case
    if (balance < -1 && rank < root->right->rank) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void inOrder(Node* root) {
    if (root == NULL) return;
    inOrder(root->left);
    printf("Name: %s, Rank: %d\n", root->name, root->rank);
    inOrder(root->right);
}

void drawTree(Node* root, int space) {
    if (root == NULL) return;

    // Increase distance between levels
    space += 6;

    // Process right child first
    drawTree(root->right, space);

    // Print current node after space count
    printf("\n");
    for (int i = 6; i < space; i++) printf(" ");
    printf("%s(R:%d)", root->name, root->rank);

    // Process left child
    drawTree(root->left, space);
}



void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

Node* buildTree() {
    Node* root = NULL;

    // Sample data: name, rank
    char *names[] = {"Alice", "Bob", "Charlie", "Diana", "Eve"};
    int ranks[] = {30, 20, 40, 10, 25};
    int size = sizeof(ranks) / sizeof(ranks[0]);

    for (int i = 0; i < size; i++) {
        root = insert(root, names[i], ranks[i]);
    }

    printf("AVL Balanced Tree built with sample data.\n");
    return root;
}

int main() {
    Node* root = NULL;
    int choice;

    do {
        printf("\n===== Menu =====\n");
        printf("1. Insert\n");
        printf("2. Show Leaderboard (In-Order)\n");
        printf("3. Draw Tree\n");
        printf("4. Exit\n");
        printf("5. Build Sample AVL Tree\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char name[50];
            int rank;
            printf("Enter Name: ");
            scanf("%s", name);
            printf("Enter Rank: ");
            scanf("%d", &rank);
            root = insert(root, name, rank);
            printf("Inserted Successfully!\n");
        }
        else if (choice == 2) {
            printf("\n===== Leaderboard =====\n");
            inOrder(root);
        }
        else if (choice == 3) {
            printf("\n===== Tree Structure =====\n");
            drawTree(root, 0);
        }
        else if (choice == 4) {
            printf("Exiting...\n");
            freeTree(root);
        }
        else if (choice == 5) {
            freeTree(root); // free existing tree before building new
            root = buildTree();
        }
        else {
            printf("Invalid Choice. Try again.\n");
        }

    } while (choice != 4);

    return 0;
}

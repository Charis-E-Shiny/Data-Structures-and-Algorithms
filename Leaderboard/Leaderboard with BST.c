#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char name[50];
    int rank;
    struct Node *left, *right;
} Node;

Node* createNode(char *name, int rank) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    newNode->rank = rank;
    newNode->left = newNode->right = NULL;
    return newNode;
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

    // Increase space for right subtree
    space += 6;

    // Recur for right subtree first
    drawTree(root->right, space);

    // Print the current node with appropriate spaces
    for (int i = 6; i < space; i++) printf(" ");
    printf("%s(R:%d)\n", root->name, root->rank);

    // Print the left and right connectors
    if (root->left != NULL || root->right != NULL) {
        for (int i = 6; i < space - 3; i++) printf(" ");
        if (root->right != NULL) {
            printf("  /");
        } else {
            printf("  \\");
        }
    }

    // Recur for left subtree
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

    printf("Tree built with sample data.\n");
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
        printf("5. Build Sample Tree\n");
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
            root = buildTree();
        }
        else {
            printf("Invalid Choice. Try again.\n");
        }

    } while (choice != 4);

    return 0;
}

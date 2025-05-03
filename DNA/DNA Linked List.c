#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char nucleotide;
    struct Node* next;
} Node;

// Function prototypes
Node* createNode(char nucleotide);
void append(Node** head, char nucleotide);
void display(Node* head);
int count(Node* head);
void deleteNode(Node** head, char nucleotide);
int searchNode(Node* head, char nucleotide);
void reverseList(Node** head);
void freeList(Node* head);
void displayAsciiArt(Node* head);

int main() {
    Node* dna = NULL;
    char nucleotide;
    int choice;

    do {
        printf("\nMenu:\n");
        printf("1. Append Nucleotide\n");
        printf("2. Display DNA Sequence (ASCII Art)\n");
        printf("3. Count Nucleotides\n");
        printf("4. Delete Nucleotide\n");
        printf("5. Search Nucleotide\n");
        printf("6. Reverse DNA Sequence\n");
        printf("7. Free Memory and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter nucleotide to append (A, T, G, C): ");
                scanf(" %c", &nucleotide);
                append(&dna, nucleotide);
                break;
            case 2:
                displayAsciiArt(dna);
                break;
            case 3:
                printf("Total Nucleotides: %d\n", count(dna));
                break;
            case 4:
                printf("Enter nucleotide to delete (A, T, G, C): ");
                scanf(" %c", &nucleotide);
                deleteNode(&dna, nucleotide);
                break;
            case 5:
                printf("Enter nucleotide to search (A, T, G, C): ");
                scanf(" %c", &nucleotide);
                if (searchNode(dna, nucleotide)) {
                    printf("Nucleotide '%c' found in the sequence!\n", nucleotide);
                } else {
                    printf("Nucleotide '%c' not found in the sequence.\n", nucleotide);
                }
                break;
            case 6:
                reverseList(&dna);
                printf("DNA Sequence reversed.\n");
                break;
            case 7:
                freeList(dna);
                printf("Memory freed and program exited.\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

// Create a new node
Node* createNode(char nucleotide) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->nucleotide = nucleotide;
    newNode->next = NULL;
    return newNode;
}

// Append nucleotide to end
void append(Node** head, char nucleotide) {
    Node* newNode = createNode(nucleotide);
    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Count nucleotides
int count(Node* head) {
    int cnt = 0;
    while (head != NULL) {
        cnt++;
        head = head->next;
    }
    return cnt;
}

// Delete a specific nucleotide from the list
void deleteNode(Node** head, char nucleotide) {
    Node* temp = *head;
    Node* prev = NULL;

    // If the head node itself holds the nucleotide
    if (temp != NULL && temp->nucleotide == nucleotide) {
        *head = temp->next; // Changed head
        free(temp); // Free memory
        return;
    }

    // Search for the nucleotide to delete
    while (temp != NULL && temp->nucleotide != nucleotide) {
        prev = temp;
        temp = temp->next;
    }

    // If the nucleotide was not present
    if (temp == NULL) {
        printf("Nucleotide '%c' not found in the sequence.\n", nucleotide);
        return;
    }

    // Unlink the node from the linked list
    prev->next = temp->next;
    free(temp); // Free memory
    printf("Nucleotide '%c' deleted from the sequence.\n", nucleotide);
}

// Search for a nucleotide
int searchNode(Node* head, char nucleotide) {
    while (head != NULL) {
        if (head->nucleotide == nucleotide) {
            return 1; // Found
        }
        head = head->next;
    }
    return 0; // Not found
}

// Reverse the DNA sequence
void reverseList(Node** head) {
    Node *prev = NULL, *current = *head, *next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head = prev;
}

// Free the list
void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Display DNA sequence as ASCII Art
void displayAsciiArt(Node* head) {
    if (head == NULL) {
        printf("DNA sequence is empty.\n");
        return;
    }
    
    printf("DNA Sequence (ASCII Art):\n");
    while (head != NULL) {
        switch (head->nucleotide) {
            case 'A':
                printf("   A   \n  /   \\ \n |     | \n  \\   / \n   A   \n\n");
                break;
            case 'T':
                printf("   T   \n   |   \n   T   \n   |   \n   T   \n\n");
                break;
            case 'G':
                printf("   GGG  \n  G   G \n  GGGG  \n  G   G \n   GGG  \n\n");
                break;
            case 'C':
                printf("   CCC  \n  C     \n  C     \n  C     \n   CCC  \n\n");
                break;
            default:
                printf("Invalid nucleotide detected!\n");
        }
        head = head->next;
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 127 // Prime number for hash table size
#pragma warning(disable : 4996) // Disable warning for unsafe functions

// Function prototypes
struct Parcel* createParcel(const char* destination, int weight, float valuation);
unsigned long hashFunction(const char* str);
struct TreeNode* createNode(struct Parcel* parcel);
struct TreeNode* insertNode(struct TreeNode* root, struct Parcel* parcel);
void inOrderTraversal(struct TreeNode* root);
struct TreeNode* findMin(struct TreeNode* root);
struct TreeNode* findMax(struct TreeNode* root);
struct TreeNode* findMinValuation(struct TreeNode* root);
struct TreeNode* findMaxValuation(struct TreeNode* root);
void searchWeight(struct TreeNode* root, int weight, int isHigher);
float calculateTotalValuation(struct TreeNode* root);
int calculateTotalWeight(struct TreeNode* root);
void freeMemory(struct TreeNode* root);
void initializeHashTable(struct HashTable* table);
void insertParcel(struct HashTable* table, struct Parcel* parcel);
void displayParcels(struct HashTable* table, const char* country);
void searchWeightForCountry(struct HashTable* table, const char* country, int weight, int isHigher);
void displayTotalForCountry(struct HashTable* table, const char* country);
void displayCheapestMostExpensive(struct HashTable* table, const char* country);
void displayLightestHeaviest(struct HashTable* table, const char* country);
void loadParcelsFromFile(struct HashTable* table, const char* filename);
void displayMenu(struct HashTable* table);

struct Parcel // Parcel structure
{
    char* destination;
    int weight;
    float valuation;
};

struct TreeNode // Tree node structure for BST
{
    struct Parcel* parcel; // Parcel data
    struct TreeNode* left; // Left child
    struct TreeNode* right; // Right child
};

struct HashTable // Hash table structure
{
    struct TreeNode* table[TABLE_SIZE];
};

struct Parcel* createParcel(const char* destination, int weight, float valuation) // Create a new parcel
{
    struct Parcel* newParcel = (struct Parcel*)malloc(sizeof(struct Parcel)); // Allocate memory for the parcel
    if (!newParcel) // Check if memory allocation failed 
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    newParcel->destination = (char*)malloc(strlen(destination) + 1); // Allocate memory for the destination string
    if (!newParcel->destination) // Check if memory allocation failed
    {
        free(newParcel);
        return NULL;
    }
    strcpy(newParcel->destination, destination); // Copy the destination string
    newParcel->weight = weight;
    newParcel->valuation = valuation;
    return newParcel; // Return the new parcel
}

unsigned long hashFunction(const char* str)
{
    unsigned long hash = 5381; // Initial hash value
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % TABLE_SIZE; // Return the hash value modulo the table size
}

struct TreeNode* createNode(struct Parcel* parcel) // Create a new tree node
{
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode)); // Allocate memory for the node
    if (!newNode)
    {
        printf("Memory allocation failed\n"); // Check if memory allocation failed
        return NULL;
    }
    newNode->parcel = parcel;
    newNode->left = newNode->right = NULL;
    return newNode;     // Return the new node
}

struct TreeNode* insertNode(struct TreeNode* root, struct Parcel* parcel) // Insert a new node into the tree
{
    if (!root) // If the root is NULL, create a new node
    {
        return createNode(parcel);
    }
    if (parcel->weight < root->parcel->weight) // Sort by weight
    {
        root->left = insertNode(root->left, parcel);
    }
    else
    {
        root->right = insertNode(root->right, parcel); // Otherwise, insert into the right subtree
    }
    return root;
}

void inOrderTraversal(struct TreeNode* root) // In-order traversal of the tree
{
    if (root)
    {
        inOrderTraversal(root->left);
        printf("Destination: %s, Weight: %d, Valuation: $%.2f\n", root->parcel->destination, root->parcel->weight, root->parcel->valuation);
        inOrderTraversal(root->right);
    }
}

struct TreeNode* findMin(struct TreeNode* root)
{
    while (root && root->left)
    {
        root = root->left;
    }
    return root;
}

struct TreeNode* findMax(struct TreeNode* root)
{
    while (root && root->right)
    {
        root = root->right;
    }
    return root;
}

struct TreeNode* findMinValuation(struct TreeNode* root)
{
    if (!root) return NULL; // Return NULL if the root is NULL

    struct TreeNode* minNode = root;
    if (root->left) // Traverse the left subtree
    {
        struct TreeNode* leftMin = findMinValuation(root->left); // Find the minimum valuation in the left subtree
        if (leftMin->parcel->valuation < minNode->parcel->valuation)
        {
            minNode = leftMin;
        }
    }
    if (root->right) // Traverse the right subtree
    {
        struct TreeNode* rightMin = findMinValuation(root->right); // Find the minimum valuation in the right subtree
        if (rightMin->parcel->valuation < minNode->parcel->valuation)
        {
            minNode = rightMin;
        }
    }
    return minNode; // Return the minimum valuation node
}

struct TreeNode* findMaxValuation(struct TreeNode* root)
{
    if (!root) return NULL;

    struct TreeNode* maxNode = root;
    if (root->left) // Traverse the left subtree
    {
        struct TreeNode* leftMax = findMaxValuation(root->left);
        if (leftMax->parcel->valuation > maxNode->parcel->valuation)
        {
            maxNode = leftMax; // Find the maximum valuation in the left subtree
        }
    }
    if (root->right)
    {
        struct TreeNode* rightMax = findMaxValuation(root->right); // Traverse the right subtree
        if (rightMax->parcel->valuation > maxNode->parcel->valuation)
        {
            maxNode = rightMax; // Find the maximum valuation in the right subtree
        }
    }
    return maxNode; // Return the maximum valuation node
}

void searchWeight(struct TreeNode* root, int weight, int isHigher)
{
    if (root)
    {
        searchWeight(root->left, weight, isHigher); // Search the left subtree
        if ((isHigher && root->parcel->weight > weight) || (!isHigher && root->parcel->weight < weight))
        {
            printf("Destination: %s, Weight: %d, Valuation: $%.2f\n", root->parcel->destination, root->parcel->weight, root->parcel->valuation); // Print the parcel details
        }
        searchWeight(root->right, weight, isHigher); // Search the right subtree
    }
}

float calculateTotalValuation(struct TreeNode* root)
{
    if (!root) // Return 0 if the root is NULL
    {
        return 0;
    }
    return root->parcel->valuation + calculateTotalValuation(root->left) + calculateTotalValuation(root->right); // Calculate the total valuation
}

int calculateTotalWeight(struct TreeNode* root)
{
    if (!root)  // Return 0 if the root is NULL
    {
        return 0;
    }
    return root->parcel->weight + calculateTotalWeight(root->left) + calculateTotalWeight(root->right); // Calculate the total weight
}

void freeMemory(struct TreeNode* root)
{
    if (root) // Check if the root is not NULL
    {
        freeMemory(root->left); // Free memory for the left subtree
        freeMemory(root->right);    // Free memory for the right subtree
        free(root->parcel->destination); // Free memory for the destination string
        free(root->parcel); // Free memory for the parcel
        free(root); // Free memory for the node
    }
}

void initializeHashTable(struct HashTable* table) // Initialize the hash table
{
    for (int i = 0; i < TABLE_SIZE; i++) // Iterate through the table
    {
        table->table[i] = NULL; // Set each element to NULL
    }
}

void insertParcel(struct HashTable* table, struct Parcel* parcel) // Insert a parcel into the hash table
{
    unsigned long index = hashFunction(parcel->destination); // Get the hash value

    table->table[index] = insertNode(table->table[index], parcel); // Insert the parcel into the BST
}

void displayParcels(struct HashTable* table, const char* country) // Display parcels for a given country
{
    unsigned long index = hashFunction(country); // Get the hash value
    printf("Parcels for %s:\n", country); // Print the country name
    if (table->table[index] != NULL)
    {
        inOrderTraversal(table->table[index]); // Traverse the BST
    }
}

void searchWeightForCountry(struct HashTable* table, const char* country, int weight, int isHigher)
{
    unsigned long index = hashFunction(country); // Get the hash value
    printf("Parcels for %s with weight %s than %d:\n", country, isHigher ? "higher" : "lower", weight); // Print the country name and weight
    if (table->table[index] != NULL) // Check if the table is not NULL
    {
        searchWeight(table->table[index], weight, isHigher); // Search for parcels by weight
    }
}

void displayTotalForCountry(struct HashTable* table, const char* country) // Display the total weight and valuation for a given country
{
    unsigned long index = hashFunction(country); // Get the hash value
    if (table->table[index] != NULL) // Check if the table is not NULL
    {
        printf("Total weight of parcels for %s: %d grams\n", country, calculateTotalWeight(table->table[index]));
        printf("Total valuation of parcels for %s: $%.2f\n", country, calculateTotalValuation(table->table[index]));
    }
}

void displayCheapestMostExpensive(struct HashTable* table, const char* country) // Display the cheapest and most expensive parcels for a given country
{
    unsigned long index = hashFunction(country);
    if (table->table[index] != NULL) // Check if the table is not NULL
    {
        struct TreeNode* minNode = findMinValuation(table->table[index]); // Find the minimum valuation node
        struct TreeNode* maxNode = findMaxValuation(table->table[index]); // Find the maximum valuation node
        if (minNode && maxNode) // Check if the nodes are not NULL 
        {
            printf("Cheapest parcel for %s:\n", country); // Print the country name
            printf("Weight: %d, Valuation: $%.2f\n", minNode->parcel->weight, minNode->parcel->valuation);
            printf("Most expensive parcel for %s:\n", country); // Print the country name
            printf("Weight: %d, Valuation: $%.2f\n", maxNode->parcel->weight, maxNode->parcel->valuation); // Print the parcel details
        }
        else
        {
            printf("No parcels found for %s.\n", country);  // Print an error message
        }
    }
    else
    {
        printf("No parcels found for %s.\n", country); // Print an error message
    }
}

void displayLightestHeaviest(struct HashTable* table, const char* country)
{
    unsigned long index = hashFunction(country); // Get the hash value
    if (table->table[index] != NULL) // Check if the table is not NULL
    {
        struct TreeNode* minNode = findMin(table->table[index]); // Find the minimum node
        struct TreeNode* maxNode = findMax(table->table[index]); // Find the maximum node
        if (minNode && maxNode) // Check if the nodes are not NULL
        {
            printf("Lightest parcel for %s:\n", country);
            printf("Weight: %d, Valuation: $%.2f\n", minNode->parcel->weight, minNode->parcel->valuation);
            printf("Heaviest parcel for %s:\n", country);
            printf("Weight: %d, Valuation: $%.2f\n", maxNode->parcel->weight, maxNode->parcel->valuation);
        }
        else
        {
            printf("No parcels found for %s.\n", country);
        }
    }
    else
    {
        printf("No parcels found for %s.\n", country);
    }
}

void loadParcelsFromFile(struct HashTable* table, const char* filename)
{
    FILE* file = fopen(filename, "r"); // Open the file in read mode
    if (!file)
    {
        printf("Error opening file"); // Print an error message
        return;
    }

    char destination[21] = { "Undefined" }; // Increase the buffer size to accommodate longer names
    int weight = 0;
    float valuation = 0;
    char line[200]; // Increase the buffer size to handle longer lines

    while (fgets(line, sizeof(line), file) != NULL) // Read each line from the file
    {
        // Use a format specifier that reads until the last two numbers
        int scan = sscanf(line, "%[^,], %d, %f", destination, &weight, &valuation);
        if (scan != 3)
        {
            printf("Error reading line\n");
            continue;
        }
        // Trim trailing whitespace from destination name
        int len = (int)strlen(destination); // Get the length of the destination string
        if (len > 0 && destination[len - 1] == ' ') // Check if the last character is a space
        {
            destination[len - 1] = '\0';
        }

        struct Parcel* parcel = createParcel(destination, weight, valuation); // Create a new parcel
        if (!parcel)
        {
            printf("Error creating parcel\n"); // Print an error message
            continue;
        }
        insertParcel(table, parcel); // Insert the parcel into the hash table
    }

    int close = fclose(file); // Close the file
    if (close != 0) // Check if the file was closed successfully
    {
        perror("Error closing file"); // Print an error message
    }
}

void displayMenu(struct HashTable* table)
{
    int choice;
    char country[21];
    char input[21];
    int weight;
    int isHigher;

    do // Display the menu
    {
        printf("\n1. Enter country name and display all the parcels details\n");
        printf("2. Enter country and weight pair\n");
        printf("3. Display the total parcel load and valuation for the country\n");
        printf("4. Enter the country name and display cheapest and most expensive parcels details\n");
        printf("5. Enter the country name and display lightest and heaviest parcel for the country\n");
        printf("6. Exit the application\n");
        printf("Enter your choice: ");
        char* errorCheck = fgets(input, 21, stdin); // Read the user input
        choice = atoi(input); // Convert the input to an integer
        if (errorCheck == NULL) // Check if the input was read successfully
        {
            printf("Error reading input\n");
            continue;
        }
        switch (choice)
        {
        case 1:
            printf("Enter country name: ");
            fgets(country, 21, stdin);
            if (errorCheck == NULL) // Check if the input was read successfully
            {
                printf("Error reading input\n");
                continue;
            }
            country[strcspn(country, "\n")] = '\0';
            displayParcels(table, country);
            break;
        case 2:
            printf("Enter country name: ");
            fgets(country, 21, stdin);
            if (errorCheck == NULL) // Check if the input was read successfully
            {
                printf("Error reading input\n");
                continue;
            }
            country[strcspn(country, "\n")] = '\0';
            printf("Enter weight: ");
            fgets(input, 21, stdin);
            if (errorCheck == NULL) // Check if the input was read successfully
            {
                printf("Error reading input\n");
                continue;
            }
            weight = atoi(input);
            printf("Display parcels with weight higher (1) or lower (0): ");
            fgets(input, 21, stdin);
            if (errorCheck == NULL) // Check if the input was read successfully
            {
                printf("Error reading input\n");
                continue;
            }
            isHigher = atoi(input);
            searchWeightForCountry(table, country, weight, isHigher);  // Search parcels by weight
            break;
        case 3:
            printf("Enter country name: ");
            fgets(country, 21, stdin);
            if (errorCheck == NULL) // Check if the input was read successfully
            {
                printf("Error reading input\n");
                continue;
            }
            country[strcspn(country, "\n")] = '\0';
            displayTotalForCountry(table, country); // Display the total weight and valuation
            break;
        case 4:
            printf("Enter country name: ");
            fgets(country, 21, stdin);
            if (errorCheck == NULL)
            {
                printf("Error reading input\n");
                continue;
            }
            country[strcspn(country, "\n")] = '\0';
            displayCheapestMostExpensive(table, country); // Display the cheapest and most expensive parcels
            break;
        case 5:
            printf("Enter country name: ");
            fgets(country, 21, stdin);
            if (errorCheck == NULL)
            {
                printf("Error reading input\n");
                continue;
            }
            country[strcspn(country, "\n")] = '\0';
            displayLightestHeaviest(table, country); // Display the lightest and heaviest parcels
            break;
        case 6:
            printf("Exiting application.\n");
            break;
        default:
            ;
        }
    } while (choice != 6);
}

// Main function
int main(void)
{
    struct HashTable table; // Create a hash table
    initializeHashTable(&table); // Initialize the hash table
    loadParcelsFromFile(&table, "couriers.txt"); // Load parcels from file
    displayMenu(&table); // Display the menu

    // Free allocated memory
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        freeMemory(table.table[i]); // Free memory for the BST
    }

    return 0;
}

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

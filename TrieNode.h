TrieNode.c                                                                                          0000644 0063363 0014010 00000002101 12621503207 013124  0                                                                                                    ustar   lamemmet                        ugrad_uw                                                                                                                                                                                                               // Emmett Lam
// CSE374 HW5
// TrieNode.c
//
// Methods used by the TrieNode struct

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TrieNode.h"

// Constructor method for a new TrieNode
// initializes 'word' field to input parameter or null
struct TrieNode* newNode(char* word) {
  struct TrieNode* new = (struct TrieNode*) malloc(sizeof(struct TrieNode));
  new->word = NULL;
  for (int i = 0; i < 9; i++) {
    new->children[i] = NULL;
  }
  if (word != NULL) {
    new->word = (char*) malloc(sizeof(char)*strlen(word));
    strncpy(new->word, word, strlen(word));
  }
  return new;
}

// Updates the word field of a node, allocates memory for the new string
void updateWord(struct TrieNode* node, char* word) {
  node->word = (char*) malloc(sizeof(char) * strlen(word));
  strncpy(node->word, word, strlen(word));
}

// Frees the memory of given node and all its children
void freeEverything(struct TrieNode* root) {
  if (root != NULL) {
    for (int i = 0; i < 9; i++) {
      freeEverything(root->children[i]);
    }
    free(root->word);
    free(root);
  }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                               t9.c                                                                                                0000644 0063363 0014010 00000011337 12621503104 011756  0                                                                                                    ustar   lamemmet                        ugrad_uw                                                                                                                                                                                                               // Emmett Lam
// CSE374 HW5
// t9.c
//
// This source file defines the interaction with the t9 program.
// This program:
//  * Accepts a dictionary file from the user
//  * Converts words in the dictionary fie to their t9 numeric equivalents
//  * Builds a trie tree that can search for a word given a numeric value

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TrieNode.h"

#define POUND_KEY 8
#define MAX_LINE_LENGTH   500
#define MAX_STRING_LENGTH 100

int getValue(char a);
struct TrieNode* buildTree(char* fileName);
struct TrieNode* searchTree(struct TrieNode* root, char* input);

int main(int argc, char** argv) {
  struct TrieNode* root;
  struct TrieNode* searchNode;

  if (argc == 0) {
    fprintf(stderr, "ERROR: No dictionary file provided\n");
    return 1;
  }
  root = buildTree(argv[1]);
  searchNode = root;

  char input[MAX_LINE_LENGTH];
  printf("Enter \"exit\" to quit.\n");
  printf("Enter Key Sequence (or \"#\" for next word)\n > ");
  while (fgets(input, MAX_LINE_LENGTH, stdin)) {
    if (strstr(input, "exit")) {
      freeEverything(root);
      exit(1);
    } else if (input[0] == '#') {
      if (searchNode != NULL) {
        searchNode = searchNode->children[POUND_KEY];
      }
      if (searchNode == NULL) {
        printf("    There are no more T9onyms\n");
      } else {
        printf("        \'%s\'\n", searchNode->word);
      }
    } else {
      searchNode = searchTree(root, input);
      if (searchNode == NULL || searchNode->word == NULL) {
        printf("    Not found in current dictionary.\n");
      } else {
        printf("        \'%s\'\n", searchNode->word);
      }
    }
    printf("Enter Key Sequence (or \"#\" for next word)\n > ");
  }
  freeEverything(root);
}

// maps an integer value to each of the alphabet characters used in t9
//    * Maps the '#' key to the last child index of a TrieNode
int getValue(char a) {
  switch (a) {
    case 'a':
    case 'b':
    case 'c':
      return 0;
    case 'd':
    case 'e':
    case 'f':
      return 1;
    case 'g':
    case 'h':
    case 'i':
      return 2;
    case 'j':
    case 'k':
    case 'l':
      return 3;
    case 'm':
    case 'n':
    case 'o':
      return 4;
    case 'p':
    case 'q':
    case 'r':
    case 's':
      return 5;
    case 't':
    case 'u':
    case 'v':
      return 6;
    case 'w':
    case 'x':
    case 'y':
    case 'z':
      return 7;
    case '#':
      return 8;
    default:
      return 10;
  }
}

// This method builds a Trie based on words provided in the given file
struct TrieNode* buildTree(char* fileName) {
  struct TrieNode* root;
  root = newNode(NULL);

  FILE* file = fopen(fileName, "r");
  char word[MAX_LINE_LENGTH];

  if (file == NULL) {
    fprintf(stderr, "ERROR: Failed to read file \"%s\"\n", fileName);
    exit(1);
  }
  // for each word in the file, create a new node for it in the trie
  // or add the word to an existing node
  while (fgets(word, MAX_LINE_LENGTH, file)) {
    word[strlen(word)-1] = '\0';
    struct TrieNode* currentNode = root;
    struct TrieNode* targetNode;

    for (int i = 0; i < strlen(word); i++) {
      int val = getValue(word[i]);
      targetNode = currentNode->children[val];

      // Case: No node exists at this child, create a new node
      if (targetNode == NULL) {
        if (i == strlen(word) - 1) {
          currentNode->children[val] = newNode(word);
        } else {
          currentNode->children[val] = newNode(NULL);
        }
     } else {
        // Case: A node exists, some additional error checking is needed
        if (i == strlen(word) - 1) {
          // The target node exists, but does not have a word assigned to it
          // update the nodes word field
          if (targetNode->word == NULL) {
            updateWord(targetNode, word);
          } else {
            // Target node has a word already, create a '#' linked child node
            while (targetNode->children[POUND_KEY] != NULL) {
              targetNode = targetNode->children[POUND_KEY];
            }
            targetNode->children[POUND_KEY] = newNode(word);
          }
        }
      }
      currentNode = currentNode->children[val];
    }
  }
  fclose(file);
  return root;
}

// Searches for the input number sequence given the root node
struct TrieNode* searchTree(struct TrieNode* root, char* input) {
  struct TrieNode* currentNode = root;

  // For each number in the input, go to the appropriate child
  // until all numbers have been exhausted
  for (int i = 0; i < strlen(input) - 1; i++) {
    int num = input[i] - '0';
    num -= 2;

    // '#' case, go to last child
    if (input[i] == '#') {
      num = 8;
    }
    currentNode = currentNode->children[num];
    if (currentNode == NULL) {
      return currentNode;
    }
  }
  return currentNode;
}
                                                                                                                                                                                                                                                                                                 Makefile                                                                                            0000644 0063363 0014010 00000000437 12620506657 012734  0                                                                                                    ustar   lamemmet                        ugrad_uw                                                                                                                                                                                                               #!/bin/bash
# Emmett Lam
# CSE374 HW5
# Makefile
#
# Compiles all the files for the t9 program

t9: t9.o TrieNode.o
	gcc -Wall -std=c99 -g -o t9 t9.o TrieNode.o

t9.o: TrieNode.h t9.c
	gcc -Wall -std=c99 -g -c t9.c

TrieNode.o: TrieNode.h TrieNode.c
	gcc -Wall -std=c99 -g -c TrieNode.c
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
// Emmett Lam
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

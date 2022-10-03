/*
Colleen Lemak
Professor Zhang
CPSC 346
Project 1
09/07/2022

To run: ./a.exe
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Define function prototypes
struct node* merge(struct node* head1, struct node* head2);
struct node* insert(struct node** head, int new_data);
struct node* mergesort(struct node* head, int size);
int get_len(struct node* head);
void print_list(struct node* node);

// Create struct of linked-list with nodes
struct node {
   int data;
   struct node* next;
};

// Combines two sub-lists and returns pointer to the new list
struct node* merge(struct node* head1, struct node* head2) {
   struct node* new_head = NULL;

   // if head1 is empty, link to head2
   if(head1 == NULL) {
      return head2;
   }
   // if head2 is empty, link to head1
   if(head2 == NULL) {
      return head1;
   }
   
   // keep merging recursively
   if(head1->data <= head2->data) {
      new_head = head1;
      new_head->next = merge(head1->next, head2);
   }
   else {
      new_head = head2;
      new_head->next = merge(head1, head2->next);
   }
   return new_head;
}

// Inserts a node with its head pointer and data value
struct node* insert(struct node** head, int new_data) {
   struct node* new_node = (struct node*) malloc(sizeof(struct node));
   new_node->data = new_data;
   new_node->next = *head;
   *head = new_node;

   return new_node;
}

// Sorts a list given its head pointer and size
struct node* mergesort(struct node* head, int size) {
  if(size <= 1)
   return head;
  
  int mid = size / 2;

  struct node* right_ptr = head;
  struct node* left_ptr = head;
  struct node* tmp = head;

  // loop to find first node on right
  for(int i = 0; i < mid - 1; i++)
      tmp = tmp->next;

  right_ptr = tmp->next;
  tmp->next = NULL;

  left_ptr = mergesort(head, mid);
  right_ptr = mergesort(right_ptr, size - mid);

  struct node* cur = (struct node*) malloc(sizeof(struct node));
  struct node* cur_head = cur;
  
  // splice back together
  while(left_ptr != NULL && right_ptr != NULL) {
    if(left_ptr->data < right_ptr->data) {
      cur->next = left_ptr;
      cur = cur->next;
      left_ptr = left_ptr->next;
    }
    else {
      cur->next = right_ptr;
      cur = cur->next;
      right_ptr = right_ptr->next;
    }

   // case if right partition finished before left
   while(left_ptr != NULL) {
      cur->next = left_ptr;
      cur = cur->next;
      left_ptr = left_ptr->next;
   }
   // case if left partition finished before right
   while(right_ptr != NULL) {
      cur->next = right_ptr;
      cur = cur->next;
      right_ptr = right_ptr->next;
   }
   
   right_ptr = cur_head->next;
   free(cur_head);

   return right_ptr;
  }
}

// Returns the length of a given linked list
int get_len(struct node* head) {
   int len = 0;
   struct node* cur = head;
   while(cur != NULL) {
      len++;
      cur = cur->next;
   }
   return len;
}

// Prints linked-list to console
void print_list(struct node* node) {
   while(node != NULL) 
   {
      printf("%d ", node->data);
      node = node->next;
   }
}


// Main program driver
int main() 
{
   struct node* head1 = NULL;
   struct node* head2 = NULL;
   struct node* list1 = NULL;
   struct node* list2 = NULL;
   struct node* list3 = NULL;
   struct node* list4 = NULL;

   // Create test cases for merge()
   insert(&list1, 9);
   insert(&list1, 5);
   insert(&list1, 3);

   insert(&list2, 7);
   insert(&list2, 6);
   insert(&list2, 1);

   printf("\n********************merge()*************************");

   printf("\nOriginal list1: \n");
   print_list(list1);

   printf("\nOriginal list2: \n");
   print_list(list2);

   head1 = merge(list1, list2);
   printf("\nMerged linked list: \n");
   print_list(head1);
   
   printf("\n****************************************************\n");


   // Create test cases for mergesort()
   insert(&list3, 28);
   insert(&list3, 12);
   insert(&list3, 2);

   insert(&list4, 3);
   insert(&list4, 44);
   insert(&list4, 7);

   printf("\n******************mergesort()***********************");

   printf("\nOriginal list3: \n");
   print_list(list3);

   printf("\nOriginal list4: \n");
   print_list(list4);

   head2 = merge((mergesort(list3, get_len(list3))), (mergesort(list4, get_len(list4))));
   printf("\nMerged linked list: \n");
   print_list(head2);

   printf("\n****************************************************\n");


   return 0;
}
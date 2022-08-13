/* LinkedList.c
 * 
 * Example linked list review done in class
 */
 
#include <stdio.h>
#include <stdlib.h> 
 
 //creates a node with one char and a pointer called letter_node
typedef struct node{
  char data;
  struct node *next;  //pointer still needs to be set to something
} letter_node; 
 
 
int printList(letter_node *start){
  letter_node *location = start;
  
  while(location != NULL){
	printf("%c", location->data);
	location = location->next;	
  }//while
  printf("\n");
  return 0;

}//printList 
 
 
int main(){
  
  letter_node first_node;
  
  first_node.data = 'c';
  
  first_node.next = NULL;
  
  printf("First node data: %c next: %p \n", first_node.data, first_node.next);
  
  letter_node *start = NULL;
  start = malloc(sizeof(letter_node));
  
  start->data = 'A';
  start->next = NULL;
  
  printf("Start is: %c and it points to: %p \n", start->data, start->next);
  
  //lets link the two variables
  start->next = &first_node;
  
//  first_node.next = start;
  
  printf("First node data: %c next: %p \n", first_node.data, first_node.next);
  printf("Start is: %c and it points to: %c \n", start->data, start->next->data);
  
  printf("Testing printList\n");
  printList(start); //takes a pointer to a node 
  
  return 0;
}//main
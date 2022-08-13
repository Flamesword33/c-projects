#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct node{
	char data;
	int is_upper_case;
	struct node *next;
	struct node *last;
} letter_node;

/* remove_doubles(pointer)
 * only use if you know you have doubles 
 * assumes you are on the second letter of the double
 * must assume that the you only have one pointer
 */
int remove_doubles(letter_node *double_letter){
	letter_node *temp = double_letter->last;
	double_letter = double_letter->next;
	//cut from the in front of the linked list
	printf("made it to here\n");
	temp->next->last = NULL;
	temp->next = double_letter;
	printf("and here\n");
	//cut from behind the linked list
	double_letter->last->next = NULL;
	double_letter->last = temp;
	printf("all done\n");
  return 0;
} //remove_doubles

 

/* main()
 * 
 */
int main(){
	letter_node *a;
	letter_node *b;
	letter_node *c;
	letter_node *d;
	
	a = malloc(sizeof(letter_node));
	b = malloc(sizeof(letter_node));
	c = malloc(sizeof(letter_node));
	d = malloc(sizeof(letter_node));
	
	a->next = b;
	b->next = c;
	c->next = d;
	d->next = NULL;
	
	a->data = 'e';
	b->data = 'e';
	c->data = ' ';
	d->data = 'l';
	
	a->last = NULL;
	b->last = a;
	c->last = b;
	d->last = c;
	
	printf("%c%c%c\n",a->next->data,b->data,c->last->data);
	remove_doubles(b);
	printf("%c%c", a->next->data,c->next->data );
	return 0;
}//main
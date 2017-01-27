/*
 * Annie Shen
 * ashen7 #1562848
 * CMPS 101 pa2
 * List.c
 */

#include<stdio.h>
#include<stdlib.h>
#include"List.h"

//structs -------------------------------------------------------

//Private NodeObj type
typedef struct NodeObj{
    int item;
    struct NodeObj* next;
    struct NodeObj* prev;
} NodeObj;

//Private Node type
typedef NodeObj* Node;

//Private ListObj type
typedef struct ListObj{
   Node head;
   Node tail;
   Node cursor;
   int numItems;
   int cIndex;
} ListObj;

//Constructors-Destructor ----------------------------------------

//newNode()
//Returns reference to new Node object. Initializes item, next, and prev fields.
Node newNode(int item) {
    Node N = malloc(sizeof(NodeObj));
    N->item = item;
    N->next = NULL;
    N->prev = NULL;
    return(N);
}

//freeNode()
//Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN){
    if( pN!=NULL && *pN!=NULL ){
        free(*pN); //prevent memory leak
        *pN = NULL;
    }
}

//newList()
//Returns reference to new empty List object.
List newList(void){
	List L = malloc(sizeof(ListObj));
	L->head=NULL;
	L->tail=NULL;
	L->cursor=NULL;
	L->numItems=0;
	L->cIndex=-1;

	return L;
}

//freeList()
//Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL){
	if(pL != NULL && *pL != NULL){
		while( length(*pL) != 0 ){
			deleteBack(*pL);
		}
		free(*pL);
		*pL=NULL;
	}
}

//Access functions ---------------------------------------------

//length()
//Returns the number of elements in this List.
int length(List L){
	return L->numItems;
}

//index()
//If cursor is defined, returns the index of the cursor element, otherwise returns -1.
int index(List L){
	if(L->cursor == NULL){
		return -1; //set cursor index out of range
	}
	return L->cIndex;
}

//front()
//Returns front element. Pre: length()>0
int front(List L){
	if(length(L) <= 0){
		printf("front() Error: List is empty");
	}
	return L->head->item;
}

//back()
//Returns back element. Pre: length()>0
int back(List L){
	if(length(L) <= 0){
		printf("back() Error: List is empty");
	}
	return L->tail->item;
}

//get()
//Returns cursor element. Pre: length()>0, index()>=0
int get(List L){
	if(length(L) <= 0){
		printf("get() Error: List is empty");
	}
	if(index(L) < 0){
		printf("get() Error: invalid index");
	}
	return L->cursor->item;
}

//equals(List A, List B)
//Returns true if A and B are the same integer sequence.
//The cursor is ignored in both lists.
int equals(List A, List B){
	if(length(A) != length(B)){
		return 0;
	}
	Node M = A->head;
	Node N = B->head;
	while(M != NULL && N != NULL){
		if(M->item == N->item){
			//If items match up, set next node as the new head and continue comparing
			M = M->next;
			N = N->next;
		}else	return 0;
	}
	return 1;
}

//clear()
//Resets this List to its original empty state.
void clear(List L){
	freeList(&L);
	L->head = NULL;
	L->tail = NULL;
	L->cursor = NULL;
	L->cIndex = -1;
	L->numItems = 0;
}

//moveFront()
//If List is non-empty, places the cursor under the front
//element, otherwise does nothing.
void moveFront(List L){
	if(length(L) > 0){
		L->cursor = L->head;
		L->cIndex = 0;
	}
}

//moveBack()
//If List is non-empty, places the cursor under the back element,
//otherwise does nothing.
void moveBack(List L){
	if(length(L) > 0){
		L->cursor = L->tail;
		L->cIndex = L->numItems -1;
	}
}

//movePrev()
//If cursor is defined and not at front, moves cursor one step toward
//front of this List, if cursor is defined and at front, cursor becomes
//undefined, if cursor is undefined does nothing.
void movePrev(List L){
	if(L->cursor == L->head){
		L->cursor = NULL;
		L->cIndex = -1;
	} else{
		L->cursor = L->cursor->prev;
		L->cIndex--;
	}
}

//moveNext()
//If cursor is defined and not at back, moves cursor one step toward
//back of this List, if cursor is defined and at back, cursor becomes
//undefined, if cursor is undefined does nothing.
void moveNext(List L){
	if(L->cursor != NULL){
		if(L->cursor == L->tail){
			L->cursor = NULL;
			L->cIndex = -1;
		} else{
			L->cursor = L->cursor->next;
			L->cIndex++;
		}
	}
	//Do nothing if cursor is null.
}

//prepend(List L, int data)
//Insert new element into this List. If List is non-empty,
//insertion takes place before front element.
void prepend(List L, int data){
	Node A = newNode(data);
	if(length(L) == 0){ //List is empty
		L->head = A;
		L->tail = A;
	}
	if(length(L) > 0){ //List is Not empty
		A->next = L->head;
		L->head->prev = A;
		L->head = A;
		if(L->cIndex != -1){
			L->cIndex++;
		}
	}
	L->numItems++; //increment number of items
}

//append(int data)
//Insert new element into this List. If List is non-empty,
//insertion takes place after back element.
void append(List L, int data){
	Node A = newNode(data);
	if(length(L) == 0){ //List is empty
		//System.out.println("reached here 1?");
		L->head = A;
		L->tail = A;
	} else if(length(L) > 0){ //List is Not empty
		L->tail->next = A; //Insert after tail
		A->prev = L->tail;
		L->tail = A;
	}
	L->numItems++;
}

//insertBefore(int data)
//Insert new element before cursor.
//Pre: length()>0, index()>=0
void insertBefore(List L, int data){
	if(length(L) <= 0){ //If the list is empty
		printf("insertBefore() Error: List is empty");
	}
	if(index(L) < 0){ //If cursor index is invalid
		printf("insertBefore() Error: invalid index");
	}

	Node A = newNode(data);
	if(length(L) == 1){ //if cursor is at head and null is in front of the cursor
		L->cursor->prev = A; //Set the null before the cursor to be the new node
		A->next = L->cursor; //Set new node's next as the cursor
	} else { // when length is more than 1, that cursor.prev is NOT a null
		Node temp = L->cursor->prev; //Save current cursor.prev as a new node named temp
		L->cursor->prev = A;
		A->next = L->cursor;
		if(temp != NULL){
			temp->next = A; //The old cursor.prev now is in front of node A
			A->prev = temp;
		}
		if(L->cursor == L->head){
			L->head = A;
		}
	}
	L->cIndex++;
	L->numItems++;
}

//insertAfter(int data)
//Inserts new element after cursor.
//Pre: length()>0, index()>=0
void insertAfter(List L, int data){
	if(length(L) <= 0){
		printf("insertAfter() Error: List is empty");
	}
	if(index(L) < 0){
		printf("insertAfter() Error: invalid index");
	}

	Node A = newNode(data);
	if(length(L) == 1){ //if cursor is at head and null is after the cursor
		L->cursor->next = A;
		A->prev = L->cursor;
	} else { // when length is more than 1, that cursor.prev is NOT a null
		Node temp = L->cursor->next; //Save current cursor.prev as a new node named temp
		L->cursor->next = A;
		A->prev = L->cursor;
		temp->prev = A; //The old cursor.prev now is in front of node A
		A->next = temp;
	}
	L->numItems++;
}

//deleteFront()
//Deletes the front element. Pre: length()>0
void deleteFront(List L){
	if(length(L) <= 0){
		printf("deleteFront() Error: List is empty");
	}
	if(length(L) == 1){ //If the list has only one element
		L->head = L->tail = L->cursor = NULL; //Reset
		L->cIndex = -1;
	} else{ //The list has more than 1 element
		L->head = L->head->next;
		L->head->prev = NULL;
		L->numItems--;
	}
}

//deleteBack()
//Deletes the back element. Pre: length()>0
void deleteBack(List L){
	if(length(L) <= 0){
		printf("deleteBack() Error: List is empty");
	}
	if(length(L) == 1){ //If the list has only one element
		L->head = L->tail = L->cursor = NULL; //Reset
		L->cIndex = -1;
	} else{ //The list has more than 1 element
		L->tail = L->tail->prev;
		L->tail->next = NULL;
		L->numItems--;
	}
}

//delete()
//Deletes cursor element, making cursor undefined.
//Pre: length()>0, index()>=0
void delete(List L){
	if(length(L) <= 0){
		printf("delete() Error: List is empty");
	}
	if(index(L) < 0){
		printf("delete() error: invalid index");
	}
	if(L->cIndex == 0){ //If cursor is at head
		deleteFront(L); //Call deleteFront() to do the same job
	} else if(L->cIndex == length(L) -1){ //If the cursor is at tail
		deleteBack(L); //Call deleteBack() to do the same job
	} else{ //If the cursor is in the middle of the list
		Node before = L->cursor->prev;
		Node after = L->cursor->next;
		before->next = after;
		after->prev = before;
		L->cursor = NULL;
		L->cIndex = -1;
		L->numItems--;
	}
}

void printList(FILE* out, List L){
	if(length(L) == 0){
		printf("printList() Error: calling empty list.");
		exit(EXIT_FAILURE);
	}
	Node N = L->head;
	for(; N != NULL; N = N->next){
		fprintf(out, "%d ", N->item);
	}
}


//Returns a new List representing the same integer sequence as this
//List. The cursor in the new list is undefined, regardless of the
//state of the cursor in this List. This List is unchanged.
List copyList(List L){
	List cp = newList();
	Node temp = L->head;
	while(temp != NULL){
		append(cp, temp->item);
		temp = temp->next;
	}
	return cp;
}

////Returns a new List which is the concatenation of this list followed
////by L. The cursor in the new List is undefined, regardless of the
////states of the cursors in this List and L. The states of this List
////and L are unchanged.
//List concatList(List A, List B);






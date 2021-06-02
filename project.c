#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------
| Type: Node
| Description:
---------------------------------------------------------------------*/
typedef struct Node Node;

struct Node{
  int* Ti;
  int head;
  int sdep;
  Node *slink;
  Node **child;
  Node *hook;
};

/*--------------------------------------------------------------------
| Type: Point
| Description:
---------------------------------------------------------------------*/
typedef struct State State;

struct State{
  Node *active_node;
  Node *last_newnode;
  int active_edge;
  int active_length;
  int remain;
};

/*--------------------------------------------------------------------
| Type: String
| Description:
---------------------------------------------------------------------*/
typedef struct{
  char *text;
  int size;
} string;

/*--------------------------------------------------------------------
| Global variables
---------------------------------------------------------------------*/
string **strings;
int n_strings;

int count;

Node *root;
State *s;

/*--------------------------------------------------------------------
| Function: read_string
| Description: Read string i from input and add '\0' at the end
---------------------------------------------------------------------*/
void read_string(int index){
  int i, c, size;

  i = 0;
  size = strings[index]->size;
  strings[index]->text = malloc((size + 1)*sizeof(char));

  while('\n' != (c = getchar())){
    strings[index]->text[i] = (char)c;
    i++;
  }

  strings[index]->text[i] = '$'; /*Terminator*/
}


/*--------------------------------------------------------------------
| Function: process_input
| Description: Read all the input file and saves the data
---------------------------------------------------------------------*/
void process_input(){
  int s_size, index;

  scanf("%d\n", &n_strings); /*Read number of strings*/

  index = 0;
  strings = malloc(n_strings * sizeof(string*));

  while(scanf("%d", &s_size) == 1){
    strings[index] = malloc(sizeof(string));
    strings[index]->size = s_size;
    getchar(); /*Read whitespace character*/
    read_string(index++);
  }
}


/*--------------------------------------------------------------------
| Function: print_strings
| Description: Print set of strings
---------------------------------------------------------------------*/
void print_strings(){
  int k, i;

  for(k = 0; k < n_strings; k++){
    for(i = 0; i < strings[k]->size + 1; i++){
      printf("%c", strings[k]->text[i]);
    }
    printf("\n");
  }
}


/*--------------------------------------------------------------------
| Function: free_strings
| Description: Free memory allocated for input
---------------------------------------------------------------------*/
void free_strings(){
  int k;

  for(k = 0; k < n_strings; k++){
    free(strings[k]->text);
    free(strings[k]);
  }
  free(strings);
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
Node *newNode(int istring, int start, int end){
  int i;

  Node *node = malloc(sizeof(Node));

  count++;

  if(node != NULL){

    node->Ti = malloc(n_strings*sizeof(int));

    for (i = 0; i < n_strings; i++){
      node->Ti[i] = 0;
    }

    node->child = malloc(5 * sizeof(Node*));

    for (i = 0; i < 5; i++){
      node->child[i] = NULL;
    }

    node->hook = NULL;
    node->slink = root;
    node->head = start;
    node->sdep = end;
    node->Ti[istring] = 1;
  }

  return node;
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
int map_char(int c){
  if(c == 'A') return 0;
  else if (c == 'C') return 1;
  else if (c == 'G') return 2;
  else if (c == 'T') return 3;
  else return 4;
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
int descend(int istring, Node *current){

  int length;

  length = current->sdep - current->head + 1;

  if(s->active_length >= length) {
    s->active_edge = (int)strings[istring]->text[s->active_edge + length];
    s->active_length -= length;
    s->active_node = current;
    return 1;
  }

  return 0;
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
Node *split_edge(int istring, int index, Node *current){
  int end, i;
  Node *split;

  end = current->head + s->active_length - 1;

  split = newNode(istring, current->head, end);
  s->active_node->child[map_char(s->active_edge)] = split;

  for(i = 0; i < n_strings; i++) {
    if(current->Ti[i] == 1){
      s->active_node->child[map_char(s->active_edge)]->Ti[i] = 1;
    }
  }

  split->child[map_char((int)strings[istring]->text[index])] = newNode(istring, index, strings[istring]->size);
  split->child[map_char((int)strings[istring]->text[index])]->Ti[istring] = 1;

  current->head += s->active_length;
  split->child[map_char(s->active_edge)] = current;

  return split;
}



/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
void extendTree(int istring, int index){

  s->remain++;

  s->last_newnode = NULL;


  while(s->remain > 0) {

    printf("active edge: %c\n", s->active_edge);

    if(s->active_length == 0) {
      s->active_edge = (int)strings[istring]->text[index];
    }

    if(s->active_node->child[map_char(s->active_edge)] == NULL){ /* There is no outgoing edge*/

      s->active_node->child[map_char(s->active_edge)] = newNode(istring, index, strings[istring]->size); /*Create leaf*/

      if(s->last_newnode != NULL) {
        s->last_newnode->slink = s->active_node;
        s->last_newnode = NULL;
      }
    }

    else{ /*There is an outgoing edge*/
      Node *split;
      Node *next = s->active_node->child[map_char(s->active_edge)];

      if(descend(istring, next)) {
        next->Ti[istring] = 1;
        continue;
      }

      /*character is already on the edge*/
      if(strings[istring]->text[next->head + s->active_length] == strings[istring]->text[index]) {
          printf("active length: %d \n", s->active_length);
          printf("char %c is already on the edge.\n", strings[istring]->text[next->head + s->active_length]);

        if(s->last_newnode != NULL && s->active_node != root) {
          s->last_newnode->slink = s->active_node;
          s->last_newnode = NULL;
        }

        s->active_length++;

        /*if(index == strings[istring]->size){
          next->Ti[istring] = 1;
        }*/

        break;

      }

      /*split edge*/
      split = split_edge(istring, index, next);

      if(s->last_newnode != NULL) {
        s->last_newnode->slink = split;
      }

      s->last_newnode = split;

    }

    s->remain--;

    if(s->active_node == root && s->active_length > 0) {
      s->active_length--;
      s->active_edge = (int)strings[istring]->text[index - s->remain + 1];
    }

    else if(s->active_node != root) {
      s->active_node = s->active_node->slink;
    }

  }

}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
void printPathLabel(int istring, int start, int end){

  int i;

  for(i = start; i <= end; i++) {
    printf("%c", strings[istring]->text[i]);
  }
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
/*void printTree(Node* node){

  int i;

  if(node == NULL) return;

  if(node->head != -1) printPathLabel(node->Ti, node->head, node->sdep);

  for(i = 0; i < 5; i++) {

    if(node->child[i] != NULL) {
      printTree(node->child[i]);
    }

  }

}*/


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
void printOccurrences(Node* node){

  int i, k;

  if(node == NULL) return;

  if(node->head != -1){

    for(k = 0; k < n_strings; k++) {

      if(node->Ti[k] == 1){
        printPathLabel(k, node->head, node->sdep);
        break;
      }
    }

    for(k = 0; k < n_strings; k++) {
      printf("  %d", node->Ti[k]);
    }

    printf("\n");
  }

  for(i = 0; i < 5; i++) {
    if(node->child[i] != NULL) {
      /*printf("%d\n",i);*/
      printOccurrences(node->child[i]);
    }
  }


}


/*--------------------------------------------------------------------
| Function: buildTree
| Description: Build the suffix tree using Ukkonen's algorithm
---------------------------------------------------------------------*/
void buildTree(){

  int k, i;
  k = 0;

  root = newNode(-1, -1, -1);
  root->slink = root;

  s = malloc(sizeof(State));

  while(k < n_strings){ /*For each string k*/
    i = 0;
    s->active_node = root;
    s->last_newnode = NULL;
    s->active_edge = -1;
    s->active_length = 0;
    s->remain = 0;

    printOccurrences(root);

    while(i < strings[k]->size + 1){ /*For each char of string k*/
      extendTree(k, i);
      i++;
    }
    k++;
  }
}


/*--------------------------------------------------------------------
| Function: main
| Description:
---------------------------------------------------------------------*/
int main(){
  count = 0;

  process_input();
  buildTree();

  /*printTree(root);*/
  printOccurrences(root);
  printf("\nNumber of nodes in suffix tree are %d\n", count);

  free(s);
  free_strings();

  return 0;




}

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
  int *sdep;
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
} String;

/*--------------------------------------------------------------------
| Global variables
---------------------------------------------------------------------*/
String *string;
int n_strings;
int count;

int *strings_sz;
int *leafEnd;
int *rootEnd;
int *splitEnd;

Node *root;
State *s;


/*--------------------------------------------------------------------
| Function: print_strings
| Description: Print set of strings
---------------------------------------------------------------------*/
void print_strings(){
  int i;

  for(i = 0; i < string->size; i++){
      printf("%c", string->text[i]);
  }
  printf("\n");

}


/*--------------------------------------------------------------------
| Function: read_string
| Description: Read string i from input and add '\0' at the end
---------------------------------------------------------------------*/
void read_string(int index){
  int i, c;

  i = index;

  while('\n' != (c = getchar())){
    string->text[i] = (char)c;
    i++;
  }

  string->text[i] = '$'; /*Terminator*/

  print_strings();
}


/*--------------------------------------------------------------------
| Function: process_input
| Description: Read all the input file and saves the data
---------------------------------------------------------------------*/
void process_input(){
  int s_size, index, k;

  scanf("%d\n", &n_strings); /*Read number of strings*/

  index = 0;
  k = 0;

  string = malloc(sizeof(String));
  string->text = malloc(sizeof(char));
  string->size = 0;

  strings_sz = malloc(n_strings*sizeof(int));

  while(scanf("%d", &s_size) == 1){
    string->size += s_size + 1;
    strings_sz[k] = string->size - 1;


    string->text = realloc(string->text, string->size*sizeof(char));

    getchar(); /*Read whitespace character*/
    read_string(index);

    index += string->size;
    k++;
  }

  string->text = realloc(string->text, (string->size+1)*sizeof(char));
  string->text[string->size] = '\0';
}


/*--------------------------------------------------------------------
| Function: free_strings
| Description: Free memory allocated for input
---------------------------------------------------------------------*/
void free_strings(){
  free(string->text);
  free(string);
}


/*--------------------------------------------------------------------
| Function: free_strings
| Description: Free memory allocated for input
---------------------------------------------------------------------*/
int map_index_string(int index){
  int i;

  for(i = 0; i < n_strings; i++) {
    if(index <= strings_sz[i]){
      return i;
    }
  }

  return 0;
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
Node *newNode(int start, int *end){
  int i;

  Node *node = malloc(sizeof(Node));

  count++;

  if(node != NULL){

    node->child = malloc(128 * sizeof(Node*));

    for (i = 0; i < 128; i++){
      node->child[i] = NULL;
    }

    node->hook = NULL;
    node->slink = root;
    node->head = start;
    node->sdep = end;
  }

  return node;
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
int map_char(int c){
  int i;

  if(65 == c) i = 0;
  else if (67 == c) i = 1;
  else if (71 == c) i = 2;
  else if (c == 84) i = 3;
  else i = 4;

  return i;
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
int descend(Node *current){
  int length;
  length = *current->sdep - current->head + 1;

  if(s->active_length >= length) {
    s->active_edge = (int)string->text[s->active_edge + length];
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
Node *split_edge(int index, Node *current){

  Node *split;

  splitEnd = (int*) malloc(sizeof(int));

  *splitEnd = current->head + s->active_length - 1;

  split = newNode(current->head, splitEnd);
  s->active_node->child[s->active_edge] = split;

  split->child[(int)string->text[index]] = newNode(index, &leafEnd[map_index_string(index)]);

  current->head += s->active_length;
  split->child[s->active_edge] = current;

  return split;
}



/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
void extendTree(int index){

  leafEnd[map_index_string(index)] = index;

  s->remain++;

  s->last_newnode = NULL;


  while(s->remain > 0) {

    if(s->active_length == 0) {
      s->active_edge = (int)string->text[index];
    }

    if(s->active_node->child[s->active_edge] == NULL){ /* There is no outgoing edge*/
      s->active_node->child[s->active_edge] = newNode(index, &leafEnd[map_index_string(index)]); /*Create leaf*/

      if(s->last_newnode != NULL) {
        s->last_newnode->slink = s->active_node;
        s->last_newnode = NULL;
      }
    }

    else{ /*There is an outgoing edge*/
      Node *split;
      Node *next;
      next = s->active_node->child[s->active_edge];

      if(descend(next)) {
        continue;
      }

      /*character is already on the edge*/
      if(string->text[next->head + s->active_length] == string->text[index]) {

        if(s->last_newnode != NULL && s->active_node != root) {
          s->last_newnode->slink = s->active_node;
          s->last_newnode = NULL;
        }

        s->active_length++;

        break;

      }

      /*split edge*/
      split = split_edge(index, next);

      if(s->last_newnode != NULL) {
        s->last_newnode->slink = split;
      }

      s->last_newnode = split;

    }

    s->remain--;

    if(s->active_node == root && s->active_length > 0) {
      s->active_length--;
      s->active_edge = (int)string->text[index - s->remain + 1];
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
void printPathLabel(int start, int end){

  int i;
  for(i = start; i <= end; i++) {
    printf("%c", string->text[i]);
  }
  printf("\n");
}


/*--------------------------------------------------------------------
| Function:
| Description:
---------------------------------------------------------------------*/
void printTree(Node* node){

  int i;

  if(node == NULL) return;

  if(node->head != -1) printPathLabel(node->head, *node->sdep);

  for(i = 0; i < 5; i++) {

    if(node->child[i] != NULL) {
      printTree(node->child[i]);
    }

  }

}


/*--------------------------------------------------------------------
| Function: buildTree
| Description: Build the suffix tree using Ukkonen's algorithm
---------------------------------------------------------------------*/
void buildTree(){

  int i, k;
  int terminator;

  char t1 = '$';
  char t2 = '$';

  terminator = 0;

  rootEnd = (int*)malloc(sizeof(int));
  *rootEnd = - 1;

  root = newNode(-1, rootEnd);
  root->slink = root;

  leafEnd = malloc(n_strings*sizeof(int));

  for(k = 0; k < n_strings; k++){
    leafEnd[k] = -1;
  }

  s = malloc(sizeof(State));

  i = 0;
  s->active_node = root;
  s->last_newnode = NULL;
  s->active_edge = -1;
  s->active_length = 0;
  s->remain = 0;
  splitEnd = NULL;

  while(i < string->size){ /*For each char of string k*/

    if(string->text[i] == t1){
      string->text[i] = t2;
      terminator = 1;
    }

    extendTree(i);

    if(terminator){
      string->text[i] = t1;
      terminator = 0;
    }

    i++;
  }

}


/*--------------------------------------------------------------------
| Function: main
| Description:
---------------------------------------------------------------------*/
int main(){
  count = 0;

  process_input();
  print_strings();

  /*buildTree();*()*/

  /*printTree(root);*/
  printf("---------------------------------------\n");
  printf("Number of nodes in suffix tree are %d\n", count);

  free(s);
  free_strings();

  return 0;




}

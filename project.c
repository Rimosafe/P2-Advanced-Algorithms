#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------
| Type: Node
| Description: Represents a node from the tree.
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
| Description: Can be root, internal node or a point in among the edge.
---------------------------------------------------------------------*/
typedef struct Point Point;

struct Point{
  Node *active_node;
  int active_edge;
  int active_length;
};

/*--------------------------------------------------------------------
| Type: String
| Description: Represents a string.
---------------------------------------------------------------------*/
typedef struct{
  char *text;
  int size;
} String;

/*--------------------------------------------------------------------
| Global variables
---------------------------------------------------------------------*/
String **strings;
String *string;
int n_strings;
int *strings_sz;

Node *root;
Node *last_newnode;
int *leafEnd;
int *rootEnd;
int *splitEnd;
Point *point;

int remain;

/*--------------------------------------------------------------------
| Function: print_strings
| Description: Print each string in each line.
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
| Function: print_concatenated
| Description: Print concatenated set of strings.
---------------------------------------------------------------------*/
void print_concatenated(){
  int i;

  for(i = 0; i < string->size; i++){
      printf("%c", string->text[i]);
  }
  printf("\n");

}

/*--------------------------------------------------------------------
| Function: init_string
| Description: Initialize variables regaring the concatenated string.
---------------------------------------------------------------------*/
void init_string(int size){

  string = malloc(sizeof(String));

  strings_sz = malloc(n_strings*sizeof(int));

  string->text = malloc(size*sizeof(char));

  string->size = size;
}

/*--------------------------------------------------------------------
| Function: free_strings
| Description: Free memory allocated dedicated to receive the input.
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
| Function: free_memory
| Description: Free memory allocated for auxiliar variables during the
|              program.
---------------------------------------------------------------------*/
void free_memory(){
  /*String variables*/
  free(string->text);
  free(string);
  free(strings_sz);

  /*Tree variables*/
  free(leafEnd);
  free(rootEnd);
  free(splitEnd);
  free(last_newnode);
  free(point);
}

/*--------------------------------------------------------------------
| Function: free_tree
| Description: Free memory allocated for the tree.
---------------------------------------------------------------------*/
void free_tree(Node *node){

  int i;

  if (node == NULL) return;

  for (i = 0; i < 5; i++) {
     if (node->child[i] != NULL) {
         free_tree(node->child[i]);
     }
  }

  free(node);
}

/*--------------------------------------------------------------------
| Function: read_string
| Description: Read string i from input and add terminator '$' at the
|              end.
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
| Description: Read all the input file and concatenate all the strings
|              in one. S = s1 + s2 + ... + sk
---------------------------------------------------------------------*/
void process_input(){
  int s_size, index, i, k, total_size;

  scanf("%d\n", &n_strings); /*Read number of strings*/

  strings = malloc(n_strings * sizeof(String*));

  index = 0;
  total_size = 0;

  while(scanf("%d", &s_size) == 1){
   strings[index] = malloc(sizeof(String));
   strings[index]->size = s_size + 1;

   total_size += strings[index]->size;

   getchar(); /*Read whitespace character*/
   read_string(index++);
  }

  init_string(total_size);

  index = 0;

  for(k = 0; k < n_strings; k++){
    for(i = 0; i < strings[k]->size; i++){
      string->text[index] = strings[k]->text[i];
      index++;
    }
    strings_sz[k] = index - 1;
  }

  free_strings();
}

/*--------------------------------------------------------------------
| Function: newNode
| Description: Creates new nodes.
---------------------------------------------------------------------*/
Node *newNode(int start, int *end){
  int i;

  Node *node = malloc(sizeof(Node));

  if(node != NULL){

    node->child = malloc(5 * sizeof(Node*));

    for (i = 0; i < 5; i++){
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
| Function: map_char
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
| Function: init_leafEnd
| Description: Init the array where each position representes string n
|              and saves the final position of that string.
---------------------------------------------------------------------*/
void init_leafEnd(){
  int k;

  leafEnd = malloc(n_strings*sizeof(int));

  for(k = 0; k < n_strings; k++){
    leafEnd[k] = -1;
  }

}

/*--------------------------------------------------------------------
| Function: init_Point
| Description: Init point values where the algorithm will start.
---------------------------------------------------------------------*/
void init_Point(){

  point = malloc(sizeof(Point));

  point->active_edge = -1;
  point->active_length = 0;
  point->active_node = root;

}

/*--------------------------------------------------------------------
| Function: create_root
| Description: Create root node.
---------------------------------------------------------------------*/
void create_root(){

  rootEnd = (int*)malloc(sizeof(int));
  *rootEnd = - 1;

  root = newNode(-1, rootEnd);
  root->slink = root;

}

/*--------------------------------------------------------------------
| Function: map_index_string
| Description: Map the current index to the string it belongs.
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
| Function: descend
| Description: Check if can jump to the next node without checking all
|              the character of the edge. Skip/Count trick.
---------------------------------------------------------------------*/
int descend(Node *current){
  int length;
  length = *current->sdep - current->head + 1;

  if(point->active_length >= length) {
    point->active_edge = (int)string->text[point->active_length+ length];
    point->active_length -= length;
    point->active_node = current;
    return 1;
  }

  return 0;
}

/*--------------------------------------------------------------------
| Function: split_edge
| Description: New leaf and new internal node get created.
---------------------------------------------------------------------*/
Node *split_edge(int index, Node *current){

  Node *split;

  splitEnd = (int*) malloc(sizeof(int));

  *splitEnd = current->head + point->active_length - 1;

  split = newNode(current->head, splitEnd);
  point->active_node->child[map_char(point->active_edge)] = split;

  split->child[map_char((int)string->text[index])] = newNode(index, &leafEnd[map_index_string(index)]);

  current->head += point->active_length;
  split->child[map_char(point->active_edge)] = current;

  return split;
}

/*--------------------------------------------------------------------
| Function: extendTree
| Description:
---------------------------------------------------------------------*/
void extendTree(int index){

  leafEnd[map_index_string(index)] = index;

  remain++;

  last_newnode = NULL;

  while(remain > 0) {

    if(point->active_length == 0) {
      point->active_edge = (int)string->text[index];
    }

    if(point->active_node->child[map_char(point->active_edge)] == NULL){ /* There is no outgoing edge*/
      point->active_node->child[map_char(point->active_edge)] = newNode(index, &leafEnd[map_index_string(index)]); /*Create leaf*/

      if(last_newnode != NULL) {
        last_newnode->slink = point->active_node;
        last_newnode = NULL;
      }
    }

    else{ /*There is an outgoing edge*/
      Node *split;
      Node *next;
      next = point->active_node->child[map_char(point->active_edge)];

      if(descend(next)) {
        continue;
      }

      /*character is already on the edge*/
      if(string->text[next->head + point->active_length] == string->text[index]) {

        if(last_newnode != NULL && point->active_node != root) {
          last_newnode->slink = point->active_node;
          last_newnode = NULL;
        }

        point->active_length++;

        break;

      }

      /*split edge*/
      split = split_edge(index, next);

      if(last_newnode != NULL) {
        last_newnode->slink = split;
      }

      last_newnode = split;

    }

    remain--;

    if(point->active_node == root && point->active_length > 0) {
      point->active_length--;
      point->active_edge = (int)string->text[index - remain + 1];
    }

    else if(point->active_node != root) {
      point->active_node = point->active_node->slink;
    }

  }

}

/*--------------------------------------------------------------------
| Function: buildTree
| Description: Build the suffix tree using Ukkonen's algorithm
---------------------------------------------------------------------*/
void buildTree(){

  int i, terminator;

  create_root();
  init_leafEnd();
  init_Point();

  remain = 0;
  last_newnode = NULL;
  splitEnd = NULL;

  i = 0;
  terminator = 0;

  while(i < string->size){ /*For each char of string k*/
    if(string->text[i] == '$'){
      string->text[i] = '#';
      terminator = 1;
    }

    extendTree(i);

    if(terminator){
      string->text[i] = '$';
      terminator = 0;
    }

    i++;
  }

}

/*--------------------------------------------------------------------
| Function: printPathLabel
| Description: Print edge label.
---------------------------------------------------------------------*/
void printPathLabel(int start, int end){

  int i;
  for(i = start; i <= end; i++) {
    printf("%c", string->text[i]);
  }
  printf("\n");
}

/*--------------------------------------------------------------------
| Function: printTree
| Description: Print the all edge labels.
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
| Function: main
| Description:
---------------------------------------------------------------------*/
int main(){

  process_input();


  buildTree();


  printTree(root);


  /*free_tree(root);
  free_memory();*/

  return 0;

}

#include <stdio.h>
#include <stdlib.h>


/*--------------------------------------------------------------------
| Type: String
| Description:
---------------------------------------------------------------------*/
typedef struct{
  char* text;
  int size;
} String;

/*--------------------------------------------------------------------
| Global variables
---------------------------------------------------------------------*/
String** strings;
int n_strings;

/*--------------------------------------------------------------------
| Function: read_string
| Description: Read string i from input and add '\0' at the end
---------------------------------------------------------------------*/
void read_string(int index){
  int i, c, size;

  i = 0;
  size = strings[index]->size;
  strings[index]->text = malloc((size + 1)*sizeof(char));

  while('\n' != (c = getchar())) {
    strings[index]->text[i] = (char)c;
    i++;
  }

  strings[index]->text[i] = '$';
}


/*--------------------------------------------------------------------
| Function: process_input
| Description: Read all the input file and saves the data
---------------------------------------------------------------------*/
void process_input(){
  int s_size, index;

  scanf("%d\n", &n_strings); /*Read number of strings*/

  index = 0;
  strings = malloc(n_strings * sizeof(String*));

  while(scanf("%d", &s_size) == 1) {
    strings[index] = malloc(sizeof(String));
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

  for(k = 0; k < n_strings; k++) {
    for(i = 0; i < strings[k]->size + 1; i++) {
      printf("%c", strings[k]->text[i]);
    }
    printf("\n");
  }
}

/*--------------------------------------------------------------------
| Function: main
| Description:
---------------------------------------------------------------------*/
int main(){
  process_input();
  /*print_strings();*/
  return 0;
}

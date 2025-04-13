#include "kernel/types.h"
#include "user/user.h"

#define LEN 9000


int main(int argc, char* argv[]) {
  
  print_pgtable(0, 0, 0);

  printf("\nAfter working with stack variable\n");
  char var = 1;
  print_pgtable(&var, 1, 2);


  printf("\nAfter working with array on stack\n");
  char arr_stack[LEN];
  arr_stack[0] = 1;
  arr_stack[8000] = 1;
  arr_stack[8500] = 1;
  print_pgtable(arr_stack, LEN, 1);


  printf("\nAfter creating array on heap\n");
  char* arr_heap = malloc(LEN);
  if ((uint64)arr_heap == -1) {
    printf("Malloc error\n");
  }
  print_pgtable(arr_heap, LEN, 3);


  printf("\nAfter reset flags D and A\n");
  reset_flags(arr_heap, LEN, 3);
  print_pgtable(arr_heap, LEN, 3);


  printf("\nAfter reading from array\n");
  char tmp = arr_heap[1000];
  var = tmp;
  print_pgtable(arr_heap, LEN, 3);


  printf("\nAfter changing array values\n");
  arr_heap[0] = 1;
  arr_heap[4000] = 1;
  arr_heap[8000] = 1;
  arr_heap[8500] = 1;
  print_pgtable(arr_heap, LEN, 3);
  

  printf("\nAfter freeing the array\n");
  free(arr_heap);
  print_pgtable(arr_heap, LEN, 3);


  printf("\nPages with flag A\n");
  print_pgtable(0, 0, 2);


  printf("\nPages with flag D\n");
  print_pgtable(0, 0, 1);


  printf("\nPages with flag A or D\n");
  print_pgtable(0, 0, 3);

}

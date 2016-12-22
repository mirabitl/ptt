#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <algorithm>
      

#define MAX 60000
#undef SHOWPASS
void print(int *a, int n) {
  int i;
  for (i = 0; i < n; i++)
    printf("%d\t", a[i]);
}
void radix_sort(uint32_t *a, int n) {
  int i, b[MAX], m = 0, exp = 1,npass=0;
  for (i = 0; i < n; i++) {
    if (a[i] > m)
      m = a[i];
  }
  while (m / exp > 0) {
    int box[10] = {
      0
    }
    ;
    for (i = 0; i < n; i++)
      box[a[i] / exp % 10]++;
    for (i = 1; i < 10; i++)
      box[i] += box[i - 1];
    for (i = n - 1; i >= 0; i--)
      b[--box[a[i] / exp % 10]] = a[i];
    for (i = 0; i < n; i++)
      a[i] = b[i];
    exp *= 10;
#ifdef SHOWPASS
    printf("\n\nPASS   : ");
    print(a, n);
#endif
    npass++;
  }
  //printf("%d passes \n",npass);
}
int main() {
  uint32_t arr[MAX],arc[MAX];
  int i, num;
  printf("\nEnter total elements (num < %d) : ", MAX);
  scanf("%d", &num);
  printf("\nEnter %d Elements : ", num);
  srand(num*num);
  for (i = 0; i < num; i++)
    arr[i]=rand();

  time_t t0= time(0);
  for (int k=0;k<100;k++)
    {
      memcpy(arc,arr,num*sizeof(uint32_t));
  //printf("\nARRAY  : ");
  //print(&arr[0], num);
      //std::sort(arc, arc + num);
       radix_sort(&arc[0], num);
  //printf("\n\nSORTED  : ");
    }
   time_t t1= time(0);
   printf("%d %d \n",t1,t0);
  return 0;
}

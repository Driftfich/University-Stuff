/*
   Sobel filter

        N               N          M
   y = Sum n(x)*m(x) + Sum n(x) + Sum m(x)
       x=1             x=1        x=1

 */

#define N 512

// two int vectors of length N
int n[N], m[N];

int main(void)
{
  int i;
  long y;

  /* init n[] and m[] with some values */
  // Takes O(N) time
  for (i=0; i<N; i++) {
    n[i] = i;
    m[i] = N-i;
  }

  /* Sobel operation */
  // Takes O(N) time
  y = 0;
  for (i=0; i<N; i++) {
    y = y + n[i]*m[i] + n[i] + m[i];
  }


}

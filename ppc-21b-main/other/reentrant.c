/* Credits
Reentrant Function
 https://www.geeksforgeeks.org/reentrant-function/
Writing reentrant and threadsafe code
 https://www.ibm.com/docs/en/aix/7.2?topic=programming-writing-reentrant-threadsafe-code

*/
/*
A reentrant function:
1. Does not hold static data over successive calls
2. Does not return a pointer to static data; all data is provided by the caller of the function
3. Uses local data or ensures protection of global data by making a local copy of it
4. Must not call any non-reentrant functions
*/


// ------------------------------------------
// Non-reentrant
  
int i;
  
int fun1() {
    return i * 5;
}
  
int fun2() {
   return fun1();
}

// ------------------------------------------
// Reentrant

int fun1(int i) {
    return i * 5;
}
  
int fun2(int i) {
   return fun1(i);
}





// ------------------------------------------
// Non-reentrant

char *strtoupper(char *string) {
  static char buffer[MAX_STRING_SIZE];
  int index;

  for (index = 0; string[index]; index++)
    buffer[index] = toupper(string[index]);
  buffer[index] = 0

  return buffer;
}


// ------------------------------------------
// Reentrant (a poor solution)

char *strtoupper(char *string) {
  char *buffer;
  int index;

  /* error-checking should be performed! */
  buffer = malloc(MAX_STRING_SIZE);

  for (index = 0; string[index]; index++)
    buffer[index] = toupper(string[index]);
  buffer[index] = 0

  return buffer;
}

// ------------------------------------------
// Reentrant (better solution)

char *strtoupper_r(char *in_str, char *out_str) {
  int index;

  for (index = 0; in_str[index]; index++)
    out_str[index] = toupper(in_str[index]);
  out_str[index] = 0

  return out_str;
}




// Problemas con swap no reentrante

main thread:

   swap()  // 2
   signal_handler() // Ctrl+C
   swap()  // 1
   main()

secondary thread:

   swap() // 3
   run()

 /*
 Considere la pila de invocaciones (se llena de abajo hacia arriba)
 Entre el swap 3 y los swaps 1/2 hay concurrencia. Entra en juego el concepto de thread-safe.
 
 Entre el swap 1 y el 2 no hay concurrencia porque es el mismo hilo, y por tanto, afecta si swap es o no reentrante.

 El signal_handler() es solo un ejemplo si se invoca, el OS interrumpe al primer swap(). 
 Pero el signal_handler() invoca a otro swap(). Aquí también hay problema si swap() no es reentrante.

 */
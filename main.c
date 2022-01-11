/**
 * Program that prints a random string between 8 and 2048 characters in
 * length to standard output.
 *
 * Author: Jose Hernandez
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

 /* Constants */
#define L_SIZE 26
#define N_SIZE 10
#define S_SIZE 29
#define ASCII_FOR_0 48
#define ASCII_FOR_9 57
#define LOWER_LEN 8
#define UPPER_LEN 2048
const char letters[L_SIZE] = "abcdefghijklmnopqrstuvwxyz";
const char numbers[N_SIZE] = "0123456789";
const char symbols[S_SIZE] = "~!@#$^&*()%\\-=_+[]{}|;:,./<>?";

/* Types */
typedef char (*randchar)(); // Function pointer type for functions that generate random char

/* Function Declarations */
int getopt(int, char**, char*);
int toupper(int);
char* generate_password(randchar*, int, int);
void print_password(char*, int);
char get_rand_symbol();
char get_rand_number();
char get_rand_u_letter();
char get_rand_l_letter();
long get_passwd_len(char*);
int validate_passwdlen(int);
int rand_num(int, int);
char* help();

int main(int argc, char* argv[]) {
   randchar* randfuncs = malloc(sizeof(randchar) * 4);
   int opt, opts_len = 0, passwdlen = 0;

   // Populates randfuncs if options are given, except -h.
   while ((opt = getopt(argc, argv, "snlLh")) != -1) {
      switch (opt) {
      case 's': *(randfuncs + opts_len++) = &get_rand_symbol; break;
      case 'n': *(randfuncs + opts_len++) = &get_rand_number; break;
      case 'l': *(randfuncs + opts_len++) = &get_rand_l_letter; break;
      case 'L': *(randfuncs + opts_len++) = &get_rand_u_letter; break;
      case 'h': printf("%s", help()); free(randfuncs); return 0; break;
      default: printf("%s", help()); free(randfuncs); return 0; break;
      }
   }

   // Obtain password length
   if (argc == 2) passwdlen = get_passwd_len(argv[1]);
   else if (argc == 3 && opts_len > 0) passwdlen = get_passwd_len(argv[2]);
   if (passwdlen == 0) {
      printf("%s", help());
      free(randfuncs);
      return 0;
   }
   // Seed rand with nanoseconds.
   struct timespec ts;
   timespec_get(&ts, TIME_UTC);
   srand(ts.tv_nsec);

   // Only generate a string with selected options.
   if (validate_passwdlen(passwdlen) && opts_len > 0) {
      if (opts_len < 4) randfuncs = realloc(randfuncs, sizeof(randchar) * opts_len);
      char* password = (char*)malloc(sizeof(char) * passwdlen);
      password = generate_password(randfuncs, opts_len, passwdlen);
      print_password(password, passwdlen);
      free(password);
   }
   // Generate a string with all options
   else if (validate_passwdlen(passwdlen) && opts_len == 0) {
      opts_len = 4;
      *(randfuncs + 0) = &get_rand_symbol;
      *(randfuncs + 1) = &get_rand_number;
      *(randfuncs + 2) = &get_rand_u_letter;
      *(randfuncs + 3) = &get_rand_l_letter;
      char* password = (char*)malloc(sizeof(char) * passwdlen);
      password = generate_password(randfuncs, opts_len, passwdlen);
      print_password(password, passwdlen);
      free(password);
   }

   free(randfuncs);
   return 0;
}

/* Randomly calls an array of randchar functions passwdlen times and appends
the random char result into the final password value. */
char* generate_password(randchar* funcs, int opts_len, int passwdlen) {
   char* password = malloc(sizeof(char) * passwdlen);
   for (int i = 0; i < passwdlen; i++)
      *(password + i) = funcs[(int)rand_num(0, opts_len)]();
   return password;
}

/* Outputs the password array of chars one character at a time. */
void print_password(char* password, int passwdlen) {
   for (int i = 0; i < passwdlen; i++)
      printf("%c", password[i]);
   printf("\n");
}

/* Retrieve random character from symbols array. */
char get_rand_symbol() {
   return symbols[rand_num(0, S_SIZE)];
}

/* Retrieve random character from numbers array. */
char get_rand_number() {
   return numbers[rand_num(0, N_SIZE)];
}

/* Retrieve random character from letters array and make it
uppercase. */
char get_rand_u_letter() {
   return toupper(letters[rand_num(0, L_SIZE)]);
}

/* Retrieve random character from letters array. */
char get_rand_l_letter() {
   return letters[rand_num(0, L_SIZE)];
}

/* Parses/Sanitizes a string for numbers and converts it into a long value. */
long get_passwd_len(char* str) {
   int len = strlen((char*)str);
   int* digits = (int*)malloc(sizeof(int) * len);
   int i = 0, num_of_digits = 0;
   // Retrieve only digits from potentially polluted input.
   for (i = 0; i < len; i++) {
      int ch = str[i];
      if (ch >= ASCII_FOR_0 && ch <= ASCII_FOR_9)
         *(digits + num_of_digits++) = str[i] - ASCII_FOR_0;
   }
   // Reduce memory allocation of digits if necessary
   if (num_of_digits < len) digits = realloc(digits, sizeof(int) * num_of_digits);
   // Generate final number
   long result = 0;
   int multiplier = 1; // Defines a digit's place value in the new long result
   for (i = num_of_digits; i > 0; i--) {
      int digit = *(digits + i - 1) * multiplier;
      result += digit;
      multiplier = multiplier * 10;
   }
   free(digits);
   return result;
}

/* Validates password within the length of the defined constants. */
int validate_passwdlen(int passwdlen) {
   return passwdlen >= LOWER_LEN && passwdlen <= UPPER_LEN;
}

/* Generates a random number within a range. */
int rand_num(int lower, int upper) {
   return (rand() % (upper - lower)) + lower;
}

/* Returns program description and usage information. */
char* help() {
   return
      "Usage: passgen [OPTION]... LENGTH\n"
      "  or   passgen LENGTH\n\n"
      "Generates a random string of length LENGTH, where LENGTH is "
      "an integer between 8 and 2048.\n\n"
      "NOTE: passgen LENGTH is equivalent to passgen -snlL LENGTH\n\n"
      "OPTIONS:\n"
      "     -h\t\tRead program description and view available options\n"
      "     -l\t\tInclude lowercase letters\n"
      "     -L\t\tInclude uppercase letters\n"
      "     -n\t\tInclude numbers\n"
      "     -s\t\tInclude symbols\n\n"
      ;
}

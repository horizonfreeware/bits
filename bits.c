#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void print_help() {
  printf(
  // logo, but it looks weird because of the escape chars
  " ____  _ _        \n"
  "| __ )(_) |_ ___  \n"
  "|  _ \\| | __/ __| \n"
  "| |_) | | |_\\__ \\ \n"
  "|____/|_|\\__|___/ \n"
  );
  printf("\n");
  printf("Usage: bits [-r,-v,-h] FILEPATH\n");
  printf("\n");
  printf("Options:\n");
  printf("\t-p FILEPATH: [p]retty\n");
  printf("\t-r FILEPATH: [r]aw binary\n");
  printf("\t-v FILEPATH: [v]erbose\n");
  printf("\t-h FILEPATH: [h]elp\n");
}

// Func - handle errors
int handle_error(char* message) {
  printf("Error: ");
  printf(message);
  printf("\n");
  print_help();
  exit(1);
}

// Func - print binary output
void print_raw(FILE *file_pointer_1, FILE *file_pointer_2) {
  int lineCount = 1;
  int charCount = 1;
  char *bitString;
  unsigned char byte;

  while (fread(&byte, sizeof(byte), 1, file_pointer_1) == 1) {
    for (int i = 7; i >= 0; i--) {
      bitString = bitString + (byte >> i);
      printf("%d", (byte >> i) & 1); // Print bit
    }
  }

  printf("\n\n");

  while (fread(&byte, sizeof(byte), 1, file_pointer_2) == 1) {
    if (byte == '\n') {
      printf("\\n");
      charCount = 1;
      lineCount += 1;
    }
    else if (byte == '\t') {
      printf("\\t");
    }
    else if (byte == '\r') {
      printf("\\r");
    }
    else if (byte == ' ') {
      printf("[space]");
      charCount += 1;
    }
    else {
      wchar_t unicode_char = byte;
      printf("%lc", unicode_char); // Print Unicode Value
      charCount += 1;
    }
  }

  fclose(file_pointer_1);
  fclose(file_pointer_2);
}

// Func - print detailed output
void print_verbose(FILE *file_pointer) {
  int lineCount = 1;
  int charCount = 1;
  unsigned char byte;
  setlocale(LC_CTYPE, "");
  while (fread(&byte, sizeof(byte), 1, file_pointer) == 1) {
    printf("%d:%-6d ", lineCount, charCount); // Index of Byte

    printf("0x%02x ", byte); // Print Hex

    for (int i = 7; i >= 0; i--) {
      printf("%d", (byte >> i) & 1); // Print bit
    }

    if (byte == '\n') {
      printf(" \\n      ");
      charCount = 1;
      lineCount += 1;
    }
    else if (byte == '\t') {
      printf(" \\t       ");
    }
    else if (byte == '\r') {
      printf(" \\r       ");
    }
    else if (byte == ' ') {
      printf(" [space] ");
      charCount += 1;
    }
    else {
      wchar_t unicode_char = byte;
      printf(" %lc       ", unicode_char); // Print Unicode Value
      charCount += 1;
    }
    printf("\n");

  }
  fclose(file_pointer);
}

void print_pretty(FILE *file_pointer) {

  // required vars
  int line_count = 1;
  int char_count = 1;
  int prior_count = 1;
  char prior_char;
  unsigned char byte;

  setlocale(LC_CTYPE, "");
  while (fread(&byte, sizeof(byte), 1, file_pointer) == 1) {
    if (prior_char == byte) {
      prior_count += 1;
      char_count += 1;
      if (byte == '\n') { line_count += 1; char_count = 1; }
    } else {

      // print count of prior byte
      if (prior_count > 1) printf(" (x%d)", prior_count);
      prior_count = 1;

      printf("\n");

      // index of byte
      printf("%d:%-6d ", line_count, char_count);

      // Print Hex
      printf("0x%02x ", byte);

      // Print bit
      for (int i = 7; i >= 0; i--) {
        printf("%d", (byte >> i) & 1);
      }

      if (byte == '\n') {
        printf(" \\n      ");
        char_count = 1;
        line_count += 1;
      }
      else if (byte == '\t') {
        printf(" \\t       ");
      }
      else if (byte == '\r') {
        printf(" \\r       ");
      }
      else if (byte == ' ') {
        printf(" [space] ");
        char_count += 1;
      }
      else {
        wchar_t unicode_char = byte;
        printf(" %lc       ", unicode_char); // Print Unicode Value
        char_count += 1;
      }

    }

    prior_char = byte;

  }
  fclose(file_pointer);
}

int main(int argc, char *argv[]) {
  FILE *fptr;
  FILE *fptr2;

  // HandleErr user only inputs one argument
  if (argc <= 2) handle_error("Only one argument provided");

  // open file path input
  fptr = fopen(argv[argc - 1], "rb");
  fptr2 = fopen(argv[argc - 1], "rb");

  // HandleErr file path invalid
  if(fptr == 0) handle_error("Path invalid");

  // argument parsing
  if (strcmp(argv[1], "-h") == 0)
    print_help();
  else if (strcmp(argv[1], "-r") == 0)
    print_raw(fptr, fptr2);
  else if (strcmp(argv[1], "-v") == 0)
    print_verbose(fptr);
  else if (strcmp(argv[1], "-p") == 0)
    print_pretty(fptr);
  else {
    handle_error("Flag not recongized");
  }

  return 0;
}

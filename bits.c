#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void print_help() {
printf(
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
  printf("\t-r: [r]aw binary\n");
  printf("\t-v: [v]erbose\n");
  printf("\t-h: [h]elp\n");
}

void print_raw(FILE *file_pointer) {
  char *bitString;
  unsigned char byte;
  while (fread(&byte, sizeof(byte), 1, file_pointer) == 1) {
    for (int i = 7; i >= 0; i--) {
      bitString = bitString + (byte >> i);
      printf("%d", (byte >> i) & 1); // Print bit
    }
  }
  printf("\n");
  fclose(file_pointer);
}

void print_verbose(FILE *file_pointer) {
  int byteCount = 0;
  unsigned char byte;
  setlocale(LC_CTYPE, "");
  while (fread(&byte, sizeof(byte), 1, file_pointer) == 1) {
    printf("Byte #: %-10d ", byteCount); // Index of Byte

    printf("0x%02x ", byte); // Print Hex

    for (int i = 7; i >= 0; i--) {
      printf("%d", (byte >> i) & 1); // Print bit
    }

    if (byte == '\n')
      printf(" \\n      ");
    else if (byte == '\t')
      printf(" \\t       ");
    else if (byte == '\r')
      printf(" \\r       ");
    else if (byte == ' ')
      printf(" [space] ");
    else {
      wchar_t unicode_char = byte;
      printf(" %lc       ", unicode_char); // Print Unicode Value
    }
    printf("\n");

    byteCount = byteCount + 1;
  }
  fclose(file_pointer);
}

int main(int argc, char *argv[]) {
  FILE *fptr;

  if (argc == 1) {
    print_help();
    return -1;
  }

  fptr = fopen(argv[argc - 1], "rb");

  if (argc < 3) {
    print_verbose(fptr);
  } else if (strcmp(argv[1], "-h") == 0) {
    print_help();
  } else if (strcmp(argv[1], "-r") == 0) {
    print_raw(fptr);
  } else if (strcmp(argv[1], "-v") == 0) {
    print_verbose(fptr);
  } else {
    printf("flag not recognized: %s\n", argv[2]);
  }

  return 0;
}

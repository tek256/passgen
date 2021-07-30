/*  USAGE:
 * ./passgen [-n/-uu/-us/-len]
 *  --use_nums, -use_nums, -un Use numbers in the generator (on by default)
    --use_upper, -use_upper, -uu Use uppercase in the generator (on by default)
    --use_symbols, -use_symbols, -us Use symbols in the generator (on by
 default)
    --length=x, -length=x, -len=x Generate specific length password (16 by
 default)
    --seed=x, -seed=x, -s=x Use specific seed
    --output=x, -out=x, -o=x Output to specific location*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static char symbol_list[] = {'!', '@', '#', '$', '%', '^', '&', '*', '(',
                             ')', '_', '-', '=', '+', ',', '<', '.', '>',
                             '?', '/', '[', '{', ']', '}', '|', '\\'};

// djb2
unsigned long hash(unsigned char* str) {
  unsigned long hash = 5381;
  int           c;
  while (c = *str++)
    hash = ((hash << 5) + hash) + c;

  return hash;
}

int main(int argc, const char** argv) {
  if (argc == 1) {
    printf("USAGE: \n"
           "--use_nums, -use_nums, -n         Use numbers in the generator (on "
           "by default)\n"
           "--use_upper, -use_upper, -uu      Use uppercase in the generator "
           "(on by default)\n"
           "--use_symbols, -use_symbols, -us  Use symbols in the generator (on "
           "by default)\n"
           "--length=x, -length=x, -len=x     Generate specific length "
           "password (16 by default)\n"
           "--seed=x, -seed=x, -seed=x        Use specific seed (default is "
           "current timestamp)\n"
           "--output=x, -out=x, -o            Output to specific location "
           "(default is stdout)\n"
           "--defaults, -defaults, -d         Use defaults to generate a "
           "password\n");
    return EXIT_SUCCESS;
  }

  int           length        = 16;
  int           is_length_set = 0;
  int           symbols       = 0;
  int           upper         = 0;
  int           numbers       = 0;
  unsigned long seed          = 0;
  const char*   output        = 0;

  for (int i = 1; i < argc; ++i) {
    char* obv = argv[i];
    while (*obv == '-') {
      *obv++;
    }

    if (*obv == 'u') {
      if (!strcmp(obv, "use_nums") || !strcmp(obv, "un")) {
        numbers = 1;
      } else if (!strcmp(obv, "use_symbols") || !strcmp(obv, "us")) {
        symbols = 1;
      } else if (!strcmp(obv, "use_upper") || !strcmp(obv, "uu")) {
        upper = 1;
      } else {
      }
    } else if (*obv == 'd') {
      if (!strcmp(obv, "default") || !strcmp(obv, "defaults") ||
          !strcmp(obv, "d")) {
        numbers = 1;
        symbols = 1;
        upper   = 1;
        if (!is_length_set)
          length = 16;
      }
    } else {
      char* delim = "=";
      char* tok   = strtok(obv, delim);

      tok = strtok(0, delim);

      if (!tok)
        continue;

      if (*obv == 's') {
        if (!strcmp(obv, "s") || !strcmp(obv, "seed")) {
          seed = hash(tok);
        }
      } else if (*obv == 'o') {
        if (!strcmp(obv, "output") || !strcmp(obv, "out") ||
            !strcmp(obv, "o")) {
          int len = strlen(tok);
          output  = calloc(len, sizeof(char));
          memcpy(output, tok, sizeof(char) * len);
        }
      } else if (*obv == 'l') {
        if (!strcmp(obv, "length") || !strcmp(obv, "len") ||
            !strcmp(obv, "l")) {
          length        = atoi(tok);
          is_length_set = 1;
        }
      }
    }
  }

  if (length == 0) {
    return EXIT_SUCCESS;
  }

  // number of characters required to meet minimum strength standard
  int remaining_required = numbers + symbols + upper;
  // number of characters placed
  int in_place       = 0;
  int symbols_placed = 0;
  int upper_placed   = 0;
  int numbers_placed = 0;

  if (seed) {
    srand(seed);
  } else {
    clock_t c = clock();
    srand(c);
  }

  char* str = (char*)calloc(length, sizeof(char));
  for (int i = 0; i < length; ++i) {
    int  remaining = length - in_place;
    char placement = 0;

    if (remaining == remaining_required) {
      if (symbols && !symbols_placed) {
        placement = symbol_list[rand() % 26];
        ++symbols_placed;
        --remaining_required;
      } else if (numbers && !numbers_placed) {
        placement = '0' + (rand() % ('9' - '0'));
        ++numbers_placed;
        --remaining_required;
      } else if (upper && !upper_placed) {
        placement = 'A' + (rand() % ('Z' - 'A'));
        ++upper_placed;
        --remaining_required;
      }
    } else {
      if (symbols && (rand() % 4) > 2) {
        placement = symbol_list[rand() % 26];
        if (!symbols_placed) {
          --remaining_required;
        }
        ++symbols_placed;
      } else if (numbers && (rand() % 4) > 2) {
        placement = '0' + (rand() % ('9' - '0'));
        if (!numbers_placed) {
          --remaining_required;
        }
        ++numbers_placed;
      } else if (upper && (rand() % 4) > 2) {
        placement = 'A' + (rand() % ('Z' - 'A'));
        if (!upper_placed) {
          --remaining_required;
        }
        ++upper_placed;
      } else {
        placement = 'a' + (rand() % ('z' - 'a'));
      }
    }

    str[i] = placement;
    ++in_place;
  }

  if (output) {
    FILE* f = fopen(output, "a");
    if (f) {
      fprintf(f, "%s", str);
      fclose(f);
    }
    free(output);
  } else {
    printf("%s\n", str);
  }

  free(str);

  return EXIT_SUCCESS;
}

#include "str2long.h"

int error;

typedef long (* fp)(const char *);

/*
 * in-memory testcase for speed test
 *
 * we want it to be cache line sized
 */
struct testcase {
  long int_result;
  int error_result;
#define TEST_STR_LEN 52
  char str[TEST_STR_LEN];
};

struct fps {
  fp func;
  const char *name;
  bool works;
};

static const struct fps funcs[] = {
  { str2long_john, "john", true },
  { str2long_pascal, "pascal", true },
  { str2long_toby, "toby", true },
  { str2long_bernd, "bernd", false }, // signed overflow
  { str2long_bernd_2, "bernd_2", true },
  { str2long_francois, "francois", false }, // signed overflow
  { str2long_francois_2, "francois_2", true },
  { str2long_yolanpa, "yolanpa", true },
  { str2long_patrick, "patrick", true },
  { str2long_daniel, "daniel", false }, // incorrect output
  { str2long_daniel_2, "daniel_2", true }, 
  { str2long_phil, "phil", true },
  { str2long_mikael, "mikael", false }, // incorrect output
  { str2long_mikael_2, "mikael_2", true },
  { str2long_magnus, "magnus", false }, // signed overflow
  // { str2long_magnus_2, "magnus_2", false }, // infinite loop due to being developed on MSVC
  { str2long_ryan, "ryan", true },
  { str2long_matthew, "matthew", false }, // signed overflow
  { str2long_matthew_2, "matthew_2", false }, // signed overflow
  { str2long_matthew_3, "matthew_3", true }, 
  { str2long_stefan, "stefan", true },
  { str2long_david, "david", false }, // incorrect output
  { str2long_david_2, "david_2", true },
  { str2long_mats, "mats", true },
  { str2long_peter, "peter", true },
  { str2long_thomas, "thomas", true },
  { str2long_ryanf, "ryanf", false }, // incorrect output
  { str2long_ryanf_2, "ryanf_2", false }, // signed overflow
  { str2long_mattias, "mattias", true },
  { str2long_matthewf, "matthewf", false }, // incorrect output
  { str2long_matthewf_2, "matthewf_2", true },
  { str2long_kevin, "kevin", true }, 
  { str2long_chucky, "chucky", false }, // incorrect output
  { str2long_chucky_2, "chucky_2", true },
  { str2long_andrew, "andrew", true },
  { str2long_tordek, "tordek", false }, // incorrect output
  { str2long_tordek_2, "tordek_2", true }, 
  { str2long_jeffrey, "jeffrey", true },
  { str2long_andrewh, "andrewh", false }, // incorrect output
  { str2long_andrewh_2, "andrewh_2", false },  // incorrect output
  { str2long_andrewh_3, "andrewh_3", false },  // incorrect output
  { str2long_yang, "yang", false }, // incorrect output in 32-bit mode
  { str2long_yang_2, "yang_2", true },
  { str2long_adam, "adam", false }, // incorrect output
  { str2long_adam_2, "adam_2", false }, // incorrect output
  { str2long_adam_3, "adam_3", false }, // incorrect output
  { str2long_adam_4, "adam_4", false }, // signed overflow
  { str2long_olivier, "olivier", true },
  { str2long_ahmed, "ahmed", false }, // incorrect output
  { str2long_ahmed_2, "ahmed_2", false }, // incorrect output
  { str2long_ken, "ken", false }, // incorrect output
  { str2long_ken_2, "ken_2", false }, // signed overflows
  { str2long_ken_3, "ken_3", false }, // signed overflows 
  { str2long_ken_4, "ken_4", true }, 
  { str2long_davidl, "davidl", true },
  { str2long_davidl_2, "davidl_2", true },
  { str2long_bastian, "bastian", true },
  { str2long_ethan, "ethan", false }, // incorrect output
  { str2long_ethan_2, "ethan_2", true }, 
  { str2long_renaud, "renaud", true },
  { str2long_davec, "davec", false },  // incorrect output
  { str2long_davec_2, "davec_2", false }, // incorrect output
  { str2long_dokoto, "dokoto", false }, // incorrect output
  { str2long_dokoto_2, "dokoto_2", false }, // incorrect outptu 
  { str2long_robert, "robert", false }, // incorrect output
  { str2long_robert_2, "robert_2", true }, 
  { str2long_till, "till", true },
  { str2long_gedare, "gedare", false }, // incorrect output
  { str2long_gedare_2, "gedare_2", false }, // incorrect output
  { str2long_markus, "markus", false }, // incorrect output
  { str2long_markus_2, "markus_2", false }, // incorrect output 
  { str2long_tennessee, "tennessee", true },
  { str2long_greg, "greg", true },
  { str2long_jonathan, "jonathan", false }, // incorrect output
  { str2long_dario, "dario", false }, // incorrect output in 32-bit mode
  { str2long_ben, "ben", true },
  { str2long_davide, "davide", true },   
  { str2long_sidney, "sidney", true },   
  { str2long_guillaume, "guillaume", false }, // incorrect output
  { NULL, NULL, false },
};

static long n_tests;
static int verbose = 0;

static void test (const char *s)
{
  // stupid compiler can't figure out these are initialized in the loop...
  long first_result = -1;
  int first_error = 1000;
  int i;
  if (verbose) fprintf (stderr, "\n'%s'\n", s);
  for (i=0; funcs[i].func; i++) {

    if (!funcs[i].works) continue;

    error = 0;
    long result = (funcs[i].func)(s);
    if (verbose && funcs[i].works) {
      fprintf (stderr, "%s : result = %ld, error = %d\n", funcs[i].name, result, error);
    }
    if (i==0) {
      assert (funcs[i].works);
      first_result = result;
      first_error = error;
    } else {
      assert (!funcs[i].works ||
	      ((first_error == 0 && error == 0 && first_result == result) ||
	       (first_error != 0 && error != 0))
	      );
    }
  }
  n_tests++;
}

static void inc (char *s)
{
  int i = 0;
  while (s[i] != 0) i++;
 repeat:
  i--;
  assert (i>=0);
  if (s[i]=='9') {
    s[i] = '0';
    goto repeat;
  }
  s[i]++;
}

int main (int argc, char *argv[]) {

  assert (sizeof(struct testcase) == 64);

  if (argc == 2 && strcmp (argv[1], "-v") == 0) verbose = 1;

  n_tests = 0;

#if 1
  test ("");
  test ("-");
  test ("0");
  test ("-0");
  test ("1");
  test ("2");
  test ("12");
  test ("-12");
  test ("3333");
  test ("-3333");
  test ("111111113331111");
  test ("-111133311111111");
  test ("-805");
  test ("000000000000000000000000000001");
  test ("000000000000000000010000000001");
  test ("100000000000000000010000000001");

  test ("1000000000000000001000000001");
  test ("100000000000000001000000001");
  test ("10000000000000001000000001");
  test ("1000000000000001000000001");
  test ("100000000000001000000001");
  test ("10000000000001000000001");
  test ("1000000000001000000001");
  test ("100000000001000000001");
  test ("10000000001000000001");
  test ("1000000001000000001");
  test ("100000001000000001");
  test ("10000001000000001");
  test ("1000001000000001");
  test ("100001000000001");
  test ("10001000000001");
  test ("1001000000001");
  test ("101000000001");
  test ("11000000001");
  test ("1000000001");

  test ("0000000000000000001000000001");
  test ("000000000000000001000000001");
  test ("00000000000000001000000001");
  test ("0000000000000001000000001");
  test ("000000000000001000000001");
  test ("00000000000001000000001");
  test ("0000000000001000000001");
  test ("000000000001000000001");
  test ("00000000001000000001");
  test ("0000000001000000001");
  test ("000000001000000001");
  test ("00000001000000001");
  test ("0000001000000001");
  test ("000001000000001");
  test ("00001000000001");
  test ("0001000000001");
  test ("001000000001");
  test ("01000000001");
  test ("1000000001");

  test ("9223372036854775805");
  test ("9223372036854775806");
  test ("9223372036854775807");
  test ("9223372036854775808");
  test ("9223372036854775809");
  test ("9223372036854775810");

  test ("-9223372036854775805");
  test ("-9223372036854775806");
  test ("-9223372036854775807");
  test ("-9223372036854775808");
  test ("-9223372036854775809");
  test ("-9223372036854775810");
#endif

#define RANGE 100000

#if 1
  {
    long l = LONG_MAX - RANGE;
    char str[50];
    sprintf (str, "%030ld", l);
    int i;
    for (i=0; i<(RANGE*2); i++) {
      test (str);
      inc (str);
    }
  }
#endif

#if 1
  {
    long l = LONG_MIN + RANGE;
    char str[50];
    sprintf (str, "%030ld", l);
    int i;
    for (i=0; i<(RANGE*2); i++) {
      test (str);
      inc (str);
    }
  }
#endif

#if 1
  {
    long l = 0;
    char str[50];
    sprintf (str, "%030ld", l);
    int i;
    for (i=0; i<RANGE; i++) {
      test (str);
      inc (str);
    }
  }
#endif

#if 1
  {
    long l = 0;
    char str[50];
    sprintf (str, "-%030ld", l);
    int i;
    for (i=0; i<RANGE; i++) {
      test (str);
      inc (str);
    }
  }
#endif

#if 1
  {
    srand (time(NULL));
    int i;
    for (i=0; i<(10*1000*1000); i++) {
      char str[35];
      int pos = 0;
      if (rand()%2==0) {
	str[pos] = '-';
	pos++;
      }
      int l = 1 + rand()%30;
      int j;
      for (j=0; j<l; j++) {
	str[pos] = '0' + rand()%10;
	pos++;
      }
      while (rand()%4 == 0) {
	str[rand()%l] = rand()%256;
      }
      str[pos] = 0;
      test (str);
    }
  }
#endif

  printf ("%ld tests succeeded\n", n_tests);
  int i;
  printf ("ran, but didn't test:\n");
  for (i=0; funcs[i].func; i++) {
    if (!funcs[i].works) printf ("  %s\n", funcs[i].name);
  }
  printf ("tested:\n");
  for (i=0; funcs[i].func; i++) {
    if (funcs[i].works) printf ("  %s\n", funcs[i].name);
  }

  return 0;
}

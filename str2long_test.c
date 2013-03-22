#include "str2long.h"

int error;

typedef long (* fp)(const char *);

#define CACHE_LINE_SIZE 64
#define TEST_STR_LEN (CACHE_LINE_SIZE-sizeof(int)-sizeof(long))

/*
 * in-memory testcase for speed test
 */
struct testcase {
  long result;
  int error;
  char str[TEST_STR_LEN];
};

#define REPS 6

struct fps {
  fp func;
  const char *name;
  bool works;
  double times[REPS];
};

static struct fps funcs[] = {
  { str2long_pascal, "pascal", true },
  // { str2long_john, "john", false },
  // { str2long_toby, "toby", false }, // infinite loop when strlen > INT_MAX
  // { str2long_bernd, "bernd", false }, // signed overflow
  { str2long_bernd_2, "bernd_2", true },
  // { str2long_francois, "francois", false }, // signed overflow
  { str2long_francois_2, "francois_2", true },
  { str2long_yolanpa, "yolanpa", true },
  { str2long_patrick, "patrick", true },
  // { str2long_daniel, "daniel", false }, // incorrect output
  { str2long_daniel_2, "daniel_2", true }, 
  { str2long_phil, "phil", true },
  // { str2long_mikael, "mikael", false }, // incorrect output
  { str2long_mikael_2, "mikael_2", true },
  // { str2long_magnus, "magnus", false }, // signed overflow
  // { str2long_magnus_2, "magnus_2", false }, // infinite loop due to being developed on MSVC
  { str2long_ryan, "ryan", true },
  // { str2long_matthew, "matthew", false }, // signed overflow
  // { str2long_matthew_2, "matthew_2", false }, // signed overflow
  { str2long_matthew_3, "matthew_3", true }, 
  { str2long_stefan, "stefan", true },
  // { str2long_david, "david", false }, // incorrect output
  { str2long_david_2, "david_2", true },
  { str2long_mats, "mats", true },
  { str2long_peter, "peter", true },
  { str2long_thomas, "thomas", true },
  // { str2long_ryanf, "ryanf", false }, // incorrect output
  // { str2long_ryanf_2, "ryanf_2", false }, // signed overflow
  { str2long_mattias, "mattias", true },
  // { str2long_matthewf, "matthewf", false }, // incorrect output
  { str2long_matthewf_2, "matthewf_2", true },
  { str2long_kevin, "kevin", true }, 
  // { str2long_chucky, "chucky", false }, // incorrect output
  { str2long_chucky_2, "chucky_2", true },
  { str2long_andrew, "andrew", true },
  // { str2long_tordek, "tordek", false }, // incorrect output
  { str2long_tordek_2, "tordek_2", true }, 
  { str2long_jeffrey, "jeffrey", true },
  // { str2long_andrewh, "andrewh", false }, // incorrect output
  // { str2long_andrewh_2, "andrewh_2", false },  // incorrect output
  // { str2long_andrewh_3, "andrewh_3", false },  // incorrect output
  // { str2long_yang, "yang", false }, // incorrect output in 32-bit mode
  { str2long_yang_2, "yang_2", true },
  // { str2long_adam, "adam", false }, // incorrect output
  // { str2long_adam_2, "adam_2", false }, // incorrect output
  // { str2long_adam_3, "adam_3", false }, // incorrect output
  // { str2long_adam_4, "adam_4", false }, // signed overflow
  { str2long_olivier, "olivier", true },
  // { str2long_ahmed, "ahmed", false }, // incorrect output
  // { str2long_ahmed_2, "ahmed_2", false }, // incorrect output
  // { str2long_ken, "ken", false }, // incorrect output
  // { str2long_ken_2, "ken_2", false }, // signed overflows
  // { str2long_ken_3, "ken_3", false }, // signed overflows 
  // { str2long_ken_4, "ken_4", false }, // fails for strlen > INT_MAX 
  { str2long_davidl, "davidl", true },
  { str2long_davidl_2, "davidl_2", true },
  { str2long_bastian, "bastian", true },
  // { str2long_ethan, "ethan", false }, // incorrect output
  { str2long_ethan_2, "ethan_2", true }, 
  { str2long_renaud, "renaud", true },
  // { str2long_davec, "davec", false },  // incorrect output
  // { str2long_davec_2, "davec_2", false }, // incorrect output
  // { str2long_dokoto, "dokoto", false }, // incorrect output
  // { str2long_dokoto_2, "dokoto_2", false }, // incorrect outptu 
  // { str2long_robert, "robert", false }, // incorrect output
  // { str2long_robert_2, "robert_2", false }, // fails for strlen > INT_MAX 
  { str2long_till, "till", true },
  // { str2long_gedare, "gedare", false }, // incorrect output
  // { str2long_gedare_2, "gedare_2", false }, // incorrect output
  // { str2long_markus, "markus", false }, // incorrect output
  // { str2long_markus_2, "markus_2", false }, // incorrect output 
  // { str2long_tennessee, "tennessee", false }, // fails for strlen > INT_MAX
  { str2long_greg, "greg", true },
  // { str2long_jonathan, "jonathan", false }, // incorrect output
  // { str2long_dario, "dario", false }, // incorrect output in 32-bit mode
  { str2long_ben, "ben", true },
  { str2long_davide, "davide", true },   
  { str2long_davide_3, "davide_3", true },   
  { str2long_sidney, "sidney", true },   
  // { str2long_guillaume, "guillaume", false }, // incorrect output
  { str2long_libc, "libc", true },
  { NULL, NULL, false },
};

static long n_tests;
static int verbose = 0;

#define MAX_TESTS 11000000
struct testcase *tests;

static void generate_test (const char *s)
{
  assert (n_tests < MAX_TESTS);
  assert (strlen(s) < TEST_STR_LEN);
  strcpy (tests[n_tests].str, s);
  error = 0;
  tests[n_tests].result = str2long_pascal (s);
  tests[n_tests].error = error;
  n_tests++;
}

static double subtimes (struct timespec stop, struct timespec start)
{
  double stopd = (1000000000.0 * stop.tv_sec) + stop.tv_nsec;
  double startd = (1000000000.0 * start.tv_sec) + start.tv_nsec;
  return stopd - startd;
}

static void run_tests (void)
{
  int rep;
  for (rep=0; rep<REPS; rep++) {
    int i;
    for (i=0; funcs[i].func; i++) {
      struct timespec start;
      int res = clock_gettime (CLOCK_MONOTONIC_RAW, &start);
      assert (res==0);
      int j;
      for (j=0; j<n_tests; j++) {
	error = 0;
	long result = (funcs[i].func)(tests[j].str);
	assert (
		((error == 0 && tests[j].error == 0 && result == tests[j].result) ||
		 (error != 0 && tests[j].error != 0))
		);
      }
      struct timespec stop;
      res = clock_gettime (CLOCK_MONOTONIC_RAW, &stop);
      assert (res==0);
      funcs[i].times[rep] = subtimes (stop, start);
    }
  }
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

  assert (sizeof(struct testcase) == CACHE_LINE_SIZE);

  if (argc == 2 && strcmp (argv[1], "-v") == 0) verbose = 1;

  n_tests = 0;
  tests = (struct testcase *) malloc (MAX_TESTS * sizeof (struct testcase));
  assert (tests);

  // #define HUGE_TEST 1

#ifdef HUGE_TEST
  {
#define L (6L * 1024 * 1024 * 1024)
    char *s = (char *) malloc (L);
    assert (s);
    long i;

    for (i=0; i<L; i++) {
      s[i] = '0';
    }
    s[0] = '-';
    s[L-1] = 0;
    s[L-2] = '7';
    for (i=0; funcs[i].func; i++) {
      if (!funcs[i].works) continue;
      error = 0;
      long result = (funcs[i].func)(s);
      printf ("%s : %ld %d\n", funcs[i].name, result, error);
      assert (result==-7 && error == 0);
    }

    for (i=0; i<L; i++) {
      s[i] = '0';
    }
    s[L-1] = 0;
    s[L-2] = 'z';
    s[L-3] = '7';
    for (i=0; funcs[i].func; i++) {
      if (!funcs[i].works) continue;
      error = 0;
      long result = (funcs[i].func)(s);
      printf ("%s : %ld %d\n", funcs[i].name, result, error);
      assert (error == 1);
    }
  }
  return 0;
#endif
  
#if 1
  generate_test ("");
  generate_test ("-");
  generate_test ("0");
  generate_test ("-0");
  generate_test ("1");
  generate_test ("2");
  generate_test ("12");
  generate_test ("-12");
  generate_test ("3333");
  generate_test ("-3333");
  generate_test ("111111113331111");
  generate_test ("-111133311111111");
  generate_test ("-805");
  generate_test ("000000000000000000000000000001");
  generate_test ("000000000000000000010000000001");
  generate_test ("100000000000000000010000000001");
  generate_test ("1000000000000000001000000001");
  generate_test ("100000000000000001000000001");
  generate_test ("10000000000000001000000001");
  generate_test ("1000000000000001000000001");
  generate_test ("100000000000001000000001");
  generate_test ("10000000000001000000001");
  generate_test ("1000000000001000000001");
  generate_test ("100000000001000000001");
  generate_test ("10000000001000000001");
  generate_test ("1000000001000000001");
  generate_test ("100000001000000001");
  generate_test ("10000001000000001");
  generate_test ("1000001000000001");
  generate_test ("100001000000001");
  generate_test ("10001000000001");
  generate_test ("1001000000001");
  generate_test ("101000000001");
  generate_test ("11000000001");
  generate_test ("1000000001");

  generate_test ("0000000000000000001000000001");
  generate_test ("000000000000000001000000001");
  generate_test ("00000000000000001000000001");
  generate_test ("0000000000000001000000001");
  generate_test ("000000000000001000000001");
  generate_test ("00000000000001000000001");
  generate_test ("0000000000001000000001");
  generate_test ("000000000001000000001");
  generate_test ("00000000001000000001");
  generate_test ("0000000001000000001");
  generate_test ("000000001000000001");
  generate_test ("00000001000000001");
  generate_test ("0000001000000001");
  generate_test ("000001000000001");
  generate_test ("00001000000001");
  generate_test ("0001000000001");
  generate_test ("001000000001");
  generate_test ("01000000001");
  generate_test ("1000000001");

  generate_test ("9223372036854775805");
  generate_test ("9223372036854775806");
  generate_test ("9223372036854775807");
  generate_test ("9223372036854775808");
  generate_test ("9223372036854775809");
  generate_test ("9223372036854775810");

  generate_test ("-9223372036854775805");
  generate_test ("-9223372036854775806");
  generate_test ("-9223372036854775807");
  generate_test ("-9223372036854775808");
  generate_test ("-9223372036854775809");
  generate_test ("-9223372036854775810");
#endif

#if 0
  generate_test ("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
  generate_test ("-000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
  generate_test ("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000099999999999999999999999999999999999999999999999");
  generate_test ("-0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009999999999999999999999999999999");
#endif

#define RANGE 20000

#if 1
  {
    long l = LONG_MAX - RANGE;
    char str[50];
    sprintf (str, "%030ld", l);
    int i;
    for (i=0; i<(RANGE*2); i++) {
      generate_test (str);
      inc (str);
    }
  }

  {
    long l = LONG_MIN + RANGE;
    char str[50];
    sprintf (str, "%030ld", l);
    int i;
    for (i=0; i<(RANGE*2); i++) {
      generate_test (str);
      inc (str);
    }
  }

  {
    long l = 0;
    char str[50];
    sprintf (str, "%030ld", l);
    int i;
    for (i=0; i<RANGE; i++) {
      generate_test (str);
      inc (str);
    }
  }

  {
    long l = 0;
    char str[50];
    sprintf (str, "-%030ld", l);
    int i;
    for (i=0; i<RANGE; i++) {
      generate_test (str);
      inc (str);
    }
  }
#endif

#if 1
  {
    srand (time(NULL));
    int i;
    for (i=0; i<(2*1000*1000); i++) {
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
      generate_test (str);
    }
  }
#endif

  printf ("generated %ld tests\n", n_tests);

  run_tests();

  int i;
  for (i=0; funcs[i].func; i++) {
    if (funcs[i].works) {
      int j;
      double min = 1e22;
      double max = 0;
      for (j=0; j<REPS; j++) {
	double t = funcs[i].times[j] / n_tests;
	if (t<min) min = t;
	if (t>max) max = t;
      }
      printf ("%lf %s\n", min, funcs[i].name);
    }
  }

  return 0;
}

#include <cstdlib>
#include <iostream>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>

using namespace std;

#ifdef NOOUTPUT
    #define print(x)
#else
    #define print(x) x
#endif

long int freq = 5;

jmp_buf error_handler;

long int Ackermann(long int m, long int n) {
    if (m == 0) {
        if (random() % freq == 0) {
            longjmp(error_handler, 1);
        }

        return n + 1;
    } else if (n == 0) {
        if (random() % freq == 0) {
            longjmp(error_handler, 1);
        }

        jmp_buf previous_handler;
        memcpy(&previous_handler, &error_handler, sizeof(jmp_buf));
        if (!setjmp(error_handler)) {
            long int retval = Ackermann(m - 1, 1);
            memcpy(&error_handler, &previous_handler, sizeof(jmp_buf));
            return retval;
        } else {
            print(cout << "E1 " << m << " " << n << endl);
        }
    } else {
        jmp_buf previous_handler;
        memcpy(&previous_handler, &error_handler, sizeof(jmp_buf));
        if (!setjmp(error_handler)) {
            jmp_buf previous_handler_param;
            memcpy(&previous_handler_param, &error_handler, sizeof(jmp_buf));
            long int retval_param = Ackermann(m, n - 1);
            memcpy(&error_handler, &previous_handler_param, sizeof(jmp_buf));

            long int retval = Ackermann(m - 1, retval_param);
            memcpy(&error_handler, &previous_handler, sizeof(jmp_buf));
            return retval;
        } else {
            print(cout << "E2 " << m << " " << n << endl);
        }
    }

    return 0;
}

int main(int argc, const char *argv[]) {
    long int Ackermann(long int m, long int n);
    long int m = 4;
    long int n = 6;
    long int seed = getpid();

    switch (argc) {
      case 5: freq = atoi(argv[4]);
      case 4: seed = atoi(argv[3]);
      case 3: n = atoi(argv[2]);
      case 2: m = atoi(argv[1]);
    }

    srandom(seed);
    cout << m << " " << n << " " << seed << " " << freq << endl;

    if (!setjmp(error_handler)) {
        cout << Ackermann(m, n) << endl;
    } else {
        print(cout << "E3" << endl);
    }
}

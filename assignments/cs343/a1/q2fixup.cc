#include <iostream>
#include <functional>

using namespace std;

#ifdef NOOUTPUT
    #define print(x)
#else
    #define print(x) x
#endif

int C(int i, int n, std::function<void(int &, int &)> *fixup1,
      std::function<void(int &)> *fixup2, std::function<void(int)> *fixup3) {
    print(cout << i << " " << n << endl);
    (*fixup1)(i, n);

    print(cout << i << " " << n << endl);
    (*fixup2)(n);

    print(cout << i << " " << n << endl);
    (*fixup3)(27);

    return i;
}

int B(int i, int n, std::function<void(int &, int &)> *fixup1,
      std::function<void(int &)> *fixup2, std::function<void(int)> *fixup3) {

    if (i > 0) {
        B(i - 1, n, fixup1, fixup2, fixup3);
    }

    return C(i, n, fixup1, fixup2, fixup3);
}

int A(int m, int n, int times) {
    int k = 27, ret;

    std::function<void(int &, int &)> fixup1 = [&m, &n](int &a, int &b) {
        a = m;
        b = n;
    };
    std::function<void(int &)> fixup2 = [&k](int &a) {
        a = k;
    };
    std::function<void(int)> fixup3 = [&n](int a) {
        print(cout << a << " " << n << endl);
    };

    for (int i = 0; i < times; i += 1) {
        ret = B(i, n, &fixup1, &fixup2, &fixup3);
    }

    return ret;
}

int main(int argc, char* argv[]) {
    long int m = 4, n = 6, times = 1;

    switch (argc) {
      case 4: times = atoi(argv[3]);
      case 3: n = atoi(argv[2]);
      case 2: m = atoi(argv[1]);
    }

    cout << m << " " << n << " " << A(m, n, times) << endl;
}

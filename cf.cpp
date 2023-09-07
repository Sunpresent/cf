/**
 * @author 4Zephyr.
 * @date 2023-09-08.
 */

#include <bits/stdc++.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

#define forn(s, n) for (int i(s); i < int(n); i++)
#define rforn(s, n) for (int i(s); i > int(n); i--)

#define err_exit(...)                                                                              \
    do {                                                                                           \
        printf(__VA_ARGS__);                                                                       \
        _exit(-1);                                                                                 \
    } while (0);

#define perror_exit(s)                                                                             \
    do {                                                                                           \
        perror(s);                                                                                 \
        _exit(-1);                                                                                 \
    } while (0);

#define ASSERT(v)                                                                                  \
    do {                                                                                           \
        if (v)                                                                                     \
            perror_exit("ASSERT");                                                                 \
    } while (0);

int t, n, opt;
int main(int argc, char **argv)
{
    while ((opt = getopt(argc, argv, ":t:")) != -1) {
        switch (opt) {
        case 't':
            break;
        default:
            printf("Bad option\n");
            _exit(1);
        }
    }

    cin >> t;
    while (t--) {
        cin >> n;
    }

    return 0;
}

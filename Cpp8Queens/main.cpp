#include <string.h>

#include "main.h"
#include "queens.h"


namespace globals
{
} // Someone's been around Python users too long. 


int main(int argc, const char **argv)
{
    if (argc > 1 && 0 == strcmp(argv[1], "-v"))
        qns::set_verbose(true);

    qns::solve();

    return 0;
}
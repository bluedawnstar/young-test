#include "ytest.h"

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++)
        ytest::TestFramework::instance().addFilters(argv[i], ',');

    ytest::TestFramework::instance().run();

    return 0;
}

#include "options.h"
#include <algorithm>

const struct option Options::sGetOpts[] = {
    { "testing", no_argument, NULL, 't' },
    { "help", no_argument, NULL, 'h' },
    { NULL, 0, NULL, 0 }
};

const Options::OptionHelp Options::sGetOptsHelp[] = {
    { "", "Put this in testing mode sending out predictable alternating test positions" },
    { "", "Print help" },
    { NULL, NULL },
};

Options::Options()
    : mArgc(0)
    , mArgv(nullptr)
    , mTesting(false)
{
}

void Options::printHelp()
{
    int i;
    const char* name = (mArgc == 0) ? "unknown" : mArgv[0];
    printf("Usage for %s:\n", name);

    for (i = 0; (sGetOpts[i].name != NULL) && (sGetOptsHelp[i].argument != NULL); i++) {
        char tmpstr[2];
        tmpstr[0] = (char)sGetOpts[i].val;
        tmpstr[1] = '\0';

        printf("\t--%-15.15s %s%1.1s  %-8.8s %-37.37s\n",
               sGetOpts[i].name,
               sGetOpts[i].flag == NULL ? "-" : " ",
               sGetOpts[i].flag == NULL ? tmpstr : " ",
               sGetOptsHelp[i].argument,
               sGetOptsHelp[i].description);
        std::string desc(sGetOptsHelp[i].description);
        std::string remainingDesc(desc.substr(std::min(37, (int)desc.length())));
        while (!remainingDesc.empty())
        {
            printf("\t                               %-37.37s\n", remainingDesc.c_str());
            remainingDesc = remainingDesc.substr(std::min(37, (int)remainingDesc.length()));
        }
    }
}

int Options::parseArgs(int argc, char** argv)
{
    mArgc = argc;
    mArgv = argv;

    int c;
    while (1) {
        int option_index = 0;
        c = getopt_long(mArgc, mArgv, "th",
                        sGetOpts, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 't':
            mTesting = true;
            break;

        case 'h':
            printHelp();
            exit(EXIT_FAILURE);
            break;

        case '?':
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
            break;
        }
    }

    if (optind < mArgc) {
        printf("non-option ARGV-elements: ");
        while (optind < mArgc)
            printf("%s ", mArgv[optind++]);
        printf("\n");
    }

    return (0);
}

#ifndef OPTIONS_H
#define OPTIONS_H

#include <getopt.h>

class Options
{
public:
    Options();

    int parseArgs(int argc, char**argv);
    void printHelp();

    inline bool isTesting() const { return mTesting; }

private:
    struct OptionHelp {
        const char * argument;
        const char * description;
    };

    const static struct option sGetOpts[];
    const static OptionHelp sGetOptsHelp[];

    int mArgc;
    char** mArgv;

    bool mTesting;
};

#endif // OPTIONS_H

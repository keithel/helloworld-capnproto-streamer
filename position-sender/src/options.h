#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <getopt.h>

enum class LogLevel{ VERB_ERROR = 0, VERB_WARN, VERB_DEBUG};

class Options
{
public:
    Options();

    int parseArgs(int argc, char**argv);
    void printHelp();

    inline LogLevel getVerbosity() { return mVerbosity; }
    inline std::string getRpmsgFile() { return mRpmsgFile; }
    inline uint32_t getFrequencyDivisor() { return mDivisor; }
    inline uint32_t getLoopCount() { return mLoopCount; }
    inline bool isTesting() { return mTesting; }

private:
    struct OptionHelp {
        const char * argument;
        const char * description;
    };

    const static struct option sGetOpts[];
    const static OptionHelp sGetOptsHelp[];

    int mArgc;
    char** mArgv;

    LogLevel    mVerbosity;
    std::string mRpmsgFile;
    uint32_t    mDivisor;
    uint32_t    mLoopCount;
    bool mTesting;
};

#endif // OPTIONS_H

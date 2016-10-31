#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <getopt.h>

enum class LogLevel{ VERB_ERROR = 0, VERB_WARN, VERB_INFO, VERB_DEBUG};

class Options
{
public:
    Options();

    int parseArgs(int argc, char**argv);
    void printHelp();

    inline LogLevel getVerbosity() const { return mVerbosity; }
    inline std::string getRpmsgFile() const { return mRpmsgFile; }
    inline std::string getDestAddr() const { return mDestIp; }
    inline uint32_t getFrequencyDivisor() const { return mDivisor; }
    inline uint32_t getLoopCount() const { return mLoopCount; }
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

    LogLevel    mVerbosity;
    std::string mRpmsgFile;
    std::string mDestIp;
    uint32_t    mDivisor;
    uint32_t    mLoopCount;
    bool mTesting;
};

#endif // OPTIONS_H

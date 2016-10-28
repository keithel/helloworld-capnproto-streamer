#ifndef RPMSGPOSITIONSTREAMER_H
#define RPMSGPOSITIONSTREAMER_H

#include "options.h"
#include <netinet/in.h>
#include <capnp/message.h>
#include "position.capnp.h"

struct AhrsOwnInfo;

class RpmsgPositionStreamer
{
public:
    RpmsgPositionStreamer(const Options& opts, struct sockaddr_in multicastAddr, int multicastFd);

    int init();
    void streamHeadings();

private:

    int openRpmsgFile(const std::string& rpmsgFile);
    /**
     * Send Rpmsg Request to Start Heading Updates with the Given Divisor
     */
    bool startHeadingUpdates ( int fd, uint32_t divisor );
    /**
     * Send Rpmsg Request to Stop sending Heading Updates with the Given Divisor
     */
    bool stopHeadingUpdates ( int fd );

    bool getLatestHeading( int fd, AhrsOwnInfo * heading );


private:
    Options m_opts;
    struct sockaddr_in m_multicastAddr;
    int m_multicastFd;
    int m_rpmsgFd;
    ::capnp::MallocMessageBuilder m_message;
    L3::Position::Builder m_position;
};

#endif // RPMSGPOSITIONSTREAMER_H

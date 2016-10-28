/*
 * position-sender -- Multicasts a small capnproto message to a multicast group once a second
 *
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <iostream>

#include "options.h"
#include "positionstreamtest.h"

#include <capnp/message.h>
#include <capnp/serialize.h>
#include "position.capnp.h"

typedef ::kj::Array< ::capnp::word> WordArray;

#define PORT 11212
#define GROUP_IP "233.253.12.1"

#define dprintf( level, fmt, ... ) { if ( level <= s_opts.getVerbosity() ) { printf( fmt, ##__VA_ARGS__ ); } }

static Options s_opts;

int initMulticastSocket(struct sockaddr_in& addr, int& fd)
{
    // Create an ordinary UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    // set up destination address to the multicast group
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(GROUP_IP);
    addr.sin_port = htons(PORT);
}

int openRpmsgFile(const std::string& rpmsgFile)
{
    int fd = -1;
//    if (!rpmsgFile.empty()) {
//        /* Open the rpmsg device for sending messages to FreeRTOS */
//        if ((fd = Rpmsg_OpenDevice(rpmsgFile.c_str())) < 0) {
//            perror("Open Failed");
//        }
//    }

    if (fd < 0) {
        dprintf(LogLevel::VERB_ERROR, "Error with RPMSG file handle\n");
    }
    return fd;
}

/**
 * Send Rpmsg Request to Start Heading Updates with the Given Divisor
 */
static bool StartHeadingUpdates ( int fd, uint32_t divisor )
{
    bool  retVal  = false;

//    dprintf( LogLevel::VERB_DEBUG, "Sending Heading Update Request with Divisor: %u\n", (unsigned int) divisor );
//    retVal = Rpmsg_SendMsgNoAck( fd, eIpcMsg_HeadingRequest, &divisor, sizeof(divisor) ) >= 0 ? true : false;

    return ( retVal );
}

/**
 * Send Rpmsg Request to Stop sending Heading Updates with the Given Divisor
 */
static bool StopHeadingUpdates ( int fd )
{
   bool     retVal  = false;
   uint32_t divisor = 0;

//   dprintf( LogLevel::VERB_DEBUG, "Sending Heading Update Request with Divisor: %u\n", (unsigned int) divisor );
//   retVal = Rpmsg_SendMsgNoAck( fd, eIpcMsg_HeadingRequest, &divisor, sizeof(divisor) ) >= 0 ? true : false;

   return ( retVal );
}

//static bool GetLatestHeading( int fd, AhrsOwnInfo * heading )
//{
//   int bytesAv = 0;
//   bool retval  = false;

//   /* Grab the first available hading */
//   retval = Rpmsg_RecvMsg( fd, heading, sizeof(AhrsOwnInfo));

//   /**
//    * If we are recieiving good headings and there are more available,
//    * keep reading until we get the last one (most recent). Do not block
//    * to read the data. Only grab what's available.
//    */
//   while( ( retval >= sizeof(AhrsOwnInfo) ) &&
//          ( (retval = ioctl( fd, FIONREAD, &bytesAv)) >= 0 ) &&
//          ( bytesAv >= sizeof(AhrsOwnInfo) ) )
//   {
//      retval  =  Rpmsg_RecvMsg( fd, heading, sizeof(AhrsOwnInfo));
//      bytesAv = 0;
//   }

//   return ( retval >= sizeof(AhrsOwnInfo) ? true : false );
//}

void streamHeadings(int fd)
{
    /* Request headings updates with divisor on 120hz, 1 per second by default */
    if (StartHeadingUpdates(fd, s_opts.getFrequencyDivisor())) {
        int i = 0;
//        AhrsOwnInfo loc;

//        for (i = 0; i < s_opts.getLoopCount(); i++) {
//            if (GetLatestHeading(fd, &loc) != true) {
//                dprintf(LogLevel::VERB_ERROR, "Failed to get update\n");
//            } else {
//                dprintf(LogLevel::VERB_DEBUG, "Updated Heading[%02d]\n", (int)i);
//                dprintf(LogLevel::VERB_DEBUG, "\tHeading   : %f\n", (float)loc.heading);
//                dprintf(LogLevel::VERB_DEBUG, "\tElevation : %f\n", (float)loc.elevation);
//                dprintf(LogLevel::VERB_DEBUG, "\tLatitude  : %f\n", (float)loc.lat);
//                dprintf(LogLevel::VERB_DEBUG, "\tLongitude : %f\n", (float)loc.lon);
//                dprintf(LogLevel::VERB_DEBUG, "\tHAE       : %f\n", (float)loc.hae);
//                dprintf(LogLevel::VERB_DEBUG, "\tRoll      : %f\n\n", (float)loc.roll);
//            }
//        }

        if (StopHeadingUpdates(fd) != true) {
            dprintf(LogLevel::VERB_ERROR, "Could not send request to stop heading\n");
        }
    } else {
        dprintf(LogLevel::VERB_ERROR, "Could not send request to start heading\n");
    }
}

int main(int argc, char* argv[])
{
    int rpmsgFd = -1;
    int res = s_opts.parseArgs(argc, argv);
    std::cout << "parseArgs result: " << res << std::endl;

    rpmsgFd = openRpmsgFile(s_opts.getRpmsgFile());
    if (rpmsgFd < 0 && !s_opts.isTesting())  // It's ok to fail to open rpmsg file if testing..
        return 1;

    struct sockaddr_in addr;
    int multicastFd;
    if (initMulticastSocket(addr, multicastFd) < 0)
        exit(1);

    if (s_opts.isTesting())
    {
        // Send test positions.
        PositionStreamTest tester(addr, multicastFd);
        tester.sendTestPositions();  // Runs to infinity...Ctrl-C to abort.
    }
    else
    {
        streamHeadings(rpmsgFd);
    }
}

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
#include "rpmsgpositionstreamer.h"

#define PORT 11212
#define GROUP_IP "233.253.12.1"

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

int main(int argc, char* argv[])
{
    int rpmsgFd = -1;
    int res = s_opts.parseArgs(argc, argv);

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
        RpmsgPositionStreamer streamer(s_opts, addr, multicastFd);
        if (streamer.init() < 0)
            exit(1);

        streamer.streamHeadings();
    }
}

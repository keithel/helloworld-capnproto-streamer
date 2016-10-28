#ifndef POSITIONSTREAMTEST_H
#define POSITIONSTREAMTEST_H

#include <stdint.h>
#include <netinet/in.h>

#define RATE 60 // positions/sec

class PositionStreamTest
{
public:
    // TODO: Might need to take sockaddr_in by reference, and copy it with memcpy
    PositionStreamTest(struct sockaddr_in addr, int fd, uint32_t rate = 60);

    void sendTestPositions();

private:
    void initPositions();

    struct sockaddr_in mAddr;
    int mFd;
    uint32_t mRate;
};

#endif // POSITIONSTREAMTEST_H

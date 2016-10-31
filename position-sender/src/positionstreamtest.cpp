#include "positionstreamtest.h"
#include <iostream>
#include <list>
#include <vector>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "position.capnp.h"
#include <sys/socket.h>
#include <unistd.h>

typedef ::kj::Array< ::capnp::word> WordArray;
using std::list;
using std::vector;

vector<unsigned char> generateAlternatingMessage();

list<vector<unsigned char> > s_positionBuffers;


PositionStreamTest::PositionStreamTest(struct sockaddr_in addr, int fd, uint32_t rate)
    : mAddr(addr)
    , mFd(fd)
    , mRate(rate)
{
    initPositions();
}

void PositionStreamTest::initPositions()
{
    for (int i = 0; i < 12; i = i + 11) {
        ::capnp::MallocMessageBuilder message;
        L3::Position::Builder position = message.initRoot<L3::Position>();
        position.setHeading(44 + i); // booogus values
        position.setElevation(66 + i);
        position.setLatitude(88 + i);
        position.setLongitude(0 + i);
        position.setHeightAboveEllipsoid(22 + i);
        position.setRoll(44 + i);

        WordArray array;
        array = ::capnp::messageToFlatArray(message);
        s_positionBuffers.push_back(vector<unsigned char>(array.asBytes().begin(), array.asBytes().end()));
        //s_positionBuffers.push_back(array.asPtr());
    }
}

void PositionStreamTest::sendTestPositions()
{
    // now just sendto() the destination
    while (1) {
        static int printCount = 0;
        vector<unsigned char> messageBytes = generateAlternatingMessage();
        if ((printCount++) % RATE == 0)
            std::cout << "Sending " << messageBytes.size() << " bytes" << std::endl;
        if (sendto(mFd, messageBytes.data(), messageBytes.size(), 0, (struct sockaddr*)&mAddr,
                   sizeof(mAddr)) < 0) {
            perror("sendto");
            exit(1);
        }
        usleep(1000000 / mRate); // was 16450
    }
}

vector<unsigned char> generateAlternatingMessage()
{
    static list<vector<unsigned char> >::iterator bufIter = s_positionBuffers.begin();
    vector<unsigned char>& messageArray = *(bufIter++);
    if (bufIter == s_positionBuffers.end())
        bufIter = s_positionBuffers.begin();

    return messageArray;
}

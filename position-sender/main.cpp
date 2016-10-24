/*
 * position-sender -- Multicasts a small capnproto message to a multicast group once a second
 *
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "position.capnp.h"
#include <list>
#include <vector>

typedef ::kj::Array<::capnp::word> WordArray;
using std::list;
using std::vector;


#define PORT 11212
#define GROUP_IP "233.253.12.1"

list<vector<unsigned char>> s_positionBuffers;

void initPositions()
{
    for (int i = 0; i < 12; i = i + 11)
    {
        ::capnp::MallocMessageBuilder message;
        L3::Position::Builder position = message.initRoot<L3::Position>();
        position.setHeading(44 + i); // booogus values
        position.setElevation(66 + i);
        position.setLatitude(88 + i);
        position.setLongitude(0+ i);
        position.setHeightAboveEllipsoid(22 + i);

        WordArray array;
        array = ::capnp::messageToFlatArray(message);
        s_positionBuffers.push_back(vector<unsigned char>(array.asBytes().begin(), array.asBytes().end()));
        //s_positionBuffers.push_back(array.asPtr());
    }
}

vector<unsigned char> generateAlternatingMessage()
{
    static list<vector<unsigned char>>::iterator bufIter = s_positionBuffers.begin();
    vector<unsigned char>& messageArray = *(bufIter++);
    if(bufIter == s_positionBuffers.end()) bufIter = s_positionBuffers.begin();

    return messageArray;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;
    struct sockaddr_in addr;
    int fd;

    initPositions();

    // Create an ordinary UDP socket
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    // set up destination address to the multicast group
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(GROUP_IP);
    addr.sin_port = htons(PORT);

    // now just sendto() the destination
    while (1)
    {
        vector<unsigned char> messageBytes = generateAlternatingMessage();
        //std::cout << "Sending " << messageBytes.size() << " bytes" << std::endl;
        if (sendto(fd, messageBytes.data(), messageBytes.size(), 0, (struct sockaddr*)&addr,
                   sizeof(addr)) < 0)
        {
            perror("sendto");
            exit(1);
        }
        usleep(16450);
    }
}

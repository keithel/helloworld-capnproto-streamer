#include "rpmsgpositionstreamer.h"
#include "rpmsg/rpmsg.h"
#include <sys/ioctl.h>

#include <capnp/serialize.h>

#define dprintf( level, fmt, ... ) { if ( level <= m_opts.getVerbosity() ) { printf( fmt, ##__VA_ARGS__ ); } }

typedef ::kj::Array< ::capnp::word> WordArray;

RpmsgPositionStreamer::RpmsgPositionStreamer(const Options &opts, sockaddr_in muticastAddr, int multicastFd)
    : m_opts(opts)
    , m_multicastAddr(muticastAddr)
    , m_multicastFd(multicastFd)
    , m_position(m_message.initRoot<L3::Position>())
{
}


int RpmsgPositionStreamer::init()
{
    const std::string& file(m_opts.getRpmsgFile());
    if (file.empty())
    {
        dprintf(LogLevel::VERB_ERROR, "Required Rpmsg file not specified! Please use -f option to specify.\n\n");
        m_opts.printHelp();
        return -1;
    }

    m_rpmsgFd = openRpmsgFile(file);
    if (m_rpmsgFd < 0)
    {
        return 1;
    }

    return 0;
}

void RpmsgPositionStreamer::streamHeadings()
{
    /* Request headings updates with divisor on 120hz, 1 per second by default */
    if (startHeadingUpdates(m_rpmsgFd, m_opts.getFrequencyDivisor()))
    {
        int i = 0;
        AhrsOwnInfo loc;

        for (i = 0; i < m_opts.getLoopCount(); i++)
        {
            if (getLatestHeading(m_rpmsgFd, &loc) != true)
            {
                dprintf(LogLevel::VERB_ERROR, "Failed to get update\n");
            }
            else
            {
                // Capnproto magic to get a message buffer to send over UDP.
                m_position.setHeading(loc.heading); // booogus values
                m_position.setElevation(loc.elevation);
                m_position.setLatitude(loc.lat);
                m_position.setLongitude(loc.lon);
                m_position.setHeightAboveEllipsoid(loc.hae);
                m_position.setRoll(loc.roll);
                WordArray array;
                array = ::capnp::messageToFlatArray(m_message);
                //s_positionBuffers.push_back(vector<unsigned char>(array.asBytes().begin(), array.asBytes().end()));

                static int printCount = 0;
                bool logHeading = (printCount++) % (120/m_opts.getFrequencyDivisor()) == 0;
                if (logHeading)
                {
                    dprintf(LogLevel::VERB_INFO, "Updated Heading[%02d]\n", (int)i);
                    dprintf(LogLevel::VERB_INFO, "\tHeading   : %f\n", (float)loc.heading);
                    dprintf(LogLevel::VERB_INFO, "\tElevation : %f\n", (float)loc.elevation);
                    dprintf(LogLevel::VERB_INFO, "\tLatitude  : %f\n", (float)loc.lat);
                    dprintf(LogLevel::VERB_INFO, "\tLongitude : %f\n", (float)loc.lon);
                    dprintf(LogLevel::VERB_INFO, "\tHAE       : %f\n", (float)loc.hae);
                    dprintf(LogLevel::VERB_INFO, "\tRoll      : %f\n", (float)loc.roll);
                    dprintf(LogLevel::VERB_INFO, "\tContents %ld bytes", (long)array.asBytes().size());
                }

                size_t sentBytes = sendto(m_multicastFd, array.asBytes().begin(), array.asBytes().size(),
                                          0, (struct sockaddr*)&m_multicastAddr, sizeof(m_multicastAddr));
                if (sentBytes < 0)
                {
                    if (logHeading)
                       dprintf(LogLevel::VERB_INFO, "\n\n");
                    perror("sendto");
                    exit(1);
                }
                else
                {
                    if (logHeading)
                       dprintf(LogLevel::VERB_DEBUG, ", ");
                    dprintf(LogLevel::VERB_DEBUG, "sent %u bytes multicast.\n", (unsigned int)sentBytes);
                    if (logHeading)
                       dprintf(LogLevel::VERB_INFO, "\n");
                }
            }
        }

        if (stopHeadingUpdates(m_rpmsgFd) != true)
        {
            dprintf(LogLevel::VERB_ERROR, "Could not send request to stop heading\n");
        }
    }
    else
    {
        dprintf(LogLevel::VERB_ERROR, "Could not send request to start heading\n");
    }
}



int RpmsgPositionStreamer::openRpmsgFile(const std::string& rpmsgFile)
{
    int fd = -1;
    if (!rpmsgFile.empty())
    {
        /* Open the rpmsg device for sending messages to FreeRTOS */
        if ((fd = Rpmsg_OpenDevice(rpmsgFile.c_str())) < 0)
        {
            perror("Open Failed");
        }
    }

    if (fd < 0)
    {
        dprintf(LogLevel::VERB_ERROR, "Error with RPMSG file handle\n");
    }
    return fd;
}

/**
 * Send Rpmsg Request to Start Heading Updates with the Given Divisor
 */
bool RpmsgPositionStreamer::startHeadingUpdates ( int fd, uint32_t divisor )
{
    bool  retVal  = false;

    dprintf( LogLevel::VERB_INFO, "Sending Heading Update Request with Divisor: %u\n", (unsigned int) divisor );
    retVal = Rpmsg_SendMsgNoAck( fd, eIpcMsg_HeadingRequest, &divisor, sizeof(divisor) ) >= 0 ? true : false;

    return ( retVal );
}

/**
 * Send Rpmsg Request to Stop sending Heading Updates with the Given Divisor
 */
bool RpmsgPositionStreamer::stopHeadingUpdates ( int fd )
{
   bool     retVal  = false;
   uint32_t divisor = 0;

   dprintf( LogLevel::VERB_INFO, "Sending Heading Update Request with Divisor: %u\n", (unsigned int) divisor );
   retVal = Rpmsg_SendMsgNoAck( fd, eIpcMsg_HeadingRequest, &divisor, sizeof(divisor) ) >= 0 ? true : false;

   return ( retVal );
}

bool RpmsgPositionStreamer::getLatestHeading( int fd, AhrsOwnInfo * heading )
{
   int bytesAv = 0;
   int retval  = 0;

   /* Grab the first available heading */
   retval = Rpmsg_RecvMsg( fd, heading, sizeof(AhrsOwnInfo));
   if (retval < 0)
      dprintf(LogLevel::VERB_INFO, "getLatestHeading recvMsg failed %d\n", retval);

   /**
    * If we are recieiving good headings and there are more available,
    * keep reading until we get the last one (most recent). Do not block
    * to read the data. Only grab what's available.
    */
   while( ( retval >= sizeof(AhrsOwnInfo) ) &&
          ( (retval = ioctl( fd, FIONREAD, &bytesAv)) >= 0 ) &&
          ( bytesAv >= sizeof(AhrsOwnInfo) ) )
   {
      retval  =  Rpmsg_RecvMsg( fd, heading, sizeof(AhrsOwnInfo));
      if (retval < 0)
          dprintf(LogLevel::VERB_INFO, "getLatestHeading recvMsg failed %d\n", retval);
      bytesAv = 0;
   }

   return ( retval >= sizeof(AhrsOwnInfo) ? true : false );
}

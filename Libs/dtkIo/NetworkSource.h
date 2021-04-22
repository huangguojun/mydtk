
#ifndef NETWORKSOURCE_H
#define NETWORKSOURCE_H

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread/thread.hpp>

#include "NetworkPacket.h"

#include <deque>
#include <queue>

class PacketConsumer;
class PacketReceiver;
class PacketFileWriter;

class NetworkSource
{
public:
    NetworkSource(std::shared_ptr<PacketConsumer> _consumer, int argLidarPort,
                  int ForwardedLidarPort_, std::string ForwardedIpAddress_, bool isForwarding_,
                  bool isCrashAnalysing_)
        : LidarPort(argLidarPort),
          ForwardedLidarPort(ForwardedLidarPort_),
          ForwardedIpAddress(ForwardedIpAddress_),
          IsForwarding(isForwarding_),
          IsCrashAnalysing(isCrashAnalysing_),
          IOService(),
          Thread(),
          LidarPortReceiver(),
          Consumer(_consumer),
          Writer(),
          DummyWork(new boost::asio::io_service::work(this->IOService))
    {
    }

    ~NetworkSource();

    void QueuePackets(NetworkPacket *packet);

    void Start();

    void Stop();

    int LidarPort; /*!< The port to receive LIDAR information. Default is 2368*/
    int ForwardedLidarPort; /*!< The port to send LIDAR forwarded packets*/
    std::string ForwardedIpAddress; /*!< The ip to send forwarded packets*/
    bool IsForwarding; /*!< Allowing the forwarding of the packets*/
    bool IsCrashAnalysing;

    boost::asio::io_service IOService;
    boost::shared_ptr<boost::thread> Thread;

    boost::shared_ptr<PacketReceiver> LidarPortReceiver;

    boost::shared_ptr<PacketReceiver> PositionPortReceiver;

    std::shared_ptr<PacketConsumer> Consumer;
    std::shared_ptr<PacketFileWriter> Writer;

    boost::asio::io_service::work *DummyWork;
};

#endif // NETWORKSOURCE_H

#include "PacketReceiver.h"
#include "NetworkPacket.h"
#include "NetworkSource.h"

#include <vtkMath.h>

//-----------------------------------------------------------------------------
PacketReceiver::PacketReceiver(boost::asio::io_service &io, int port, int forwardport,
                               std::string forwarddestinationIp, bool isforwarding,
                               NetworkSource *parent)
    : isForwarding(isforwarding),
      Port(port),
      PacketCounter(0),
      Socket(io),
      ForwardedSocket(io),
      Parent(parent),
      IsReceiving(true),
      ShouldStop(false)
{
    this->Socket.open(boost::asio::ip::udp::v4()); // Opening the socket with an
                                                   // UDP v4 protocol
    this->Socket.set_option(
            boost::asio::ip::udp::socket::reuse_address(true)); // Tell the OS we accept to re-use
                                                                // the port address for an other app
    this->Socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));

    /*
    // Check that the provided ipadress is valid
    boost::system::error_code errCode;
    boost::asio::ip::address ipAddressForwarding =
    boost::asio::ip::address_v4::from_string(forwarddestinationIp, errCode);

    if (errCode)
    {
        ipAddressForwarding =
    boost::asio::ip::address_v4::from_string("0.0.0.0"); if (this->isForwarding)
        {
            vtkGenericWarningMacro("Forward ip address not valid, packets won't
    be forwarded"); this->isForwarding = false;
        }
    }

    this->ForwardEndpoint = boost::asio::ip::udp::endpoint(ipAddressForwarding,
    forwardport); this->ForwardedSocket.open(ForwardEndpoint.protocol()); //
    Opening the socket with an UDP v4 protocol
    // toward the forwarded ip address and port
    this->ForwardedSocket.set_option(boost::asio::ip::multicast::enable_loopback(true));
    // Allow to send the packet on the same machine
    */
}

//-----------------------------------------------------------------------------
PacketReceiver::~PacketReceiver()
{
    this->Socket.cancel();
    this->ForwardedSocket.cancel();
    {
        boost::unique_lock<boost::mutex> guard(this->IsReceivingMtx);
        this->ShouldStop = true;
        while (this->IsReceiving) {
            this->IsReceivingCond.wait(guard);
        }
    }

    // Close and delete the logs files. So that,
    // if a log file is present in the next session
    // it means that the software has been closed
    // properly (potentially a crash)
    /*
       if (this->IsCrashAnalysing)
       {
       this->CrashAnalysis.CloseAnalyzer();
       this->CrashAnalysis.DeleteLogFiles();
       }*/
}

//-----------------------------------------------------------------------------
void PacketReceiver::StartReceive()
{
    {
        boost::lock_guard<boost::mutex> guard(this->IsReceivingMtx);
        this->IsReceiving = true;
    }

    this->Socket.async_receive_from(
            boost::asio::buffer(this->RXBuffer, BUFFER_SIZE), this->SenderEndpoint,
            boost::bind(&PacketReceiver::SocketCallback, this, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
}

//-----------------------------------------------------------------------------
void PacketReceiver::EnableCrashAnalysing(std::string filenameCrashAnalysis_,
                                          unsigned int nbrPacketToStore_, bool isCrashAnalysing_)
{
    /*
       this->IsCrashAnalysing = isCrashAnalysing_;

    // Opening crash analysis file
    if (this->IsCrashAnalysing)
    {
    this->CrashAnalysis.SetNbrPacketsToStore(nbrPacketToStore_);
    this->CrashAnalysis.SetFilename(filenameCrashAnalysis_);
    this->CrashAnalysis.ArchivePreviousLogIfExist();
    }*/
}

//-----------------------------------------------------------------------------
void PacketReceiver::SocketCallback(const boost::system::error_code &error,
                                    std::size_t numberOfBytes)
{
    if (error || this->ShouldStop) {
        boost::lock_guard<boost::mutex> guard(this->IsReceivingMtx);
        this->IsReceiving = false;
        this->IsReceivingCond.notify_one();
        return;
    }

    unsigned short ourPort = static_cast<unsigned short>(this->Port);
    unsigned short sourcePort = this->SenderEndpoint.port();
    unsigned char sourceIP[4] = { 192, 168, 0, 200 };
    // unsigned char sourceIP[4] = { 127, 0, 0, 1 };

    if (this->SenderEndpoint.address().is_v4()) {
        for (int i = 0; i < 4; i++) {
            sourceIP[i] = this->SenderEndpoint.address().to_v4().to_bytes()[i];
        }
    }

    std::cout << SenderEndpoint.address().to_v4().to_string() << std::endl;

    NetworkPacket *packet = NetworkPacket::BuildEthernetIP4UDP(this->RXBuffer, numberOfBytes,
                                                               sourceIP, sourcePort, ourPort);

    // std::cout << this->Socket.remote_endpoint().address() << std::endl;

    /*


    if (this->isForwarding)
    {
       // ForwardedSocket.send_to(boost::asio::buffer(packet->GetPayloadData(),
    packet->GetPayloadSize()), ForwardEndpoint);
    }

    if (this->IsCrashAnalysing)
    {
        // this->CrashAnalysis.AddPacket(*packet);
    }
    */

    this->Parent->QueuePackets(packet);

    this->StartReceive();

    if ((++this->PacketCounter % 5) == 0) {
        std::cout << "RECV packets: " << this->PacketCounter << " on " << this->Port << std::endl;
    }
}

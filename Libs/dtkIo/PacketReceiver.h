#ifndef PACKETRECEIVER_H
#define PACKETRECEIVER_H

// LOCAL
//#include "CrashAnalysing.h"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <fstream>
#include <iostream>

class NetworkSource;

#define BUFFER_SIZE 1500
#define NBR_PACKETS_SAVED 1500

class PacketReceiver
{
public:
    PacketReceiver(boost::asio::io_service& io, int port, int forwardport,std::string forwarddestinationIp, bool isforwarding, NetworkSource* parent);

    ~PacketReceiver();

    void StartReceive();

    void EnableCrashAnalysing(std::string filenameCrashAnalysis_, unsigned int nbrPacketToStore_, bool isCrashAnalysing_);

    void SocketCallback(const boost::system::error_code& error, std::size_t numberOfBytes);

private:
    bool isForwarding;

    boost::asio::ip::udp::endpoint SenderEndpoint;

    boost::asio::ip::udp::endpoint ForwardEndpoint;

    int Port;

    int PacketCounter;

    boost::asio::ip::udp::socket Socket;

    boost::asio::ip::udp::socket ForwardedSocket;

    NetworkSource* Parent;

    unsigned char RXBuffer[BUFFER_SIZE];

    bool IsReceiving; 
    bool ShouldStop;  
    boost::mutex IsReceivingMtx; 
    boost::condition_variable IsReceivingCond;
    boost::mutex IsWriting;
    bool IsCrashAnalysing = false;
    //CrashAnalysisWriter CrashAnalysis;
};

#endif // PACKETRECEIVER_H

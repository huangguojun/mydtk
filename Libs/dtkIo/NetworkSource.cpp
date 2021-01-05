//=========================================================================
//
// Copyright 2018 Kitware, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

// LOCAL
#include "NetworkSource.h"
#include "PacketConsumer.h"
#include "PacketFileWriter.h"
#include "PacketReceiver.h"
//#include "vtkPacketFileWriter.h"
#include <stdio.h>
#include <stdlib.h>

#define LIDAR_PACKET_TO_STORE_CRASH_ANALYSIS 5000
#define GPS_PACKET_TO_STORE_CRASH_ANALYSIS 5000

//-----------------------------------------------------------------------------
NetworkSource::~NetworkSource()
{
    this->Stop();

    delete this->DummyWork;

    if (this->Thread) {
        this->Thread->join();
    }
}

//-----------------------------------------------------------------------------
void NetworkSource::QueuePackets(NetworkPacket *packet)
{
    NetworkPacket *packet2 = nullptr;

    if (this->Consumer) {
        this->Consumer->Enqueue(packet);
    }

    if (this->Writer) {
        packet2 = new NetworkPacket(*packet);
        this->Writer->Enqueue(packet2);
    }
}

//-----------------------------------------------------------------------------
void NetworkSource::Start()
{
    if (!this->Thread) {
        this->Thread.reset(
                new boost::thread(boost::bind(&boost::asio::io_service::run, &this->IOService)));
    }

    // Create work
    this->LidarPortReceiver = boost::shared_ptr<PacketReceiver>(
            new PacketReceiver(this->IOService, LidarPort, ForwardedLidarPort, ForwardedIpAddress,
                               IsForwarding, this));

    this->LidarPortReceiver->StartReceive();
}

//-----------------------------------------------------------------------------
void NetworkSource::Stop()
{
    this->LidarPortReceiver.reset();
}

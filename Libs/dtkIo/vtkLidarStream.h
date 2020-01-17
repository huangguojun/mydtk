
#ifndef VTKLIDARSTREAM_H
#define VTKLIDARSTREAM_H

#include <memory>
#include "vtkLidarProvider.h"

class PacketConsumer;
class PacketFileWriter;
class NetworkSource;

class VTK_EXPORT vtkLidarStream : public vtkLidarProvider
{
public:
  //static vtkLidarStream* New();
  vtkLidarStream();
  ~vtkLidarStream();
  vtkTypeMacro(vtkLidarStream, vtkLidarProvider)

  int GetNumberOfFrames() override;

  void Start();
  void Stop();

  std::string GetOutputFile();
  void SetOutputFile(const std::string& filename);

  void SetLidarPort(int) override;
  int GetLidarPort() override;

  int GetGPSPort();
  void SetGPSPort(int);

  std::string GetForwardedIpAddress();
  void SetForwardedIpAddress(const std::string& ipAddress);

  int GetForwardedLidarPort();
  void SetForwardedLidarPort(int);

  int GetForwardedGPSPort();
  void SetForwardedGPSPort(int);

  void EnableGPSListening(bool);

  bool GetIsForwarding();
  void SetIsForwarding(bool);

  bool GetIsCrashAnalysing();
  void SetIsCrashAnalysing(bool value);

  bool GetNeedsUpdate();

protected:

  int RequestData(vtkInformation* request,
              vtkInformationVector** inputVector,
              vtkInformationVector* outputVector) override;

  //! where to save a live record of the sensor
  std::string OutputFileName = "";
  std::shared_ptr<PacketConsumer> Consumer;
  std::shared_ptr<PacketFileWriter> Writer;
  std::unique_ptr<NetworkSource> Network;
private:
  vtkLidarStream(const vtkLidarStream&) = delete;
  void operator=(const vtkLidarStream&) = delete;
};

#endif // VTKLIDARSTREAM_H

#include <vector>
#include <complex>

class VAD {
  private:
    int lastVoice;
    int fileNumber;

    std::vector<std::complex<double>> fft(std::vector<std::complex<double>> sample);
    int isVoice(std::vector<signed char> &packet);
  public:
    const double PI = 3.1415926535897932384626433832795;
    const int PACKET_SIZE = 160;
    const double maxFrequency = 3500;
    const double minFrequency = 200;
  
  void processData();
  VAD(int fileNum);
};

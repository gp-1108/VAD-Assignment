#include <vector>
#include <complex>
#include <iostream>

using namespace std;

class VAD {
  private:
    double maxFrequency;
    double minFrequency;
    double samplingFrequency;
    double rumorEnergy;
    int lastVoice;
  public:
    const double PI = 3.1415926535897932384626433832795;

  VAD(double minFreq, double maxFreq, double sampFreq) {
    lastVoice = 0;
    rumorEnergy = 0;
    samplingFrequency = sampFreq;
    maxFrequency = maxFreq;
    minFrequency = minFreq;
  }

  bool isVoiceEnergy(vector<signed char> &packet, bool earlyTime) {
    // TODO spostare parametri
    double k = 1.3;
    double p = 0.9;


    double currentEnergy = 0;
    for(signed char num : packet) {
      currentEnergy += (double) num * (double) num;
    }
    currentEnergy /= packet.size();

    if(earlyTime || currentEnergy <= (k * rumorEnergy)) {
      rumorEnergy = (1-p) * rumorEnergy + p * currentEnergy;
      return false;
    }
    return true;
  }

  int isVoice(vector<signed char> &packet) {
    if(lastVoice > 0) {
      lastVoice--;
      return true;
    }
    vector<complex<double>> complexValues;
    for(int i = 0; i < packet.size(); i++) {
      complex<double> c((double) packet[i], 0.0);
      complexValues.push_back(c);
    }
    vector<complex<double>> fftOutput = fft(complexValues);


    double maxMagnitude = -1;
    int maxIndex = -1;
    for(int i = 0; i < (fftOutput.size() / 2); i++) {
      double magnitude = sqrt((fftOutput[i].real() * fftOutput[i].real()) + (fftOutput[i].imag() * fftOutput[i].imag()));
      if(magnitude > maxMagnitude) {
        maxMagnitude = magnitude;
        maxIndex = i;
      }
    }


    // previous one somewhat functioning
    // double peak = samplingFrequency * (packet.size() / maxMagnitude);

    // double peak = samplingFrequency * (packet.size() / maxMagnitude);
    double peak = maxMagnitude;

    if(peak <= maxFrequency && peak >= minFrequency) {
      lastVoice = 0;
      return true;
    }
    return false;
  }

  vector<complex<double>> fft(vector<complex<double>> sample) {

    if(sample.size() == 1) return sample;

    vector<complex<double>> even;
    for(int i = 0; i < (sample.size() / 2); i++) {
      complex<double> c(sample[i*2]);
      even.push_back(c);
    }
    even = fft(even);

    vector<complex<double>> odd;
    for(int i = 0; i < (sample.size() / 2); i++) {
      complex<double> c(sample[i*2 + 1]);
      odd.push_back(c);
    }
    odd = fft(odd);

    vector<complex<double>> res(sample.size());

    for(int i = 0; i < (sample.size() / 2); i++) {
      double s = (i * PI * 2.0) / sample.size();
      
      complex<double> sk(cos(s), sin(s));
      complex<double> evenNum = even[i];
      complex<double> oddNum = odd[i];

      res[i] = evenNum + (sk * oddNum);
      res[i + (sample.size() / 2)] = evenNum - (sk * oddNum);
    }

    return res;
  }
};

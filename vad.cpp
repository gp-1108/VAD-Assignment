#include "vad.hpp"    
#include <complex>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

VAD::VAD(int fileNum) {
  fileNumber = fileNum;
  lastVoice = 0;
}

void VAD::processData() {
  string inputFileName = "inputData/inputaudio" + to_string(fileNumber) + ".data";
  string outputFileName = "outputData/outputaudio" + to_string(fileNumber) + ".data";

  ifstream inputStream(inputFileName, ifstream::binary);
  ofstream outputStream(outputFileName, ofstream::binary);

  vector<signed char> lastSent;

  while(inputStream.good()) {
    vector<signed char> packet;
    for(int i = 0; i < PACKET_SIZE && inputStream.good(); i++) {
      signed char sample;
      inputStream.read((char*) &sample, sizeof(sample));
      packet.push_back(sample);
    }

    if(isVoice(packet)) {
      if(lastSent.size() > 0) {
        for(int i = 0; i < lastSent.size(); i++) {
          outputStream.write((char *) &packet[i], sizeof(packet[i]));
        } 
      }
      for(int i = 0; i < PACKET_SIZE; i++) {
        outputStream.write((char *) &packet[i], sizeof(packet[i]));
      }
      lastSent.clear();
    } else {
      if(lastSent.size() > 0) {
        signed char zero = 0;
        for(int i = 0; i < PACKET_SIZE; i++) {
          outputStream.write((char *) &zero, sizeof(zero));
        }
      }
      lastSent = packet;
    }
}

  inputStream.close();
  outputStream.close();

  return;
}

int VAD::isVoice(vector<signed char> &packet) {
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
  for(int i = 0; i < (fftOutput.size() / 2); i++) {
    double magnitude = sqrt((fftOutput[i].real() * fftOutput[i].real()) + (fftOutput[i].imag() * fftOutput[i].imag()));
    if(magnitude > maxMagnitude) {
      maxMagnitude = magnitude;
    }
  }

  double peak = maxMagnitude;

  if(peak <= maxFrequency && peak >= minFrequency) {
    lastVoice = 3;
    return true;
  }
  return false;
}

vector<complex<double>> VAD::fft(vector<complex<double>> sample) {

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

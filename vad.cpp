#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


int main() {
  const int PACKET_SIZE = 160;

  string inputFileName = "inputData/inputaudio1.data";
  string outputFileName = "outputData/outputaudio1.data";

  ifstream inputStream(inputFileName, ifstream::binary);
  ofstream outputStream(outputFileName, ofstream::binary);

  while(inputStream.good()) {
    vector<signed char> packet;
    for(int i = 0; i < PACKET_SIZE && inputStream.good(); i++) {
      signed char sample;
      inputStream.read((char*) &sample, sizeof(sample));
      packet.push_back(sample);
    }
    
    // VAD operation
    int isVoice = 1; // it will be later on decided by the VAD

    if(isVoice == 1) {
      for(int i = 0; i < PACKET_SIZE; i++) {
        outputStream.write((char *) &packet[i], sizeof(packet[i]));
      }
    } else {
      signed char zero = 0;
      for(int i = 0; i < PACKET_SIZE; i++) {
        outputStream.write((char *) &zero, sizeof(zero));
      }
    }
  }

  inputStream.close();
  outputStream.close();

  cout << "end";
  return 0;
}
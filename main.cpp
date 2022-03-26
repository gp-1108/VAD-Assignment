#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "vad.cpp"

using namespace std;


int chooseAudio() {
  int number;
  cout << "Please select one of the available tracks to process (1-5): ";
  cin >> number;
  cout << endl;
  while(number < 1 || number > 5) {
    cout << "Selected value not available, choose a file audio from 1 to 5: ";
    cin >> number;
  }
  return number;
}

int main() {
  const int PACKET_SIZE = 160;
  VAD vad(200, 3500, 8000);

  int choice = chooseAudio();

  string inputFileName = "inputData/inputaudio" + to_string(choice) + ".data";
  string outputFileName = "outputData/outputaudio" + to_string(choice) + ".data";

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

    if(vad.isVoice(packet)) {
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

  return 0;
}
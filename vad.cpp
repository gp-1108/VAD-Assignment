#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

  int choice = chooseAudio();

  string inputFileName = "inputData/inputaudio" + to_string(choice) + ".data";
  string outputFileName = "outputData/outputaudio" + to_string(choice) + ".data";

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

  cout << "end" << endl;
  return 0;
}
#include <iostream>
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
  int choice = chooseAudio();

  VAD vad(choice);
  vad.processData();

  return 0;
}
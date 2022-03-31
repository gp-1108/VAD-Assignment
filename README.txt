1. Compilare il contento della cartella con il comando "g++ *.cpp *.hpp -o vad"
2. Eseguire il programma con "./vad"
3. Selezionare uno fra i 5 file di input localizzati nella cartella "inputData"
(3.1) Se si desidera utilizzare un file specifico è necessario utilizzare un main.cpp a se stante passando al costruttore di VAD l'intero path
      al file da leggere sottoforma di stringa
4. Il file generato sarà presente nella cartella "outputData" sotto il nome "outputVAD<INT>.data" (di default è zero)
5. L'ouput del VAD in 0 - 1 viene salvato nella cartella "outputTXT" sotto il nome "outputVAD<INT>.txt" (di default è zero)

/*
  Galih Setyawan
  30 Des 2018
*/
#define length 5
#define length2 10
float alpha = 0.8;
double current_output, previous_output;
int current_input, afterFilter;

int temp[length];
int denyutJantung[length2];

bool statusSiklus, statusSiklusSebelumnya;

int beat, bpm;

unsigned long previousMillis = 0;
const long interval = 15000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {

  // Membaca data analog mentah
  current_input = analogRead(A0);

  //Low Pas Filter
  current_output  = alpha * current_input + (1 - alpha) * previous_output;
  previous_output = current_output;


  //Moving Average
  afterFilter = movAvg(current_output);

  //pendeteksi awal siklus
  int awalSiklus = deteksiAwalSiklus(afterFilter);


  //pendeteksi RISING
  if (statusSiklus == true && statusSiklusSebelumnya == false) {
    beat ++;
  }
  statusSiklusSebelumnya = statusSiklus;

  //perhitungan
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    bpm = beat * 4;

    beat=0;
    
  }
  //Print to monitor
  //  Serial.print(current_input);
  //  Serial.print("\t");
  //  Serial.print(current_output);
  //  Serial.print("\t");
  Serial.print(afterFilter);
  Serial.print("\t");
  Serial.print(awalSiklus);
  Serial.print("\t");
  Serial.print(beat);
  Serial.print("\t");
  Serial.print(bpm);
  Serial.println();
  delay(5);
}

int movAvg(int in) {
  double sum = 0;
  for (int i = length - 1; i >= 1; i--) {
    temp[i] = temp[i - 1];
    sum += temp[i];
  }
  temp[0] = in;
  sum += in;
  return (sum / length);
}

int deteksiAwalSiklus(int in) {
  for (int i = length2 - 1; i >= 1; i--) {
    denyutJantung[i] = denyutJantung[i - 1];
  }
  denyutJantung[0] = in;

  if (denyutJantung[0] - denyutJantung[5] >= 13) {
    statusSiklus = true;
    return 690;
  } else {
    statusSiklus = false;
    return 680;
  }
}


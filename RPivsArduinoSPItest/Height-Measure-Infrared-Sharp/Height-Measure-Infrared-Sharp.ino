//pin for pezzo buzzer functionality
int pinRedLED = 7;
int pinPezzoBuzzer = 11;
int pinStartSwitch = 9;
int pinYellowPowerStateLED = 10;



int IRpin = A0;
int countDataCollected = 0;
int countSmoothDataCollected = 0;
int maxDataCollected = 100;
int maxSmoothedDataCollected = 25;
//copy max data size from above variable to the array below
float distancesMeasured[100];
float voltMeasured[100];

//smoothed average height array
float smoothedAverageHeight[25];

//smoothed median height array
float smoothedMedianHeight[25];

// analog pin for reading the IR sensor

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  // start the serial port

  pinMode(pinRedLED, OUTPUT);
  pinMode(pinPezzoBuzzer, OUTPUT);
  pinMode(pinStartSwitch, INPUT);
  pinMode(pinYellowPowerStateLED, OUTPUT);

  
  //digitalWrite(pinStartSwitch,LOW);
  digitalWrite(pinYellowPowerStateLED,HIGH);
  //digitalWrite(pinPezzoBuzzer,HIGH);
}

void loop() {

  if(digitalRead(pinStartSwitch) == LOW)
  {
    Serial.print("ok!");
    analogWrite(pinPezzoBuzzer,40);
    delay(200);
    analogWrite(pinPezzoBuzzer,0);
    delay(500);
  }


  // put your main code here, to run repeatedly:
  // 0.0048828125; value for resolution
  float volts = analogRead(IRpin) * 0.0048828125;
  // value from sensor*(5/1024)

  // float distance = 65*pow(volts, -1.10);

  //  float sensor_feed = (volts - 1.05)/(1/.007);

  // float sensor_feed =

  // float distance = 1/sensor_feed;

  float distance = 61.681 * pow(volts, -1.133);

  // float distance = 16.2537 * pow(volts,4)-129.893 * pow(volts,3)+382.268*pow(volts,2)-512.611*volts+306.439;

  // worked out from graph 65 = theoretical distance / (1/Volts)



 // Serial.print("Distance:   ");
 // Serial.println(distance);

 // Serial.print("Volts:   ");
  //Serial.println(volts);

  for (countDataCollected; countDataCollected < maxDataCollected; countDataCollected++)
  {
    distancesMeasured[countDataCollected] = distance;

    voltMeasured[countDataCollected] = volts;

   // Serial.println(countDataCollected);

    //Serial.println(maxDataCollected);

  }
  
  //Serial.println(maxDataCollected);
  
  if(countDataCollected == maxDataCollected)
  {
  float measuredAverageHeight = GetAverage(distancesMeasured, maxDataCollected);
  float nearRealMedianHeight =  GetMedian(distancesMeasured, maxDataCollected);
  float  calculatedAverageHeight = 213.00 - measuredAverageHeight;
   
   
   //Serial.println(measuredAverageHeight); 
  // printDistance(distancesMeasured, maxDataCollected);

  // printVolt(voltMeasured, maxDataCollected);
  /*
  Serial.println("Your Calculated Average Height is:");
  Serial.println(calculatedAverageHeight);
  delay (500);
  Serial.println("Your Calculated Median Height is:");
  Serial.println(nearRealMedianHeight);
   Serial.println("****************************Next Person*******************************************");
  delay(3000);
  */

          if(countSmoothDataCollected < maxSmoothedDataCollected)
          {
            smoothedAverageHeight[countSmoothDataCollected] = measuredAverageHeight;
            smoothedMedianHeight[countSmoothDataCollected] = nearRealMedianHeight;
            //Serial.println(countSmoothDataCollected);
            countSmoothDataCollected++;
          }
          else if(countSmoothDataCollected == maxSmoothedDataCollected)
          {
            float doubleSmoothedMedianHeightAv = GetAverage(smoothedMedianHeight, maxSmoothedDataCollected);
            float doubleSmoothedRealMedianHeightMd = GetMedian(smoothedMedianHeight, maxSmoothedDataCollected);

            Serial.println("######################################################################");
            Serial.println("The Final and Strict Height Measurment Result is(Av):");
            float confirmedAvHeightMeasured = 213.00-doubleSmoothedMedianHeightAv;
           Serial.println(confirmedAvHeightMeasured);
            //delay(1000);
          //  Serial.println("***CONFIRMED***The Final and Strict Height Measurment Result is(Median):");
           // float confirmedMedHeightMeasured = 213.00-doubleSmoothedRealMedianHeightMd;
           // Serial.println(confirmedMedHeightMeasured);
            delay(2000);

             countSmoothDataCollected = 0;
          }
  }

  
  // print the distance

 // delay(3000);

  //arbitrary wait time

 
  //delay(500);
  countDataCollected = 0;
 
}


float GetMedian(float daArray[], int iSize) {
  // Allocate an array of the same size and sort it.
  float* dpSorted = new float[iSize];
  //    int arraySize = daArray.length();
  for (int i = 0; i < iSize; ++i) {
    dpSorted[i] = daArray[i];
  }
  for (int i = iSize - 1; i > 0; --i) {
    for (int j = 0; j < i; ++j) {
      if (dpSorted[j] > dpSorted[j + 1]) {
        float dTemp = dpSorted[j];
        dpSorted[j] = dpSorted[j + 1];
        dpSorted[j + 1] = dTemp;
      }
    }
  }

  // Middle or average of middle values in the sorted array.
  float dMedian = 0.0;
  if ((iSize % 2) == 0) {
    dMedian = (dpSorted[iSize / 2] + dpSorted[(iSize / 2) - 1]) / 2.0;
  } else {
    dMedian = dpSorted[iSize / 2];
  }
  delete [] dpSorted;
  return dMedian;
}

float GetAverage(float daArray[], int iSize)
{
  float sum = 0.0;
  for (int i = 0; i < iSize; ++i)
  {
    sum =  daArray[i] + sum;
  }
  float average = sum / (iSize - 1);
  return average;
}

void printDistance(float daArray[], int iSize)
{
  Serial.println("Distance Reading");
  for (int i = 0; i < iSize; ++i) {
   Serial.println(daArray[i]);
  }
  delay(4000);
}

void printVolt(float daArray[], int iSize)
{
  Serial.println("Volts Reading");
  for (int i = 0; i < iSize; ++i) {
   Serial.println(daArray[i]);
  }
}




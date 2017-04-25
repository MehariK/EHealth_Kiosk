#include <avr/interrupt.h>
//#include <bits/stdc++.h>
#include <string.h>

 String intBitA = "";
 String intBitB = "";
volatile byte seqA = B00000000;
volatile byte seqB = B00000000;
byte A_val2 = B00000000;
 byte B_val2 = B00000000;
 byte AandB_val = B00000000;
 byte AandB_valPrime = B00000000;
volatile byte cnt1 = 0;
volatile byte cnt2 = 0;
volatile boolean right = false;
volatile boolean left = false;
volatile boolean button = false;
boolean backlight = true;
byte menuitem = 1;
byte page = 1;

unsigned char initA_val;
unsigned char tempA_val;

unsigned long durationA;
unsigned long durationB;
unsigned long timeA;
unsigned long timeB;

void setup() {
 // pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  // Enable internal pull-up resistors
 // digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
 
 /* PCICR =  0b00000010; // 1. PCIE1: Pin Change Interrupt Enable 1
  PCMSK1 = 0b00000111; // Enable Pin Change Interrupt for A0, A1, A2
  */
 
  cli();    // switch interrupts off while messing with their settings  
  PCICR =B00000010;          // Enable PCINT1 interrupt
  PCMSK1 = B00000011;
  sei();    // turn interrupts back on

  initA_val = bitRead((PINC & B00000010),1);
  
  Serial.begin(9600);
}

void loop() {
// Serial.println("Wait");
  // MAIN LOOP 
}



void InitialiseInterrupt(){
  cli();    // switch interrupts off while messing with their settings  
  PCICR = B00000010;          // Enable PCINT1 interrupt
  PCMSK1 = B00000110;
  sei();    // turn interrupts back on
}



ISR (PCINT1_vect) {

Serial.println("Start new interrupt");

// If interrupt is triggered by the button
/*  if (!digitalRead(A0)) {
    
    button = true;
    }
*/

// Else if interrupt is triggered by encoder signals
 // else {
   
    // Read A and B signals
    //Serial.println(PINC,BIN);
    AandB_val = PINC & B00000110;

/*    
//if(bitRead(AandB_valPrime,2)==0 || bitRead(AandB_valPrime,1)==0){
  if(bitRead(AandB_val,2)==0 && bitRead(AandB_val,1)==1){
    
    AandB_val = PINC & B00000110;
   // timeB =  -(pulseIn(A2, LOW));
    timeA = pulseIn(A1, HIGH);
    Serial.println("1,0");
  }else if(bitRead(AandB_val,1)==0 && bitRead(AandB_val,2)==1){
     
     AandB_val = PINC & B00000110;
    // timeA = -(pulseIn(A1, LOW));
      timeB =  pulseIn(A2, HIGH);
      Serial.println("0,1");
  }else if(bitRead(AandB_val,1)==0 && bitRead(AandB_val,2)==0){
     
     AandB_val = PINC & B00000110;
    // timeA = -(pulseIn(A1, LOW));
     // timeB =  -(pulseIn(A2, LOW));
      Serial.println("0,0");
  
  }else if(bitRead(AandB_val,1)==1 && bitRead(AandB_val,2)==0){
     AandB_val = PINC & B00000110;
     timeA = pulseIn(A1, HIGH);
   //  timeB = -(pulseIn(A2, LOW));
     Serial.println("1,0");
  } else if(bitRead(AandB_val,1)==1&& bitRead(AandB_val,2)==1){
     
     AandB_val = PINC & B00000110;
     timeA = pulseIn(A1, HIGH);
     timeB = pulseIn(A2, HIGH);
     Serial.println("1,1");
  }
//}

Serial.println(timeA);
Serial.println(timeB);



timeA = 0;
timeB = 0;

*/

   // durationA = pulseIn(A1, HIGH);
    //durationB = pulseIn(A2, HIGH);
    
    Serial.println(AandB_val,BIN);
    Serial.println("===========================");
  ///  Serial.println(durationA);
  //  Serial.println(durationB);
// A_val2 = AandB_val & B00000010;
// B_val2 = AandB_val & B00000100;

A_val2 = bitRead(AandB_val,1);

B_val2 = bitRead(AandB_val,2);

tempA_val = bitRead((AandB_val,1),1);

if(A_val2 != initA_val){
//rotating

  if(A_val2 != B_val2){
    Serial.println("============Clockwise===============");
  }
  else{
    Serial.println("============Counterlockwise===============");
  }
  
  
}

    Serial.println("/////////////////////////////////////");
   // boolean A_val = digitalRead(A1);
 //   boolean  A_val1 = PINC & B00000010;
 //   Serial.println(A_val1,BIN);
    Serial.println(A_val2,BIN);
 //  boolean  A_val = PINC & B00000010;
 //  Serial.println(A_val,BIN);
  //  boolean B_val = digitalRead(A2);
    //boolean  B_val1 = PINC & B00000100;
    Serial.println("*********");
    
   
    
 //   Serial.println(B_val1,BIN);
     Serial.println(B_val2,BIN);
 //   boolean B_val = PINC & B00000100;
 //   Serial.println(B_val,BIN);
   // Serial.println(A_val);
   // Serial.println(B_val);
    
    // Record the A and B signals in seperate sequences
    seqA <<= 1;
    seqA |= A_val2;

 intBitA += String(A_val2);    
    seqB <<= 1;
    seqB |= B_val2;

    intBitB += String(B_val2); 
    Serial.println("+++++++++++++++++++++++++++++++++++++");
    
Serial.println(intBitA);
Serial.println(intBitB);
    
    // Mask the MSB four bits
 //   seqA &= B00001111;
 //   seqB &= B00001111;
    
    // Compare the recorded sequence with the expected sequence
    if (seqA == B00001001 && seqB == B00000011) {
      cnt1++;
      Serial.println(cnt1);
      left = true;
      }
     
    if (seqA == B00000011 && seqB == B00001001) {
      cnt2++;
      Serial.println(cnt2);
      right = true;
      }
 // }



}  

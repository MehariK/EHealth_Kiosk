
#include <String.h>


char binTest1 = B00000011;
char binTest2 = 'C';
String comingMessage;
char c;
void setup() {
  // put your setup code here, to run once:
 
    Serial.begin(57600);
//String hex_SYSTOLIC_DIASTOLIC = (binTest1).c_str() + (binTest2).c_str();
//  Serial.println(hex_SYSTOLIC_DIASTOLIC);
    Serial.println(String((binTest1)*1)+String(binTest2));
    Serial.println(int(binTest2), DEC);
//   Serial.println(binTest1+binTest2);
   char* result;
   //result = concat(binTest1, binTest2);
   //Serial.println(result);
}

void loop() {
  // put your main code here, to run repeatedly:

  //serialBufferUsed = Serial.available();
     //while(Serial.available()==0){} 
      if (Serial.available()) {
            //for (int j = 0; j < serialBufferUsed; j++) {
                // ack_message[j] = Serial.read();
               //Serial.readBytes(measurement_data,32);
              //Serial.println(readSize);
           // }
        
          c = Serial.read();
                 if(c=='\n'){
                  comingMessage += '\n';
                 }else{
                  comingMessage += c;
                 }

            Serial.println(c, HEX);
           
         }
      //Serial.readBytes(measurement_data,32);
     // for (int x = 0; x < comingMessage.length();x++){
           //Serial.print(char(comingMessage[x]),HEX);
        
      //}
          

           Serial.print("\n");
  

 // if (Serial.available() > 0) {
    //for (int j = 0; j < serialBufferUsed; j++) {
      // Serial.println(Serial.read());

      //Serial.readBytes(measurement_data,32);
      //Serial.println(readSize);
    //}
 // }
 //delay(200);
}

char* concat(char *s1,char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);//+1 to copy the null-terminator
    return result;
}

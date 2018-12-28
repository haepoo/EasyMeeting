
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>



int receiver = 2;                                 // Signal Pin of IR receiver connect with Arduino Digital Pin 11

IRrecv irrecv(receiver);                           // create instance of 'irrecv'  (Object Declaring)
decode_results results;                            // create instance of 'decode_results'

void setup()                                       // setup code runs once
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn();                            // Start the receiver
  
}                                                  


void loop()                                        // loop runs constantly
{
  if (irrecv.decode(&results))                     // have we received an IR signal?
    {
      translateIR(); 
      irrecv.resume();                             // receive the next value
    }  
}                                                    
                                                    
void translateIR()                                // takes action based on IR code received 
{ 
     switch(results.value)                          //Declaring IR Remote codes
     {                                
                                      
      case 0x511DBB:     Serial.println(" ON");   break;
      case 0x52A3D41F:   Serial.println(" OFF");      break;
      default:           Serial.println(" other button");
     }
delay(50); 
} 

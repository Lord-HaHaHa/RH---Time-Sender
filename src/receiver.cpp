#include <Arduino.h>
#include <vars.h>
#include <receiver.h>


        unsigned long receiver::getCalculatedTime(){
            return(UTC0 + timeshift);
        }

        String receiver::generatePayload(){
            String payload = String(id) + String(getCalculatedTime());
            return(payload);
        }
        receiver::receiver(int idin){
            id = idin;
        }
        receiver::receiver(){

        }

        void receiver::notifyUpdate(){
            auto pl = generatePayload();
            const char *payload = pl.c_str();
            if(driver.send((uint8_t *)payload, sizeof(payload))){
                Serial.print("Transmitted: ");
                Serial.print(payload);
                Serial.print(" Lenght: ");
                Serial.println(strlen(payload));
            }
            else
                Serial.println("Failed to Send");
            driver.waitPacketSent();
        }

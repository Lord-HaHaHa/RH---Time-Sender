#ifndef receiver_h
#define receiver_h

#include <Arduino.h>

class receiver{
     private:
        unsigned int id;
        

        unsigned long getCalculatedTime();
        String generatePayload();

    public:
        int timeshift;
        void notifyUpdate();
        receiver(int id);
        receiver();
};

#endif
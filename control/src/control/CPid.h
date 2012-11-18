#include <limits>
#include <iostream>

class CPid
{
    public:
        //proportional, integral and derivative gains
        float pGain, iGain, dGain;

        float targetVal, curentVal, previousVal;

        float error;
        float derivative;
        float integral, integralMin, integralMax;

        float previousTime;

    public:
        void init();

        CPid() ;
        CPid(float ipGain, float iiGain, float idGain);
        CPid(float ipGain, float iiGain, float idGain, float iIntegralMin, float iIntegralMax);

        float updatePid(float curentVal, float curentTime);
        void setTargetVal(float sTargetVal);
        float getTargetVal();

        void reset();
};


#include "CPid.h"

void CPid::init() {
    pGain = iGain = dGain = 0;

    targetVal = previousVal = 0;

    error = 0;
    integral = 0;
    derivative = 0;

    integralMin = std::numeric_limits<float>::min();
    integralMax = std::numeric_limits<float>::max();

    previousTime = -1;
}

CPid::CPid() {
    init();
}

CPid::CPid(float ipGain, float iiGain, float idGain) {
    init();
    pGain = ipGain; iGain = iiGain; dGain = idGain;
}

CPid::CPid(float ipGain, float iiGain, float idGain, float iIntegralMin, float iIntegralMax) {
    init();
    pGain = ipGain; iGain = iiGain; dGain = idGain;
    integralMin = iIntegralMin; integralMax = iIntegralMax;
}

float CPid::updatePid(float curentVal, float curentTime) {
    error = targetVal - curentVal;

    //Calculates time interval between calls, finds integral and derivative values
    if(previousTime >= 0) {
        float dt = curentTime - previousTime;

        integral += error*dt;
        derivative = (curentVal - previousVal) / dt;
    } else { //This is the first call - can't compute dt
        integral = derivative = 0;
    }

    //integral constraints
    if(integral < integralMin)
        integral = integralMin;
    else if(integral > integralMax)
        integral = integralMax;

    float output = pGain*error + iGain*integral + dGain*derivative;

    previousTime = curentTime;
    previousVal = curentVal;

    return output;
}

void CPid::setTargetVal(float sTargetVal) {
    targetVal = sTargetVal;
}

float CPid::getTargetVal() {
    return targetVal;
}

void CPid::reset() {
    //@ Todo Write normal reset
    init();
}

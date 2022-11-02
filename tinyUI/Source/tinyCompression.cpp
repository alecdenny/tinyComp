//
//  tinyCompression.cpp
//  tinyUI
//
//  Created by Alec Denny on 9/30/22.
//



#include "tinyCompression.h"


float TinyCompressor::coeffFromMs(float timeInMs, int sampleRate)
{
    
    float timeInSamps = timeInMs * 0.001 * sampleRate;
    if(timeInSamps == 0)
        timeInSamps = 1;
    
    float coeff = exp(-1/(timeInSamps));
    return coeff;
    
}


TinyCompressor::TinyCompressor(juce::AudioProcessorValueTreeState& ap, int sampleRate)
{
    
    //so obviously eventually i want to do this in a much more effecient way using the other stuff i started
    //but we just want a working plug-in at the moment
    std::vector<float> tempPS;
    tempPS.push_back(*ap.getRawParameterValue("THRESHOLD"));
    tempPS.push_back(*ap.getRawParameterValue("RATIO"));
    tempPS.push_back(*ap.getRawParameterValue("ATTACK"));
    tempPS.push_back(*ap.getRawParameterValue("RELEASE"));
    
    paramsFrom0to1(tempPS, sampleRate);
    
}

void TinyCompressor::paramsFrom0to1(std::vector<float>& ps, int sampleRate)
{
    localParams[0] = atodb(pow(ps[0], 2.0));
    localParams[1] = (ps[1] * 19.0) + 1.0;
    localParams[2] = coeffFromMs(ps[2] * 100.0, sampleRate); //ms range
    localParams[3] = coeffFromMs(ps[3] * 100.0, sampleRate); //ms range
}

void TinyCompressor::updateParameters(juce::AudioProcessorValueTreeState &ap, int sampleRate)
{
    std::vector<float> tempPS;
    tempPS.push_back(*ap.getRawParameterValue("THRESHOLD"));
    tempPS.push_back(*ap.getRawParameterValue("RATIO"));
    tempPS.push_back(*ap.getRawParameterValue("ATTACK"));
    tempPS.push_back(*ap.getRawParameterValue("RELEASE"));
    
    paramsFrom0to1(tempPS, sampleRate);
}

float TinyCompressor::dbtoa(float db)
{
    auto a = pow(10.0, (db / 20.0));
    return a;
}

float TinyCompressor::atodb(float a)
{
    if(a <= 0.000001)
        return -120.0;
    
    auto db = 20.0 * log10(abs(a));
    return db;
}


std::vector<float> TinyCompressor::process(std::vector<float>& signal, int channels, int sampleRate)
{

    
    float level{0.0};
    for(int i = 0; i < channels; i++)
    {
        level += abs(signal[i]) / channels;
    }
    
    level = atodb(level);
    
    float y_g;
    if(level >= localParams[0])
    {
        y_g = ((level - localParams[0]) / localParams[1]) + localParams[0];
        
    }
    else
    {
        y_g = level;
    }
    
    float x_l = level - y_g;
    float y_l;
    if(x_l > envLevel)
    {
        y_l = (localParams[2] * envLevel) + ((1 - localParams[2]) * x_l);
    }
    else
    {
        y_l = (localParams[3] * envLevel) + ((1 - localParams[3]) * x_l);
    }
    
    float control = dbtoa(-1.0 * y_l);
    envLevel = y_l;
    
    
    std::vector<float> newSignal;
    for(int j = 0; j < channels; j++)
    {
        newSignal.push_back(signal[j] * control);
    }
    
    delta = control;
    
    

    return newSignal;
}

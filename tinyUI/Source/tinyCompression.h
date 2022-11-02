//
//  tinyCompression.hpp
//  tinyUI
//
//  Created by Alec Denny on 9/30/22.
//

#ifndef tinyCompression_hpp
#define tinyCompression_hpp


#include <stdio.h>
#include <JuceHeader.h>
#include <atomic>
#include <vector>

//when adding valueStateTree this will be included from else where 
enum Parameters
{
    threshold,
    ratio,
    attack,
    release,
    numParams
};

class TinyCompressor
{
public:
    TinyCompressor(juce::AudioProcessorValueTreeState& ap, int sampleRate);
    ~TinyCompressor(){};
    
    float coeffFromMs(float timeInMs, int sampleRate);
    /*
     * ok here is the idea:
     * input 0, 1, 1, 1, 1, 1, 1 ... 1
     * take N samples to ramp from 0 to 1 by applying the process function
     * i.e. 0 + coeff + coeff + coeff ... + coeff = 1
     *
     * n = attackTimeinSamps
     *    E  (coeff)
     * n = 0
     *^^^ this is dumb
     * it is just sample rate to time conversion and then 1 / samps
     * since we recieve a new envelope value every sample we dont have to do anything else
     */
    //wondering if this needs to do anything not already accomplished by updateParameters
    //void prepare(std::array<std::atomic<float>, Parameters::numParams>& values, int sampleRate);
    
    void updateParameters(juce::AudioProcessorValueTreeState& ap, int sampleRate);
    
    std::vector<float> process(std::vector<float>& signal, int channels, int sampleRate);
    /*
     * prevLevel{0}
     * level = (signal - threshold) * ( > 0)
     *
     * if level > prevLevel
     *  newLevel = prevLevel + (attackCoeff * (level - prevLevel))
     * elif level < prevLevel
     *  newLevel = prevLevel + (releaseCoeff * (level - prevLevel))
     * else
     *  newLevel = prevLevel
     *
     * gainReduction = 1.0 - (ratio * newLevel)
     * signal = signal * gainReduction
     *
     * notably may need to do some dB conversion and absolute value thingy
     * potentially some RMS calculation in the future
     */
    float delta;
    
private:
    
    void paramsFrom0to1(std::vector<float>& p, int sampleRate);
    
    
    float dbtoa(float db);
    float atodb(float a);
    /*
    std::atomic<float> threshold;
    std::atomic<float> ratio;
    
    std::atomic<float> attackCoeff;
    std::atomic<float> releaseCoeff;
     */
    
    //indexed by enum params
    
    std::array<float, Parameters::numParams> localParams;
    
    //std::atomic<float> attackCoeff;
    //std::atomic<float> releaseCoeff;
    
    float envLevel{1.0};
    float prevGainredux{1.0};
};

#endif /* tinyCompression_hpp */

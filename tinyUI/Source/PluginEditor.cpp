/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

//==============================================================================
TinyUIAudioProcessorEditor::TinyUIAudioProcessorEditor (TinyUIAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 400);
   
    Timer::startTimerHz(120);
    
    
    
    
    screenParams.push_back(audioProcessor.apvts.getParameter("INGAIN"));
    screenParams.push_back(audioProcessor.apvts.getParameter("OUTGAIN"));
    screenParams.push_back(audioProcessor.apvts.getParameter("MIX"));
    
    
    scrn.prepParams(screenParams);
    addAndMakeVisible(&scrn);
    
    addAndMakeVisible(&knobTest);
    addAndMakeVisible(&ratioKnob);
    addAndMakeVisible(&attackKnob);
    addAndMakeVisible(&releaseKnob);
    
    
    
}

TinyUIAudioProcessorEditor::~TinyUIAudioProcessorEditor()
{
}

//==============================================================================
void TinyUIAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colour(59, 205, 0));
    //g.fillAll (juce::Colour(30, 40, 0));

    g.setColour (juce::Colours::white);
    
    //paintUI(g);
    scrn.updateThresholdRatioAttenuation(knobTest.getValue(), ratioKnob.getValue(), audioProcessor.redux.load());
    
    scrn.paint(g);
    scrn.toBack();
    
    knobTest.paint(g);
    ratioKnob.paint(g);
    attackKnob.paint(g);
    releaseKnob.paint(g);
    
    
    
    
}

void TinyUIAudioProcessorEditor::timerCallback()
{
    repaint();
}

void TinyUIAudioProcessorEditor::resized()
{
    //in the future this can be implemented with 'parentSizeChanged()' i think.
    //i am also not sure if this gets called when the thing is made so i dont know whne bounding box gets initially set
    scrn.setBounds(scrn.boundingBox);
    knobTest.setBounds(knobTest.boundingBox);
    ratioKnob.setBounds(ratioKnob.boundingBox);
    attackKnob.setBounds(attackKnob.boundingBox);
    releaseKnob.setBounds(releaseKnob.boundingBox);
    
    
    
}

void TinyUIAudioProcessorEditor::paintUI(juce::Graphics& g)
{
    /*
    myScreen->draw(g, 1.0, juce::AffineTransform(juce::AffineTransform::scale(0.5, 0.5)));
    myOverlay->draw(g, 1.0, juce::AffineTransform::translation(37.3, 20.3).followedBy(juce::AffineTransform::scale(0.5, 0.5)));
    */
    g.setColour(juce::Colour(0, 63, 84));
    g.drawRect(220, 100, 80, 80, 10);
    auto len = knobTest.getValue() * 80;
    
    auto dyRatio = 8.0 - ratioKnob.getValue();
    
    //draw Threshold
    g.drawLine(220, 180, 220 + len, 180 - len, 5);
    
    //draw Ratio
    g.drawLine(220 + len, 180 - len, 300, (180 - len) - ((300 - (220 + len)) * dyRatio), 5);
    
    auto rectHeight = static_cast<float>(audioProcessor.redux.load());
    //rectHeight = -20.0 * log10(rectHeight);
    rectHeight = 80.0 * rectHeight;
    int rectHeightScaled = static_cast<int>(rectHeight);
    
    g.setColour(juce::Colour(30,60,90));
    g.fillRect(300, 100 + rectHeightScaled, 50, 80 - rectHeightScaled);

}

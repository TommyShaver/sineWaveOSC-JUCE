#pragma once

#include <JuceHeader.h>

//==============================================================================
class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider (juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider) override
    {
        float diameter = juce::jmin(width, height);
        float radius = diameter / 2;
        float centerX = x + width / 2;
        float centerY = y + height / 2;
        float rx = centerX - radius;
        float ry = centerY - radius;
        float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));
        
        juce::Rectangle<float> dialArea (rx, ry, diameter, diameter);
        g.setColour(juce::Colours::grey);
        g.fillEllipse(dialArea);
        
        juce::Path dialTick;
        g.setColour(juce::Colours::red);
        
        dialTick.addRectangle(0, -radius, 5.0f, radius / 1.5);
        g.fillPath(dialTick);
        g.fillPath(dialTick,juce::AffineTransform::rotation(angle).translated(centerX, centerY));
        
        g.setColour(juce::Colours::silver);
        g.drawEllipse(rx, ry, diameter, diameter, 2.0f);
    }
    
};




class MainComponent  : public juce::AudioAppComponent,
                       public juce::Slider::Listener
{
    
public:
    
    void sliderValueChanged (juce::Slider* Slider) override
    {
        if (Slider == &frequnciesSlider)
        {
            frequency = frequnciesSlider.getValue();
        } else if (Slider == &ampSlider)
        {
            amplitude = ampSlider.getValue();
        }
            
    }
    void updateFrequency()
    {
        increment = frequency *wtSize / currentSampleRate;
        phase = fmod ((phase + increment), wtSize);
    
    }
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    juce::Array<double> wavetable;
    double wtSize;
    double frequency;
    double phase;
    double increment;
    double amplitude;
    double currentSampleRate;
    
    juce::Slider frequnciesSlider;
    juce::Slider ampSlider;
    OtherLookAndFeel otherLookAndFeelDail;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

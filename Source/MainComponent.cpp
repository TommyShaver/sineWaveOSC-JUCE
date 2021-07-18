#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(frequnciesSlider);
    frequnciesSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    frequnciesSlider.setRange(20.0, 20000.0,1);
    frequnciesSlider.setValue(500);
    frequnciesSlider.setSkewFactorFromMidPoint(5000);
    frequnciesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    frequnciesSlider.addListener(this);
    frequnciesSlider.setTextValueSuffix(" hz");
    frequnciesSlider.setLookAndFeel(&otherLookAndFeelDail);
    
    
    addAndMakeVisible(ampSlider);
    ampSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    ampSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    ampSlider.setRange(0.0, 1.0);
    ampSlider.setTextValueSuffix("%");
    ampSlider.setSkewFactorFromMidPoint(0.5);
    ampSlider.setValue(0.0);
    ampSlider.addListener(this);
    ampSlider.setLookAndFeel(&otherLookAndFeelDail);
    
    
    
    setSize (600, 350);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    frequnciesSlider.setLookAndFeel(nullptr);
    ampSlider.setLookAndFeel(nullptr);
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    frequency = frequnciesSlider.getValue();
    phase = 0;
    wtSize = 1024;
    increment = frequency *wtSize / currentSampleRate;
    amplitude = ampSlider.getValue();
    currentSampleRate = sampleRate;
    
    //this is one cycle of a sine wave
    for (int i = 0; i < wtSize; i++)
    {
        wavetable.insert(i, sin(2.0 * juce::double_Pi * i / wtSize));
    }
    
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    //Left speaker is = to 0
    float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    //Right speaker is = to 1
    float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    
    
    
    for (int sample = 0; sample < bufferToFill.numSamples; ++ sample)
    {
        //need to do to both speakers
        leftSpeaker[sample] = wavetable[(int)phase] * amplitude;
        rightSpeaker[sample] = wavetable[(int)phase] * amplitude;
        updateFrequency();
        
    }
    
    
    
}

void MainComponent::releaseResources()
{
    
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::rosybrown);
    g.setFont(40.0f);
    g.setColour(juce::Colours::white);
    g.drawText("SineWave Gen", 10, 10, 400, 400, juce::Justification::topLeft);
    
    g.setFont(15.0f);
    g.drawText("Frequecy", 100, 80, frequnciesSlider.getWidth(), frequnciesSlider.getHeight(), juce::Justification::centredTop);
    g.drawText("Volume", 300, 80, ampSlider.getWidth(), ampSlider.getHeight(), juce::Justification::centredTop);
    
}

void MainComponent::resized()
{
    frequnciesSlider.setBounds(100, 100, 200, 200);
    ampSlider.setBounds(300, 100, 200, 200);
}

#include "PluginProcessor.h"
#include "PluginEditor.h"

ProcessorEditor::ProcessorEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

ProcessorEditor::~ProcessorEditor()
{
}

//==============================================================================
void ProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Schroeder Reverb", getLocalBounds(), Justification::centred, 1);
}

void ProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

project(SchroederReverb VERSION 0.0.1)

set (BaseTargetName ${PROJECT_NAME})

juce_add_plugin("${BaseTargetName}"
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
	COMPANY_NAME "MirrorAudio"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE ADex
        PLUGIN_CODE BMfx
        FORMATS AU VST3
	PRODUCT_NAME "SchroederReverb")

juce_generate_juce_header(${BaseTargetName})

target_sources(${BaseTargetName} PRIVATE
		src/PluginProcessor.cpp
        src/PluginEditor.cpp)

target_compile_definitions(${BaseTargetName} PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JucePlugin_PreferredChannelConfigurations={2,2},{2,2})

target_link_libraries(${BaseTargetName} PRIVATE juce::juce_audio_utils dsp)

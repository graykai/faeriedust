# faeriedust

An system for dance performance with projection

## Dependencies

- Windows 10
- OpenFrameworks 0.11.2
  - Note that the Visual Studio solutions expect OpenFrameworks to exist in an `of` folder that is a peer to this repository.
- Kinect V2

## OSC Ports

- Kinect Bridge -> 8000 -> Renderer OSC
- TouchOSC -> 9000 -> Kinect Bridge

## OpenFrameworks Addons

- [ofxQuadtree](https://github.com/graysonarts/ofxQuadtree)
- [ofxMidi](https://github.com/danomatika/ofxMidi)

## Architecture

This system is split into a couple of pieces:

- Renderer which uses shaders and physics to simulate the Faerie Dust. It listens to OSC messages for controls
- KinectBridge which sends tracking points over OSC
- TouchOSC to tune how the kinect and physics works
  - All data is sent to KinectBridge which will relay physics params to the renderer
- midiScratchPad which sends messages based on midi input from a small pad controller so the screen and color aesthetics can be switched on the fly

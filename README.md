# faeriedust

An system for dance performance with projection

## Dependencies

- Windows 10
- OpenFrameworks 0.11.2
- Kinect V2

## OSC Ports

- Kinect Bridge -> 8000 -> Renderer OSC
- TouchOSC -> 9000 -> Kinect Bridge

## OpenFrameworks Addons

- Something

## Architecture

This system is split into a couple of pieces:

- Renderer which uses shaders and physics to simulate the Faerie Dust. It listens to OSC messages for controls
- KinectBridge which sends tracking points over OSC
- TouchOSC to tune how the kinect and physics works
  - All data is sent to KinectBridge which will relay physics params to the renderer

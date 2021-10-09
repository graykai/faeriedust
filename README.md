# faeriedust

An system for dance performance with projection

## Dependencies

* Windows 10
* OpenFrameworks 0.11.2
* Kinect V2

## OpenFrameworks Addons

* Something

## Architecture

This system is split into a couple of pieces:

* Renderer which uses shaders and physics to simulate the Faerie Dust. It listens to OSC messages for controls
* KinectBridge which sends tracking points over OSC
* Control Surface which is a debug and control panel that is helpful for tuning and troubleshooting

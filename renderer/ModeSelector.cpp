#include "ModeSelector.h"

void ModeSelector::setup() {
	midi.openPort(0);
	midi.setVerbose(true);
	midi.addListener(this);
}

void ModeSelector::newMidiMessage(ofxMidiMessage& msg) {
	if (msg.status != MIDI_NOTE_ON) return;
	ofLog() << msg.channel << ":" << msg.pitch;
	if (msg.channel != 10) return;
	mode = msg.pitch;
	changed = true;
}

void ModeSelector::update() {
	if (!changed) return;
	changed = false;
	auto activate = activators.find(mode);
	if (activate == activators.end()) return;

	activate->second();
}

void ModeSelector::addMode(int mode, Setter fnc) {
	activators.insert_or_assign(mode, fnc);
}
#include "driverwindow.h"
#include "./ui_driverwindow.h"

#define MAX_SYSEX_BUFFER 65535

DriverWindow::DriverWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DriverWindow)
    , midiPort(virtualMIDICreatePortEx2(L"Nano Breath Controller", nullptr, 0, MAX_SYSEX_BUFFER, TE_VM_FLAGS_INSTANTIATE_BOTH))
//    midiOut(new RtMidiOut(RtMidi::Api::WINDOWS_MM))
{
    ui->setupUi(this);
//    midiOut->openVirtualPort("NanoBreathController");
}

DriverWindow::~DriverWindow()
{
//    midiOut->closePort();
    if (midiPort) {
        virtualMIDIClosePort(midiPort);
    }
    delete ui;
}


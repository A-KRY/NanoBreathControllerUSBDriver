#ifndef DRIVERWINDOW_H
#define DRIVERWINDOW_H

#include <QWidget>
// #include "deprecated/RtMidi.h"
#include "teVirtualMIDI.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DriverWindow; }
QT_END_NAMESPACE

class DriverWindow : public QWidget
{
    Q_OBJECT

//    std::unique_ptr<RtMidiOut> midiOut;
    LPVM_MIDI_PORT midiPort;
//    std::unique_ptr<VM_MIDI_PORT> midiPort;

public:
    DriverWindow(QWidget *parent = nullptr);
    ~DriverWindow();

private:
    Ui::DriverWindow *ui;
};
#endif // DRIVERWINDOW_H

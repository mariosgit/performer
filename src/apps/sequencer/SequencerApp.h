#include "Config.h"

#include "drivers/Adc.h"
#include "drivers/ButtonLedMatrix.h"
#include "drivers/ClockTimer.h"
#include "drivers/Dac.h"
#include "drivers/Dio.h"
#include "drivers/Encoder.h"
#include "drivers/GateOutput.h"
#include "drivers/Lcd.h"
#include "drivers/Midi.h"
#include "drivers/SdCard.h"
#include "drivers/UsbMidi.h"

#include "core/fs/Volume.h"

#include "model/Model.h"
#include "model/FileManager.h"
#include "engine/Engine.h"
#include "ui/Ui.h"

#include "os/os.h"

static os::PeriodicTask<1024> fsTask("file", CONFIG_FILE_TASK_PRIORITY, os::time::ms(10), [] () {
    FileManager::processTask();
});

struct SequencerApp {
    // drivers
    ClockTimer clockTimer;
    ButtonLedMatrix blm;
    Lcd lcd;
    Adc adc;
    Dac dac;
    Dio dio;
    Encoder encoder;
    GateOutput gateOutput;
    Midi midi;
    UsbMidi usbMidi;
    SdCard sdCard;

    // filesystem
    fs::Volume volume;

    // application
    Model model;
    Engine engine;
    Ui ui;

    SequencerApp() :
        volume(sdCard),
        engine(model, clockTimer, adc, dac, dio, gateOutput, midi, usbMidi),
        ui(model, engine, lcd, blm, encoder)
    {
        model.init();
        engine.init();
        ui.init();
    }

    void update() {
        engine.update();
        ui.update();
    }
};

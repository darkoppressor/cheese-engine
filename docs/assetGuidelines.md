# Asset guidelines

## Asset format

### Audio

* Frequency: 44100 Hz
* Format: Signed 16-bit samples, in system byte order
* Channels: Stereo

## Asset creation

### Sound effects

#### SFXR

* Export as .wav, all default stuff
* Then, open in Audacity and export as ogg, default quality

#### LMMS

* Same as music

### Music

#### LMMS

* Export as .ogg, samplerate 44100 Hz, bitrate 160 Kbits/s, depth 16 bit integer, interpolation Sinc Medium,
oversampling 1x (None), Export as loop

### Notes

* Sound effects and music are referred to in the engine by their filename minus their file extension.
* So, to the engine, example.wav and example.ogg are the same sound. Do not include both.

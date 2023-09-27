# Techflash VGA Media (TVM)

## Specification v0.1

## Header


## Flags

| Bit # | Description                 | Recommended | What it does to the parsing                                                                                                                           |
|:-----:|-----------------------------|:-----------:|-------------------------------------------------------------------------------------------------------------------------------------------------------|
|   0   | Enable Attribute Byte       |      1      | Saves space on disk by not including the color/attribute byte.                                                                                        |
|   1   | Enable PC Speaker Audio     |      0      | (***VERY BAD, slows playback SIGNIFICANTLY***) If enabled, the app will try to initialize the PC speaker, and play [PC Speaker chunks](#pc-spekaker). |
|   2   | Enable MIDI Audio Chunks    |      1      | If enabled, increases speed by jumping over the code for checking for [MIDI chunks](#midi).                                                           |
|   3   | Enable SoundBlaster Support |      0      | If enabled, the app will try to find and initialize a SoundBlaster compatible card.  This will implicitly disable MIDI.                               |
|   4   | Enable Title Bar            |      1      | Puts the title on the top of the screen.  Improves rendering performance by drawing a few less characters, at the cost of 1 less graphics line.       |
|   5   | Enable FPS Changer chunks   |      1      | If disabled, increases speed by jumping over the code for checking for [FPS Changer chunks](#fps-changer)                                             |
|   6   | Reserved                    |      0      |                                                                                                                                                       |
|   7   | Reserved                    |      0      |                                                                                                                                                       |
|   8   | Reserved                    |      0      |                                                                                                                                                       |
|   9   | Reserved                    |      0      |                                                                                                                                                       |
|   10  | Reserved                    |      0      |                                                                                                                                                       |
|   11  | Reserved                    |      0      |                                                                                                                                                       |
|   12  | Reserved                    |      0      |                                                                                                                                                       |
|   13  | Reserved                    |      0      |                                                                                                                                                       |
|   14  | Reserved                    |      0      |                                                                                                                                                       |
|   15  | Reserved                    |      0      |                                                                                                                                                       |``


## Chunk Format

|     Offset     | Data Type |       Field       |  Value | Required to be this value |
|:--------------:|:---------:|:-----------------:|:------:|:-------------------------:|
|      0 - 3     |  char []  |    Chunk Magic    | "VMCH" |            YES            |
|        4       |  uint8_t  |     Identifier    |  0x01  |             NO            |
|      5 - 6     |  uint16_t |     Data Size     |   775  |             NO            |
|      7 - 8     |  uint16_t | Frame Data Offset |   125  |             NO            |
| 8 - (size + 8) |    any    | Data (if not gfx) |        |             NO            |



# Techflash VGA Media (TVM)

## Specification v0.1

## Header

|  Byte  |           Description           |      Data type      |          Value         | Requried to be this value |                                                                Notes                                                                 |
|:------:|:-------------------------------:|:-------------------:|:----------------------:|:-------------------------:|--------------------------------------------------------------------------------------------------------------------------------------|
|  0 - 3 |            File magic           |       char []       |         "TFVM"         |            YES            |                                                                                                                                      |
|    4   |   Format version number (BCD)   |       uint8_t       |         0x0100         |             NO            | e.g. 0x011A would be v1.1a, 0x0100 would be v1.0, etc. A player (like TFVMPLAY) should only play versions enabled at compile time.   |
|  5 - 6 |          Flags bitmask          |       uint16_t      |         0x7F04         |             NO            | See [details](#flags) about what flags are supported                                                                                 |
|    7   | Compression type & Starting FPS |       bitfield      |    47 (gzip, 30fps)    |             NO            | Compression types (3 bits): 0 = raw; 1 = gzip; 2 = zstd; 3 = lzma; 4 = lz4; FPS (5 bits, >> by 1 to store <= 60, only even numbers). |
| 7 - 63 |            Video Name           | char [] (NULL Term) | NULL Terminated String |             NO            | Any NULL terminated string.  Any remaining space after the end should be padded with NULLs.                                          |

### Flags

| Bit # | Description                 | Recommended | What it does to the parsing                                                                                                                          |
|:-----:|-----------------------------|:-----------:|------------------------------------------------------------------------------------------------------------------------------------------------------|
|   0   | Enable Attribute Byte       |      1      | Saves space on disk by not including the color/attribute byte.                                                                                       |
|   1   | Enable PC Speaker Audio     |      0      | (***VERY BAD, slows playback SIGNIFICANTLY***) If enabled, the app will try to initialize the PC speaker, and play [PC Speaker chunks](#pc-speaker). |
|   2   | Enable MIDI Audio Chunks    |      1      | If enabled, increases speed by jumping over the code for checking for [MIDI chunks](#midi).                                                          |
|   3   | Enable SoundBlaster Support |      0      | If enabled, the app will try to find and initialize a SoundBlaster compatible card.  This will implicitly disable MIDI.                              |
|   4   | Enable Title Bar            |      1      | Puts the title on the top of the screen.  Improves rendering performance by drawing a few less characters, at the cost of 1 less graphics line.      |
|   5   | Enable FPS Changer chunks   |      1      | If disabled, increases speed by jumping over the code for checking for [FPS Changer chunks](#fps-changer)                                            |
|   6   | Enable Partial GFX Chunks   |      1      | If enabled, the partial graphics chunks are supported.  This allows the chunk to only hold a diff of what was changed.                               |
|   7   | Reserved                    |      0      |                                                                                                                                                      |
|   8   | Reserved                    |      0      |                                                                                                                                                      |
|   9   | Reserved                    |      0      |                                                                                                                                                      |
|   10  | Reserved                    |      0      |                                                                                                                                                      |
|   11  | Reserved                    |      0      |                                                                                                                                                      |
|   12  | Reserved                    |      0      |                                                                                                                                                      |
|   13  | Reserved                    |      0      |                                                                                                                                                      |
|   14  | Reserved                    |      0      |                                                                                                                                                      |
|   15  | Reserved                    |      0      |                                                                                                                                                      |

### Chunk Format

|     Offset     | Data Type |    Field    |  Value | Required to be this value |
|:--------------:|:---------:|:-----------:|:------:|:-------------------------:|
|      0 - 3     |  char []  | Chunk Magic | "VMCH" |            YES            |
|        4       |  uint8_t  |  Identifier |  0x01  |             NO            |
|      5 - 6     |  uint16_t |  Data Size  |   775  |             NO            |
| 7 - (size + 7) |    any    |     Data    |        |             NO            |

### Chunk Types

The chunk types have been split up:

* <span name="null-chunk">`0x00`: NULL Chunk</span>
  * Ecountering one of these will be logged as [LOGLEVEL_INFO](#logging-levels).  Perhaps it is for padding purposes, or perhaps a corrupted file.
    * If the size for said chunk is also 0, this event will be logged is [LOGLEVEL_ERROR](#logging-levels), and the player may choose to:
    * Treat this as fatal, give up attempting to parse the rest of the file.
    * Follow [the recovery chunk parsing rules](#recovery-parsing-rules)

* `0x01` - `0x2F`: "Normal" Chunks
  * If encountered, it should follow [the normal parsing rules](#parsing-rules)

* `0x30` - `0xDF`: Reserved Chunks
  * These chunks are reserved for future expansion for either normal, special, or any other types of chunks.
  * If encountered, they should be treated the same as [NULL chunks](#null-chunk)

* `0xE0` - `0xFF`: Special Chunks
  * These are chunks that perform special functions outside of actual content.
  * If one should be encountered it follow [the normal parsing rules](#parsing-rules)

#### Graphics

* `Identifier`: `0x01`
* `Data Size`:  Size of the uncompressed graphics data in RAM
* `Data`: Offset (in frames) of the uncompressed graphics data
  * It is the choice of the app how it should find this point within the compressed data.

#### PC Speaker

* `Identifier`: `0x02`
* `Data Size`:  Size of the PC Speaker instructions data in bytes
* `Data`:  The PC Speaker instructions data

#### MIDI

* `Identifier`: `0x03`
* `Data Size`:  Size of the MIDI instructions data in bytes
* `Data`:  The MIDI instructions data
  * The first bytes is a [MIDI Device ID Bitmask](#midi-device-id-bitmask), detailing which MIDI card(s) this audio clip should play on.

##### MIDI Device ID Bitmask

TODO

#### SoundBlaster PCM Audio

* `Identifier`: `0x03`
* `Data Size`:  Size of the uncompressed PCM Data to push to the sound card.
* `Data`:  The offset in the [compressed PCM data](#compressed-pcm-data)

#### FPS Changer

* `Identifier`: `0xE0`
* `Data Size`:  Should always be 1, for the 1 byte new FPS.  Otherwise, follow the [recovery chunk parsing rules](#recovery-parsing-rules)
* `Data`:  The offset in the [compressed PCM data](#compressed-pcm-data)

#### Title Changer

* `Identifer`: `0xE1`
* `Data Size`: Size of the new title.  If larger than 80, it shall be truncated to 80, with a [LOGLEVEL_ERROR](#logging-levels) event.
* `Data`: The new title to be displayed.

## Data

Now, after the 63 byte header, comes the data, starting at offset `0x40` in the file.

They go in the following order.

### Graphics Data

A giant blob of data (this helps with compression) containing every frame of graphics data.
After being decompressed (or if stored as raw data), each frame has the following header:

| Byte #         | Data Type  | Description | Value  |
|----------------|------------|-------------|--------|
| 0-3            | char []    | Gfx Magic   | "VMGX" |
| 4-5            | uint16_t   | Size        |        |
| 6 - (size + 6) | uint8_t [] | Gfx Data    |        |

It can be accessed via index.  It is accessed like this:

1. Get index of graphics data that you want to access
2. Start from the beginning of the data, or the last cached position.
3. Do something like: `chunk->size += (chunk->size + sizeof(chunk))` to skip to the next chunk
4. Is the current index a multiple of 64?  If so, cache what byte offset we're at so that we don't need to start from the beginning next time.
5. Are we at the index we want yet?  If not, go back to step #3.  Otherwise return where we are.

### Compressed PCM Data

The compressed PCM Data (for SoundBlaster cards) comes after the final graphics chunk.
It has a very similar format to the [graphics data](#graphics-data)

| Byte #         | Data Type  | Description | Value  |
|----------------|------------|-------------|--------|
| 0-3            | char []    | PCM Magic   | "VMPC" |
| 4-5            | uint16_t   | Size        |        |
| 6 - (size + 6) | uint8_t [] | PCM Data    |        |

It can be accessed in the same way as the graphics data.

## Parsing Rules

The chunk should be compared to the list of chunks that are known to be valid at the compile time of the player.  What happens after depend on the state of the ID.

* Known to be valid, and has a corresponding handler: Run the handler
* Known to be valid, but has no handler (disabled at compile time, or perhaps just never implemented): Log a warning with [LOGLEVEL_WARN](#logging-levels), and skip this chunk.
* **NOT** known to be valid, and as such has no handler: Log an error with [LOGLEVEL_ERROR](#logging-levels), perhaps the file version is incorrect and it actually uses newer features than it's supposed to, or perhaps this plsyer does not follow the specification.

## Recovery Parsing Rules

Try to recover, reading every byte until either:

* Finding the next chunk header, continuing playback and logging this with [LOGLEVEL_WARN]
* Encountering invalid data (neither more 0's, nor a chunk header), giving up and logging with [LOGLEVEL_FATAL](#logging-levels).

## Logging Levels

There are 5 basic log levels available:

* `LOGLEVEL_DEBUG` (0): For extremely verbose logs.  For speed purposes, these should be disabled by default at compile time.
* `LOGLEVEL_INFO`  (1): Default informational logs.  These should be disabled if it is detected at runtime that your computer is too slow.  If this is the case, the log function will return instantly if the log level is info.
* `LOGLEVEL_WARN`  (2): For Warnings.  These should be kept enabled, unless somehow the TVM file is badly formed, causing constant warnings.
* `LOGLEVEL_ERROR` (3): Major errors.  For this and below, it should never be allowed to be disabled.
* `LOGLEVEL_FATAL` (4): Fatal errors that there is no way to recover from.  This will cause the player to exit, and print the fatal error message in the console.

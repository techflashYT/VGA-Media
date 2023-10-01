#ifndef FLAGS_H
#define FLAGS_H

#define FLAG_ENABLE_ATTRIBUTE_BYTE       (1 << 0)
#define FLAG_ENABLE_PC_SPEAKER_AUDIO     (1 << 1)
#define FLAG_ENABLE_MIDI_AUDIO_CHUNKS    (1 << 2)
#define FLAG_ENABLE_SOUNDBLASTER_SUPPORT (1 << 3)
#define FLAG_ENABLE_TITLE_BAR            (1 << 4)
#define FLAG_ENABLE_FPS_CHANGER_CHUNKS   (1 << 5)
#define FLAG_ENABLE_PARTIAL_GFX_CHUNKS   (1 << 6)
#define FLAG_RESERVED_7                  (1 << 7)
#define FLAG_RESERVED_8                  (1 << 8)
#define FLAG_RESERVED_9                  (1 << 9)
#define FLAG_RESERVED_10                 (1 << 10)
#define FLAG_RESERVED_11                 (1 << 11)
#define FLAG_RESERVED_12                 (1 << 12)
#define FLAG_RESERVED_13                 (1 << 13)
#define FLAG_RESERVED_14                 (1 << 14)
#define FLAG_RESERVED_15                 (1 << 15)

#define FLAGS_DEFAULT (FLAG_ENABLE_ATTRIBUTE_BYTE | FLAG_ENABLE_MIDI_AUDIO_CHUNKS | FLAG_ENABLE_TITLE_BAR | FLAG_ENABLE_FPS_CHANGER_CHUNKS | FLAG_ENABLE_PARTIAL_GFX_CHUNKS)

#endif // FLAGS_H
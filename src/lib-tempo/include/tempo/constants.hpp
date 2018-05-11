#define BPM 138                // Beats per minutes
#define PHASE 0                // Microseconds
#define DELTA 100       // Delta around a beat a player can hit (millisecs)
#define TIME(x) (60000000.f / x)  // Time between beats (microsecs)
#define SONG ("resources/sound/rave-cave-boss-1.ogg") // song file
#define PLAYER_MAX_HEALTH 1000000000
#define PLAYER_DAMAGE 10
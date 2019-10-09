#ifndef DEF_CONSTANTS
#define DEF_CONSTANTS

// 30x30 tiles
//#define TILE_SIZE_PIXEL 16
#define TILE_SIZE_PIXEL 50
#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400
#define FPS 60
#define DROP_TILE_THRESHOLD 2.f
#define GRAVITY 1800
#define MAX_FALLING_SPEED 600
#define MIN_JUMP_SPEED 300
#define CHAR_JUMP_SPEED -600
#define CHAR_WALK_SPEED 300

#define NB_KEY_CHARACTER 6
enum KeyInputChar{GoLeft=0,GoRight,GoDown,GoJump,SizeUp,SizeDown};

#endif
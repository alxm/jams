typedef struct ZCompSprite ZCompSprite;

typedef enum ZCompSpriteDirection {
    Z_COMP_SPRITE_DIRECTION_UP,
    Z_COMP_SPRITE_DIRECTION_DOWN,
    Z_COMP_SPRITE_DIRECTION_LEFT,
    Z_COMP_SPRITE_DIRECTION_RIGHT,
    Z_COMP_SPRITE_DIRECTION_NUM
} ZCompSpriteDirection;

extern size_t z_comp_sprite_size(void);
extern AComponentFree z_comp_sprite_free;
extern void z_comp_sprite_init(ZCompSprite* Sprite, const char* Keys);

extern void z_comp_sprite_stop(ZCompSprite* Sprite);
extern void z_comp_sprite_move(ZCompSprite* Sprite);
extern ZCompSpriteDirection z_comp_sprite_getDir(const ZCompSprite* Sprite);
extern void z_comp_sprite_setDir(ZCompSprite* Sprite, ZCompSpriteDirection Direction);
extern ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite);

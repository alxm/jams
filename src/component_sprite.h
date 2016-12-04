typedef struct ZCompSprite ZCompSprite;

extern size_t z_comp_sprite_size(void);
extern AComponentFree z_comp_sprite_free;
extern void z_comp_sprite_init(ZCompSprite* Sprite, const char* Key);

extern void z_comp_sprite_stop(const ZCompSprite* Sprite);
extern void z_comp_sprite_move(const ZCompSprite* Sprite);
extern ASprite* z_comp_sprite_getFrame(const ZCompSprite* Sprite);

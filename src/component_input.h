typedef struct ZCompInput ZCompInput;
typedef void ZCompInputHandler(AEntity* Entity);

extern size_t z_comp_input_size(void);
extern void z_comp_input_init(ZCompInput* Input, ZCompInputHandler* Handler);

extern void z_comp_input_callHandler(ZCompInput* Input);

typedef struct ZCompVelocity ZCompVelocity;

extern size_t z_comp_velocity_size(void);
extern void z_comp_velocity_init(ZCompVelocity* Velocity);

extern int z_comp_velocity_getSpeed(const ZCompVelocity* Velocity);

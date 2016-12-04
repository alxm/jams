#pragma once

typedef struct ZControls {
    AInput* up;
    AInput* down;
    AInput* left;
    AInput* right;
} ZControls;

extern ZControls z_controls;

extern void z_controls_load(void);

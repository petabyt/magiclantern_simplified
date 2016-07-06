/**
 * 70D version
 */
struct mpu_init_spell mpu_init_spells_70D[] = { {
    { 0x06, 0x04, 0x02, 0x00, 0x00 }, {                         /* spell #1 */
        { 0x70, 0x6e, 0x02, 0x0e, 0x03, 0x03, 0x01, 0x00, 0x03, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x14, 0x50, 0x00, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x06, 0x04, 0x00, 0x00, 0x06, 0x04, 0x00, 0x00, 0x06, 0x04, 0x01, 0x01, 0x00, 0x00, 0x3c, 0x00, 0x60, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x80, 0x48, 0x01, 0x00, 0x78, 0x48, 0x00, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #1.1 */
        { 0x06, 0x05, 0x01, 0x4f, 0x00, 0x00 },                 /* reply #1.2 */
        { 0x2c, 0x2b, 0x02, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00 },/* reply #1.3 */
        { 0x34, 0x32, 0x02, 0x0f, 0x01, 0x00, 0x00, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 },/* reply #1.4 */
        { 0x40, 0x3e, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x02, 0x01, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x01, 0x05, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00 },/* reply #1.5 */
        { 0x3a, 0x38, 0x02, 0x11, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00 },/* reply #1.6 */
        { 0x0a, 0x08, 0x01, 0x6e, 0x80, 0x48, 0x01, 0x00, 0x00 },/* reply #1.7 */
        { 0x08, 0x06, 0x01, 0x0b, 0x00, 0x00, 0x00 },           /* reply #1.8 */
        { 0x06, 0x05, 0x01, 0x76, 0x00, 0x00 },                 /* reply #1.9 */
        { 0x06, 0x05, 0x01, 0x71, 0x00, 0x00 },                 /* reply #1.10 */
        { 0x06, 0x05, 0x01, 0x7e, 0x00, 0x00 },                 /* reply #1.11 */
        { 0x06, 0x05, 0x01, 0x7c, 0x00, 0x00 },                 /* reply #1.12 */
        { 0x10, 0x0f, 0x01, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #1.13 */
        { 0x0a, 0x08, 0x01, 0x85, 0x00, 0x00, 0x00, 0x01, 0x00 },/* reply #1.14 */
        { 0x08, 0x07, 0x01, 0x6f, 0x00, 0x00, 0x00, 0x00 },     /* reply #1.15 */
        { 0x08, 0x06, 0x01, 0x47, 0x0a, 0x02, 0x00 },           /* reply #1.16 */
        { 0x06, 0x05, 0x01, 0x7b, 0x00, 0x00 },                 /* reply #1.17 */
        { 0x08, 0x06, 0x01, 0x57, 0x00, 0x00, 0x00 },           /* reply #1.18 */
        { 0x06, 0x05, 0x01, 0x86, 0x00, 0x00 },                 /* reply #1.19 */
        { 0x06, 0x05, 0x01, 0x87, 0x00, 0x00 },                 /* reply #1.20 */
        { 0x06, 0x05, 0x01, 0x8d, 0x00, 0x00 },                 /* reply #1.21 */
        { 0x06, 0x05, 0x01, 0x8e, 0x00, 0x00 },                 /* reply #1.22 */
        { 0x06, 0x05, 0x01, 0x90, 0x00, 0x00 },                 /* reply #1.23 */
        { 0x06, 0x05, 0x01, 0x8b, 0x00, 0x00 },                 /* reply #1.24 */
        { 0x08, 0x06, 0x01, 0x91, 0x00, 0x0f, 0x00 },           /* reply #1.25 */
        { 0x08, 0x06, 0x01, 0x9c, 0x00, 0x00, 0x00 },           /* reply #1.26 */
        { 0x06, 0x05, 0x01, 0x98, 0x00, 0x00 },                 /* reply #1.27 */
        { 0x06, 0x05, 0x01, 0x92, 0x00, 0x00 },                 /* reply #1.28 */
        { 0x06, 0x05, 0x01, 0x8c, 0x00, 0x00 },                 /* reply #1.29 */
        { 0x26, 0x24, 0x01, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #1.30 */
        { 0x18, 0x17, 0x03, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #1.31 */
        { 0x10, 0x0e, 0x02, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x05, 0x03, 0x00 },/* reply #1.32 */
        { 0x28, 0x26, 0x02, 0x07, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x07, 0x07, 0x11, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #1.33 */
        { 0x22, 0x20, 0x02, 0x08, 0x05, 0x00, 0x00, 0x03, 0x00, 0x14, 0x04, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x00 },/* reply #1.34 */
        { 0x28, 0x26, 0x02, 0x07, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x07, 0x07, 0x11, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #1.35 */
        { 0x06, 0x05, 0x03, 0x37, 0x00, 0x00 },                 /* reply #1.36 */
        { 0x0a, 0x08, 0x03, 0x2f, 0x00, 0x40, 0x00, 0x00, 0x00 },/* reply #1.37 */
        { 0x06, 0x05, 0x03, 0x20, 0x01, 0x00 },                 /* reply #1.38 */
        { 0x0c, 0x0b, 0x03, 0x42, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #1.39 */
        { 0x06, 0x05, 0x03, 0x6f, 0x00, 0x00 },                 /* reply #1.40 */
        { 0x06, 0x05, 0x03, 0x6f, 0x00, 0x00 },                 /* reply #1.41 */
/* fixme: 0x80000001 does not complete */
//      { 0 } } }, {
//  { 0x08, 0x06, 0x00, 0x00, 0x02, 0x0e, 0x00 }, {             /* spell #2, Complete WaitID = 0x80000001 */
        { 0x06, 0x05, 0x03, 0x6f, 0x00, 0x00 },                 /* reply #2.1 */
        { 0x06, 0x05, 0x03, 0x6f, 0x00, 0x00 },                 /* reply #2.2 */
        { 0x06, 0x05, 0x01, 0x48, 0x01, 0x00 },                 /* reply #2.3 */
        { 0x06, 0x05, 0x01, 0x49, 0x01, 0x00 },                 /* reply #2.4 */
        { 0x0a, 0x08, 0x01, 0x85, 0x00, 0x00, 0x00, 0x01, 0x00 },/* reply #2.5 */
        { 0x2c, 0x2b, 0x02, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00 },/* reply #2.6 */
        { 0x34, 0x32, 0x02, 0x0f, 0x01, 0x00, 0x00, 0x10, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 },/* reply #2.7 */
        { 0x40, 0x3e, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x00, 0x00, 0x02, 0x01, 0x01, 0x00, 0x03, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x01, 0x05, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00 },/* reply #2.8 */
        { 0x3a, 0x38, 0x02, 0x11, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x23, 0x00, 0x00, 0x00, 0x00 },/* reply #2.9 */
        { 0x0a, 0x08, 0x01, 0x6e, 0x80, 0x48, 0x01, 0x00, 0x00 },/* reply #2.10 */
        { 0x08, 0x06, 0x01, 0x0b, 0x00, 0x00, 0x00 },           /* reply #2.11 */
        { 0x06, 0x05, 0x01, 0x76, 0x00, 0x00 },                 /* reply #2.12 */
        { 0x06, 0x05, 0x01, 0x71, 0x00, 0x00 },                 /* reply #2.13 */
        { 0x06, 0x05, 0x01, 0x7e, 0x00, 0x00 },                 /* reply #2.14 */
        { 0x06, 0x05, 0x01, 0x7c, 0x00, 0x00 },                 /* reply #2.15 */
        { 0x10, 0x0f, 0x01, 0x8f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #2.16 */
        { 0x0a, 0x08, 0x01, 0x85, 0x00, 0x00, 0x00, 0x01, 0x00 },/* reply #2.17 */
        { 0x08, 0x07, 0x01, 0x6f, 0x00, 0x00, 0x00, 0x00 },     /* reply #2.18 */
        { 0x08, 0x06, 0x01, 0x47, 0x0a, 0x02, 0x00 },           /* reply #2.19 */
        { 0x06, 0x05, 0x01, 0x7b, 0x00, 0x00 },                 /* reply #2.20 */
        { 0x08, 0x06, 0x01, 0x57, 0x00, 0x00, 0x00 },           /* reply #2.21 */
        { 0x06, 0x05, 0x01, 0x86, 0x00, 0x00 },                 /* reply #2.22 */
        { 0x06, 0x05, 0x01, 0x87, 0x00, 0x00 },                 /* reply #2.23 */
        { 0x06, 0x05, 0x01, 0x8d, 0x00, 0x00 },                 /* reply #2.24 */
        { 0x06, 0x05, 0x01, 0x8e, 0x00, 0x00 },                 /* reply #2.25 */
        { 0x06, 0x05, 0x01, 0x90, 0x00, 0x00 },                 /* reply #2.26 */
        { 0x06, 0x05, 0x01, 0x8b, 0x00, 0x00 },                 /* reply #2.27 */
        { 0x08, 0x06, 0x01, 0x91, 0x00, 0x0f, 0x00 },           /* reply #2.28 */
        { 0x08, 0x06, 0x01, 0x9c, 0x00, 0x00, 0x00 },           /* reply #2.29 */
        { 0x06, 0x05, 0x01, 0x98, 0x00, 0x00 },                 /* reply #2.30 */
        { 0x06, 0x05, 0x01, 0x92, 0x00, 0x00 },                 /* reply #2.31 */
        { 0x06, 0x05, 0x01, 0x8c, 0x00, 0x00 },                 /* reply #2.32 */
        { 0x26, 0x24, 0x01, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #2.33 */
        { 0x18, 0x17, 0x03, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #2.34 */
        { 0x28, 0x26, 0x02, 0x07, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x07, 0x07, 0x11, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #2.35 */
        { 0x0a, 0x09, 0x01, 0x55, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01 },/* reply #2.36 */
        { 0 } } }, {
    { 0x08, 0x06, 0x00, 0x00, 0x01, 0x55, 0x00 }, {             /* spell #3, Complete WaitID = 0x8000003F */
        { 0x0a, 0x09, 0x01, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 },/* reply #3.1 */
        { 0 } } }, {
    { 0x08, 0x06, 0x00, 0x00, 0x01, 0x70, 0x00 }, {             /* spell #4, Complete WaitID = 0x80000045 */
        { 0x06, 0x05, 0x01, 0x97, 0x00, 0x01 },                 /* reply #4.1 */
        { 0 } } }, {
    { 0x08, 0x06, 0x00, 0x00, 0x01, 0x97, 0x00 }, {             /* spell #5, Complete WaitID = 0x8003006F */
        { 0 } } }, {
    { 0x08, 0x07, 0x03, 0x6a, 0x00, 0x01, 0x00, 0x00 }, {       /* spell #6 */
        { 0x70, 0x6e, 0x02, 0x0e, 0x03, 0x03, 0x01, 0x00, 0x03, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x14, 0x50, 0x00, 0x00, 0x00, 0x00, 0x87, 0x00, 0x00, 0x06, 0x04, 0x00, 0x00, 0x06, 0x04, 0x00, 0x00, 0x06, 0x04, 0x01, 0x01, 0x00, 0x00, 0x3c, 0x00, 0x60, 0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x00, 0x80, 0x48, 0x01, 0x00, 0x78, 0x48, 0x00, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #6.1 */
        { 0 } } }, {
    { 0x08, 0x06, 0x00, 0x00, 0x02, 0x0e, 0x00 }, {             /* spell #7, Complete WaitID = 0x80000001 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 }, { /* spell #8 */
        { 0 } } }, {
    { 0x06, 0x04, 0x03, 0x10, 0x00 }, {                         /* spell #9 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x07, 0xff, 0x00 }, {                   /* spell #10 */
        { 0x64, 0x62, 0x02, 0x12, 0x01, 0x20, 0x50, 0x00, 0xae, 0x00, 0x32, 0x00, 0x96, 0x91, 0x15, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55, 0x55, 0x65, 0x55, 0x55, 0x55, 0x00, 0xae, 0x00, 0xae, 0x00, 0xae, 0x00, 0xae, 0x00, 0xae, 0x00, 0xae, 0x00 },/* reply #10.1 */
        { 0x06, 0x05, 0x03, 0x35, 0x01, 0x00 },                 /* reply #10.2 */
        { 0x1c, 0x1b, 0x03, 0x1d, 0x41, 0x02, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x4c, 0x50, 0x2d, 0x45, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x4c, 0x35, 0x7a, 0xbd, 0x00 },/* reply #10.3 */
        { 0x06, 0x04, 0x03, 0x36, 0x00 },                       /* reply #10.4 */
        { 0x06, 0x05, 0x03, 0x05, 0x04, 0x00 },                 /* reply #10.5 */
        { 0 } } }, {
    { 0x06, 0x05, 0x01, 0x2e, 0x01, 0x00 }, {                   /* spell #11 */
        { 0x06, 0x05, 0x01, 0x2e, 0x01, 0x00 },                 /* reply #11.1 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x03, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00 }, { /* spell #12 */
        { 0 } } }, {
    { 0x08, 0x07, 0x01, 0x2a, 0x16, 0xf3, 0x00, 0x00 }, {       /* spell #13 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x07, 0x0e, 0x00 }, {                   /* spell #14 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x03, 0x06, 0x00, 0x00, 0x02, 0x07, 0x00 }, { /* spell #15 */
        { 0 } } }, {
    { 0x06, 0x05, 0x01, 0x56, 0x00, 0x00 }, {                   /* spell #16 */
        { 0 } } }, {
    { 0x06, 0x05, 0x04, 0x0e, 0x01, 0x00 }, {                   /* spell #17 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x11, 0x01, 0x00 }, {                   /* spell #18 */
        { 0 } } }, {
    { 0x06, 0x05, 0x02, 0x0a, 0x00, 0x00 }, {                   /* spell #19 */
        { 0x06, 0x05, 0x01, 0x2c, 0x02, 0x00 },                 /* reply #19.1 */
        { 0x12, 0x10, 0x03, 0x00, 0xb1, 0x00, 0x00, 0x32, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #19.2 */
        { 0x06, 0x05, 0x03, 0x23, 0x09, 0x00 },                 /* reply #19.3 */
        { 0x06, 0x05, 0x03, 0x04, 0x00, 0x00 },                 /* reply #19.4 */
        { 0x0e, 0x0d, 0x03, 0x24, 0x35, 0x30, 0x2d, 0x31, 0x35, 0x30, 0x6d, 0x6d, 0x00, 0x00 },/* reply #19.5 */
        { 0x26, 0x24, 0x03, 0x15, 0x01, 0x20, 0x50, 0x00, 0xae, 0x00, 0x32, 0x00, 0x96, 0x91, 0x15, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00 },/* reply #19.6 */
        { 0x24, 0x22, 0x03, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #19.7 */
        { 0x06, 0x04, 0x03, 0x25, 0x00 },                       /* reply #19.8 */
        { 0x06, 0x05, 0x03, 0x17, 0x95, 0x00 },                 /* reply #19.9 */
        { 0x06, 0x05, 0x03, 0x6c, 0x96, 0x00 },                 /* reply #19.10 */
        { 0x26, 0x24, 0x03, 0x15, 0x01, 0x20, 0x50, 0x00, 0xae, 0x00, 0x32, 0x00, 0x96, 0x91, 0x15, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00 },/* reply #19.11 */
        { 0x06, 0x05, 0x01, 0x3d, 0x00, 0x00 },                 /* reply #19.12 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x0d, 0x00, 0x00 }, {                   /* spell #20 */
        { 0x06, 0x05, 0x03, 0x0d, 0x00, 0x00 },                 /* reply #20.1 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x0c, 0x00, 0x00 }, {                   /* spell #21 */
        { 0x06, 0x05, 0x03, 0x0c, 0x00, 0x00 },                 /* reply #21.1 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x40, 0x00, 0x00 }, {                   /* spell #22 */
        { 0 } } }, {
    { 0x0c, 0x0b, 0x03, 0x53, 0x02, 0x00, 0x48, 0x81, 0x81, 0x00, 0x00, 0x00 }, {/* spell #23 */
        { 0 } } }, {
    { 0x0c, 0x0b, 0x03, 0x53, 0x02, 0x00, 0x48, 0x81, 0x81, 0x00, 0x00, 0x00 }, {/* spell #24 */
        { 0 } } }, {
    { 0x06, 0x05, 0x04, 0x2a, 0x0c, 0x00 }, {                   /* spell #25 */
        { 0 } } }, {
    { 0x06, 0x05, 0x09, 0x11, 0x01, 0x00 }, {                   /* spell #26 */
        { 0 } } }, {
    { 0x12, 0x11, 0x09, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, {/* spell #27 */
        { 0 } } }, {
    { 0x14, 0x13, 0x09, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, {/* spell #28 */
        { 0 } } }, {
    { 0x06, 0x05, 0x01, 0x5a, 0x00, 0x00 }, {                   /* spell #29 */
        { 0x06, 0x05, 0x01, 0x59, 0x01, 0x00 },                 /* reply #29.1 */
        { 0 } } }, {
    { 0x14, 0x13, 0x09, 0x25, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, {/* spell #30 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x09, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x00 }, { /* spell #31 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x09, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00 }, { /* spell #32 */
        { 0 } } }, {
    { 0x06, 0x04, 0x0e, 0x08, 0x00 }, {                         /* spell #33 */
        { 0 } } }, {
    { 0x06, 0x04, 0x0e, 0x0c, 0x00 }, {                         /* spell #34 */
        { 0 } } }, {
    { 0x08, 0x06, 0x03, 0x18, 0x00, 0x00, 0x00 }, {             /* spell #35 */
        { 0 } } }, {
    { 0x08, 0x06, 0x03, 0x1f, 0x00, 0x00, 0x00 }, {             /* spell #36 */
        { 0x06, 0x05, 0x01, 0x58, 0x00, 0x00 },                 /* reply #36.1 */
        { 0 } } }, {
    { 0x08, 0x06, 0x04, 0x24, 0x00, 0x00, 0x00 }, {             /* spell #37 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x13, 0x00, 0x00 }, {                   /* spell #38 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x1e, 0x00, 0x00 }, {                   /* spell #39 */
        { 0x06, 0x05, 0x01, 0x58, 0x00, 0x00 },                 /* reply #39.1 */
        { 0 } } }, {
    { 0x06, 0x05, 0x04, 0x23, 0x00, 0x00 }, {                   /* spell #40 */
        { 0 } } }, {
    { 0x08, 0x06, 0x04, 0x24, 0x00, 0x00, 0x00 }, {             /* spell #41 */
        { 0x06, 0x05, 0x01, 0x58, 0x00, 0x00 },                 /* reply #41.1 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x01, 0x3b, 0xff, 0xff, 0x00, 0x00, 0x00 }, { /* spell #42 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x01, 0x3b, 0xff, 0x00, 0x00, 0x00, 0x00 }, { /* spell #43 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x01, 0x3b, 0xff, 0x00, 0x00, 0x00, 0x00 }, { /* spell #44 */
        { 0 } } }, {
    { 0x06, 0x05, 0x01, 0x8a, 0x00, 0x00 }, {                   /* spell #45 */
        { 0 } } }, {
    { 0x08, 0x06, 0x03, 0x1f, 0x00, 0x00, 0x00 }, {             /* spell #46 */
        { 0 } } }, {
    { 0x0a, 0x08, 0x01, 0x3b, 0xff, 0x00, 0x00, 0x00, 0x00 }, { /* spell #47 */
        { 0x06, 0x05, 0x01, 0x8a, 0x00, 0x00 },                 /* reply #47.1 */
        { 0 } } }, {
    { 0x08, 0x07, 0x03, 0x68, 0x00, 0x00, 0x00, 0x00 }, {       /* spell #48 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x69, 0x00, 0x00 }, {                   /* spell #49 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x69, 0x00, 0x00 }, {                   /* spell #50 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x69, 0x00, 0x00 }, {                   /* spell #51 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x69, 0x00, 0x00 }, {                   /* spell #52 */
        { 0 } } }, {
    { 0x08, 0x07, 0x01, 0x2a, 0x16, 0xf3, 0x00, 0x00 }, {       /* spell #53 */
        { 0 } } }, {
    { 0x08, 0x07, 0x01, 0x2a, 0x16, 0xf3, 0x00, 0x00 }, {       /* spell #54 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x19, 0x01, 0x00 }, {                   /* spell #55 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x19, 0x01, 0x00 }, {                   /* spell #56 */
        { 0 } } }, {
    { 0x26, 0x24, 0x01, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, {/* spell #57 */
        { 0x26, 0x24, 0x01, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #57.1 */
        { 0x06, 0x05, 0x01, 0x53, 0x00, 0x00 },                 /* reply #57.2 */
        { 0x06, 0x05, 0x01, 0x58, 0x00, 0x00 },                 /* reply #57.3 */
        { 0x06, 0x05, 0x01, 0x59, 0x01, 0x00 },                 /* reply #57.4 */
        { 0x06, 0x05, 0x01, 0x49, 0x01, 0x00 },                 /* reply #57.5 */
        { 0 } } }, {
    { 0x26, 0x24, 0x01, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, {/* spell #58 */
        { 0x26, 0x24, 0x01, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #58.1 */
        { 0x06, 0x05, 0x01, 0x53, 0x00, 0x00 },                 /* reply #58.2 */
        { 0x06, 0x05, 0x01, 0x58, 0x00, 0x00 },                 /* reply #58.3 */
        { 0x06, 0x05, 0x01, 0x59, 0x01, 0x00 },                 /* reply #58.4 */
        { 0x06, 0x05, 0x01, 0x49, 0x01, 0x00 },                 /* reply #58.5 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x19, 0x01, 0x00 }, {                   /* spell #59 */
        { 0 } } }, {
    { 0x06, 0x05, 0x02, 0x0a, 0x01, 0x00 }, {                   /* spell #60 */
        { 0x06, 0x05, 0x06, 0x11, 0x01, 0x00 },                 /* reply #60.1, bindReceiveSwitch(17, 1) */
        { 0x06, 0x05, 0x06, 0x12, 0x00, 0x00 },                 /* reply #60.2, bindReceiveSwitch(18, 0) */
        { 0x06, 0x05, 0x06, 0x13, 0x00, 0x00 },                 /* reply #60.3, bindReceiveSwitch(19, 0) */
        { 0x06, 0x05, 0x06, 0x22, 0x00, 0x00 },                 /* reply #60.4, bindReceiveSwitch(34, 0) */
        { 0x06, 0x05, 0x06, 0x26, 0x01, 0x00 },                 /* reply #60.5, bindReceiveSwitch(38, 1) */
        { 0x48, 0x46, 0x0a, 0x08, 0xff, 0x1f, 0x01, 0x00, 0x01, 0x01, 0xa0, 0x10, 0x00, 0x60, 0x01, 0x01, 0x50, 0x20, 0x23, 0x01, 0x01, 0x00, 0x50, 0x04, 0x01, 0x02, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/* reply #60.6 */
        { 0x08, 0x06, 0x01, 0x04, 0x03, 0x00, 0x00 },           /* reply #60.7 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x1d, 0x1f, 0x00 }, {                   /* spell #61 */
        { 0x06, 0x05, 0x03, 0x35, 0x01, 0x00 },                 /* reply #61.1 */
        { 0x1c, 0x1b, 0x03, 0x1d, 0x41, 0x02, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x4c, 0x50, 0x2d, 0x45, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x4c, 0x35, 0x7a, 0xbd, 0x00 },/* reply #61.2 */
        { 0x06, 0x04, 0x03, 0x36, 0x00 },                       /* reply #61.3 */
        { 0 } } }, {
    { 0x06, 0x05, 0x08, 0x06, 0xff, 0x00 }, {                   /* spell #62 */
        { 0x06, 0x05, 0x08, 0x06, 0x00, 0x00 },                 /* reply #62.1 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x19, 0x00, 0x00 }, {                   /* spell #63 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x19, 0x00, 0x00 }, {                   /* spell #64 */
        { 0x06, 0x05, 0x06, 0x26, 0x01, 0x00 },                 /* reply #64.1, bindReceiveSwitch(38, 1) */
        { 0x06, 0x05, 0x06, 0x00, 0x01, 0x00 },                 /* reply #64.2, bindReceiveSwitch(0, 1) */
        { 0 } } }, {
    { 0x06, 0x05, 0x04, 0x00, 0x01, 0x00 }, {                   /* spell #65, NotifyGUIEvent(1) */
        { 0x06, 0x05, 0x06, 0x0a, 0x00, 0x00 },                 /* reply #65.1, bindReceiveSwitch(10, 0) */
        { 0x06, 0x05, 0x06, 0x09, 0x00, 0x00 },                 /* reply #65.2, bindReceiveSwitch(9, 0) */
        { 0x06, 0x05, 0x04, 0x00, 0x01, 0x01 },                 /* reply #65.3 */
        { 0x0e, 0x0c, 0x0a, 0x08, 0x11, 0x00, 0x15, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00 },/* reply #65.4 */
        { 0x06, 0x05, 0x03, 0x6f, 0x00, 0x00 },                 /* reply #65.5 */
        { 0x08, 0x06, 0x01, 0x0b, 0x00, 0x00, 0x00 },           /* reply #65.6 */
        { 0 } } }, {
    { 0x08, 0x06, 0x00, 0x00, 0x04, 0x00, 0x00 }, {             /* spell #66, Complete WaitID = 0x80020000 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x34, 0x00, 0x00 }, {                   /* spell #67 */
        { 0 } } }, {
    { 0x06, 0x05, 0x03, 0x19, 0x00, 0x00 }, {                   /* spell #68 */
        { 0x06, 0x05, 0x06, 0x26, 0x01, 0x00 },                 /* reply #68.1, bindReceiveSwitch(38, 1) */
        { 0x06, 0x05, 0x06, 0x0b, 0x06, 0x00 },                 /* reply #68.2, bindReceiveSwitch(11, 6) */
        { 0x06, 0x05, 0x06, 0x0b, 0x00, 0x00 },                 /* reply #68.3, bindReceiveSwitch(11, 0) */
    { 0 } } }
};


#include "CacheL1.hpp"

void f(double w) {
    for (size_t i = 0; i < size; ++i) {
        // x[i] = xx[i] * w + x[i];
        double xx_i = L1.read(3 * size + i);
        double x_i = L1.read(i);
        L1.write(i, xx_i * w + x_i);
    }
    for (size_t i = 0; i < size; ++i) {
        // y[i] = yy[i] * w + y[i];
        double yy_i = L1.read(4 * size + i);
        double y_i = L1.read(size + i);
        L1.write(size + i, yy_i * w + y_i);
    }
    for (size_t i = 0; i < size; ++i) {
        // z[i] = zz[i] * w + z[i];
        double zz_i = L1.read(5 * size + i);
        double z_i = L1.read(2 * size + i);
        L1.write(2 * size + i, zz_i * w + z_i);
    }
}

/*void f(double w)
{
    for (size_t i = 0; i < size; ++i)
    {
        // x[i] = xx[i] * w + x[i];
        double xx_i = L1.read(3 * size + i);
        double x_i = L1.read(i);
        L1.write(i, xx_i * w + x_i);

        // y[i] = yy[i] * w + y[i];
        double yy_i = L1.read(4 * size + i);
        double y_i = L1.read(size + i);
        L1.write(size + i, yy_i * w + y_i);

        // z[i] = zz[i] * w + z[i];
        double zz_i = L1.read(5 * size + i);
        double z_i = L1.read(2 * size + i);
        L1.write(2 * size + i, zz_i * w + z_i);
    }
}*/
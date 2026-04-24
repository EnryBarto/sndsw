#pragma once

namespace snd3D {
    struct HitData {
        const double x;
        const double y;
        const double z;
        const double energy;

        HitData(double _x, double _y, double _z, double _energy)
            : x(_x), y(_y), z(_z), energy(_energy) {}
    };
}

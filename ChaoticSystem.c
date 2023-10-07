#include "ChaoticSystem.h"

//得重写啊
void iterateIpvdLorenz(int num, ipvLorenz *ipvLnz) {
    int i;
    double h = 0.0005;
    double a = 10, b = 8/3, c = 28, d = 0.4;
    double K1, L1, M1, K2, L2, M2, K3, M3, L3, K4, L4, M4;
    for (i = 0; i < num; ++i) {
        K1 = -a * (ipvLnz->x - ipvLnz->y) + d * ipvLnz->y * ipvLnz->z;
        L1 = -ipvLnz->x * ipvLnz->z + c * ipvLnz->x - ipvLnz->y;
        M1 = ipvLnz->x * ipvLnz->y - b * ipvLnz->z;

        K2 = -a * (ipvLnz->x + h/2*K1 - ipvLnz->y + h/2*L1) + d * (ipvLnz->y + h/2*L1) * (ipvLnz->z + h/2*M1);
        L2 = -(ipvLnz->x + h/2*K1) * (ipvLnz->z + h/2*M1) + c * (ipvLnz->x + h/2*M1) - (ipvLnz->y + h/2*L1);
        M2 = (ipvLnz->x + h/2*K1) * (ipvLnz->y + h/2*L1) - b * (ipvLnz->z* + h/2*M1);

        K3 = -a * (ipvLnz->x + h/2*K2 - ipvLnz->y + h/2*L2) + d * (ipvLnz->y + h/2*L2) * (ipvLnz->z + h/2*M2);
        L3 = -(ipvLnz->x + h/2*K2) * (ipvLnz->z + h/2*M2) + c * (ipvLnz->x + h/2*M2) - (ipvLnz->y + h/2*L2);
        M3 = (ipvLnz->x + h/2*K2) * (ipvLnz->y + h/2*L2) - b * (ipvLnz->z + h/2*M2);

        K4 = -a * (ipvLnz->x + h*K3 - ipvLnz->y + h*L3) + d * (ipvLnz->y + h*L3) * (ipvLnz->z + h*M3);
        L4 = -(ipvLnz->x + h*K3) * (ipvLnz->z + h*M3) + c * (ipvLnz->x + h*M3) - (ipvLnz->y + h*L3);
        M4 = (ipvLnz->x + h*K3) * (ipvLnz->y + h*L3) - b * (ipvLnz->z + h/2*M3);

        ipvLnz->x += h/6*(K1+2*K2+2*K3+K4);
        ipvLnz->y += h/6*(L1+2*L2+2*L3+L4);
        ipvLnz->z += h/6*(M1+2*M2+2*M3+M4);
    }
}

void preIterateIpvLorenz(int num, ipvLorenz *ipvLnz) {
    iterateIpvdLorenz(num, ipvLnz);
}

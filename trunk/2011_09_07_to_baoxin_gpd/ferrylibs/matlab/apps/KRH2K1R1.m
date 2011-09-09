addpath('../geometry/');

KRHValues;
M = inv(H1) * K0 * R0;
[K1, R1] = KR_Decomposition(M)

K1 = K1 ./ K1(3, 3)
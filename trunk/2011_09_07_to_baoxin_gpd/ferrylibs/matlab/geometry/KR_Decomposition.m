function [K, R] = KR_Decomposition(M)

invM = inv(M);
[q, r] = qr(invM);
K = inv(r);
R = inv(q);

K(:, 1) = K(:, 1) * -1;
K(:, 2) = K(:, 2) * -1;

R(1, :) = R(1, :) * -1;
R(2, :) = R(2, :) * -1;



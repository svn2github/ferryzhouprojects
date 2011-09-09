function X = triangulate(P1, P2, x1, x2);

p11t = P1(1, :);
p12t = P1(2, :);
p13t = P1(3, :);

a1 = x1(1) * p13t - p11t;
a2 = x1(2) * p13t - p12t;

p21t = P2(1, :);
p22t = P2(2, :);
p23t = P2(3, :);

a3 = x2(1) * p23t - p21t;
a4 = x2(2) * p23t - p22t;

A = [a1; a2; a3; a4];

[U, S, V] = svd(A);

X = V(:, 4);

end

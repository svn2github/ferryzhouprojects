function [P1, P2] = EstimateP(F, w, h, x1, x2, alpha);

f = (w + h) / 2;

if (nargin == 6)
	f = w * alpha;
end

K = [f, 0, w/2; 0, -f, h/2; 0, 0, 1];
K1 = K;
K2 = K;

E = K2'* F * K1
[U, S, V] = svd(E);
W = [0, -1, 0; 1, 0, 0; 0, 0, 1];
u3 = U(:, 3);

UWVt = U * W * V';
UWtVt = U * W' * V';

if (det(UWVt) < 0) 
    UWVt = -UWVt;
end

if (det(UWtVt) < 0) 
    UWtVt = -UWtVt;
end

NP1 = [eye(3), zeros(3, 1)];
P1 = K1 * NP1;

NP2 = [UWVt, u3];
P2 = K2 * NP2;
t = isValid(P1, P2, x1, x2);
if (t == 1) 
    return;
end

NP2 = [UWVt, -u3];
P2 = K2 * NP2;
t = isValid(P1, P2, x1, x2);
if (t == 1) 
    return;
end

NP2 = [UWtVt, u3];
P2 = K2 * NP2;
t = isValid(P1, P2, x1, x2);
if (t == 1) 
    return;
end

NP2 = [UWtVt, -u3];
P2 = K2 * NP2;
t = isValid(P1, P2, x1, x2);
if (t == 1) 
    return;
end

error('can not find a valid matrix');

end

function t = isValid(P1, P2, x1, x2);
X = triangulate(P1, P2, x1, x2);
P1X = P1 * X;
P2X = P2 * X;
if (P1X(3) > 0 & P2X(3) > 0) 
    t = 1;
else
    t = 0;
end

end



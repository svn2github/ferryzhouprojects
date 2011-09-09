function Test_CindyData

F = [1.14932e-006 2.28292e-005 -0.00138963;
1.28077e-005 -1.85765e-005 0.00907747;
-0.00133879 -0.0116969 0.278371];

%image width and height
w = 320;
h = 240;

alpha = 1.6; %f = w * alpha

x1 = [35 196];
x2 = [239 222];

[P1 P2] = EstimateP(F, w, h, x1, x2, alpha);

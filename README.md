# Kalman-Filter
Q = 1e-4; 
R = 0.5; 
X(k|k-1) = AX(k-1|k-1) + BU(k) + W(k), A=1, BU(k) = 0
P(k|k-1) = AP(k-1|k-1)A + Q(k), A=1
Kg(k)=P(k|k-1)H / (HP(k|k-1)H + R), H=1
X(k|k) = X(k|k-1) + Kg(k)(Z(k) - HX(k|k-1)), H=1, Z(k):series
P(k|k) = (1 - Kg(k)H)P(k|k-1), H=1

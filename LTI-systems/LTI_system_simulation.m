% Continuous

alpha = deg2rad(60); % deg
v_0 = 10; % m/s
height = 1.5;

% posX, posY, velocityX, velocityY
initial_condition = [0, v_0*cos(alpha), height, v_0*sin(alpha)]

A = [0 1 0 0; 
    0 0 0 0; 
    0 0 0 1; 
    0 0 0 0];

K = [0; 0; 0; -9.81];

tspan = [0 5];

[t, X] = ode45(@(t,X) A*X + K, tspan, initial_condition);

pos_x = X(:,1);
pos_y = X(:,3);

% figure('Name','Continuous time');
plot(pos_x, pos_y,'-')
title("Continuous time (position y vs x)")
xlabel("x position (m)")
ylabel("y position (m)")
ylim([0, 10])
legend("trajectory of a ball (no drag)")

% figure('Name','Continuous time');
plot(t, pos_y,'-')
title("Continuous time (position y vs t)")
xlabel("time (s)")
ylabel("y position (m)")
ylim([0, 10])
legend("y position of a ball (no drag)")

% figure('Name','Continuous time');
plot(t, pos_x,'-')
title("Continuous time (position x vs t)")
xlabel("time (s)")
ylabel("x position (m)")
ylim([0, 10])
legend("x position of a ball (no drag)")



% ----------
% Discrete

h = 0.001;
N = 100;

state_X = zeros(4,N);

for n = 1:N
    state_X(:,n+1) = (eye(4)+h*A)*state_X(:,n) + h*K;
end

pos_x = X(:,1);
pos_y = X(:,3);

figure('Name','Discrete');
plot(pos_x, pos_y,'o')
title("Discrete (position y vs x)")
xlabel("x position (m)")
ylabel("y position (m)")
ylim([0, 10])
legend("trajectory of a ball (no drag)")
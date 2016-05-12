clear
close all

x0 = [1 4]';
x1 = [4 1]';

X0 = [];
X1 = [];
for i = 1:80
    X0 = [X0 normrnd(x0, 1)];
    X1 = [X1 normrnd(x1, 1)];
end
X = [X0 X1]';
t = [-ones(size(X0,2),1); ones(size(X1,2),1)];
save data X0 X1 X t

plot(X0(1,:),X0(2,:), 'ko', 'MarkerFaceColor', 'y', 'MarkerSize', 7);
hold on
plot(X1(1,:),X1(2,:), 'k+','LineWidth', 2, 'MarkerSize', 7);

C = 0.01;
w = svm(X, t, C);
x_new = [10 5; 3 4; 5 6; 4 9];

k = -w(2)/w(3); b = -w(1)/w(3);
h = refline(k,b); 
set(h, 'Color', 'r')

b = -(w(1)+1)/w(3);
h = refline(k,b);
b = -(w(1)-1)/w(3);
h = refline(k,b);
title(['5 steps Linear-SVM: \C = ' num2str(C)] )

[y_new, test_error, training_error]=train_svm(X,t,x_new,C);

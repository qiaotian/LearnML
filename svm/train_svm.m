function [y_new, test_error, training_error]=train_svm(x,t,x_new,C)

w = svm(x,t,C);
k = -w(2)/w(3); b = -w(1)/w(3);
h = refline(k,b); 
set(h, 'Color', 'r')

b = -(w(1)+1)/w(3);
h = refline(k,b);
b = -(w(1)-1)/w(3);
h = refline(k,b);
title(['5 steps Linear-SVM: \C = ' num2str(C)] );

x_new_row = size(x_new,1);

% y_new
for i=1:x_new_row
    gx = x_new(i,1)*k+b;
    if(gx>x_new(i,2))
        y_new = [y_new' 1]';
    else
        y_new = [y_new' -1]';
    end
end

% test_error
test_error = 0;

% training_error
training_error = 0;

end
function [optW cost]= svm(X, y, C)

[N dim] = size(X);
w = rand(dim+1,1);
X = [ones(N,1) X]; % x = [1 x]

% minFunc From: http://www.di.ens.fr/~mschmidt/Software/minFunc.html
options.Method = 'lbfgs';
options.maxIter = 100;	  
options.display = 'on';
[optW, cost] = minFunc( @(p) svmCost(p, X, y, C), w, options);

end

function [cost grad] = svmCost(w, X, y, C)
% cost = HingeLoss^2 + C*||w||^2
% 1 2 3 4 5 step
yp = X*w;
idx = find(yp.*y<1);
err = yp(idx)-y(idx);
cost = err'*err + C*w'*w;
grad = 2*X(idx,:)'*err + 2*C*w;
end

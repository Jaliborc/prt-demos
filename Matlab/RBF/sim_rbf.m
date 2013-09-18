function [z phi]=sim_rbf(Xc,X,W,k_i,basisfunction)
%simulates a radial basis function
%Xc is a N_r by N_dim matrix of rbf centres
%X is a N_p by N_dim matrix of the points to simulate
%W is the weight vector
%basisfunction may be 'gaussian' or 'polyharmonicspline'
%k_i is a prescaler for 'gaussian' rbf and function order for
%'polyharmonicspline'. See k_i(i)=0 for constant bias

%
%Copyright (c) 2009, Travis Wiens
%All rights reserved.
%
%Redistribution and use in source and binary forms, with or without 
%modification, are permitted provided that the following conditions are 
%met:
%
%    * Redistributions of source code must retain the above copyright 
%      notice, this list of conditions and the following disclaimer.
%    * Redistributions in binary form must reproduce the above copyright 
%      notice, this list of conditions and the following disclaimer in 
%      the documentation and/or other materials provided with the distribution
%      
%THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
%AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
%IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
%ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
%LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
%CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
%SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
%INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
%CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
%ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
%POSSIBILITY OF SUCH DAMAGE.
%
% If you would like to request that this software be licensed under a less
% restrictive license (i.e. for commercial closed-source use) please
% contact Travis at travis.mlfx@nutaksas.com

if nargin<4
    k_i=1;
end

if nargin<5
    basisfunction='gaussian';
end

N_r=size(Xc,1);%number of rbf centres
N_p=size(X,1);%number of points

if numel(k_i)==1
    k_i=k_i*ones(N_r);
end

phi=zeros(N_p,N_r);%rbf outputs

for i=1:N_r
    if k_i(i)==0
        phi(:,i)=1;
    else
        r=sqrt(sum((repmat(Xc(i,:),N_p,1)-X(:,:)).^2,2));%distance from point Xc to X

        switch basisfunction
            case {'gaussian','Gaussian'}
                phi(:,i)=exp(-k_i(i).*r.^2);
            case {'phs','polyharmonicspline'}
                if r==0
                    phi(:,i)=0;
                else
                    if round(k_i(i)/2)==k_i(i)/2%even
                        phi(:,i)=r.^k_i(i).*log(r);
                        phi(r==0,i)=0;%avoid log(0)
                    else
                        phi(:,i)=r.^k_i(i);
                    end
                end
            otherwise
                error('unknown basis function')
        end
    end
end

z=phi*W;
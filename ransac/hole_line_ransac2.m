%function hole_line_ransac( cc_2, cd_2)
clear all;

load D:\MATLAB\work\data\hole_f23

cc_3(1:64) = 0;

for k = 1:50
    tmpx = x(cd_2(k,1:cc_2(k)));
    tmpy = y(cd_2(k,1:cc_2(k)));
   
    round = 0;

    tmp_c = 0; c_max = 0;
    cc_3(k) = 0;

    while round < 200 && cc_2(k) > 3
        round = round + 1;
        p(1:3) =  1;
        while p(1) == p(2) || p(1) == p(3) || p(2) == p(3)
            p = ceil( rand(1,3) * cc_2(k));
        end
        tmpl = polyfit( x(cd_2(k,p)),y(cd_2(k,p)),2);        
        tmpy = polyval( tmpl,x(cd_2(k,1:cc_2(k))));        
        error_y = tmpy - y(cd_2(k,1:cc_2(k)));
        
        tmp_c = 0;
        
        for i =  1 : cc_2(k)
            if  abs(error_y(i)) <= 0.3
                tmp_c = tmp_c + 1;
            end
        end
        
        
       % [tmp_c c_max]
        if tmp_c > c_max            
            c_max = tmp_c;
            
            cc_3(k) = 0;
            for i =  1 : cc_2(k)
                if abs(error_y(i)) <= 0.3
                    cc_3(k) = cc_3(k) + 1;
                    cd_3(k,cc_3(k)) = cd_2(k,i);
                end
            end
        end
    end
    
    % tmpl = polyfit( x(cd_2(k,1:cc_2(k))),y(cd_2(k,1:cc_2(k))),2);
   
 %[ c_max cc_3(k) tmpl]
    tmpl = polyfit( x(cd_3(k,1:cc_3(k))),y(cd_3(k,1:cc_3(k))),2);
     tmpy = polyval( tmpl,tmpx);
     cd_2_flag(k,1:cc_2(k)) =  polyval( tmpl,x(cd_2(k,1:cc_2(k))));
     clear error_y;
         error_y = y(cd_2(k,1:cc_2(k))) - tmpy  ;
         cc_3(k) = 0;
     for i =  1 : cc_2(k)
            if abs(error_y(i)) > 0.5 && error_y(i) > 0
                %cd_2_flag(i) = 1;
                cc_3(k) = cc_3(k) + 1;
                cd_3(k,cc_3(k)) = cd_2(k,i);            
            end
        end
    
   
end
 


for i  = 1:5
    figure( 3+i);
    for j = 1:10
        k = (i-1) * 10 + j;
        subplot(5,2,j);
        if cc_2(k)> 0
         %plot( x(cd_2(k,1:cc_2(k))),y(cd_2(k,1:cc_2(k))) ,'r.', x(cd_2(k,1:cc_2(k))),y(cd_2(k,1:cc_2(k))) ,'b')%,'MarkerSize',1)
         %plot( dist(cd_2(k,1:cc_2(k)))/500 ,'r.')
         %plot3( x(cd_2(k,1:cc_2(k))),y(cd_2(k,1:cc_2(k))) ,z(cd_2(k,1:cc_2(k))) ,'b.')%,'MarkerSize',1)
         plot( x(cd_2(k,1:cc_2(k))),y(cd_2(k,1:cc_2(k))) ,'r.', x(cd_2(k,1:cc_2(k))),cd_2_flag(k,1:cc_2(k)) ,'b')%,'MarkerSize',1)
         axis([ -15 5 min(y(cd_2(k,1:cc_2(k))))-1  max(y(cd_2(k,1:cc_2(k)))+1)])
         title( k );
        end
    end
end

figure(1)
    %plot( x(cd(1,1:cc(1)))/1000,y(cd(1,1:cc(1)))/1000 ,'b.', 'MarkerEdgeColor',[.5 .5 .5],'MarkerSize',2)
     plot( x,y ,'b.', 'MarkerEdgeColor',[.5 .5 .5],'MarkerSize',2)
     hold on

    
     for k = 1:50
        if cc_2(k) > 0
           plot( x(cd_2(k,1:cc_2(k))),y(cd_2(k,1:cc_2(k))) ,'r.','MarkerSize',1)
          
        end
        if cc_3(k) > 0
           
           plot( x(cd_3(k,1:cc_3(k))),y(cd_3(k,1:cc_3(k))) ,'b.')
        end
    end
     hold off
    axis([ -15 15 -40 50])
 
hold off
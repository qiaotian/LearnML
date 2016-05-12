%function hole_line_ransac( cc_2, cd_2)
clear all;

load D:\MATLAB\work\data\hole_f21

cc_3(1:64) = 0;

for k = 1:50
    tmpx = x(cd_2(k,1:cc_2(k)));
    tmpy = y(cd_2(k,1:cc_2(k)));
    tmpl = polyfit( tmpx, tmpy, 1);
    tmpy = polyval( tmpl,tmpx);
    error_y = tmpy - y(cd_2(k,1:cc_2(k)));
    mrs(k) = sqrt(error_y(1:cc_2(k)) * error_y(1:cc_2(k))'/(cc_2(k)-1));
    
    round = 0;
    %mrs0 = mrs(k)
    

    while mrs(k) > 0.01 && round < 50
        round = round + 1;
        cc_3(k) = 0;
        for i =  1 : cc_2(k)
            if abs(error_y(i)) <= 1*mrs(k)
                %cd_2_flag(i) = 1;
                cc_3(k) = cc_3(k) + 1;
                cd_3(k,cc_3(k)) = cd_2(k,i);
            else
                %cd_2_flag(i) = 0;
            end
        end
        tmpx1 = x(cd_3(k,1:cc_3(k)));
        tmpy1 = y(cd_3(k,1:cc_3(k)));
        tmpl = polyfit( tmpx1, tmpy1, 2);
        tmpy1 = polyval( tmpl,tmpx1);  
        clear error_y1;
        error_y1 = tmpy1 - y(cd_3(k,1:cc_3(k)));
        mrs(k) = sqrt(error_y1(1:cc_3(k)) * error_y1(1:cc_3(k))'/cc_3(k));
        
        tmpy = polyval( tmpl,tmpx);
        cd_2_flag(k,1:cc_2(k)) = tmpy;
        clear error_y;
         error_y = tmpy - y(cd_2(k,1:cc_2(k)));
    end
    
    cc_3(k) = 0;
     tmpy = polyval( tmpl,tmpx);
     clear error_y;
         error_y = tmpy - y(cd_2(k,1:cc_2(k)));
     for i =  1 : cc_2(k)
            if abs(error_y(i)) > 0.6
                %cd_2_flag(i) = 1;
                cc_3(k) = cc_3(k) + 1;
                cd_3(k,cc_3(k)) = cd_2(k,i);
            else
                %cd_2_flag(i) = 0;
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

    
     for k = 1:55
        if cc_2(k) > 0
           plot( x(cd_2(k,1:cc_2(k))),y(cd_2(k,1:cc_2(k))) ,'r.','MarkerSize',1)
          
        end
        if cc_3(k) > 0
           
           plot( x(cd_3(k,1:cc_3(k))),y(cd_3(k,1:cc_3(k))) ,'b.')
        end
    end
     hold off
    axis([ -15 15 0 50])
 
hold off
%import data produced for meanWaitingtime manually and plot the curves for
%different producer threads using x axis for consumers and y axis for
%mean waiting time. The plots are made using a base 10 logarithmic scale 
%for the y-axis and a linear scale for the x-axis for better visualisation.
clc;
workspace;
clear figures;
table1=[
NaN	      1	     2	         4	         8	            16	     32	          64	      128	      256;
1	43.8549	   123.8086  	121.6286	51.8691  	44.7561	    55.855	    74.9349	    143.2619	77.5235;
2	627.24325	86.57465	162.7435	84.72035	86.9377	    104.73775	105.7279	76.6994	    103.5267;
4	837.89445	967.3318	660.64905	94.166875	87.288275	117.996375	100.346075	113.56245	106.896175;
8	603.363987	612.00265	621.498275	166.515162	71.242875	89.40155	86.983763	92.861325	90.813775;
16	353.904369	336.368788	389.925681	379.218275	192.929187	102.062031	94.121294	99.989287	103.092862;
32	233.274434	239.502147	241.281744	248.680156	248.478394	186.506931	104.270031	99.636253	105.321734
];
table2=[
NaN	   1	        2	           4	            8	          16	      32	      64	      128	     256;
1	397.9825	  848.0119	    214.563	        26.0169	       6.1945	    44.0586	    33.3225	    28.9501	    55.5011;
2	1475.96645	  385.4544	    127.8596	    317.11275	   149.83885	87.36405	54.93735	49.24425	83.44655;
4	1859.2349	  1212.761725	1851.606575	    259.547725	   133.055825	92.9085	    74.7703	    64.4485  	92.166425;
8	50162.349113  7613.18075	1948.950638	    429.939312	   199.660088	106.66885	64.996025	89.430625	96.838512;
16	26303.076081  15966.034931	19611.026563	18974.136337   1076.966088	188.512575	135.659094	126.264231	90.497306;
32	13882.005703  20198.450488	19626.291269	15789.687828   14328.807097	1142.185878	150.429897	77.668794	106.167553];
xdata=table2(1,2:end);
tables={table1; table2};
title1=sprintf('Mean waiting time plot for several producers (QUEUESIZE=10)');
title2=sprintf('Mean waiting time plot for several producers (QUEUESIZE=1000)');
titles={title1 title2};
for k=1:2
 table=tables{k};
 figure(k);
for i=3:7
semilogy(xdata,table(i,2:end),'-*');
hold on;   
end
title(titles(k));
xlabel('Number of consumer threads q');
ylabel('Mean waiting time (usec)');
legend('number of producers p=2','number of producers p=4','number of producers p=8','number of producers p=16','number of producers p=32');
end
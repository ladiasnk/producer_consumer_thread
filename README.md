# producer_consumer_thread
A modified example of the classic problem producer-consumer using multiple threads for various consumers and producers. The producers place a pointer to a struct containing one of the five simple functions to execute each time . The consumers receive that and execute the function. 

To run the program open your bach cell, access the producer_consumer_thread-main/ folder (in a UNIX based system) and then run : 

gcc producer_consumer_modified.c Functions.c -pthread -o program.out -lm -O3
to compile the code
then run:

./multiple_p_q.sh

and the programm will start executing for various number of producers and consumers, multiple files will be created during the execution as well as the file holding the mean waiting times for every producers and consumers number combinations.

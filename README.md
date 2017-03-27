# OS_project4

Zach Janicki : zjanicki 
Dan Wilborn : dwilborn


Our system reads in a config file, setting up various parameters via a config file wrapper class. The sites and search term files are similarly read in, placing each in their own vectors. Within the main while loop (the main functionality of the program), pthreads are created to first read in each website's information and separate pthreads are created to then parse the information to look for specific words. Pthreads created to read in the website are handled within the producer() function and pthreads to parse the data are handled within the consumer() function. Accompanying these two functions are two queues to act as buffers. The queues are made up of MemoryStructs, each representing a website, containing the website name, the information pulled from it, and the size. The producerBuffer is created outside of the two functions and is handled within the producerBuffer; while the producerBuffer is full, the producer waits for a thread to finish before broadcasting and adding to the consumerBuffer. Inside the consumer() function, the consumerBuffer is checked to see if it is empty. While empty, the consumer() function waits until an item is added to the consumerBuffer. It then removes an item from the buffer, broadcasts, and preforms wordCount. After the threads have preformed their tasks, their memory space is freed up.



# CS485-Project2
Implementation of flow size sketches, including countMin and countSketch

# How to run
1. Was compiled using minGW on Windows, can use any compiler that does the equivalent of this compile command
    - g++ <fileName.cpp> -o <fileName.exe>
2. Run the program in your terminal
    - ./fileName.exe
3. Check the directory you ran the exe file in, there should be a properly named .txt file that was created that shows the first 100 lines of the highest counted id's and the average error in the sketch counts

# Results
- Count min and count sketch work very similarly, taking the input.txt file given for the project and creating a 2d table of d rows and w columns that uses a hash function to map the flows to columns for given rows. They also use generally the same update query and sort functions
- Count min outerperformed count sketch which is odd, however, given the context of the data set, it makes sense. Typically countSketch should produce a smaller average error compared to count min from what I believe, however the opposite happened in my case. I believe the disparity the caused this was the small d value of 3 and the simpler hash functions that caused collisions for count sketch.
- Count sketch showed its weaknesses when seeing how many small flows were way overestimated. This is because of the sacrifice of consistency in the algorithm to attempt to gain accuracy on other flows in the input. 
- With count min, I saw a lot of consistency but not necessarily too much accuracy. All counts were overestimates as they should be, however a majority of them did overestimate by 50 or more counts while there were only a few flows that were overestimated by only about 2-10.
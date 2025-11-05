[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/7RAFczHS)
# Project 4 - HashTable

Project description can be found [in the repo](Project4_HashTable.pdf)

Place your complexity analysis below.

- insert:

    - At worst case insert should perform with a complexity of O(N) when resizing, and hashing and iterating through all buckets.
- remove:
    
    - At worst searching all the way through the table to find whether a key is or isn't there, will make remove perform witha complexity of O(N).
- contains:

    - For the same reason as remove, the potential to search the entire table to find if a key is there or not, will cause contains to run with a complexity of O(N) at worst.
- get:

    - For the same reason as the last two, the program might need to search all the way through the table. At worst it will run with O(N) complexity.
- operator[]:

    - At worst operator[] will need to search the entire table, all N buckets, to find a key and get its value. O(N).

---
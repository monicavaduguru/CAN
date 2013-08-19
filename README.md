CAN
===

Implementation of  Content Addressable Networks(P2P infrastructure) 



File discription:

    list.h          -  consists of all the data structures defination for linked list operations
    stringtocart.h  -  consists the functionality to convert given input string to cartesian
    parsing.h       -  takes the stdin input and converts that to functions and arguments
    main_file.c     -  the core logic has been implemented, i.e splitting the planes when nodes are added,
                       inserting items and hashtables,deleting items,finding items ,deleting nodes and
                       finding the route from source to destination
 Functionality:

    Program takes the input commands from stdin and parses to give arguments and commands to functions. Outputs the route
    from the source to destination.

 Debugging:

    At any time insert a command print to get the current state of the nodes,its neighbours and the zone owned by it.

 Compiling:

    For compiling the program please do make in the direcotry P1
       Usage:  make

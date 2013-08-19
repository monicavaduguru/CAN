//this function is used to parse the inputs which are entered by the user in the form of the sample input provided to 
//inputs for the program

extern int dimension;
extern float cart[20];
extern char inp_str[200];

parse_str(char *str,int no_str) {
    char *s = "(";
    char *t ,*k;
    char pstr[200];
    float f;
    int i,count = 1;

    if(!((strstr(str,"(") !=NULL) && (strstr(str,")") !=NULL)) || !dimension) {
        printf("Wrong input or no dimension set exiting");
        exit(0);
    }

    t = strtok(str,s);
    t = strtok(NULL,s);
    t = strtok(t,")");

    /* wrong number of inputs */
    strcpy(pstr,t);
    s = strtok(pstr,",");
    while(1) {
        k = strtok(NULL,",");
        if(k == NULL)
            break;
        count++;
    }
    if(no_str)
        count--;
    if(count != dimension) {
        printf("Wrong input");
        exit(0);
    }

    for(i=0;i<dimension;i++) {
        if(i==0)
            t = strtok(t,",");
        else
            t = strtok(NULL,",");
        f = atof(t);
        cart[i] = f;
        if(f>= 1 || f <0) {
            printf("Wrong input");
            exit(0);
        }
    }
    inp_str[0] = '\0';
    if(no_str) {
        t = strtok(NULL,",");
        strcpy(pstr,t);
        count = 0;
        s = strtok(pstr,"\"");
        while(1) {
            k = strtok(NULL,"\"");
            if(k == NULL)
                break;
            count++;
        }
        if(count) {
            t = strtok(t,"\"");
            t = strtok(NULL,"\"");
        } else {
            t = strtok(t,"\"");
        }
        strcpy(inp_str,t);
        //TODO string should end with "
    }
}

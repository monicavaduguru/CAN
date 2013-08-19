#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"stringtocart.h"
#include"list.h"
#include"parsing.h"


int dimension;
float cart[20];
float ni_cart[20];
char inp_str[200];
char node_name[50];
node *head = NULL;
struct cindex ind[100];
float min_c,max_c;
int min_cart,min_dim;
extern float cartesian[20];

//creates a new table when the dimension is entered initially

create_table() {
    int i,j;

    for(i=0;i<100;i++) {
        ind[i].data = NULL;
    }
}

//adds the cartesian values and the zones of the node in the table when a new node is added
add_table() {
    int i=0,j,k,min,max;

    while(ind[i].data) {
        i++;
    }
    //when first node is added it owns the whole cartesian space
    if(i ==0) {
        ind[i].data = malloc(3*dimension*sizeof(float));
        for(j=0,k=0;j<3*dimension;k++) {
            ind[i].data[j++] = cart[k];
            ind[i].data[j++] = 0;
            ind[i].data[j++] = 1;
        }
    } 
    //when additional nodes are added it addes the cartesian points of the node and splits the zone and saves these zones
    //in the table
    else {
        find_split_1();
        ind[i].data = malloc(3*dimension*sizeof(float));
        for(j=0,k=0;j<3*dimension;k++) {
            ind[i].data[j++] = cart[k];
            if(k == min_dim) {
                ind[i].data[j++] = min_c;
                ind[i].data[j++] = max_c;
            } else {
                ind[i].data[j] = ind[min_cart].data[j++];
                ind[i].data[j] = ind[min_cart].data[j++];
            }
        }
    }
    return i;
}
//creating a node name for each node added
get_node_name() {
    int i;
    char c[10];
    node_name[0] = '\0';
    for(i=0;i<dimension;i++) {
        sprintf(c,"%.2f",cart[i]);
        strcat(node_name,c);
    }
}
//when a new node is added the existing zones need to be split depending on where the new node lies
find_split_1()
{
    int i=0,j=0;
    float a,cut,x,b ;
    int cart_found = 0,n=0,k=0,l=0;
    int count = 0;
    int loop_cont = 0;

    while(cart_found != 1 ) {
        n++;
        for(i=0;i<dimension && !cart_found ;i++ ) {
            j = 0;
            //printf("dimention %d n=%d cut %f\n",i,n,1/(pow(2,n)));
            while(ind[j].data && !cart_found) {
                a = ind[j].data[i*3];
                b = cart[i];
                for(x=0;(x+2/pow(2,n))<=1 && !cart_found;x =(x+1/pow(2,n))) {
                    //printf(" x = %f,a %f pow %f - %f b %f %f \n",x,a,1/pow(2,n),(x+1/(pow(2,n))),b,(x + 2/pow(2,n)));
                    if((x<= a && a < (x+ 1/pow(2,n))) && ((x+1/(pow(2,n))<= b) && (b <= (x + 2/pow(2,n))))) {

                        // if the cut is equal to the min or max dont use the cut
                        if(!((x+1/pow(2,n) > ind[j].data[i*3+1] ) && (x+1/pow(2,n) < ind[j].data[i*3+2]))) {
                            //printf("not Found the cut at %f cartsian at %f %f\n",x+1/pow(2,n),a,b);
                            continue;
                        }

                        // if the cartesian does not fall into the min and max limits
                        for(k=0,l=0;k<dimension;k++,l++) {
                            if(k == i) {
                                if(!(x+1/pow(2,n) <= cart[k]  && cart[k] < x+2/pow(2,n))) {
                                    //printf("A not Found the cut at %f cartsian at %f %f %f\n",x+1/pow(2,n),cart[k],x+1/pow(2,n),x+2/pow(2,n));
                                    loop_cont =1;
                                    break;
                                }
                            }
                            else {
                                if(!(ind[j].data[k*3+1] <= cart[k]  && cart[k] < ind[j].data[k*3+2])) {
                                    //printf("B not Found the cut at %f cartsian at %f %f %f\n",x+1/pow(2,n),cart[k],
                                    //        ind[j].data[l*3+1],ind[j].data[l*3+2]);
                                    loop_cont =1;
                                    break;
                                }
                            }
                        }
                        if(loop_cont == 1) {
                            loop_cont = 0;
                            continue;
                        }

                        cart_found = 1;
                        ind[j].data[i*3+1] = x;
                        ind[j].data[i*3+2] = x+1/pow(2,n);
                        min_c = x+1/pow(2,n);
                        max_c = x+2/pow(2,n);
                        min_dim = i;
                        min_cart = j;
                        return;
                    }
                    else if ((x<= b && b < (x+1/pow(2,n))) && ((x+1/(pow(2,n))<= a) && (a <= x + 2/pow(2,n)))) {
                        //if the cut lies same as min and max discard the cut
                        if(!((x+1/pow(2,n) > ind[j].data[i*3+1] ) && (x+1/pow(2,n) < ind[j].data[i*3+2]))) {
                            //printf("not Found the cut at %f cartsian at %f %f\n",x+1/pow(2,n),a,b);
                            continue;
                        }
                        // if the cartesian does not fall into the min and max limits
                        for(k=0,l=0;k<dimension;k++,l++) {
                            if(k == i) {
                                if(!(x <= cart[k]  && cart[k] < x+1/pow(2,n))) {
                                    //printf("A not Found the cut at %f cartsian at %f %f %f\n",x+1/pow(2,n),cart[k],x,x+1/pow(2,n));
                                    loop_cont =1;
                                    break;
                                }
                            }
                            else {
                                if(!(ind[j].data[k*3+1] <= cart[k]  && cart[k] < ind[j].data[k*3+2])) {
                                    //printf("B not Found the cut at %f cartsian at %f %f %f\n",x+1/pow(2,n),cart[k],
                                    //        ind[j].data[l*3+1],ind[j].data[l*3+2]);
                                    loop_cont =1;
                                    break;
                                }
                            }
                        }
                        if(loop_cont == 1) {
                            loop_cont = 0;
                            continue;

                        }

                        cart_found = 1;
                        ind[j].data[i*3+1] = x+1/pow(2,n);
                        ind[j].data[i*3+2] = x+2/pow(2,n);
                        min_c = x;
                        max_c = x+1/pow(2,n);
                        min_dim = i;
                        min_cart = j;
                        return;
                    }
                }
                j++;
            }
        }
    }
}

//prints the table at any point
print_ctable() {
    int i=0,j,k;
    while(1) {
        if(ind[i].data == NULL)
            break;
        for(j=0;j<3*dimension;) {
            printf(" %f ",ind[i].data[j++]);
            printf("(%f ",ind[i].data[j++]);
            printf("%f)",ind[i].data[j++]);
        }
        printf("\n");
        i++;
    }
}

//used to parse the dimension input the user
parse_dim(char *str) {
    char *s = " ";
    char *t ;
    int i;
    t = strtok(str,s);
    t = strtok(NULL,s);
    dimension = atoi(t);
    //create the table for indexes
    create_table();
}

main() {
    char str[200];
    int index,index1;
    int i;
    while(1) {
        //get the string upto 200 characters
        fgets (str, 200, stdin);
        //if string contains dimension call parse_dim
        if(strstr(str,"dimension") != NULL) {
            parse_dim(str);
            printf("\nDimension :%d ",dimension);
            str[0] = '\0';
        }
        //if string contains removeNode call parse_str
        else if(strstr(str,"removeNode") != NULL) {
            parse_str(str,0);
            printf("\nCommand:  Remove Node, Cartesians:");
            for (i=0;i<dimension;i++)
                printf(" %f ", cart[i]);
            find_min_volume();
            str[0] = '\0';
            //if string contains addNode call parse_str
        }else if(strstr(str,"addNode") != NULL) {
            parse_str(str,0);
            printf("\nCommand:  Add Node, Cartesians:");
            for (i=0;i<dimension;i++)
                printf(" %f ", cart[i]);
            get_node_name();
            index = add_table();
            add_node(index);
            build_neighbour();
            str[0] = '\0';
        }//if string contains insertItem call parse_str
        else if(strstr(str,"insertItem") != NULL) {
            parse_str(str,1);
            index = insert_item();
            printf("\nCommand:  Insert Item, Cartesians:");
            for (i=0;i<dimension;i++)
                printf(" %f ", cart[i]);
            printf("Item :%s ",inp_str);
            add_hashentry(ind[index].data,cart,cartesian);
            str[0] = '\0';
        }//if string contains deleteItem call parse_str
        else if(strstr(str,"deleteItem") != NULL) {
            parse_str(str,1);
            index = insert_item();
            printf("\nCommand:  Delete Item, Cartesians:");
            for (i=0;i<dimension;i++)
                printf(" %f ", cart[i]);
            printf("Item :%s ",inp_str);
            delete_hashentry(ind[index].data,cart,cartesian);
            str[0] = '\0';
        }//if string contains find call parse_str
        else if(strstr(str,"find") != NULL) {
            parse_str(str,1);
            printf("\nCommand: Find Item, Requesting node Cartesians:");
            for (i=0;i<dimension;i++)
                printf(" %f ", cart[i]);
            printf("Item :%s \n",inp_str);
            index = insert_item();
            index1 = find_zone(cart);
            route(ind[index1].data,ind[index].data);
            find_hash_song(ind[index].data,cartesian);
            str[0] = '\0';
        }//if string contains print call print_node and print_ctable
        else if(strstr(str,"print") != NULL) {
            print_node();
            print_ctable();
        }else if(str[0] == '\0') {
            printf("no more commands exiting\n");
            return;
        } else {
         ;//   printf("Wrong input");
        }
    }
}
//this is used to find the minimum volume of the neighbouring node to decide which neighbour will get the zone when a node
//is deleted
find_min_volume() {
    node *temp1=head;
    struct neigh *ni;
    int i,j=0,flag=0;
    float sum, min_vol = 123;

    while(temp1 != NULL) {
        for(i=0;i<dimension;i++) {
            if(temp1->cord[i] != cart[i]) {
                flag =1;
                break;
            }
        }
        if(flag) {
            flag =0;
        }
        else {
            ni = temp1->nei;
            while(ni) {
                sum = find_volume(ni->cord);
                if(min_vol > sum) {
                    min_vol = sum;
                    for(i=0;i<dimension;i++) {
                        ni_cart[i] = ni->cord[i];
                    }
                }
                ni = ni->next;
            }
           /*
            printf("\n Min volumen is \n");
            for(i=0;i<dimension;i++)
                printf(" %f ",ni_cart[i]);
           */
            /* remove the cordinate from ind */
            replace_cord(cart,ni_cart);
            remove_neighbour(temp1,ni_cart);
            //remove_neighbout_list();
            build_neighbour();
            remove_node(temp1);

            break;
        }
        temp1 = temp1->next;
    }
}

//when a delete node command is issued this function is used to delete this node from the table and reallocate its zone to
//one of its neighbours
remove_node(node *temp) {
    node *temp1=head;
    node *del_node;
    int i,flag =0;

    while(temp1 != NULL) {
        for(i=0;i<dimension;i++) {
            if(temp1->cord[i] != temp->cord[i]) {
                flag =1;
                break;
            }
        }
        if(flag) {
            flag =0;
        }
        else {
            if(temp1 == head) {
                del_node = temp1;
                head = temp1->next;
                free(del_node);
                return;
            } else {
                del_node->next = temp1->next;
                free(temp1);
                return;
            }
        }
        del_node = temp1;
        temp1 = temp1->next;
    }
}
remove_neighbour(node *temp,float *ni_cart) {
    node *temp1=head;
    struct neigh *ni=temp->nei;
    struct neigh *new_ni;
    struct neigh *del_ni;
    struct neigh *test= NULL;
    int i,j=0,flag=0,node_found=0;
    char c[20]="";

    while(temp1 != NULL) {
        for(i=0;i<dimension;i++) {
            if(temp1->cord[i] != ni_cart[i]) {
                flag =1;
                break;
            }
        }
        if(flag) {
            flag =0;
        }
        else {
            //for each neighbour check if the new one has the neighbour else do insert
            while(ni) {
                new_ni = temp1->nei;
                node_found = 0;
                while(new_ni) {
                    for(i=0;i<dimension;i++) {
                        if(new_ni->cord[i] != ni->cord[i]) {
                            flag =1;
                            break;
                        }
                    }
                    if(flag) {
                        flag =0;
                    }else {
                        node_found = 1;
                        break;
                    }
                    new_ni = new_ni->next;
                }
                if(!node_found) {
                    //add the node to new neighbour
                    test =malloc(sizeof(struct neigh));
                    test->cord = malloc(dimension*sizeof(float));
                    test->name[0]= '\0';
                    for(i=0;i<dimension;i++) {
                        test->cord[i] = ni->cord[i];
                        sprintf(c,"%.2f",ni->cord[i]);
                        strcat(test->name,c);
                    }
                    test->next = temp1->nei;
                    temp1->nei = test;
                }
                del_ni = ni;
                ni = ni->next;
                free(del_ni->cord);
                free(del_ni);
            }
            temp->nei = NULL;
            break;
        }
        temp1 = temp1->next;
    }

}

replace_cord(float *src,float *new_val) {
    int i,j,k,l,count=0;

    for(i=0;ind[i].data!= NULL;i++) {
        count = 0;
        for(j=0,k=0;j<dimension;j++,k+=3) {
            if(ind[i].data[k] == src[j])
                count++;
            else
                break;
        }
        if(count == dimension) {
            for(j=0,k=0;j<dimension;j++,k+=3) {
                ind[i].data[k] = new_val[j];
            }
        }
    }
}
find_hash_song(float *src,float *czone) {
    node *temp1=head;
    struct hashtbl *hed;
    int i,j=0,flag=0;
    char c[20]="",str[40]="";

    for(i=0;i<dimension;i++) {
        sprintf(c,"%.4f",czone[i]);
        strcat(str,c);
    }
    while(temp1 != NULL) {
        for(i=0,j=0;i<3*dimension;i+=3,j++) {
            if(temp1->cord[j] != src[i]) {
                flag =1;
                break;
            }
        }
        if(flag) {
            flag =0;
        }
        else {
            j = hash_string(str);
            if(temp1->tbl[j]) {
                hed = temp1->tbl[j];
                while(hed) {
                    if(strcmp(hed->val,inp_str) == 0) {
                        printf("\n Nodes which has the item are at cartesians:");
                        for(i=0;i<dimension;i++)
                            printf(" %f ",hed->cord[i]);
                        printf("\n");
                    }
                    hed = hed->next;
                }
            } else {
                printf("\n Item \'%s\' not found in the node\n",inp_str);
            }
        }
        temp1 = temp1->next;
    }

}
route(float *src,float *dst,char *item) {
    int i,j,count=0,k=0;
    int not_found = 0,flag=0;
    node *temp1=head;
    struct neigh *temp,*temp2;

    printf("\nRequesting node:");
    for(i=0;i<dimension;i++) {
        printf(" %f ",cart[i]);
    }

    printf("\nOwner of requesting nodes zone :");
    for(i=0;i<3*dimension;i+=3) {
        printf(" %f ",src[i]);
        ni_cart[i] = src[i];
    }
    printf("\nDestination Node :");
    for(i=0;i<3*dimension;i +=3)
        printf(" %f ",dst[i]);
    printf("\n");
    printf("Route:\n ");

    while(!not_found) {
        count =0;
        temp1=head;
        //src is equal to dst
        for(j=0;j<3*dimension;j+=3) {
            if(ni_cart[j] == dst[j]) {
                count++;
            }
        }
        if(count == dimension) {
            printf("\nNext hop : ");
            for(i=0;i<3*dimension;i +=3)
                    printf(" %f ",dst[i]);
            printf("\n");
            return;
        }

        while(temp1 != NULL) {
            for(i=0,j=0;i<dimension;i++,j+=3) {
                if(temp1->cord[i] != ni_cart[j]) {
                    flag =1;
                    break;
                }
            }
            if(flag) {
                flag =0;
            }
            else {
                not_found = find_min_neigh(temp1,dst);
                /*
                if(!not_found) {
                    printf("not found going back again \n");
                }
                printf("noe the ni is ");
                for(i=0;i<3*dimension;i+=3)
                    printf(" %f ",ni_cart[i]);
                */
                break;
            }
            temp1 = temp1->next;
        }
        //not_found = 1;
    }
}
find_min_neigh(node *temp,float *dst) {
    float min_d =1234,sum;
    struct neigh *ni= temp->nei;
    int i,j,count=0;

    while(ni) {
        sum =0;
        count = 0;
        for(i=0,j=0;i<dimension;i++,j+=3) {
            if(ni->cord[i] == dst[j])
                count++;
        }
        if(count == dimension) {
            printf("\nNEXT HOP:  ");
            for(i=0;i<dimension;i++)
                printf(" %f ",ni->cord[i]);
            return 1;
        }

        for(i=0,j=0;i<dimension;i++,j+=3) {
            sum += pow((ni->cord[i] - dst[j]),2);
        }

        if(min_d > sqrt(sum)) {
            min_d = sqrt(sum);
            for(i=0,j=0;i<dimension;i++,j+=3) {
                ni_cart[j] = ni->cord[i];
            }
        }
/*
        for(i=0;i<dimension;i++)
            printf(" %f ",ni->cord[i]);
        printf("Dist %f",sqrt(sum));
        printf("\n");
*/
        ni = ni->next;
    }
    printf("\nNEXT HOP:  ");
    for(i=0;i<3*dimension;i+=3)
        printf(" %f ",ni_cart[i]);
    return 0;
}

node_compare( float *c1,float *c2 ) {
    int i,count=0,pcount=0;
    float min1,max1,min2,max2;

    for(i=0;i<3*dimension;i +=3) {
        min1 = c1[i+1];
        max1 = c1[i+2];
        min2 = c2[i+1];
        max2 = c2[i+2];

        if((min1 == min2) && (max1 == max2)) {
            count++;
        }
        else if((min1==min2) && ((max1<max2) || (max2<max1))) {
            count++;
        }
        else if((max1==max2) && ((min1<min2) || (min2<min1))) {
            count++;
        }
        else if(((min1<min2) && (max2<max1) && (min2<max1)) || ((min2<min1) && (max1<max2)&& (min1<max2)))  {
            count++;
        }
        else if(((min1<min2) && (max1<max2) && (min2<max1)) || ((min2<min1)&&(max2<max1)&&(min1<max2))) {
            count++;
        }
        else if((min1 == max2) || (min2 == max1)) {
            pcount++;
        }
    }
    if(pcount)
        count++;

    if(count == dimension)
        return 1;
    return 0;
}

addtoNodeList(float *src,float *nei) {

    node *temp1=head;
    struct neigh *temp,*temp2;
    int i,j=0,flag=0;
    char c[20]="";

    while(temp1 != NULL) {
        for(i=0,j=0;i<3*dimension;i+=3,j++) {
            if(temp1->cord[j] != src[i]) {
                flag =1;
                break;
            }
        }
        if(flag) {
            flag =0;
        }
        else {
            if(!temp1->nei) {
                temp1->nei = malloc(sizeof(struct neigh));
                temp1->nei->cord = malloc(dimension*sizeof(float));
                temp1->nei->next = NULL;
                temp1->nei->name[0]='\0';
                for(i=0,j=0;i<3*dimension;i+=3,j++) {
                    temp1->nei->cord[j] = nei[i];
                    sprintf(c,"%.2f",nei[i]);
                    strcat(temp1->nei->name,c);
                }
            } else {
                temp =malloc(sizeof(struct neigh));
                temp->cord = malloc(dimension*sizeof(float));
                temp->name[0]='\0';
                for(i=0,j=0;i<3*dimension;i+=3,j++) {
                    temp->cord[j] = nei[i];
                    sprintf(c,"%.2f",nei[i]);
                    strcat(temp->name,c);
                }
                temp->next = temp1->nei;
                temp1->nei = temp;
            }
            break;
        }
        temp1 = temp1->next;
    }
}

build_neighbour() {
    int i,j,k,l;
    delete_neighbour_list();
    for(i=0;ind[i].data!= NULL;i++) {
        for(j=0;ind[j].data!= NULL;j++) {
            if(i==j) {
                continue;
            }else if(is_same_node_offset(ind[i].data,ind[j].data)) {
                continue;
            }
            if(node_compare( ind[i].data,ind[j].data )) {
                /*
                printf(" Nodes are neighbours\n");
                for(k=0;k<3*dimension;k +=3)
                    printf("%f",ind[i].data[k]);
                printf("==");
                for(k=0;k<3*dimension;k +=3)
                    printf("%f",ind[j].data[k]);
                printf("\n");
                */
                addtoNodeList(ind[i].data,ind[j].data);
            }
        }
    }
}

delete_neighbour_list() {
    node *temp1=head;
    struct neigh *ne=NULL,*temp= NULL;
    int i;

    while(temp1 != NULL) {
        if(temp1->nei) {
            ne = temp1->nei;
            while( ne != NULL) {
                temp = ne;
                ne =  ne->next;
                if(temp->cord)
                    free(temp->cord);
                free(temp);
            }
            temp1->nei = NULL;
        }
        temp1 = temp1->next;
    }
}
add_node(int index) {

    node *temp,*temp1=head;
    int i,j=0;

    if(head == NULL) {
        //starting node
        head = malloc(sizeof(node));
        head->cord = malloc(dimension*sizeof(float));
        head->zne  = malloc(sizeof(struct zone));
        strcpy(head->name,node_name);
        head->zne->min_range = malloc(dimension*sizeof(float));
        head->zne->max_range = malloc(dimension*sizeof(float));
        head->nei = NULL;
        for(i=0;i<dimension;i++) {
            head->cord[i] = cart[i];
            head->zne->min_range[i] = 0;
            head->zne->max_range[i] = 1;
        }
        for(i=0;i<255;i++) {
            head->tbl[i] = NULL;
        }
        head->next = NULL;
    } else {
        while(temp1->next != NULL) {
            if(ind[j].data) {
                for(i=0;i<dimension;i++) {
                    temp1->zne->min_range[i] = ind[j].data[i+1];
                    temp1->zne->max_range[i] = ind[j].data[i+2];
                }
            }
            j++;
            temp1 = temp1->next;
        }
        temp =  malloc(sizeof(node));
        temp->cord = malloc(dimension*sizeof(float));
        temp->zne  = malloc(sizeof(struct zone));
        strcpy(temp->name,node_name);
        temp->zne->min_range = malloc(dimension*sizeof(float));
        temp->zne->max_range = malloc(dimension*sizeof(float));
        temp->nei = NULL;
        for(i=0;i<255;i++) {
            temp->tbl[i] = NULL;
        }

        for(i=0;i<dimension;i++) {
            temp->cord[i] = cart[i];
            temp->zne->min_range[i] = ind[index].data[i+1];
            temp->zne->max_range[i] = ind[index].data[i+2];
        }
        temp->next = NULL;
        temp1->next = temp;
    }
}

print_node() {
    node *temp,*temp1=head;
    struct hashtbl *hed;
    struct neigh *neig;
    int i,j;

    while(temp1 != NULL) {
        printf("  Name: %s \n  Neigh: ",temp1->name);
        neig = temp1->nei;
        while(neig) {
            printf(" %s ",neig->name);
            neig = neig->next;
        }
        for(i=0;i<255;i++) {
            if(temp1->tbl[i]) {
                hed = temp1->tbl[i];
                printf("\n Hash: ");
                while(hed) {
                    for(j=0;j<dimension;j++)
                        printf("%f",hed->cord[j]);
                    printf("  ");
                    hed =  hed->next;
                }
            }
        }
        printf("\n");
        temp1 = temp1->next;
    }
}
int hash_string(char *key) {
    unsigned char hash, i;
    int len = strlen(key);

    for(hash = i = 0; i < len; ++i)
    {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

add_hashentry(float *src,float *dest,float *czone) {
    node *temp1=head;
    int i,j=0,flag=0;
    struct hashtbl *temp;
    char str[200]="",c[20];

    for(i=0;i<dimension;i++) {
        sprintf(c,"%.4f",czone[i]);
        strcat(str,c);
    }
    while(temp1 != NULL) {
        for(i=0,j=0;i<3*dimension;i+=3,j++) {
            if(temp1->cord[j] != src[i]) {
                flag =1;
                break;
            }
        }
        if(flag) {
            flag =0;
        }
        else {
            //node is there
            //TODO cleanup of node
            j = hash_string(str);
            if(!temp1->tbl[j]) {
                temp1->tbl[j]    = malloc(sizeof(struct hashtbl));
                temp1->tbl[j]->cord = malloc(dimension*sizeof(float));
                for(i=0;i<dimension;i++)
                    temp1->tbl[j]->cord[i] = dest[i];
                temp1->tbl[j]->next = NULL;
                strcpy(temp1->tbl[j]->val,inp_str);
            } else {
                temp = malloc(sizeof(struct hashtbl));
                temp->cord = malloc(dimension*sizeof(float));
                for(i=0;i<dimension;i++)
                    temp->cord[i] = dest[i];
                temp->next = temp1->tbl[j];
                strcpy(temp->val,inp_str);
                temp1->tbl[j] = temp;
            }
            break;
        }
        temp1 = temp1->next;
    }
}

delete_hashentry(float *src,float *dest,float *czone) {
    node *temp1=head;
    int i,j=0,flag=0;
    struct hashtbl *temp;
    struct hashtbl *hed;
    char str[200]="",c[20];

    for(i=0;i<dimension;i++) {
        sprintf(c,"%.4f",czone[i]);
        strcat(str,c);
    }
    while(temp1 != NULL) {
        for(i=0,j=0;i<3*dimension;i+=3,j++) {
            if(temp1->cord[j] != src[i]) {
                flag =1;
                break;
            }
        }
        if(flag) {
            flag =0;
        }
        else {
            //node is there
            //TODO cleanup of node
            j = hash_string(str);
            hed = temp1->tbl[j];
            if(hed) {
                if(is_same_node(hed->cord,dest)) {
                    temp1->tbl[j] = hed->next;
                    free(hed);
                    return;
                }
                while( hed != NULL) {
                    if(is_same_node(hed->cord,dest)) {
                        temp->next = hed->next;
                        free(hed);
                        return;
                    }
                    temp = hed;
                    hed = hed->next;
                }
            }
            break;
        }
        temp1 = temp1->next;
    }

}
int is_same_node(float *src,float *dest) {
    int i;
    for(i=0;i<dimension;i++) {
        if(src[i] != dest[i]) {
            return 0;
        }
    }
    return 1;
}
int is_same_node_offset(float *src,float *dest) {
    int i;
    for(i=0;i<3*dimension;i+=3) {
        if(src[i] != dest[i]) {
            return 0;
        }
    }
    return 1;
}
int insert_item() {
    int i,j,k,l,count=0;

    stringtocart(inp_str,dimension);

    for(i=0;ind[i].data!= NULL;i++) {
        count = 0;
        for(j=0,k=0;j<3*dimension;j +=3,k++) {
            if(!( ind[i].data[j+1] <= cartesian[k] && cartesian[k] < ind[i].data[j+2])) {
                continue;
            } else {
                count++;
            }
        }
        if(count == dimension) {
            return i;
        }
    }

    return 0;
}

int find_zone(float *cart) {
    int i,j,k,l,count=0;

    for(i=0;ind[i].data!= NULL;i++) {
        count = 0;
        for(j=0,k=0;j<3*dimension;j +=3,k++) {
            if(!( ind[i].data[j+1] <= cart[k] && cart[k] < ind[i].data[j+2])) {
                continue;
            } else {
                count++;
            }
        }
        if(count == dimension) {
            return i;
        }
    }
    return 0;

}
int find_volume(float *cart) {

    int i,j,k,l,count=0;
    float vol=1,pvol =0;

    /*
    printf("\nVolume of ");
    for(i=0;i<dimension;i++)
        printf("%f",cart[i]);
*/
    for(i=0;ind[i].data!= NULL;i++) {
        count = 0;
        for(j=0,k=0;j<dimension;j++,k+=3) {
            if(ind[i].data[k] == cart[j])
                count++;
            else
                break;
        }
        if(count == dimension) {
            vol = 1;
            for(j=0;j<3*dimension;j+=3) {
                vol = vol*(ind[i].data[j+2] - ind[i].data[j+1]);
                //printf(" %f-%f ",ind[i].data[j+2] , ind[i].data[j+1]);
            }
            pvol = pvol + vol;
        }
    }
    //printf( "  %f \n",pvol );
    return pvol;
}
-bash-3.2$

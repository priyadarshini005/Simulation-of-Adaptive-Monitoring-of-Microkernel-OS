#include<sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include<fcntl.h>
  enum nodeColor {
        RED,
        BLACK
  };

  struct rbNode {
        int data, color;
        struct rbNode *link[2];
  };

  struct rbNode *root = NULL;

  struct rbNode * createNode(int data) {
        struct rbNode *newnode;
        newnode = (struct rbNode *)malloc(sizeof(struct rbNode));
        newnode->data = data;
        newnode->color = RED;
        newnode->link[0] = newnode->link[1] = NULL;
        return newnode;
  }

  void insertion (int data) {
        struct rbNode *stack[98], *ptr, *newnode, *xPtr, *yPtr;
        int dir[98], ht = 0, index;
        ptr = root;
        if (!root) {
                root = createNode(data);
                return;
        }
        stack[ht] = root;
        dir[ht++] = 0;
        /* find the place to insert the new node */
        while (ptr != NULL) {
                if (ptr->data == data) {
                        printf("Duplicates Not Allowed!!\n");
                        return;
                }
                index = (data - ptr->data) > 0 ? 1 : 0;
                stack[ht] = ptr;
                ptr = ptr->link[index];
                dir[ht++] = index;
        }
        /* insert the new node */
        stack[ht - 1]->link[index] = newnode = createNode(data);
        while ((ht >= 3) && (stack[ht - 1]->color == RED)) {
                if (dir[ht - 2] == 0) {
                        yPtr = stack[ht - 2]->link[1];
                        if (yPtr != NULL && yPtr->color == RED) {
                                /*
                                 * Red node having red child. B- black, R-red
                                 *     B                R
                                 *    / \             /   \
                                 *   R   R  =>     B     B
                                 *  /               /   
                                 * R               R
                                 */
                                stack[ht - 2]->color = RED;
                                stack[ht - 1]->color = yPtr->color = BLACK;
                                ht = ht -2;
                        } else {
                                if (dir[ht - 1] == 0) {
                                        yPtr = stack[ht - 1];
                                } else {
                                        /*
                                         * XR - node X with red color
                                         * YR - node Y with red color
                                         * Red node having red child
                                         *(do single rotation left b/w X and Y)
                                         *         B             B
                                         *        /             /
                                         *      XR     =>      YR
                                         *        \           /
                                         *         YR        XR
                                         * one more additional processing will be
                                         * performed after this else part.  Since
                                         * we have red node (YR) with red child(XR)
                                         */
                                        xPtr = stack[ht - 1];
                                        yPtr = xPtr->link[1];
                                        xPtr->link[1] = yPtr->link[0];
                                        yPtr->link[0] = xPtr;
                                        stack[ht - 2]->link[0] = yPtr;
                                }
                                /*
                                 *  Red node(YR) with red child (XR) - single
                                 *  rotation b/w YR and XR for height balance. Still,
                                 *  red node (YR) is having red child.  So, change the
                                 *  color of Y to black and Black child B to Red R
                                 *          B           YR          YB
                                 *         /           /  \        /  \
                                 *        YR  =>   XR   B  =>  XR  R
                                 *       /
                                 *      XR
                                 */
                                xPtr = stack[ht - 2];
                                xPtr->color = RED;
                                yPtr->color = BLACK;
                                xPtr->link[0] = yPtr->link[1];
                                yPtr->link[1] = xPtr;
                                if (xPtr == root) {
                                        root = yPtr;
                                } else {
                                        stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                                }
                                break;
                        }
                } else {
                        yPtr = stack[ht - 2]->link[0];
                        if ((yPtr != NULL) && (yPtr->color == RED)) {
                                /*
                                 * Red node with red child
                                 *        B             R
                                 *      /   \         /   \
                                 *     R     R =>  B     B
                                 *             \              \
                                 *              R              R
                                 *
                                 */
                                stack[ht - 2]->color = RED;
                                stack[ht - 1]->color = yPtr->color = BLACK;
                                ht = ht - 2;
                        } else {
                                if (dir[ht - 1] == 1) {
                                        yPtr = stack[ht - 1];
                                } else {
                                        /*
                                         * Red node(XR) with red child(YR) 
                                         *   B          B
                                         *    \          \
                                         *     XR  => YR
                                         *    /            \
                                         *   YR             XR
                                         * Single rotation b/w XR(node x with red color) & YR
                                         */
                                        xPtr = stack[ht - 1];
                                        yPtr = xPtr->link[0];
                                        xPtr->link[0] = yPtr->link[1];
                                        yPtr->link[1] = xPtr;
                                        stack[ht - 2]->link[1] = yPtr;
                                }
                                /*
                                 *   B              YR          YB
                                 *    \             /  \        /  \
                                 *     YR  =>   B   XR => R    XR
                                 *      \
                                 *       XR
                                 * Single rotation b/w YR and XR and change the color to
                                 * satisfy rebalance property.
                                 */
                                xPtr = stack[ht - 2];
                                yPtr->color = BLACK;
                                xPtr->color = RED;
                                xPtr->link[1] = yPtr->link[0];
                                yPtr->link[0] = xPtr;
                                if (xPtr == root) {
                                        root = yPtr;
                                } else {
                                        stack[ht - 3]->link[dir[ht - 3]] = yPtr;
                                }
                                break;
                        }
                }
        }
        root->color = BLACK;
  }
 void searchElement(int data) 
 {
        struct rbNode *temp = root;
        int diff,fd;
	mknod("key_fifo2",0666|S_IFIFO,0);
	fd=open("key_fifo2",O_WRONLY);
        while (temp != NULL) {
                diff = data - temp->data;
                if (diff > 0) {
                        temp = temp->link[1];
                } else if (diff < 0) {
                        temp = temp->link[0];
                } else {
                        write(fd,"Search Element Found!!\n",25);
                        return;
                }
        }
        write(fd,"Given Data Not Found in RB Tree!!\n",34);
	unlink("key_fifo2");
        return;
  }

 
 int main() 
 {
        int i, temp=0,n,key=0,fd,rs_fd_in,rs_fd_out;
	FILE *fp,*fpt1;
	char ping_msg[20];
	fp=fopen("kss_input_file.txt","r");
	if(fp==NULL)
	{
	  printf("Error in Index File creation");
	  exit(0);
	}
	fseek(fp,0L,0);
	for(i=0;i<700;i++)
	{
		fscanf(fp,"%d\t",&n);
		insertion(n);
	}
	fclose(fp);
	printf("Data inserted into the tree\n");
	mknod("fifo1",0666|S_IFIFO,0);
	mknod("fifo2",0666|S_IFIFO,0);
	printf("FIFOs created to read and write from and to RS\n");
	rs_fd_in=open("fifo1",O_RDONLY);
	rs_fd_out=open("fifo2",O_WRONLY);
	printf("\nFIFOs opened for data transfer between KSS and RS\n");
	i=1;
	mknod("key_fifo1",0666|S_IFIFO,0);
	printf("\nKSS started\n");
	while(1)
	{
		fd=open("key_fifo1",O_RDONLY);
		printf("\nReceiving key\n");
		read(fd,&key,sizeof(int));
		printf("\nThe key received is %d\n",key);	
		searchElement(key);
		printf("\nResponse sent to KR\n---------------------------------\n");
		read(rs_fd_in,&ping_msg,11);
		printf("\n%s\n------------------------------------------------------------------------------------------\n",ping_msg);
		write(rs_fd_out,"\nKSS:Hello\n",11);
		i++;
	}
	unlink("key_fifo1");
	unlink("fifo1");
	unlink("fifo2");
	return 0;
 }

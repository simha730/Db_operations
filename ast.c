#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node{
    char value[20];
    struct node *left;
    struct node *right;
};
struct node *createnode(char *value)
{
    struct node *newnode=(struct node*)malloc(sizeof(struct node));
strcpy(newnode->value,value);
newnode->left=newnode->right=NULL;
return newnode;
}
void preorder(struct node *root)
{
    if(root!=NULL)
    {
        printf("%s ",root->value);
        preorder(root->left);
        preorder(root->right);
    }
}
void postorder(struct node *root)
{
    if(root!=NULL)
    {
        postorder(root->left);
        postorder(root->right);
        printf("%s ",root->value);
    }
}
int main()
{
    //left subtree wiht age>30;
    struct node *root=createnode("AND");
    root->left=createnode(">");
    root->left->left=createnode("age");
    root->left->right=createnode("30");
//right subtree with salary<50000
root->right=createnode("<");
root->right->left=createnode("salary");
root->right->right=createnode("50000");
printf("\nPreorder Traversal (Prefix Expression):\n");
    preorder(root);

    printf("\n\nPostorder Traversal (Postfix Expression):\n");
    postorder(root);

    printf("\n");
    return 0;
}

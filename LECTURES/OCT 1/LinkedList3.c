#include <stdio.h>

typedef struct NODE Node;
typedef struct LIST List;

struct NODE
{
	char* data;
	Node* next;
};

struct LIST
{
	Node* first;
};

void add(List* list, Node* newNode)
{
	(*newNode).next = (*list).first;
	(*list).first = newNode;
}

void to_string(List list)
{
	Node* curr = list.first;
	while(NULL != curr)
	{
		printf("%s\n", (*curr).data);
		printf("curr.next = %p\n", (*curr).next);
		curr = (*curr).next;
	}
}

//this works, but it is unweildy to write
int main()
{
	List list;
	Node firstNode, secondNode, thirdNode;

	list.first = NULL;

	firstNode.data = "first";
	secondNode.data = "second";
	thirdNode.data = "third";

	add(&list, &firstNode);
	add(&list, &secondNode);
	add(&list, &thirdNode);

	to_string(list);

	return 0;
}

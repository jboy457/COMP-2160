#include <stdio.h>

typedef struct NODE Node;
typedef struct LIST List;

struct NODE
{
	char* data;
	struct NODE* next;
};

struct LIST
{
	Node first;
};

void add(List list, char* data)
{
	Node newNode;
	newNode.data = data;
	newNode.next = &list.first;
	list.first = newNode;
}

void to_string(List list)
{
	Node* curr = &list.first;
	while(NULL != curr)
	{
		printf("%s\n", (*curr).data);
		curr = (*curr).next;
	}
}

int main()
{
	List list;

	//these don't actually modify list, we should use a pointer
	add(list, "first");
	add(list, "second");
	add(list, "third");

	to_string(list);

	return 0;
}

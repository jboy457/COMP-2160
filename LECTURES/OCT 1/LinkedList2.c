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
	Node first;
};

void add(List* list, char* data)
{
	Node newNode;
	newNode.data = data;
	newNode.next = &(*list).first; //sets newNode.next to point at first
	(*list).first = newNode; //copies newNode into first
	//so, we get a node pointing at itself, which leads to an infinite loop
}

//technically doesn't need to change, but this is a bit less efficient
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

	add(&list, "first");
	add(&list, "second");
	add(&list, "third");

	to_string(list);

	return 0;
}

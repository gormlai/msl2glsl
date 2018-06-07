#include "Visitor.h"
#include "Types.h"
#include <stdio.h>

void Visitor::operateOn(struct Node * node)
{
  printf("Visitor pattern not defined for NodeType = %d\n", (int)node->getNodeType());
}

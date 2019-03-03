#ifndef __ITEM_H
#define __ITEM_H

#define ITEM_STRING_SIZE 128

struct item {
	int id;
	int producer;
};

struct item *createItem(int index, int producer);
int compareToItem(const void *, const void *);
char *toStringItem(const void *);
void freeItem(void *);


#endif /* __ITEM_H */

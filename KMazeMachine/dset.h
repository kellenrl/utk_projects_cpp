#ifndef DSET_H
#define DSET_H

#include <vector>

class dset {
	struct node {
		node() { rank=0, parent=-1; }
		int rank;
		int parent;
	};

public:
	dset(int Nsets);

	int size() { return Nsets; }

	int addset();
	int merge(int, int);
	int find(int);

private:
	int Nsets;
	std::vector<node> S;
};
#endif

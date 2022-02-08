#ifndef SLIST_H
#define SLIST_H

#include <iterator>
#include <string>
#include <vector>

using namespace std;

template <class T>
class slist {
private:
	struct node {
		node() { data = T(); next = NULL; }
		node(const T &key) { data = key; next = NULL; }
		bool operator<(const node& rhs)const { return this->data < rhs.data; }
    
		T data;
		node *next;
	};
	class sptr {
		public:
			sptr(node* _ptr = NULL) { ptr = _ptr; }
			bool operator<(const sptr &rhs) const { return *ptr < *rhs.ptr; }
			operator node * () const { return ptr;}
		private:
			node *ptr;
	};
	node *head;
	node *tail;
public:
	class iterator {
		public:
		iterator() { p = NULL; }
		T & operator*() { return p->data; }
		iterator & operator++() { p = p->next; return *this; }
		bool operator!=(const iterator & rhs) const { return p != rhs.p; }

		private:
		iterator(node *n_p) { p = n_p; }
		node *p;

		friend class slist<T>;
	};
	
	slist();
	~slist();

	void push_back(const T &);
	void sort(const string &);

	iterator begin() { return iterator(head->next); }
	iterator end() { return iterator(NULL); }
};

template <typename T>
slist<T>::slist() {
  head = new node();
  tail = head;
}

template <typename T>
slist<T>::~slist() {
  while (head->next != NULL) {
    node *p = head->next;
    head->next = p->next;
    delete p;
  }
  delete head;

  head = NULL;
  tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
  tail->next = new node(din);
  tail = tail->next;
}

template <typename T>
void slist<T>::sort(const string &algname) {
  // determine number of list elements
	node* p = head;
	int count = 0;
	while(p != tail){
		p = p->next;
		count++;
	}
	//setup smart pointer array called Ap	
	vector<sptr> Ap;
	Ap.resize(count);
	p = head;
	int i = 0;
	while(p != tail){
		p = p->next;
		Ap[i] = p;
		i++;
	}
	// if quicksort, apply std::sort(...)
	if(algname == "-quicksort") {
		std::sort(Ap.begin(), Ap.end());
	}
	// if mergesort, apply std::stable_sort(...)
	else if(algname == "-mergesort") {
		stable_sort(Ap.begin(), Ap.end());
	}
	// if qsort_r3, apply qsort_r3(...)
	else if(algname == "-qsort_r3") {
		quicksort(Ap, 0, Ap.size()-1);
	}
	// use sorted Ap array to relink list 
	p = head;
	for(int j = 0; j < count; j++) {
		p->next = Ap[j];
		p = p->next;
	}
	p->next = NULL;
}

//templated quicksort function
//Parameters: vector<Tdata> A - array to be sorted
//			  int left - index of left most element
//			  int right - index of right most element
//Pre-conditions: overloaded < operator for Tdata class
//Post-condition: data in A is sorted
template <typename Tdata>
void quicksort(vector<Tdata> &A, int left, int right) {
	//chooses random pivot
	if(left >= right) return;
	int random = left + rand() % (right-left+1);
	int pindex = random;
	Tdata pivot = A[pindex];
	swap(A[pindex], A[right]);

	//partitions data
	int i = left - 1;
	int j = right;
	while(1){
		while(i <= j && A[++i] < pivot);
		while(j > i && pivot < A[--j]);
		if(i>=j) break;
		swap(A[i], A[j]);
	}
	pindex = i;
	swap(A[i], A[right]);
	//recursively calls quicksort on sub-arrays
	quicksort(A, left, pindex-1);
	quicksort(A, pindex+1, right);
}

#endif // SLIST_H

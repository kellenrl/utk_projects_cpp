#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

#include <cstdlib>
#include <vector>

using namespace std;

class rnumgen {
  public:
    rnumgen(int seed=0) { srand(seed); }
	rnumgen(int seedvalue, vector<int> &v);
    int rand() const;

  private:
	vector<float> F;
};

#endif

#include <GradSoft/ptrs.h>
#include <memory>
using std::auto_ptr;

// Hmm, really auto_array_ptr is not a good idea, so we exclude
// it from demos now.

using namespace GradSoft;

int main(int argc, char** argv)
{
 auto_ptr<int> x(new int);
 auto_ptr<int> y(new int);

 return 0;
}

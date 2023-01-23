// cwheel.cpp: 定义应用程序的入口点。
//

#include "cwheel.h"

using namespace std;

int main() {
#if defined(__clang__)
  cout << "Hello clang." << endl;
#elif defined(__GNUC__) || defined(__GNUG__)
  cout << "Hello gcc." << endl;
#elif defined(_MSC_VER)
  cout << "Hello msvc." << endl;
#endif
  return 0;
}

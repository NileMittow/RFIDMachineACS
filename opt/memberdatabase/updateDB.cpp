#include <iostream>
#include "member.h"
#include "membership_db.h"
#include "doorappMacros.h"
#include "network_tools.h"

using namespace std;

int main() {
  int tc;
  update(DB_LOCATION, &tc);
  return 0;
}

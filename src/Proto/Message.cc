#include "Proto/Message.h"

namespace proto {

void Message::PrintIndent(int num) const {
  for (int i = 0; i < num; i++) {
    std::cout << "  ";
  }
}

}  // namespace proto

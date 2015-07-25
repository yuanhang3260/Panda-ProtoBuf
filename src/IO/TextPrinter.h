#ifndef TEXT_PRINTER_
#define TEXT_PRINTER_

#include <vector>
#include <map>
#include <memory>

#include "FileDescriptor.h"

namespace IO {

class TextPrinterImpl;

class TextPrinter {
 public:
  TextPrinter(std::string outputfile);
  ~TextPrinter();
  
  void Print(std::string content);
  void Print(std::string content, std::vector<std::string> matches);
  void Print(std::string content, std::map<std::string, std::string> map);
  void Flush();

 protected:
  std::unique_ptr<TextPrinterImpl> text_printer_impl_;
};

}

#endif /* TEXT_PRINTER_ */
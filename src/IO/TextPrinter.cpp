#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>

#include "TextPrinter.h"

namespace IO {

// TextPrinter Implementor
class TextPrinterImpl {
 public:
  friend class TextPrinter;
  TextPrinterImpl(std::string outputfile);
  int Print_Impl(std::string, std::vector<std::string>);
  int Print_Impl(std::string, std::map<std::string, std::string>);
  int DoPrint(std::string content);

 private:
  std::string outputfile_;
  std::unique_ptr<FileDescriptor> fd_;
};

TextPrinterImpl::TextPrinterImpl(std::string outputfile) :
    outputfile_(outputfile) {
  fd_.reset(new FileDescriptor(outputfile, FileDescriptor::WRITE_ONLY));
}

int TextPrinterImpl::Print_Impl(
    std::string content, std::vector<std::string> matches) {
  if (matches.size() == 0) {
    return DoPrint(content);
  }

  std::vector<int> matched_indexes;
  std::vector<std::string> pieces;
  unsigned int last_index = -1;
  for (unsigned int i = 0; i < content.length(); i++) {
    if (content[i] == '$') {
      if (i > 0 && content[i - 1] == '\\') {
        continue;
      }
      else {
        matched_indexes.push_back(i);
        pieces.push_back(content.substr(last_index + 1, i));
        last_index = i;
        if (matched_indexes.size() >= matches.size()) {
          break;
        }
      }
    }
  }
  if (last_index < content.length()) {
    pieces.push_back(content.substr(last_index + 1));
  }

  int num = matched_indexes.size() < matches.size()?
                matched_indexes.size() : matches.size();
  std::stringstream sstr;
  for (unsigned int i = 0; i < pieces.size(); i++) {
    if (num > 0) {
      sstr << pieces[i] << matches[i];
      num--;
    }
    else {
      sstr << pieces[i]; 
    }
  }

  return DoPrint(sstr.str());
}

int TextPrinterImpl::Print_Impl(
    std::string content, std::map<std::string, std::string> matches) {
  return 0;
}

int TextPrinterImpl::DoPrint(std::string content) {
  return fd_->Write(content.c_str(), content.length());
}

// TextPrinter functions
TextPrinter::TextPrinter(std::string outputfile) {
  text_printer_impl_.reset(new TextPrinterImpl(outputfile));
}

TextPrinter::~TextPrinter() {
  text_printer_impl_->fd_->Close();
}

void TextPrinter::Print(std::string content, std::vector<std::string> matches) {
  text_printer_impl_->Print_Impl(content, matches);
}

void TextPrinter::Print(
    std::string content, std::map<std::string, std::string> matches) {
  text_printer_impl_->Print_Impl(content, matches);
}



}  // namespace IO

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
  std::vector<std::string> pieces;
  unsigned last_index = 0, matched_num = 0;
  int printed_size = 0;
  for (unsigned int i = 0; i < content.length(); i++) {
    if (content[i] == '$') {
      if (i > 0 && content[i - 1] == '\\') {
        continue;
      }
      else {
        printed_size += DoPrint(content.substr(last_index, i));
        if (matched_num < matches.size()) {
          printed_size += DoPrint(matches[matched_num]);
        }
        last_index = i + 1;
      }
    }
  }
  if (last_index < content.length()) {
    printed_size += DoPrint(content.substr(last_index));
  }

  return printed_size;
}

int TextPrinterImpl::Print_Impl(
    std::string content, std::map<std::string, std::string> matches) {
  std::vector<std::string> pieces;
  unsigned left = 0, right = 0, last_right = 0;
  int printed_size = 0;
  int matching = 0;
  for (unsigned int i = 0; i < content.length(); i++) {
    if (matching) {
      if (content[i] == '{') {
        if (i > 0 && content[i-1] == '\\') {
          continue;
        }
        left = i;
        printed_size += DoPrint(content.substr(last_right + 1, left));
        matching = 1;
      }
    }
    else {
      if (content[i] == '}') {
        if (i > 0 && content[i-1] == '\\') {
          continue;
        }
        right = i;
        std::string key = content.substr(left, right + 1);
        if (matches.find(key) != matches.end()) {
          printed_size += DoPrint(matches[key]);
        }
        last_right = right;
        matching = 0;
      }
    }
  }
  if (last_right < content.length()) {
    printed_size += DoPrint(content.substr(last_right));
  }

  return printed_size;
}

int TextPrinterImpl::DoPrint(std::string content) {
  int nwrite = fd_->Write(content.c_str(), content.length());
  if (nwrite < 0) {
    fprintf(stderr, "ERROR: DoPrint %d chars failed\n", content.length());
    return 0;
  }
  return nwrite;
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

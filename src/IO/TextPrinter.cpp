#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "TextPrinter.h"
#include "../Utility/BufferedDataWriter.h"

namespace IO {

// TextPrinter Implementor
class TextPrinterImpl {
 public:
  friend class TextPrinter;
  TextPrinterImpl(std::string outputfile);
  int Print_Impl(std::string, std::vector<std::string>);
  int Print_Impl(std::string, std::map<std::string, std::string>);
  int DoPrint(std::string content);
  void Flush_Impl();

 private:
  std::string outputfile_;
  std::unique_ptr<Utility::BufferedDataWriter> writer_;
};

TextPrinterImpl::TextPrinterImpl(std::string outputfile) :
    outputfile_(outputfile) {
  std::unique_ptr<FileDescriptor> fd_;
  fd_.reset(new FileDescriptor(outputfile, FileDescriptor::WRITE_ONLY));
  writer_.reset(new Utility::BufferedDataWriter(std::move(fd_)));
}

int TextPrinterImpl::Print_Impl(
    std::string content, std::vector<std::string> matches) {
  std::vector<std::string> pieces;
  unsigned last_index = 0, matched_num = 0;
  int printed_size = 0;
  for (unsigned int i = 0; i < content.length(); i++) {
    if (content[i] == '$') {
      printed_size += DoPrint(content.substr(last_index, i - last_index));
      if (i < content.length() - 1 && content[i + 1] == '$') {
        printed_size += DoPrint("$");
        last_index = (++i) + 1;
        continue;
      }
      else {
        if (matched_num < matches.size()) {
          printed_size += DoPrint(matches[matched_num++]);
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
    if (!matching) {
      if (content[i] == '{') {
        if (i > 0 && content[i-1] == '^') {
          continue;
        }
        left = i;
        matching = 1;
      }
    }
    else {
      if (content[i] == '}') {
        if (i > 0 && content[i-1] == '^') {
          continue;
        }
        right = i;
        printed_size += DoPrint(content.substr(last_right, left - last_right));
        std::string key = content.substr(left + 1, right - left - 1);
        if (matches.find(key) != matches.end()) {
          printed_size += DoPrint(matches[key]);
        }
        last_right = right + 1;
        matching = 0;
      }
    }
  }
  if (last_right < content.length()) {
    printed_size += DoPrint(content.substr(last_right));
  }

  return printed_size;
}

int TextPrinterImpl::DoPrint(std::string piece) {
  if (piece.length() <= 0) {
    return 0;
  }
  // std::cout << "printing " << piece << std::endl;
  int nwrite = writer_->Write(piece.c_str(), 0, piece.length());
  if (nwrite < 0) {
    fprintf(stderr, "ERROR: DoPrint %d chars failed\n", piece.length());
    return 0;
  }
  return nwrite;
}

void TextPrinterImpl::Flush_Impl() {
  writer_->Flush();
}

// TextPrinter functions
TextPrinter::TextPrinter(std::string outputfile) {
  text_printer_impl_.reset(new TextPrinterImpl(outputfile));
}

TextPrinter::~TextPrinter() {
}

void TextPrinter::Print(std::string content, std::vector<std::string> matches) {
  text_printer_impl_->Print_Impl(content, matches);
}

void TextPrinter::Print(
    std::string content, std::map<std::string, std::string> matches) {
  text_printer_impl_->Print_Impl(content, matches);
}

void TextPrinter::Flush() {
  text_printer_impl_->Flush_Impl();
}



}  // namespace IO

#ifndef PDFSAVER_HPP
#define PDFSAVER_HPP

#include <vector>
#include <giomm/file.h>
#include <qpdf/QPDF.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>

namespace Slicer {

class PdfSaver {
public:
    struct PageData {
        unsigned int file;
        unsigned int pageNumber;
        int rotation;
    };

    struct SaveData {
        std::vector<Glib::RefPtr<Gio::File>> files;
        std::vector<PageData> pages;
    };

    PdfSaver(const SaveData& saveData);

    void save(const Glib::RefPtr<Gio::File>& destinationFile);

private:
    struct FileData {
        std::unique_ptr<QPDF> qpdf;
        std::unique_ptr<QPDFPageDocumentHelper> qpdfPageDocumentHelper;
        std::vector<QPDFPageObjectHelper> qpdfPages;
    };

    const SaveData m_saveData;
    std::vector<FileData> m_filesData;

    void persist(const Glib::RefPtr<Gio::File>& destinationFile);
};

} // namespace Slicer

#endif // PDFSAVER_HPP

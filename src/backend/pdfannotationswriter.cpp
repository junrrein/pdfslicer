// PDF Slicer
// Copyright (C) 2019 Julián Unrrein

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "pdfannotationswriter.hpp"
#include <DictionaryContext.h>
#include <ObjectsContext.h>

namespace Slicer {

PdfAnnotationsWriter::PdfAnnotationsWriter()
{
}

EStatusCode PdfAnnotationsWriter::OnPageWrite(PDFPage*,
                                              DictionaryContext* pageDictionaryContext,
                                              ObjectsContext* pdfWriterObjectContext,
                                              DocumentContext*)
{
    if (m_annotationIds.size() == 0)
        return eSuccess;

    if (pageDictionaryContext->WriteKey("Annots") != eSuccess)
        return eFailure;

    pdfWriterObjectContext->StartArray();

    for (ObjectIDType annotation : m_annotationIds)
        pdfWriterObjectContext->WriteIndirectObjectReference(annotation);

    pdfWriterObjectContext->EndArray(eTokenSeparatorEndLine);

    m_annotationIds.clear();

    return eSuccess;
}

void PdfAnnotationsWriter::AddCopiedAnnotation(ObjectIDType newAnnotation)
{
    m_annotationIds.push_back(newAnnotation);
}

} // namespace Slicer

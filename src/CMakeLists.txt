add_executable(pdfSlicer
    main.cpp
    config.hpp
    document.hpp
    document.cpp
    view.hpp
    view.cpp
    previewwindow.hpp
    previewwindow.cpp
    window.hpp
    window.cpp)

target_link_libraries(pdfSlicer
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
    ${POPPLER_LIBRARIES}
    PDFWriter)

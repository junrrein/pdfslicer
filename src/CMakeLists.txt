configure_file(config.hpp.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp)

add_executable(pdfSlicer
    main.cpp
    config.hpp
    slicerdocument.hpp
    slicerdocument.cpp
    slicerview.hpp
    slicerview.cpp
    previewwindow.hpp
    previewwindow.cpp
    slicerwindow.hpp
    slicerwindow.cpp)

target_link_libraries(pdfSlicer
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
    ${POPPLER_LIBRARIES}
    PDFWriter)

CPMAddPackage(
        NAME CImg
        GITHUB_REPOSITORY /GreycLab/CImg
        VERSION .3.4.0
        DOWNLOAD_ONLY YES
)



if(CImg_ADDED)
    add_library(CImg INTERFACE)
    target_include_directories(CImg INTERFACE ${CImg_SOURCE_DIR})
    target_link_libraries(CImg INTERFACE ${FFTW_LIBRARIES})
endif()

CPMAddPackage(
    NAME findfftw
    GIT_REPOSITORY "https://github.com/egpbos/findFFTW.git"
    GIT_TAG "master"
    EXCLUDE_FROM_ALL YES
    GIT_SHALLOW YES
)

list(APPEND CMAKE_MODULE_PATH "${findfftw_SOURCE_DIR}")
find_package(FFTW)
if (NOT FFTW_FOUND)
        CPMAddPackage(
                NAME fftw3
                OPTIONS
                    "ENABLE_AVX2 ON"
                    "BUILD_TESTS OFF"
                    "BUILD_SHARED_LIBS OFF"
                    "ENABLE_THREADS OFF"
                    "ENABLE_OPENMP OFF"
                URL "http://www.fftw.org/fftw-${FFTW_VERSION}.tar.gz"
                URL_HASH "MD5=8ccbf6a5ea78a16dbc3e1306e234cc5c"
                EXCLUDE_FROM_ALL YES
                GIT_SHALLOW YES
        )

        CPMAddPackage(
                NAME fftw3f
                OPTIONS
                    "ENABLE_AVX2 ON"
                    "BUILD_TESTS OFF"
                    "BUILD_SHARED_LIBS OFF"
                    "ENABLE_FLOAT ON"
                    "ENABLE_THREADS OFF"
                    "ENABLE_OPENMP OFF"
                URL "http://www.fftw.org/fftw-${FFTW_VERSION}.tar.gz"
                URL_HASH "MD5=8ccbf6a5ea78a16dbc3e1306e234cc5c"
                EXCLUDE_FROM_ALL YES
                GIT_SHALLOW YES
        )
        set(FFTW_LIBRARIES fftw3 fftw3f)
else ()
    set(FFTW_LIBRARIES "FFTW::Float" "FFTW::Double")
endif ()

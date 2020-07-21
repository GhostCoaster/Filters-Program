include_directories(C:/Users/Emmet/Programming/lib/libpng-1.6.0/include)
include_directories(C:/Users/Emmet/Programming/lib/libjpeg)

target_link_libraries(Filters_Program C:/Users/Emmet/Programming/lib/libpng-1.6.0/lib/libpngstat.lib)
target_link_libraries(Filters_Program C:/Users/Emmet/Programming/lib/libpng-1.6.0/lib/zlibstat.lib)
target_link_libraries(Filters_Program C:/Users/Emmet/Programming/lib/libjpeg/build/Release/jpeg.lib)

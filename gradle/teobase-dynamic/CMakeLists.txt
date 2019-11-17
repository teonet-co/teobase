cmake_minimum_required(VERSION 3.4.1)

find_library( log-lib
              log )
add_library( teobase
             SHARED
             ../../src/teobase/logging.c
             ../../src/teobase/mutex.c
             ../../src/teobase/socket.c
             ../../src/teobase/time.c )

target_include_directories( teobase
                            PRIVATE
                            ../../include )

target_link_libraries( teobase
                       ${log-lib} )

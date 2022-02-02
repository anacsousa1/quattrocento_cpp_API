set(This OTComm)

file(GLOB Sources
     "*.h"
     "*.cpp"
)

add_executable(${This} ${Sources})
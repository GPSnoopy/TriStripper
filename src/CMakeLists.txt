
set(lib_name "TriStripper")

set(header_files
	../include/public_types.h
	../include/tri_stripper.h
)

set(detail_files
	../include/detail/cache_simulator.h
	../include/detail/connectivity_graph.h
	../include/detail/graph_array.h
	../include/detail/heap_array.h
	../include/detail/policy.h
	../include/detail/types.h
)

set(src_files
	connectivity_graph.cpp
	policy.cpp
	tri_stripper.cpp
)

source_group("Include" FILES ${header_files})
source_group("Include\\Details" FILES ${detail_files})
source_group("Source" FILES ${src_files})

include_directories(../include)

add_library(${lib_name} 
	${header_files}
	${detail_files}
	${src_files}
)

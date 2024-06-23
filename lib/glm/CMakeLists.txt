project(GLMLib CXX)

file(GLOB ROOT_SOURCE ./include/glm/*.cpp)
file(GLOB ROOT_INLINE ./include/glm/*.inl)
file(GLOB ROOT_HEADER ./include/glm/*.hpp)

file(GLOB_RECURSE CORE_SOURCE ./include/glm/detail/*.cpp)
file(GLOB_RECURSE CORE_INLINE ./include/glm/detail/*.inl)
file(GLOB_RECURSE CORE_HEADER ./include/glm/detail/*.hpp)

file(GLOB_RECURSE EXT_SOURCE ./include/glm/ext/*.cpp)
file(GLOB_RECURSE EXT_INLINE ./include/glm/ext/*.inl)
file(GLOB_RECURSE EXT_HEADER ./include/glm/ext/*.hpp)

file(GLOB_RECURSE GTC_SOURCE ./include/glm/gtc/*.cpp)
file(GLOB_RECURSE GTC_INLINE ./include/glm/gtc/*.inl)
file(GLOB_RECURSE GTC_HEADER ./include/glm/gtc/*.hpp)

file(GLOB_RECURSE GTX_SOURCE ./include/glm/gtx/*.cpp)
file(GLOB_RECURSE GTX_INLINE ./include/glm/gtx/*.inl)
file(GLOB_RECURSE GTX_HEADER ./include/glm/gtx/*.hpp)

file(GLOB_RECURSE SIMD_SOURCE ./include/glm/simd/*.cpp)
file(GLOB_RECURSE SIMD_INLINE ./include/glm/simd/*.inl)
file(GLOB_RECURSE SIMD_HEADER ./include/glm/simd/*.h)

# 添加库
add_library(glm
    ${ROOT_SOURCE} ${ROOT_INLINE} ${ROOT_HEADER}
    ${CORE_SOURCE} ${CORE_INLINE} ${CORE_HEADER}
    ${EXT_SOURCE} ${EXT_INLINE} ${EXT_HEADER}
    ${GTC_SOURCE} ${GTC_INLINE} ${GTC_HEADER}
    ${GTX_SOURCE} ${GTX_INLINE} ${GTX_HEADER}
    ${SIMD_SOURCE} ${SIMD_INLINE} ${SIMD_HEADER}
)

# 添加包含目录
target_include_directories(glm PUBLIC "include")
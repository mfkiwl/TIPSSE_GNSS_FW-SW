project(frontend)

add_custom_target(frontend
    DEPENDS dist/index.html)

add_custom_command(OUTPUT dist/index.html
    COMMAND npm run build
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS node_modules
)

add_custom_command(OUTPUT node_modules
    COMMAND npm i
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
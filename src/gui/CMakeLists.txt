target_sources(Nesper PRIVATE MainFrame.cpp Application.cpp DebugWindow.cpp GLCanvas.cpp)

target_include_directories(Nesper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Nesper PUBLIC ${wxWidgets_LIBRARIES} OpenGL::GL GLEW::GLEW)
add_library(gui OBJECT MainFrame.cpp Application.cpp DebugWindow.cpp)

target_link_libraries(gui PUBLIC ${wxWidgets_LIBRARIES})
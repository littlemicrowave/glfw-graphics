#include "App.h"

int main(void)
{
    App* MyApp = new App();
    if (!MyApp->createWindow(800, 480, "Computer Graphics"))
    {
        std::cout << "Error in window creation\n";
    } else
        MyApp->Run();
    delete MyApp;
    return EXIT_SUCCESS;
}


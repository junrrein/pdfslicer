#include "ui/application.hpp"

int main(int num_args, char* args_array[])
{
    auto app = Slicer::Application::create();

    return app->run(num_args, args_array);
}

#include "engine.hpp"
#include "input.hpp"
#include "graphics.hpp"
#include "clock.hpp"

#include <SDL.h>
//#include <gdal_priv.h>
//#include <ogrsf_frmts.h>
#include <boost/program_options.hpp>

#include <iostream>

Engine::Engine(int argc, char **argv)
    : _argc(argc), _argv(argv)
{
    parseArgs();
    init();
}

Engine::~Engine()
{
    stop();
}

void Engine::init()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Unable to initilize SDL" << std::endl;
        exit(1);
    }

    input = new Input(this);
    graphics = new Graphics(this);

    graphics->init();
    input->init();
}

int Engine::run()
{
    Clock clock;
    float dt;

    clock.init();
    while(true) {
        dt = clock.tick();

        input->tick(dt);
        graphics->tick(dt);
        graphics->render();
    }

    return 0;
}

void Engine::stop(int exit_code)
{
    input->stop();
    graphics->stop();

    delete input;
    delete graphics;

    SDL_Quit();

    exit(exit_code);
}

const Options& Engine::getOptions() const
{
    return options;
}

std::vector<std::string> Engine::getCmdArgs() const
{
    std::vector<std::string> args;
    for(int i = 0; i < _argc; i++)
        args.emplace_back(_argv[i]);

    return args;
}

void Engine::parseArgs()
{
    namespace opts = boost::program_options;

    opts::options_description desc("CS791a Program Options");
    opts::variables_map vm;

    options.water_size.push_back(2000);
    options.water_size.push_back(2000);

    desc.add_options()
        ("help,h", "Print Help Message")
        ("verbose,v", "Set Program Output to Verbose")
        ("texture,t", opts::value<std::string>(&options.color_map)->default_value("../colorMap.png"), "Texture File")
        ("wireframe,w", "Only Render Wireframes")
        ("sensitivity", opts::value<float>(&options.camera_sensitivity)->default_value(0.25f), "Mouse Sensitivity")
        ("speed", opts::value<float>(&options.camera_speed)->default_value(5.0f), "Camera Speed")
        ("density,d", opts::value<float>(&options.density)->default_value(1.0f), "Density")
        ("size,s", opts::value<std::vector<unsigned int>>(&options.water_size), "Water Size")
        ("light,l", opts::value<float>(&options.light_speed)->default_value(0.05f), "Speed of Light :p");

    opts::positional_options_description pos;
    //pos.add("terrain", -1);
    //pos.add("texture", 1);
    pos.add("size", 2);

    opts::store(opts::command_line_parser(_argc, _argv).options(desc).positional(pos).run(), vm);

    if(vm.count("help")) {
        std::cout << "Water Help\n\n" << desc << std::endl;
        exit(0);
    }

    try {
        opts::notify(vm);
    }
    catch(opts::error& e) {
        std::cerr << "Command Line Error: " << e.what() << std::endl;
        exit(1);
    }

    if(options.shapes.empty()) {
        options.shapes.push_back("../DryCreek/streamDCEW/streamDCEW.shp");
        options.shapes.push_back("../DryCreek/boundDCEW/boundDCEW.shp");
    }

    options.verbose = vm.count("verbose");
    options.wireframe = vm.count("wireframe");
}

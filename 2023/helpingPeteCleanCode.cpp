//
// Created by peter on 2023-11-02.
//
#include <iostream>
#include <filesystem>
#include "boost/process.hpp"
#include "boost/program_options.hpp"
#include <toml.hpp>


namespace po = boost::program_options;
namespace pr = boost::process;
namespace fs = std::filesystem;


int launch(const toml::value& instance, 
            const std::string& instanceName,  
            const std::string& factorioInstall,
            const fs::path modPath){
    
    fs::path factorioPath(toml::find_or<std::string>(instance, "install", factorioInstall));
    const fs::path executable = factorioPath.append("bin/x64/factorio.exe");
    if (!exists(executable)) {
        std::cout << "The specified Factorio install \"" << executable << "\" does not exist";
        return 1;
    }

    std::cout << "launching instance: " << instanceName << "\n";
    pr::child factorio(executable.c_str(), "--mod-directory", modPath.c_str());

    factorio.wait();

    std::cout << "Instance: " << instanceName << " has terminated\n";
    return 0;
}

std::string checkPath(const bool isPath, 
                    const std::string& launcherInstall, 
                    const toml::value& modTable){
    if (isPath) {
        return toml::find<std::string>(modTable, "location");
    } else {
        return launcherInstall + "/" + toml::find<std::string>(modTable, "location");
    }
}

void establishPath (const toml::value& instance, 
            const std::string& instanceName, 
            const std::string& launcherInstall, 
            const std::string& factorioInstall) {
    const auto& modTable = toml::find(instance, "modDir"); //should be a string <discard_comments>
    const bool isPath = toml::find<bool>(modTable, "isPath");

    const fs::path modPath(checkPath(isPath, launcherInstall, modTable));

    if (!exists(modPath)) {
        std::cout << "Creating directories \"" << modPath << "\n";
        fs::create_directories(modPath);
    }

    launch(instance, instanceName, factorioInstall, modPath);
}


po::options_description generateLaunchOptions () {
    po::options_description generic("General Options");
    generic.add_options()
            ("help,h", "Produce help message")
            ("version", "Print version string")
            ;

    po::options_description launcher("Launcher Options");
    launcher.add_options()
            ("list,l", "List instances")
            ("instance,i", po::value<std::string>(), "Name of instance")
            ("settings,s", po::value<std::string>(), "Path to settings file")
            //("dump-data", "Dumps data.raw as JSON to the script output folder and exits")
            //("dump-icon-sprites", "Dumps all icon sprites as png files to the script output folder and exits.")
            ;

    po::options_description instance("Instance Creation/Edit Options");
    instance.add_options()
            /*("new,n", po::value<std::string>(), "Create a new instance")
            ("edit,e", po::value<std::string>(), "Edit existing instance")
            ("config,c", po::value<std::string>(), "Set config file")
            ("mods,m", po::value<std::string>(), "Set mods folder")
            ("mp-connect", po::value<std::string>(), "Start Factorio and connect to address")
            ("password", po::value<std::string>(), "The password to use when using mp-connect (if any)")
            ("executable-path", po::value<std::string>(), "Override autodetected __PATH__executable. Usually not needed except on very weird systems.")
            ("instrument-mod", po::value<std::string>(), "Name of a mod to enable Instrument Mode")
            ("window-size", po::value<std::string>(), R"(Desired window resolution. For example "1680x1050". Or "maximized")")
            ("low-vram", "Sprites that are not put into sprite atlases won't be allocated as texture objects")*/
            ;

    po::options_description hidden("hidden options");
    hidden.add_options()

            ;
    po::options_description cmdline_options;
    cmdline_options.add(generic).add(launcher).add(instance);

    return cmdline_options;
}

    fs::path settingsFile = fs::current_path().append("settings.toml");
    if (vm.contains("settings")) {
        settingsFile = vm["settings"].as<std::string>();
    }

    if(!exists(settingsFile)) {
        std::cout << "Failed to find settings file " << settingsFile << "\n";
    }
    const auto data = toml::parse(settingsFile);
    const auto defaultFactorioInstall = toml::find<std::string>(data, "factorioInstall");
    const auto defaultLauncherInstall = toml::find_or<std::string>(data, "launcherInstall", fs::current_path().string());
    const auto defaultInstance = toml::find<std::string>(data, "defaultInstance");
int main2(po::variables_map vm){

    if (defaultFactorioInstall.empty()) {
        std::cout << "Failed to find \"factorioInstall\"\n";
        return 1;
    }

    if (defaultLauncherInstall.empty()) {
        std::cout << "Failed to find \"launcherInstall\"\n";
        return 1;
    }

    const auto instances = toml::find(data, "instance");
    if (!instances.is_table()) {
        std::cout << "Failed to find \"instance\"\n";
        return 1;
    }


    if (vm.contains("list")) {
        std::cout << "Listing instances: \n";
        for (const auto& [fst, snd] : instances.as_table()) {
            std::cout << fst << ":\n";
            for (const auto& [fst2, snd2] : snd.as_table()) {
                std::cout << "\t" <<fst2 << ": " << snd2 << "\n";
            }
        }
        return 0;
    }

    if (vm.contains("instance")) {
        if (auto instanceName = vm["instance"].as<std::string>(); instances.contains(instanceName)) {
            const auto& instTable = toml::find(instances, instanceName);
            establishPath(instTable, instanceName, defaultLauncherInstall, defaultFactorioInstall);
            return 0;
        } else {
            std::cout << "Could not find instance \"" << instanceName << "\"\n";
            return 1;
        }
    }

    if (instances.contains(defaultInstance)) {
        const auto& instTable = toml::find(instances, defaultInstance);
        establishPath(instTable, defaultInstance, defaultLauncherInstall, defaultFactorioInstall);
        return 0;
    }
}

int main(int argc, char* argv[]) {
    po::options_description cmdline_options = generateLaunchOptions();
    po::variables_map vm;
    po::store(po::command_line_parser (argc, argv).options(cmdline_options).run(), vm);
    po::notify(vm);
    if (vm.contains("help")) {
        std::cout << cmdline_options << "\n";
        return 1;
    }
}
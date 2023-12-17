//
// Created by peter on 2023-11-02.
//
#include <iostream>
#include <filesystem>
#include "boost/process.hpp"
#include "boost/program_options.hpp"
#include <toml.hpp>

#define NAME_OF( v ) #v

namespace po = boost::program_options;
namespace pr = boost::process;
namespace fs = std::filesystem;


int launch(const toml::value& instance, 
            const std::string& instanceName,  
            const std::string& factorioInstall,
            const fs::path& modPath){
    
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


fs::path searchingForSettings(po::variables_map& vm){
    fs::path settingsFile = fs::current_path().append("settings.toml");
    if (vm.contains("settings")) {
        settingsFile = vm["settings"].as<std::string>();
    }

    if(!exists(settingsFile)) {
        std::cout << "Failed to find settings file " << settingsFile << "\n";
        throw new std::runtime_error();
    }
    return settingsFile;
}

int vmDefaultInstance(po::variables_map& vm, 
                const toml::value& instances, 
                const std::string& defaultInstance, 
                const std::string& defaultFactorioInstall, 
    if (instances.contains(defaultInstance)) {
        const auto& instTable = toml::find(instances, defaultInstance);
        establishPath(instTable, defaultInstance, defaultLauncherInstall, defaultFactorioInstall);
        return 0;
    }
}

int vmInstance(po::variables_map& vm, 
            const toml::value& instances, 
            const std::string& defaultFactorioInstall, 
            const std::string& defaultLauncherInstall){
    if (vm.contains("instance")) {
        auto instanceName = vm["instance"].as<std::string>(); 
        if (instances.contains(instanceName)) {
            const auto& instTable = toml::find(instances, instanceName);
            establishPath(instTable, instanceName, defaultLauncherInstall, defaultFactorioInstall);
            return 0;
        } else {
            std::cout << "Could not find instance \"" << instanceName << "\"\n";
            return 1;
        }
    }
}

int vmList(po::variables_map& vm, const toml::value& instances){
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

}

void errorCheckString(const std::string var, const std::string name){
    if(var.empty()){
        std::cout << name << " failed to create.\n";
        throw new std::runtime_error();
    }
}

int main2(po::variables_map& vm){
    fs::path settingsFile = searchingForSettings(vm);

    const toml::value data = toml::parse(settingsFile);
    const std::string defaultFactorioInstall = toml::find<std::string>(data, "factorioInstall");
    const std::string defaultLauncherInstall = toml::find_or<std::string>(data, "launcherInstall", fs::current_path().string());
    const std::string defaultInstance = toml::find<std::string>(data, "defaultInstance");

    errorCheckString(defaultFactorioInstall, NAME_OF(defaultFactorioInstall));
    errorCheckString(defaultLauncherInstall, NAME_OF(defaultLauncherInstall));

    //As Peter had moved on at this point I can no longer ask what const auto instance is
    const toml::value instances = toml::find(data, "instance");
    if (!instances.is_table()) {
        std::cout << "Failed to find \"instance\"\n";
        return 1;
    }
    ////////////////////////////////////////////////////////////////////////////////////////

    if (vmList(vm, instances) == 0){
        return 0;
    }


    if(vmInstance(vm, instances, defaultFactorioInstall, defaultLauncherInstall) == 0){
        return 0;
    }


    if(vmDefaultInstance(vm, instances, defaultInstance, defaultFactorioInstall, defaultLauncherInstall) == 0){
        return 0;
    }
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

int main(int argc, char* argv[]) {
    po::options_description cmdline_options = generateLaunchOptions();
    po::variables_map vm;
    po::store(po::command_line_parser (argc, argv).options(cmdline_options).run(), vm);
    po::notify(vm);
    if (vm.contains("help")) {
        std::cout << cmdline_options << "\n";
        return 1;
    }

    return main2(vm);
}
/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */


#include "data_type_info.hpp"
#include "generated/version_info.hpp"
#include "parse_config_file.hpp"

#include <ctime>
#include <cxxopts.hpp>
#include <cxxsemaphore.hpp>
#include <cxxshm.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sys/ioctl.h>
#include <sysexits.h>


auto main(int argc, char **argv) -> int {
    const std::string exe_name = std::filesystem::path(*argv).filename().string();
    cxxopts::Options  options(exe_name, "format content of shared memory");
    options.add_options("shared memory")(
            "s,semaphore",
            "protect the shared memory with an existing named semaphore against simultaneous access",
            cxxopts::value<std::string>());
    options.add_options("other")("h,help", "print usage");
    options.add_options("version information")("version", "print version and exit");
    options.add_options("version information")("longversion",
                                               "print version (including compiler and system info) and exit");
    options.add_options("version information")("shortversion", "print version (only version string) and exit");
    options.add_options("version information")("git-hash", "print git hash");
    options.add_options("other")("g,gencfg", "output an example config file and exit");
    options.add_options("other")("data-types", "show list of supported data types");
    options.add_options("settings")(
            "p,pretty", "indent json output with <arg> spaces (0 - 255)", cxxopts::value<uint8_t>());

    options.add_options("shared memory")("shmname", "name of the shared memory to dump", cxxopts::value<std::string>());
    options.add_options("shared memory")("configfile", "config file", cxxopts::value<std::string>());
    options.parse_positional({"shmname", "configfile"});

    // parse arguments
    cxxopts::ParseResult opts;
    try {
        opts = options.parse(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse arguments: " << e.what() << '\n';
        std::cerr << "Use --help for more information." << '\n';
        return EX_USAGE;
    }

    // print help
    if (opts.count("help")) {
        static constexpr std::size_t MIN_HELP_SIZE = 80;
        if (isatty(STDIN_FILENO)) {
            struct winsize w {};
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {  // NOLINT
                options.set_width(std::max(static_cast<decltype(w.ws_col)>(MIN_HELP_SIZE), w.ws_col));
            }
        } else {
            options.set_width(MIN_HELP_SIZE);
        }

        std::cout << options.help() << '\n';
        std::cout << '\n';
        std::cout << "This application uses the following libraries:" << '\n';
        std::cout << "  - cxxopts by jarro2783 (https://github.com/jarro2783/cxxopts)" << '\n';
        std::cout << "  - cxxshm (https://github.com/NikolasK-source/cxxshm)" << '\n';
        std::cout << "  - cxxsemaphore (https://github.com/NikolasK-source/cxxsemaphore)" << '\n';
        return EX_OK;
    }

    // print version
    if (opts.count("shortversion")) {
        std::cout << PROJECT_VERSION << '\n';
        return EX_OK;
    }

    if (opts.count("version")) {
        std::cout << PROJECT_NAME << ' ' << PROJECT_VERSION << '\n';
        return EX_OK;
    }

    if (opts.count("longversion")) {
        std::cout << PROJECT_NAME << ' ' << PROJECT_VERSION << '\n';
        std::cout << "   compiled with " << COMPILER_INFO << '\n';
        std::cout << "   on system " << SYSTEM_INFO << '\n';
        std::cout << "   from git commit " << RCS_HASH << '\n';
        return EX_OK;
    }

    if (opts.count("git-hash")) {
        std::cout << RCS_HASH << '\n';
        return EX_OK;
    }

    if (opts.count("data-types")) {
        print_data_type_info();
        return EX_OK;
    }

    if (opts.count("gencfg")) {
        std::cout << "# Example configuration file for " << exe_name << '\n';
        std::cout << "#\n";
        std::cout << "# each line represents one data element to format: ADDR,data_type[,name]\n";
        std::cout << "#\n";
        std::cout << "# use '" << exe_name << " --data-types' to get a list of supported data types\n";
        std::cout << "#\n";
        std::cout << "# Examples:\n";
        std::cout << "#\n";
        std::cout << "#   - 32-Bit  little endian floating point at address 0x42:\n";
        std::cout << "0x42,f32l\n";
        std::cout << "#\n";
        std::cout << "#   - 16-Bit  big endian unsigned integer at address 4\n";
        std::cout << "4,u16b\n";
        std::cout << "#\n";
        std::cout << "#   - 16-Bit  big endian unsigned integer at address 6\n";
        std::cout << "6,u16b\n";
        std::cout << "#\n";
        std::cout << "#   - Bool at address 64, bit 3 - named \"My Bool\"\n";
        std::cout << "64:3,bool,My Bool\n";
        return EX_OK;
    }

    if (opts.count("shmname") == 0) {
        std::cerr << "ERROR: no shared memory name provided\n";
        std::cerr << "use --help for more information\n";
        return EX_USAGE;
    }

    if (opts.count("configfile") == 0) {
        std::cerr << "ERROR: no config file provided\n";
        std::cerr << "use --help for more information\n";
        return EX_USAGE;
    }

    // open shared memory
    const auto                            shm_name = opts["shmname"].as<std::string>();
    std::unique_ptr<cxxshm::SharedMemory> shared_memory;
    try {
        shared_memory = std::make_unique<cxxshm::SharedMemory>(shm_name, true);
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
        return EX_SOFTWARE;
    }

    std::unique_ptr<cxxsemaphore::Semaphore> semaphore;
    if (opts.count("semaphore")) {
        try {
            semaphore = std::make_unique<cxxsemaphore::Semaphore>(opts["semaphore"].as<std::string>());
        } catch (std::exception &e) {
            std::cerr << e.what() << '\n';
            return EX_SOFTWARE;
        }
    }

    // shm data from config file
    std::vector<std::unique_ptr<SHM_data>> shm_data;
    try {
        shm_data = parse_config_file(opts["configfile"].as<std::string>(), *shared_memory);
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return EX_DATAERR;
    }

    // output data
    auto execute = [&]() {
        if (semaphore) {
            if (!semaphore->wait({1, 0})) {
                std::cerr << "Failed to acquire semaphore\n";
                exit(EX_UNAVAILABLE);
            }
        }

        nlohmann::json result_json;
        auto          &data_list = result_json["data"];
        for (auto &a : shm_data) {
            auto data               = a->get_data();
            data_list[data["name"]] = data;
        }

        if (semaphore) semaphore->post();

        result_json["shm"]["name"]      = shm_name;
        result_json["shm"]["size"]      = shared_memory->get_size();
        result_json["shm"]["semaphore"] = static_cast<bool>(semaphore);
        result_json["time"]             = std::time(nullptr);

        std::cout << result_json.dump(opts.count("pretty") ? opts["pretty"].as<uint8_t>() : -1) << std::endl;
    };

    execute();
}

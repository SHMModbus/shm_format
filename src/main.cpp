/*
 * Copyright (C) 2024 Nikolas Koesling <nikolas@koesling.info>.
 */


#include "SHM_Format.hpp"
#include "data_type_info.hpp"
#include "generated/version_info.hpp"
#include "parse_config_file.hpp"

#include <csignal>
#include <ctime>
#include <cxxitimer.hpp>
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
    options.add_options("settings")(
            "i,interval", "repeat with given interval (milliseconds)", cxxopts::value<unsigned>());

    options.add_options("shared memory")("shmname", "name of the shared memory to dump", cxxopts::value<std::string>());
    options.add_options("shared memory")("configfile", "config file", cxxopts::value<std::string>());
    options.parse_positional({"shmname", "configfile"});

    options.positional_help("SHM_NAME CFG_FILE");

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

    static volatile bool terminate        = false;
    auto                 sig_term_handler = [](int) { terminate = true; };

    if (signal(SIGINT, sig_term_handler) == SIG_ERR || signal(SIGTERM, sig_term_handler) == SIG_ERR) {
        perror("signal");
        return EX_OSERR;
    }

    // open semaphore (if specified)
    std::unique_ptr<cxxsemaphore::Semaphore> semaphore;
    if (opts.count("semaphore")) {
        try {
            semaphore = std::make_unique<cxxsemaphore::Semaphore>(opts["semaphore"].as<std::string>());
        } catch (std::exception &e) {
            std::cerr << e.what() << '\n';
            return EX_SOFTWARE;
        }
    }

    // create SHM_Format instance(s)
    std::vector<std::unique_ptr<SHM_Format>> shm_format;
    try {
        const std::string &shm_name      = opts["shmname"].as<std::string>();
        const std::string &cfg_file_path = opts["configfile"].as<std::string>();
        shm_format.emplace_back(std::make_unique<SHM_Format>(shm_name, cfg_file_path));
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return EX_SOFTWARE;
    }

    // output data
    auto execute = [&]() {
        nlohmann::json result_json;
        nlohmann::json result_data;

        static std::size_t index = 0;
        if (semaphore) {
            if (!semaphore->wait({1, 0})) {
                std::cerr << "Failed to acquire semaphore\n";
                exit(EX_UNAVAILABLE);
            }
        }

        for (const auto &f : shm_format) {
            const auto &shm_name  = f->get_shm_name();
            result_data[shm_name] = f->execute();
        }

        if (semaphore) semaphore->post();

        result_json["shm_data"] = result_data;

        result_json["semaphore"] = static_cast<bool>(semaphore);
        if (semaphore) result_json["semaphore_name"] = semaphore->get_name();
        result_json["time"]  = std::time(nullptr);
        result_json["index"] = index++;

        std::cout << result_json.dump(opts.count("pretty") ? opts["pretty"].as<uint8_t>() : -1,
                                      ' ',
                                      true,
                                      nlohmann::json::error_handler_t::replace)
                  << std::endl;
    };

    const bool cyclic = opts["interval"].count();
    if (cyclic) {
        // check interval
        const auto                interval     = opts["interval"].as<unsigned>();
        static constexpr unsigned MIN_INTERVAL = 10;
        if (interval < MIN_INTERVAL) {
            std::cerr << "interval to short. (min: " << MIN_INTERVAL << ')';
            return EX_USAGE;
        }

        // signal set
        sigset_t sleep_sigset;
        sigemptyset(&sleep_sigset);
        sigaddset(&sleep_sigset, SIGALRM);
        sigprocmask(SIG_BLOCK, &sleep_sigset, nullptr);
        sigaddset(&sleep_sigset, SIGINT);   // add to sigwait, but do not block
        sigaddset(&sleep_sigset, SIGTERM);  // add to sigwait, but do not block

        // start timer
        const struct timeval interval_time {
            static_cast<__time_t>(interval / 1000),                           // NOLINT
                    static_cast<__syscall_slong_t>((interval % 1000) * 1000)  // NOLINT
        };
        cxxitimer::ITimer_Real interval_timer(interval_time);
        interval_timer.start();

        while (true) {
            // execute
            execute();

            if (terminate) break;

            // wait for timer
            int  sig = 0;
            auto tmp = sigwait(&sleep_sigset, &sig);
            if (tmp == -1) {
                perror("sigwait");
                exit(EX_OSERR);
            }

            if (sig != SIGALRM) break;
        }

    } else {
        execute();
    }
}

/**
 * SPDX-FileCopyrightText: 2020-2021 EasyCoding Team and contributors
 *
 * SPDX-License-Identifier: MIT
*/

#include <iostream>
#include <stdexcept>

#include <boost/program_options.hpp>

#include "application/application.hpp"

/**
 * The main entry point.
 * @param argc Command-line arguments count.
 * @param argv Command-line arguments array.
*/
int main(int argc, char** argv)
{
    try
    {
        std::unique_ptr<Application> App = std::make_unique<Application>(argc, argv);
        return App -> Run();
    }
    catch (boost::program_options::required_option& e)
    {
        std::cerr << "Missing command-line argument: " << e.what() << std::endl;
    }
    catch (boost::program_options::error& e)
    {
        std::cerr << "Command-line argument error: " << e.what() << std::endl;
    }
    catch (std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "An error occured: " << e.what() << std::endl;
    }
    return 1;
}
